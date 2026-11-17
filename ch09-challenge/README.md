# Challenge 9: Infiltración (2). El cerebro de los guardias. Desafío

El mismo patio del Challenge 8, visto desde arriba. Tú eres el intruso: entras por abajo a la izquierda, robas el botín de la derecha y sales por la puerta verde sin que te atrapen.

Es el proyecto del entrenamiento con los cambios del Desafío: cada ejercicio tiene un solo fichero, `src/ejercicios/ejNN.cpp`, con lo que cambia el enunciado. Pasa aquí vuestro código del papel **tal cual**, ejecuta sus pruebas, y corrige y depura lo que falle explicando cada cambio con su comentario `// FIX`.

**El motor es una librería.** Tienes todos los `.hpp`: la estructura del motor, sus componentes y sus métodos. Del código (`.cpp`) tienes algunas partes como ejemplo, y el resto ya está compilado en `lib/libarena.a`. Cada función de un ejercicio está sola en su propio fichero del motor. Así, si enlazas tu versión, el juego y las pruebas usan la tuya en lugar de la de la librería.

## Qué necesitas
- `g++` 15 o posterior y `make`.
- Raylib 6. En Manjaro: `sudo pacman -S raylib`.

## Cómo se usa
```
make                       Compila el juego para depuración (en build/debug/)
make release               Compila el juego optimizado (en build/release/)
make run                   Lanza el juego (semilla 2026)
make run SEED=1234         Lanza el juego con otra semilla: otra partida
make run CON=ej01          Lanza el juego con tu versión de un ejercicio
make run CON="ej01 ej03"   Lo mismo, con varios ejercicios a la vez
make trace                 Lanza el juego sin ventana, con el intruso en piloto
                           automático, y tras 1800 ticks muestra una huella del
                           estado. Ejecútalo dos veces: sale lo mismo
make trace-equipo          La misma traza con vuestro código: todos los ejercicios
                           que usa el juego. Si sale la misma huella, el juego
                           funciona con vuestro código
make ej01 … make ej10      Un ejercicio: sus pruebas P1 y P2
make dist                  Crea un paquete distribuible del juego (en dist/)
make clean                 Borra lo compilado (build/) y los paquetes (dist/)
```

**El intruso:** flechas o `WASD` para andar · `SHIFT` para correr (vas más rápido, pero tus pasos se oyen) · `T` lanza una piedra delante de ti: vuela y, donde cae, hace ruido (tienes 3). Al empezar, el intruso va en piloto automático; en cuanto pulsas una tecla, lo controlas tú. `I` le devuelve el piloto automático.

**Experimentar:** `1`-`3` eligen guardia · `P` le cambia la personalidad · `-` y `+` lo hacen pensar más lento o más rápido · `R` otra partida · `V` la misma partida, con o sin variación · `ESPACIO` pausa · `N` avanza un tick · `F` pliega o despliega el árbol del panel · `F11` pantalla completa · `F12` captura de pantalla.

## Cómo deciden los guardias
Igual que en el Challenge 8. Cada guardia percibe y decide cada `P` ticks. Una decisión nueva tarda `R` ticks en convertirse en lo que hace, y mientras tanto se llena la señal que tiene sobre la cabeza (`?` o `!`). Entre dos decisiones, sigue haciendo lo que decidió.

```
1 ¿Ha visto al intruso? (en los últimos 2 s)
├─ sí: 2 ¿Suena la alarma?
│      ├─ sí: Perseguir
│      └─ no: 3 ¿Tiene una campana cerca?
│             ├─ sí: Dar la alarma
│             └─ no: Perseguir
└─ no: 4 ¿Ve a un compañero persiguiéndolo?
       ├─ sí: Perseguir
       └─ no: 5 ¿Tiene una pista? (un ruido, un aviso o dónde lo vio)
              ├─ sí: 2 ¿Suena la alarma?
              │      ├─ sí: Investigar
              │      └─ no: 6 ¿La pista está en su zona?
              │             ├─ sí: Investigar
              │             └─ no: Patrullar
              └─ no: Patrullar
```

Lo que cambia en el Desafío:
- **Con la alarma sonando,** el cono de visión de un guardia es el doble de ancho, piensa el doble de rápido y, cuando llega a una pista, no se para a mirar alrededor.
- **Un guardia se une al compañero más cercano que te persigue** si lo oye: a `CALL_DIST` píxeles o menos, aunque no lo vea.
- **Volver a patrullar es inmediato:** sin esperar R ticks.
- **`build_tree`** no acepta una pregunta con los dos hijos en la misma fila.
- **El `EntityManager`** toma los huecos libres desde el final.

## Cómo está hecho: un ECS
El mismo motor del Challenge 8. Entre corchetes, el ejercicio de cada fichero:

