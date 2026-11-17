#pragma once
// The entity and its components. Like an entity of the entity_array of the Game Boy:
// every entity has room for every component, and the bits of cmp say which ones it
// really has. cmp empty: the slot is free (EntityManager::new_entity reuses it).
//
// A component is named after what it gives (moving, colliding, following a route...),
// never after a kind of entity. A tag (TAG_BELL...) is a component with no data: the
// bit alone says what the entity is (cmp/cmp_mask.hpp).
//
// The library (lib/libarena.a) was compiled with these structs. Do not add, remove
// or reorder their fields: your code and the library would read different bytes.
#include <cstdint>
#include <span>
#include <ai/dt.hpp>
#include <cmp/cmp_mask.hpp>
#include <utils/box.hpp>
#include <utils/vec2.hpp>

struct PhysicsComponent {
   Vec2 pos{};              // where its centre is (every entity has one)
   Vec2 vel{};              // pixels per tick (only entities with CMP_PHYSICS move)
   Vec2 slide{};            // a body blocked by a solid: which way it slides along it
};

enum class Shape : std::uint8_t { Wall, Guard, Intruder, Noise, Bell, Loot, Exit, Stone };

struct RenderComponent {
   Shape        shape{};
   std::uint8_t color{};    // index of a colour of the palette of the render system
};

// How it collides:
//   Solid   nothing goes through it (the walls)
//   Body    the solids stop it (guards and intruder)
//   Sensor  nothing stops it: a system checks what it touches (bell, loot, exit)
enum class ColliderKind : std::uint8_t { Solid, Body, Sensor };

struct ColliderComponent {
   Vec2         size{};     // width and height of its box, centred on phy.pos
   ColliderKind kind{};

   // Its box if its centre were at pos
   [[nodiscard]] constexpr Box box_at(Vec2 pos) const {
      Vec2 half{ size * 0.5f };
      return { pos - half, pos + half };
   }
};

// Path following: it walks its route, from point to point, and starts again.
// route is a view (std::span): a pointer to the first point and how many there are.
// It does not own the points: they are in a table of static storage (scn/level.hpp)
struct PathComponent {
   std::span<Vec2 const> route{};
   std::uint8_t          next{};     // index of the point it is walking to
   float                 speed{};    // pixels per tick
};

struct GuardComponent {
   std::uint8_t id{};             // 0-7: its number and its colour
   std::uint8_t profile{};        // index of PROFILES (ai/guards.cpp)
   Profile      traits{};         // ITS numbers: a copy of the profile, varied a little
   Vec2         post{};           // the centre of its route: its zone is around it
   Vec2         facing{ 1, 0 };   // where it looks (its cone of vision)

   // Its memory: a clue to investigate (a noise, or where it last saw the intruder)
   std::int32_t memory{};         // > 0: ticks it still remembers seeing the intruder
   bool         has_clue{};
   Vec2         clue{};
   std::int32_t search_for{};     // ticks looking around once it gets to the clue

   // How it thinks: once every P ticks it perceives and decides. A new decision
   // becomes what it does R ticks later: until then, it goes on
   std::int32_t think_in{ 1 };    // ticks until it thinks again (P: traits.think_every)
   std::int32_t react_in{};       // > 0: ticks until pending becomes action (R: traits)
   Action       action{};         // what it is doing
   Action       pending{};        // what it decided and will do in react_in ticks
   Percept      seen{};           // what it perceived the last time (the HUD shows it)
   TreePath     path{};           // how its last decision went through the tree
};

struct IntruderComponent {
   Vec2         facing{ 1, 0 };
   std::int32_t stones{};         // stones left to throw (a noise far away)
   std::int32_t step_noise_in{};  // running: ticks until its steps are heard again
   bool         has_loot{};
   bool         caught{};
   bool         escaped{};
};

// A noise: the guards that are closer than radius hear it, while it lasts
struct NoiseComponent {
   float        radius{};
   std::int32_t ticks{};          // ticks left
};

// Something thrown: it flies (CMP_PHYSICS) and, when it falls, it makes a noise
struct ProjectileComponent {
   std::int32_t ticks{};          // ticks left until it falls
   float        noise{};          // the radius of the noise it makes when it falls
};

struct Entity {
   CmpMask             cmp{};     // which components it has. Empty: free slot
   PhysicsComponent    phy{};
   RenderComponent     ren{};
   ColliderComponent   col{};
   PathComponent       path{};
   GuardComponent      guard{};
   IntruderComponent   intr{};
   NoiseComponent      noise{};
   ProjectileComponent proj{};
};
