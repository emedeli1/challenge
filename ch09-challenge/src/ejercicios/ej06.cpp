//========================================================================================
// Ejercicio 6: un dardo de aviso
//
// TIENES QUE COMPLETAR ALGO: SceneGame::new_dart, en ❂❂ Your code ❂❂.
//
// 1. Pasa aquí tu código del papel, tal cual.
// 2. Ejecuta `make ej06`: las pruebas P1 y P2.
// 3. Si algo falla, corrígelo y explica cada cambio con su comentario:
//    `// FIX <tipo><nº>: <motivo>` (N, E, S o F, como dice la rúbrica).
//
// El juego no la usa: ningún guardia lanza dardos.
//========================================================================================
#include <cstdint>
#include <scn/rules.hpp>
#include <scn/scene_game.hpp>

void
SceneGame::new_dart(Entity const& guard, Vec2 target) {
   // FIX E1: Quitar const para poder modificar los componentes y usar la máscara de proyectil
   auto& dart { EM_.new_entity(CMP_PHY | CMP_REN | CMP_PROJ) }; // <-- Verifica el nombre de la máscara en tu proyecto

   // FIX L1: Vector dirección desde el guardia hasta el objetivo (target - pos)
   Vec2 dir { target - guard.phy.pos };
   float dist { dir.length() }; // <-- Verifica si el método en Vec2 es .length(), .mod() o similar

   // FIX L2: Calcular velocidad vectorial (dirección normalizada * rapidez escalar)
   Vec2 vel { (dist > 0.0f) ? (dir / dist) * DART_SPEED : Vec2{0.0f, 0.0f} };

   // FIX S1: Usar operador '=' para asignar agregados a componentes ya creados
   dart.phy = { .pos = guard.phy.pos, .vel = vel };
   dart.ren = { .shape = SHAPE::Stone, .color = guard.ren.color };

   // FIX L3: Calcular ticks con distancia escalar / velocidad (sin bucle while sobre Vec2)
   auto ticks { static_cast<uint32_t>(dist / DART_SPEED) };

   // FIX S2: El componente se llama 'proj', no 'prog', y requiere asignación '='
   dart.proj = { .ticks = ticks, .noise = DART_NOISE };
}
