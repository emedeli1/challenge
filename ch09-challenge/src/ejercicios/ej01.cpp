//========================================================================================
// Ejercicio 1: ver, con el cono más ancho si suena la alarma
//
// TIENES QUE COMPLETAR ALGO: SceneGame::sees, en ❂❂ Your code ❂❂.
//
// 1. Pasa aquí tu código del papel, tal cual. Borra el `return false;`,
//    que solo está para que compile.
// 2. Ejecuta `make ej01`: las pruebas P1 y P2.
// 3. Si algo falla, corrígelo y explica cada cambio con su comentario:
//    `// FIX <tipo><nº>: <motivo>` (N, E, S o F, como dice la rúbrica).
// 4. Juega con tu versión: `make run CON=ej01`.
//========================================================================================
#include <scn/scene_game.hpp>
#include <sys/ai.hpp>

bool
SceneGame::sees(Entity const& guard, Vec2 target) const {
   GuardComponent const& g{ guard.guard};
   Vec2 const& to{ target - guard.phy.pos};
   float const& dist{to.length()};
   
   if(dist < station.cast<float>(g.sight)){ return false}

   float const c{dist < 1.0f ? 1.0f : to.normalized().dot(g.)}
   return false;
}
