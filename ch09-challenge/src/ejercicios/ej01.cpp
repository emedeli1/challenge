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
   
   // FIX S1: Si el objetivo está demasiado cerca, no se ve.
   // FIX F2: Se olvido de traits.sight, que es la distancia máxima a la que el guardia puede ver.
   if(dist > static_cast<float>(g.traits.sight)){ return false; }

   float const c{dist < 1.0f ? 1.0f : to.normalized().dot(g.facing)};

   float cone2{g.traits.cone};
   bool ahead {};

   if (alarm_ > 0) {
      cone2 = g.traits.cone * 2;
      // FIX E3: 
      ahead = c >= half_cone_cos(cone2);
   }
   else{
      ahead = c >= half_cone_cos(g.traits.cone);
   }
  
   return (ahead && line_of_sight(guard.phy.pos, target));
}
