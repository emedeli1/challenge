//========================================================================================
// Ejercicio 7: el árbol del panel, hasta un nivel
//
// TIENES QUE COMPLETAR ALGO: level_rows, en ❂❂ Your code ❂❂.
//
// 1. Pasa aquí tu código del papel, tal cual.
// 2. Ejecuta `make ej07`: las pruebas P1 y P2.
// 3. Si algo falla, corrígelo y explica cada cambio con su comentario:
//    `// FIX <tipo><nº>: <motivo>` (N, E, S o F, como dice la rúbrica).
//
// El juego no la usa: su panel enseña el árbol entero o plegado.
//========================================================================================
#include <ai/dt.hpp>

void level_rows(Node const& n, TreePath const& path, std::size_t level
              , std::string_view branch, bool parent_on_way, std::size_t max_level
              , std::vector<TreeRow>& rows) {
   bool const on_way{ parent_on_way && level < path.size
                      && path.steps[level].node == n.id() };
   // ❂❂ Your code ❂❂
   bool const is_leaf {!n.yes()}; //FIX sintaxis yes()
   rows.push_back({.node = &n, .branch=branch, .level=level, .on_way=on_way, .folded=(level==max_level && !is_leaf)}); //FIX sintaxis () y n.id
   if (leaf || !on_way) { return; }
   bool const answer{ path.steps[level].answer }; //FIX falta
   level_rows(*n.yes(), path, level+1, "si", answer, max_level, rows); //FIX Error
   level_rows(*n.no(), path, level+1, "no", answer, max_level, rows); //FIX falta
}
