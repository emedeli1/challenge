#pragma once
// The guards of the courtyard: their personalities, the questions they ask and their
// decision tree. All of them use the SAME tree; a personality only changes its numbers.
//
//   1 ¿Ha visto al intruso? (en los últimos 2 s)
//   ├─ sí: 2 ¿Suena la alarma?
//   │      ├─ sí: Perseguir
//   │      └─ no: 3 ¿Tiene una campana cerca?
//   │             ├─ sí: Dar la alarma
//   │             └─ no: Perseguir
//   └─ no: 4 ¿Ve a un compañero persiguiéndolo?
//          ├─ sí: Perseguir
//          └─ no: 5 ¿Tiene una pista?
//                 ├─ sí: 2 ¿Suena la alarma?
//                 │      ├─ sí: Investigar        (corre a donde lo vio quien avisó)
//                 │      └─ no: 6 ¿La pista está en su zona?
//                 │             ├─ sí: Investigar
//                 │             └─ no: Patrullar  (no lo engaña un ruido lejano)
//                 └─ no: Patrullar
//
// "Ha visto" and not "ve": it remembers the intruder for 2 s after losing sight of it.
// Without that memory it dithers: it sees the intruder and chases it, turns its back
// on it, no longer sees it and goes to investigate, turns, sees it again...
//
// Question 2 appears twice: two nodes of the tree ask the SAME question (they keep a
// reference to the same Question of QUESTIONS).
//
// It is not greedy: "investigate whatever you hear" would take every guard far from
// its post with one stone thrown far away, and the way out would be left free. The
// zone (Profile::zone) keeps each one at its post... until the alarm rings.
#include <array>
#include <ai/dt.hpp>
#include <utils/rng.hpp>

// The questions of the tree, numbered as in the drawing above
extern std::array<Question, 6> const QUESTIONS;     // defined in ai/guards.cpp
// The personalities. Change their numbers in ai/guards.cpp and run the game
extern std::array<Profile, 4> const PROFILES;
// How much the guards of a game differ from their profile (ai/guards.cpp)
extern Variation const VARIATION;

// A profile with its numbers moved a little, at most by v, as the seed says
[[nodiscard]] Profile vary(Profile const& base, Variation const& v, Xorshift32& rng);

// The tree of the guards as a table, one row per node (ai/guards.cpp)
extern std::array<NodeDesc, 11> const GUARD_TREE;
// The tree of the guards, built from GUARD_TREE (ai/guards.cpp)
[[nodiscard]] DecisionTree make_guard_tree();
