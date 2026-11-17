#pragma once
// For the tests of exercise 8: every situation a guard can be in (the 64 combinations
// of the answers to the 6 questions), and the error of a table as text.
#include <ai/guards.hpp>
#include <ejercicios/pruebas/prueba.hpp>

// The situation number `bits` for a profile: each bit says what a question answers
inline Percept situation(int bits, Profile const& prof) {
   return {
        .saw_intruder   = (bits & 1) != 0
      , .alarm          = (bits & 2) != 0
      , .bell_from_me   = (bits & 4) != 0 ? prof.bell_near : prof.bell_near + 1
      , .sees_chase     = (bits & 8) != 0
      , .has_clue       = (bits & 16) != 0
      , .clue_from_post = (bits & 32) != 0 ? prof.zone : prof.zone + 1
   };
}

// In how many of the 64 situations, with every profile, the tree decides like `rules`
template <typename Rules>
int agree(DecisionTree const& tree, Rules rules) {
   int same{};
   for (Profile const& prof : PROFILES) {
      for (int bits{}; bits < 64; bits += 1) {
         Percept const p{ situation(bits, prof) };
         TreePath      path{};
         same += tree.decide(p, prof, path) == rules(p, prof);
      }
   }
   return same;
}
inline constexpr int ALL_SITUATIONS{ 64 * 4 };

// What build_tree says, as text: "árbol de 11 nodos" or "error en la fila 3: ..."
inline std::string result(std::expected<DecisionTree, BuildError> const& r) {
   if (r) { return std::format("árbol de {} nodos", r->size()); }
   return std::format("error en la fila {}: {}", r.error().row
                    , PROBLEM_NAME[std::to_underlying(r.error().problem)]);
}
