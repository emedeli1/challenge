#pragma once
// A guard for the tests of seeing and acting: an entity on its own (not in the
// EntityManager), with the numbers each test needs.
#include <ai/guards.hpp>
#include <cmp/entity.hpp>

inline Entity guard_at(Vec2 pos, Vec2 facing, int sight = 200, int cone = 90) {
   Entity e{};
   e.cmp          = CMP_RENDER | CMP_PHYSICS | CMP_COLLIDER | CMP_GUARD;
   e.phy.pos      = pos;
   e.guard.traits = PROFILES[0];
   e.guard.traits.sight = sight;
   e.guard.traits.cone  = cone;
   e.guard.traits.walk  = 1.0f;
   e.guard.facing = facing;
   return e;
}

// The point at that distance and angle (degrees, turning from facing towards +y)
inline Vec2 at_angle(Vec2 from, Vec2 facing, float degrees, float dist) {
   float const r{ degrees * 3.14159265f / 180.0f };
   float const c{ std::cos(r) };
   float const s{ std::sin(r) };
   Vec2 const  dir{ facing.x() * c - facing.y() * s, facing.x() * s + facing.y() * c };
   return from + dir * dist;
}
