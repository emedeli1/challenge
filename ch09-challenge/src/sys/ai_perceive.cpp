// What a guard knows of the courtyard when it thinks:
//   - it sees the intruder (sees: close enough, inside its cone of vision and with no
//     wall in between), and remembers it for SIGHT_MEMORY ticks
//   - it sees a mate chasing the intruder: it learns where the mate saw it. Only from a
//     mate that SAW it: one that only follows another mate does not pass it on. If
//     not, two mates that see each other would chase an old point forever
//   - it hears the noises closer than their radius, unless it remembers the intruder
//     or is running to a bell: then a noise does not distract it
// Each of them gives it a clue: where to look
#include <scn/rules.hpp>
#include <scn/scene_game.hpp>
#include <sys/ai.hpp>

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void
SceneGame::perceive(Entity& guard) {
   GuardComponent& g{ guard.guard };
   Vec2 const      me{ guard.phy.pos };
   auto clue_at = [&g](Vec2 where) {
      g.has_clue   = true;
      g.clue       = where;
      g.search_for = 0;
   };

   Entity const* in{ intruder() };
   bool const    sees_it{ in && !in->intr.caught && !in->intr.escaped
                         && sees(guard, in->phy.pos) };
   bool          sees_chase{ false };
   if (sees_it) {
      g.memory = SIGHT_MEMORY;
      clue_at(in->phy.pos);
   } else if (Entity const* mate{ nearest_chaser(guard) }) {
      sees_chase = true;
      clue_at(mate->guard.clue);
   }
   bool const busy{ g.memory > 0 || sees_chase || g.action == Action::RaiseAlarm };
   if (!busy) {
      if (Entity const* noise{ loudest_noise(me) }) { clue_at(noise->phy.pos); }
   }

   Entity const* bell{ nearest(EM_, TAG_BELL, me) };
   g.seen = {
        .saw_intruder   = g.memory > 0
      , .alarm          = alarm_ > 0
      , .bell_from_me   = bell ? pixels(bell->phy.pos, me) : FAR
      , .sees_chase     = sees_chase
      , .has_clue       = g.has_clue
      , .clue_from_post = g.has_clue ? pixels(g.clue, g.post) : FAR
   };
}
