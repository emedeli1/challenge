// AI system: every guard perceives the courtyard, decides with the decision tree
// (ai/guards.cpp) and does what it decided. It sets where it wants to go (phy.vel)
// or gives itself back its route (CMP_PATH). It does not move it: that is the job of
// the path and physics systems.
//
// Its rhythm, like a person's (the numbers of its traits):
//   P (think_every)  it only perceives and decides once every P ticks
//   R (react)        a NEW decision becomes what it does R ticks later.
//                    Until then it goes on with what it was doing (the "?" or "!"
//                    over its head fills up, like the guards of Assassin's Creed)
//
// Each part of a guard's tick is in its own file of src/sys/ (ai_*.cpp):
//   guard_tick  the tick of one guard: memory, R, P and act       (ai_guard_tick.cpp)
//   think       perceive, decide and schedule                     (ai_think.cpp)
//   perceive    what it knows of the courtyard: its Percept       (ai_perceive.cpp)
//   act         what it does with what it decided                 (ai_act.cpp)
#include <scn/scene_game.hpp>

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void
SceneGame::ai_update() {
   EM_.for_each_matching(CMP_GUARD, [&](Entity& guard) { guard_tick(guard); });
}
