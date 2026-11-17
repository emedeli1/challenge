#pragma once
// For the tests of exercise 5: a guard in the open part of the courtyard, walking at
// 1 pixel per tick (HURRY: 1.5; RUN: 2), and the angle it is looking at.
#include <cmath>
#include <ejercicios/pruebas/guardias.hpp>
#include <ejercicios/pruebas/prueba.hpp>
#include <scn/level.hpp>
#include <scn/rules.hpp>

inline Entity walker(Vec2 pos) {
   Entity e{ guard_at(pos, { 1, 0 }) };
   e.guard.action = Action::Investigate;
   return e;
}

inline float angle_of(Vec2 v) { return std::atan2(v.y(), v.x()); }
