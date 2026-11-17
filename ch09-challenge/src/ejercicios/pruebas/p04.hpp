#pragma once
// For the tests of exercise 4: think and act only write down when they are called and
// what the guard is doing then. So the tests see the order of your guard_tick.
// (These think and act replace the ones of the engine in the tests of exercise 4.)
#include <vector>
#include <ejercicios/pruebas/prueba.hpp>

struct Call {
   int    tick;
   char   what;          // 'T': think, 'A': act
   Action doing;
};
inline std::vector<Call> calls{};
inline int               now{};

void SceneGame::think(Entity& guard) { calls.push_back({ now, 'T', guard.guard.action }); }
void SceneGame::act(Entity& guard)   { calls.push_back({ now, 'A', guard.guard.action }); }

// Call number i, as text: "think, Perseguir" (what it was doing), or "ninguna"
inline std::string call(std::size_t i) {
   if (i >= calls.size()) { return "ninguna"; }
   Call const& c{ calls[i] };
   return std::format("{} en el tick {}, {}", c.what == 'T' ? "think" : "act", c.tick
                    , ACTION_NAME[std::to_underlying(c.doing)]);
}

inline Entity tick_guard(int p, int think_in, int memory = 0) {
   Entity e{};
   e.cmp                     = CMP_GUARD;
   e.guard.traits.think_every = p;
   e.guard.traits.react      = 12;
   e.guard.think_in          = think_in;
   e.guard.memory            = memory;
   return e;
}

// guard_tick, ticks times. It returns the ticks in which it thought, as text: "1 4 7"
inline std::string run(SceneGame& game, Entity& g, int ticks) {
   calls.clear();
   for (now = 1; now <= ticks; now += 1) { game.guard_tick(g); }
   std::string thought{};
   for (Call const& c : calls) {
      if (c.what != 'T') { continue; }
      if (!thought.empty()) { thought += ' '; }
      thought += std::to_string(c.tick);
   }
   return thought;
}
