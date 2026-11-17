#pragma once
// The courtyard: walls, routes and where everything starts.
//
// Every table here has static storage duration: it exists from init to end. That is
// why a PathComponent can keep a view (std::span) of a route: the points it points
// to never disappear. A span of a local array would be left dangling as soon as the
// function that made it returned.
#include <array>
#include <cstdint>
#include <span>
#include <utils/vec2.hpp>

// The walls: top left corner, width and height
struct Wall { float x, y, w, h; };
inline constexpr std::array<Wall, 7> WALLS {{
     { 420, 290, 120, 140 }          // the keep, in the middle
   , { 200,  90,  30, 210 }          // the inner wall, with a gap in front of the gate
   , { 200, 420,  30, 210 }
   , { 690, 170,  60,  60 }          // crates
   , { 690, 490,  60,  60 }
   , { 340, 120,  50,  50 }
   , { 570, 120,  50,  50 }
}};

inline constexpr Vec2 EXIT_POS  {  16, 360 };     // the gate, in the left wall
inline constexpr Vec2 EXIT_SIZE {  32, 120 };
inline constexpr Vec2 LOOT_POS  { 905, 360 };
inline constexpr Vec2 START_POS {  60, 670 };     // the intruder

// The bells: a guard runs to the nearest one to raise the alarm
inline constexpr std::array<Vec2, 2> BELLS {{ { 480, 60 }, { 480, 640 } }};

// The routes of the guards, point by point. The last one goes back to the first
inline constexpr std::array<Vec2, 2> GATE_ROUTE  {{ { 110, 310 }, { 110, 410 } }};
inline constexpr std::array<Vec2, 4> KEEP_ROUTE  {{
     { 320, 230 }, { 640, 230 }, { 640, 500 }, { 320, 500 }
}};
inline constexpr std::array<Vec2, 2> VAULT_ROUTE {{ { 830, 150 }, { 830, 570 } }};

// Who guards each route, and with which personality (index of PROFILES)
struct GuardSpawn { std::span<Vec2 const> route; std::uint8_t profile; };
inline constexpr std::array<GuardSpawn, 3> GUARDS {{
     { GATE_ROUTE,  1 }              // Centinela: the gate
   , { KEEP_ROUTE,  0 }              // Patrulla: around the keep
   , { VAULT_ROUTE, 3 }              // Novato: in front of the loot
}};

// A reinforcement (the game does not use it): it comes in through the
// gate with this profile and walks this route
inline constexpr std::uint8_t        REINFORCE_PROFILE{ 0 };
inline constexpr std::array<Vec2, 2> REINFORCE_ROUTE {{ { 110, 200 }, { 110, 520 } }};

// The intruder on autopilot (the trace, or until you take the keys): it goes along
// the bottom, takes the loot and comes back along the top
inline constexpr std::array<Vec2, 10> INTRUDER_ROUTE {{
     {  60, 670 }, { 420, 680 }, { 900, 660 }, { 905, 360 }, { 900,  70 }
   , { 600,  70 }, { 290,  70 }, { 260, 360 }, { 140, 360 }, {  20, 360 }
}};
