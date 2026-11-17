//========================================================================================
// Ejercicio 2: el compañero más cercano que persigue y se oye
//
// TIENES QUE COMPLETAR ALGO: SceneGame::nearest_chaser, en ❂❂ Your code ❂❂.
//
// 1. Pasa aquí tu código del papel, tal cual. Borra el `return nullptr;`,
//    que solo está para que compile.
// 2. Ejecuta `make ej02`: las pruebas P1 y P2.
// 3. Si algo falla, corrígelo y explica cada cambio con su comentario:
//    `// FIX <tipo><nº>: <motivo>` (N, E, S o F, como dice la rúbrica).
// 4. Juega con tu versión: `make run CON=ej02`.
//========================================================================================
#include <scn/rules.hpp>
#include <scn/scene_game.hpp>

Entity const*
SceneGame::nearest_chaser(Entity const& guard) const {
   Entity const* nearest{ nullptr };
   // FIX S1: Falta guardar la menor distancia al cuadrado encontrada hasta ahora
   float best_d2{};

   // FIX N1: No existe la sintaxis `[&] fest { ( ... }` ni `for_each_matching(fest)`; se llama a `EM_.for_each_matching` con `CMP_GUARD` y una lambda que recibe `Entity const& mate`
   EM_.for_each_matching(CMP_GUARD, [&](Entity const& mate) {
      // FIX E1: Error de concepto al leer `guard.guard` (el propio guardia) en lugar de `mate.guard` (el compañero recorrido)
      GuardComponent const& m{ mate.guard };

      // FIX N2: No existe `guard.CMP_GUARD != g.CMP_GUARD`; para descartarse a sí mismo se comparan direcciones de memoria (`&mate == &guard`)
      if (&mate == &guard) { return; }

      // FIX E2: Error de concepto y lógico: `phy.pos` pertenece a `Entity` (no a `GuardComponent`) y es un `Vec2`, no una distancia; hay que calcular la distancia entre `mate` y `guard`
      float const d2{ (mate.phy.pos - guard.phy.pos).length2() };
      bool const heard{ d2 <= CALL_DIST * CALL_DIST };
      bool const chases{ m.action == Action::Chase && m.memory > 0 };

      if (!heard || !chases) { return; }

      // FIX F1: Fallo lógico: sin comparar `d2 < best_d2`, se sobreescribía `nearest` y devolvía el último compañero válido en vez del más cercano
      if (!nearest || d2 < best_d2) {
         // FIX E3: Error de concepto de tipos: `nearest` es `Entity const*`, por lo que debe apuntar a `&mate` y no a `&m` (`GuardComponent const*`)
         nearest = &mate;
         best_d2 = d2;
      }
   });

   return nearest;
// FIX F2: Error tipográfico: faltaba la llave de cierre `}` de la función y sobraba el paréntesis `(` tras `[&] fest {`
}
