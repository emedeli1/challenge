// The guards: their personalities, their questions and their tree.
// Change the numbers of PROFILES, or the table of the tree, build and play.
#include "guards.hpp"
#include <algorithm>
#include <exception>
#include <print>
#include <utility>

// The personalities: the same tree, other numbers. P changes the profile of the
// selected guard in the game
std::array<Profile, 4> const PROFILES {{
   //  name         sight cone  walk  bell  zone   P   R
     { "Patrulla",   230,   90, 1.2f,  300,  260,  6, 12 }
   , { "Centinela",  300,   60, 1.0f,  150,  120,  6, 12 }  // far and narrow; it chases
   , { "Curioso",    220,  120, 1.3f,  300, 2000,  6, 12 }  // greedy: it goes after all
   , { "Novato",     170,   90, 1.1f,  800,  260, 12, 24 }  // it always runs to a bell
}};

// Each game, every guard is its profile plus or minus up to these numbers (the seed
// chooses them). Small: the player can still read the guards, but has to adapt to
// each one. V in the game plays the same seed with no variation, to compare
Variation const VARIATION{
   .sight = 30, .cone = 10, .walk = 0.1f, .bell_near = 50, .zone = 40
 , .think_every = 2, .react = 6
};

// The questions. Each one is a text and a function with no name (a lambda) that
// answers it. They have static storage duration: they exist from init to end, so
// the nodes of any tree can keep a reference to them
std::array<Question, 6> const QUESTIONS {{
     { "1 ¿Ha visto al intruso?"
     , [](Percept const& p, Profile const&) { return p.saw_intruder; } }
   , { "2 ¿Suena la alarma?"
     , [](Percept const& p, Profile const&) { return p.alarm; } }
   , { "3 ¿Tiene una campana cerca?"
     , [](Percept const& p, Profile const& prof) {
          return p.bell_from_me <= prof.bell_near;
       } }
   , { "4 ¿Ve a un compañero persiguiéndolo?"
     , [](Percept const& p, Profile const&) { return p.sees_chase; } }
   , { "5 ¿Tiene una pista?"
     , [](Percept const& p, Profile const&) { return p.has_clue; } }
   , { "6 ¿La pista está en su zona?"
     , [](Percept const& p, Profile const& prof) {
          return p.clue_from_post <= prof.zone;
       } }
}};

//-----------------------------------------------------------------------------
// A copy of the profile, with each number moved by an amount from -v to +v.
// Always in the same order, so the same seed gives the same guards
//-----------------------------------------------------------------------------
Profile vary(Profile const& base, Variation const& v, Xorshift32& rng) {
   auto noise = [&rng](int amount) {
      if (amount <= 0) { return 0; }
      auto const range{ static_cast<std::uint32_t>(2 * amount + 1) };
      return static_cast<int>(rng.below(range)) - amount;
   };
   float const tenths{ static_cast<float>(noise(10)) / 10.0f };   // from -1 to +1
   Profile p{ base };
   p.sight       = std::max(40, p.sight + noise(v.sight));
   p.cone        = std::clamp(p.cone + noise(v.cone), 20, 180);
   p.walk        = std::max(0.5f, p.walk + v.walk * tenths);
   p.bell_near   = std::max(0, p.bell_near + noise(v.bell_near));
   p.zone        = std::max(40, p.zone + noise(v.zone));
   p.think_every = std::max(1, p.think_every + noise(v.think_every));
   p.react       = std::max(1, p.react + noise(v.react));
   return p;
}

//-----------------------------------------------------------------------------
// The tree of guards.hpp as a table: one row per node, from the leaves up (a question
// goes after its two children). The root is the last row. build_tree (exercise 8)
// turns it into the nodes of a DecisionTree
//-----------------------------------------------------------------------------
std::array<NodeDesc, 11> const GUARD_TREE {{
     leaf(Action::Patrol)           //  0
   , leaf(Action::Investigate)      //  1
   , leaf(Action::Chase)            //  2
   , leaf(Action::RaiseAlarm)       //  3
   , ask(5, 1, 0)                   //  4  6 ¿La pista está en su zona?
   , ask(1, 1, 4)                   //  5  2 ¿Suena la alarma?        (with a clue)
   , ask(4, 5, 0)                   //  6  5 ¿Tiene una pista?
   , ask(3, 2, 6)                   //  7  4 ¿Ve a un compañero persiguiéndolo?
   , ask(2, 3, 2)                   //  8  3 ¿Tiene una campana cerca?
   , ask(1, 2, 8)                   //  9  2 ¿Suena la alarma?        (it saw it)
   , ask(0, 9, 7)                   // 10  1 ¿Ha visto al intruso?    (the root)
}};

//-----------------------------------------------------------------------------
// The tree of the guards, from its table. A wrong table is a bug of the game: it
// says what is wrong and in which row, and stops. It returns the tree by value: the
// nodes do not move (they are in dynamic storage), so the references stay valid
//-----------------------------------------------------------------------------
DecisionTree make_guard_tree() {
   auto tree{ build_tree(GUARD_TREE, QUESTIONS) };
   if (!tree) {
      BuildError const e{ tree.error() };
      std::println(stderr, "GUARD_TREE, fila {}: {}", e.row
                  , PROBLEM_NAME[std::to_underlying(e.problem)]);
      std::terminate();
   }
   return std::move(*tree);
}