```
src/
├── main.cpp            El bucle del juego: lee el teclado, actualiza un tick y lo dibuja
├── cmp/                cmp_mask.hpp: CmpMask, los bits de componentes, y las etiquetas
│                       TAG_*. entity.hpp: los componentes y la Entity
├── man/                EntityManager: guarda las entidades y las recorre
│   ├── man_entity_alloc.cpp             new_entity y destroy_entity        [10]
│   └── man_entity_first_matching.cpp    first_matching
├── ai/                 decision.hpp: qué decide un guardia y a partir de qué.
│   │                   dt.hpp: el árbol de decisión, sus nodos y sus tablas
│   ├── guards.cpp      Las preguntas, las personalidades y la tabla GUARD_TREE
│   ├── dt_depth.cpp    La profundidad de un árbol (ejemplo de la guía)
│   ├── dt_rows.cpp     El árbol del panel, fila a fila
│   ├── dt_folded_rows.cpp  El árbol del panel, plegado
│   ├── dt_level_rows.cpp   El árbol del panel, hasta un nivel            [7]
│   └── dt_build.cpp    build_tree: de la tabla al árbol                  [8]
├── sys/                Los sistemas, uno por fichero. Son funciones de la escena
│   ├── ai.cpp          El sistema de IA: un tick de cada guardia
│   ├── ai_guard_tick.cpp      El tick de un guardia                       [4]
│   ├── ai_perceive.cpp        Lo que sabe del patio cuando piensa
│   ├── ai_sees.cpp            ¿Ve ese punto?                              [1]
│   ├── ai_nearest.cpp         La entidad más cercana
│   ├── ai_nearest_chaser.cpp  El compañero que persigue                   [2]
│   ├── ai_loudest_noise.cpp   El ruido que más fuerte se oye
│   ├── ai_think.cpp           Percibir, decidir y programar la decisión
│   ├── ai_schedule.cpp        Qué hace con una decisión                   [3]
│   ├── ai_act.cpp             Hacer lo decidido
│   ├── ai_investigate.cpp     Investigar                                  [5]
│   ├── ai_raise_alarm.cpp     Dar la alarma
│   ├── new_noise.cpp          Crear un ruido
│   ├── new_stone.cpp          Crear una piedra
│   ├── new_dart.cpp           Crear un dardo de aviso                     [6]
│   └── intruder, path, projectile, physics, noise, rules, render (el único
│                              que usa Raylib)
├── scn/                SceneGame: crea las entidades y llama a los sistemas en orden.
│                       level.hpp: muros, campanas, rutas. rules.hpp: los números
├── utils/              Vec2, Box y Xorshift32 (números aleatorios con semilla)
└── ejercicios/         Un fichero por ejercicio (ej01.cpp …; el 9, ej09.hpp) y sus
                        pruebas (pruebas/)
lib/
└── libarena.a          Las partes del motor que ya están compiladas. Se compilaron con
                        los .hpp de src/: no cambies los campos de sus struct ni los
                        números de rules.hpp, o tu código y la librería leerán bytes
                        distintos
.vscode/                El depurador (gdb) en VS Code: launch.json y tasks.json
```

La lógica avanza en ticks enteros y nunca usa el tiempo real (`GetFrameTime()`): con la misma semilla y las mismas teclas, la partida es siempre la misma.

## Ejercicios
Cada ejercicio es un fichero de `src/ejercicios/` (`ej01.cpp` … `ej10.cpp`; el 9 es un tipo, `ej09.hpp`) que explica al principio qué completar.

- `make ejNN` ejecuta sus dos pruebas: **P1**, lo que suele pasar, y **P2**, los límites (lo más pequeño, lo más grande, lo que empata, cuando no hay ninguno…). Cada prueba enseña cada comprobación con ✅ o ❌ y, si falla, qué se esperaba y qué da tu código.
- `make run CON=ejNN` juega con tu versión de un ejercicio, si el juego usa esa función.
- `make trace-equipo` comprueba que el juego entero funciona con vuestro código: los ejercicios que usa el juego (1, 2, 3, 4, 5, 8 y 10).

## Depurar con gdb en VS Code
Instala gdb (`sudo pacman -S gdb` o `sudo apt install gdb`) y, en VS Code, la extensión C/C++ de Microsoft. Abre la carpeta del proyecto: `.vscode/` ya tiene la configuración. Con el fichero de un ejercicio abierto en el editor, pon un *breakpoint* (clic a la izquierda del número de línea) y pulsa F5:

```
Las pruebas del fichero abierto    Sus pruebas, P1 y P2 (make prueba CASO=...)
El juego con el fichero abierto    El juego con tu versión (make juego CASO=...)
```

La guía del Challenge explica cómo usarlo paso a paso.

## Créditos
- [Raylib](https://www.raylib.com/), de Ramon Santamaria y colaboradores. Licencia zlib/libpng.
- (c) 2026 Francisco J. Gallego-Durán, (@FranGallegoBR, @ProfesorRetroman). Universidad de Alicante.
- Desarrollo asistido por Claudio (Claude Opus 5.5, de Anthropic).
