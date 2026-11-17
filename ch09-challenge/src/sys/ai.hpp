#pragma once
// The parts of the AI system that are not members of the scene: the helpers that the
// actions use, and two functions of their own (each one in its own file of src/sys/).
//
// A guard, every tick (SceneGame::guard_tick, sys/ai_guard_tick.cpp):
//   - its memory of the intruder fades (memory)
//   - the countdown of R: a decision that was waiting becomes what it does
//   - the clock of P: if it is time, it thinks (perceive + decide + schedule)
//   - it acts: it does what it is doing (act)
#include <cstdint>
#include <ai/decision.hpp>
#include <cmp/entity.hpp>
#include <man/man_entity.hpp>
#include <utils/vec2.hpp>

// What it does with a decision: keep on, wait for it or start the countdown of R
// (sys/ai_schedule.cpp, exercise 3)
void schedule(GuardComponent& g, Action decided);

// The nearest entity with ALL the components of mask, or nullptr if there is none.
// With two at the same distance, the first one (sys/ai_nearest.cpp)
[[nodiscard]] Entity const* nearest(EntityManager const& EM, CmpMask mask, Vec2 from);

// The helpers (sys/ai_helpers.cpp)
// Walk towards target at that speed, looking that way (it slows down when it is about
// to arrive). A target by the edge of the courtyard is moved to where its body fits
void go_to(Entity& e, Vec2 target, float speed);
// Back to its route, from its nearest point, walking
void resume_route(Entity& e);
// The same vector, turned that many radians (to look around)
[[nodiscard]] Vec2 turned(Vec2 v, float radians);
// Whole pixels from a to b
[[nodiscard]] int pixels(Vec2 a, Vec2 b);
// The cosine of half the cone of vision: a direction is inside the cone if the
// cosine of its angle with facing is at least this
[[nodiscard]] float half_cone_cos(int cone_degrees);
