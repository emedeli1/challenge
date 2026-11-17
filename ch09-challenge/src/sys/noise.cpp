// Noise system: a noise lasts NOISE_TICKS and then its slot is free again.
#include <scn/scene_game.hpp>

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void
SceneGame::noise_update() {
   EM_.for_each_matching(CMP_NOISE, [&](Entity& e) {
      e.noise.ticks -= 1;
      if (e.noise.ticks <= 0) { EM_.destroy_entity(e); }
   });
}
