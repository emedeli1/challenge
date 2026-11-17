// Path system: every entity with CMP_PATH walks its route. It heads for the next point
// and, when it gets there, the next one is the following point (after the last, the
// first again). It only sets the velocity: physics moves it.
//
// A guard stops following its route when it decides something else (the AI system
// takes CMP_PATH away) and follows it again when it goes back to patrolling.
#include <scn/rules.hpp>
#include <scn/scene_game.hpp>

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void
SceneGame::path_update() {
   EM_.for_each_matching(CMP_PATH | CMP_PHYSICS, [](Entity& e) {
      PathComponent& path{ e.path };
      if (path.route.empty()) { return; }

      // Arrived: on to the following point of the route
      Vec2 to{ path.route[path.next] - e.phy.pos };
      if (to.length() < ARRIVE_DIST) {
         path.next = static_cast<std::uint8_t>((path.next + 1u) % path.route.size());
         to        = path.route[path.next] - e.phy.pos;
      }
      e.phy.vel = to.normalized() * path.speed;
   });
}
