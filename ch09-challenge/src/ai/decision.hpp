#pragma once
// What a guard decides, and from what: the alternatives (Action), what it knows of the
// courtyard when it thinks (Percept) and its personality (Profile).
//
// The decision is a pure function: the same Percept and the same Profile always give
// the same Action. The decision tree (ai/dt.hpp) is that function, made of data.
#include <array>
#include <cstdint>
#include <string_view>

// The alternatives of a guard. All of them are valid: none is "the best" always
enum class Action : std::uint8_t {
   Patrol,        // follow its route
   Investigate,   // go and look at its clue (with the alarm ringing, running)
   Chase,         // run after the intruder
   RaiseAlarm,    // run to the nearest bell and ring it
};
inline constexpr std::array<std::string_view, 4> ACTION_NAME {
   "Patrullar", "Investigar", "Perseguir", "Dar la alarma"
};
// Every action, in order: to walk all of them with a for
inline constexpr std::array<Action, 4> ALL_ACTIONS {
   Action::Patrol, Action::Investigate, Action::Chase, Action::RaiseAlarm
};

// What a guard knows of the courtyard when it thinks: a snapshot, filled in by the game
struct Percept {
   bool saw_intruder;     // it saw it in the last SIGHT_MEMORY ticks (scn/rules.hpp)
   bool alarm;            // the bells are ringing
   int  bell_from_me;     // pixels from the guard to the nearest bell
   bool sees_chase;       // it sees a mate chasing the intruder (one that saw it)
   bool has_clue;         // somewhere to look: a noise, a sighting or the alarm
   int  clue_from_post;   // pixels from its post (the centre of its route) to the clue
};

// The personality of a guard: the SAME tree, with other numbers.
// The table PROFILES is in ai/guards.cpp: change its numbers there and run the game.
struct Profile {
   std::string_view name;
   int              sight;        // it sees up to this many pixels away
   int              cone;         // the width of its cone of vision, in degrees
   float            walk;         // pixels per tick, walking (running: RUN times this)
   int              bell_near;    // a bell closer than this: it rings it before chasing
   int              zone;         // how far from its post a clue may be to go and look
   std::int32_t     think_every;  // P: ticks between two thoughts
   std::int32_t     react;        // R: ticks from deciding something new to doing it
};

// How much each guard of a game may differ from its profile: up to plus or minus
// these numbers. The seed chooses them, so the same seed gives the same guards; a new
// game gives slightly different ones. All zeros: every guard is exactly its profile
struct Variation {
   int          sight;
   int          cone;
   float        walk;
   int          bell_near;
   int          zone;
   std::int32_t think_every;
   std::int32_t react;
};
