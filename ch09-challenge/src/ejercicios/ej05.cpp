//========================================================================================
// Ejercicio 5: investigar, sin mirar alrededor si suena la alarma
//
// TIENES QUE COMPLETAR ALGO: SceneGame::investigate, en ❂❂ Your code ❂❂.
//
// 1. Pasa aquí tu código del papel, tal cual.
// 2. Ejecuta `make ej05`: las pruebas P1 y P2.
// 3. Si algo falla, corrígelo y explica cada cambio con su comentario:
//    `// FIX <tipo><nº>: <motivo>` (N, E, S o F, como dice la rúbrica).
// 4. Juega con tu versión: `make run CON=ej05`.
//========================================================================================
#include <scn/rules.hpp>
#include <scn/scene_game.hpp>
#include <sys/ai.hpp>

void
SceneGame::investigate(Entity& guard) {
   GuardComponent& g{ guard.guard };
   if (!g.has_clue) {
      guard.phy.vel = {};
   // ❂❂ Your code ❂❂
   } else {
      go_to(guard, g.clue, (alarm_ > 0 ? RUN : HURRY) * g.traits.walk);
   }
}
