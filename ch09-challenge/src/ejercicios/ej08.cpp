//========================================================================================
// Ejercicio 8: de una tabla a un árbol, sin preguntas inútiles
//
// TIENES QUE COMPLETAR ALGO: build_tree, en ❂❂ Your code ❂❂.
//
// 1. Pasa aquí tu código del papel, tal cual.
// 2. Ejecuta `make ej08`: las pruebas P1 y P2.
// 3. Si algo falla, corrígelo y explica cada cambio con su comentario:
//    `// FIX <tipo><nº>: <motivo>` (N, E, S o F, como dice la rúbrica).
// 4. Juega con tu versión: `make run CON=ej08`.
//========================================================================================
#include <ai/dt.hpp>

std::expected<DecisionTree, BuildError>
build_tree(std::span<NodeDesc const> table, std::span<Question const> questions) {
   if (table.empty()) { return fail(BuildProblem::Empty, 0); }
   DecisionTree             t{};
   std::vector<Node const*> built{};          // built[i]: the node of row i
   for (std::size_t i{}; i < table.size(); i += 1) {
      NodeDesc const& row{ table[i] };
      // ❂❂ Your code ❂❂
   }
   t.set_root(*built.back());
   if (depth(*t.root()) > MAX_DEPTH) {
      return fail(BuildProblem::TooDeep, table.size() - 1);
   }
   return t;
}
