#pragma once
// The rules of the game: every number the systems use, in one place.
// The library (lib/libarena.a) was compiled with these numbers: if you change one
// here, the parts of the engine inside the library keep the old one.
#include <utils/box.hpp>

inline constexpr float ARENA_W        { 960.0f };
inline constexpr float ARENA_H        { 720.0f };
inline constexpr Box   ARENA          { { 0.0f, 0.0f }, { ARENA_W, ARENA_H } };

// Sizes (radius of the circle drawn; the box is 2 x radius)
inline constexpr float GUARD_RADIUS   {  14.0f };
inline constexpr float INTRUDER_RADIUS{  12.0f };
inline constexpr float SPOT_SIZE      {  28.0f };   // bell and loot: a box of 28 x 28

// Speeds, in pixels per tick. A guard walks at the speed of its profile (walk)
inline constexpr float HURRY          {   1.5f };   // investigating: 1.5 times walk
inline constexpr float RUN            {   2.0f };   // chasing, ringing or with alarm: 2
inline constexpr float SNEAK_SPEED    {   1.6f };   // the intruder, walking
inline constexpr float SPRINT_SPEED   {   2.6f };   // the intruder, running (it is heard)

// The guards
inline constexpr float ARRIVE_DIST    {   8.0f };   // closer than this: it has arrived
inline constexpr int   SIGHT_MEMORY   { 120 };      // it remembers a sighting for 2 s
inline constexpr int   SEARCH_TICKS   { 120 };      // looking around at the clue: 2 s
inline constexpr float TURN_STEP      {   0.05f };  // looking around: radians a tick
inline constexpr int   ALARM_TICKS    { 900 };      // the bells ring for 15 s
inline constexpr float CALL_DIST      { 250.0f };   // a mate who chases is heard
inline constexpr int   MAX_GUARDS     {   8 };
inline constexpr int   MAX_THINK      {  64 };      // the slowest: once every 64 ticks
inline constexpr int   FAR            { 9999 };     // "too far": no intruder, no clue

// The noises
inline constexpr float STEP_NOISE     { 150.0f };   // the steps of the intruder running
inline constexpr int   STEP_EVERY     {  20 };      // ticks between two steps heard
inline constexpr float STONE_NOISE    { 220.0f };   // a stone that falls
inline constexpr float STONE_RANGE    { 220.0f };   // how far the intruder throws it
inline constexpr float STONE_SPEED    {   8.0f };   // a stone flies 8 pixels a tick
inline constexpr int   STONE_TICKS    { static_cast<int>(STONE_RANGE / STONE_SPEED) };
inline constexpr int   NOISE_TICKS    {  30 };      // a noise lasts half a second
inline constexpr int   START_STONES   {   3 };
inline constexpr float DART_SPEED     {  10.0f };   // a dart flies 10 pixels a tick
inline constexpr float DART_NOISE     { 180.0f };   // a dart that falls
