#pragma once
// The rows of the tree of the HUD as text, for the tests of exercise 7 (cases a and b)
#include <format>
#include <initializer_list>
#include <string>
#include <vector>
#include <ejercicios/pruebas/prueba.hpp>
#include <ejercicios/pruebas/trees.hpp>

// The text of question k of the drawing (1 to 6), and of an action
inline std::string_view Q(std::size_t k)  { return QUESTIONS[k - 1].text; }
inline std::string_view A(Action a)       { return ACTION_NAME[std::to_underlying(a)]; }

// A row as text: "nivel 2, sí: Perseguir (camino)". Folded: "... [+]"
inline std::string R(std::size_t level, std::string_view branch, std::string_view text
                   , bool on_way = false, bool folded = false) {
   return std::format("nivel {}, {}: {}{}{}", level, branch.empty() ? "raíz" : branch
                    , text, on_way ? " (camino)" : "", folded ? " [+]" : "");
}
inline std::string row(std::vector<TreeRow> const& rows, std::size_t i) {
   if (i >= rows.size()) { return "(no hay fila)"; }
   TreeRow const& r{ rows[i] };
   if (!r.node) { return "(una fila con node nullptr)"; }
   return R(r.level, r.branch, r.node->text(), r.on_way, r.folded);
}

// The way of a decision, step by step: { node, answer }
inline TreePath decision(std::initializer_list<TreePath::Step> steps) {
   TreePath p{};
   for (TreePath::Step const& s : steps) { p.add(s.node, s.answer); }
   return p;
}
// The tree of the guards (trees.hpp): its nodes are 0 Patrol, 1 Investigate, 2 Chase,
// 3 RaiseAlarm, 4 question 6, 5 question 2 (pista), 6 question 5, 7 question 4,
// 8 question 3, 9 question 2 (visto), 10 question 1, the root
inline TreePath const TO_INVESTIGATE{
   decision({ { 10, false }, { 7, false }, { 6, true }, { 5, false }, { 4, true }
            , { 1, true } }) };
inline TreePath const TO_CHASE{ decision({ { 10, true }, { 9, true }, { 2, true } }) };

// Every row: how many there are, and one check per row
inline void expect_rows(Prueba& t, std::string_view what, std::vector<TreeRow> const& rows
                      , std::vector<std::string> const& want) {
   t.expect(std::format("{}: cuántas filas", what), rows.size(), want.size());
   for (std::size_t i{}; i < want.size(); i += 1) {
      t.expect(std::format("{}: fila {}", what, i + 1), row(rows, i), want[i]);
   }
}
// The rows on the way of the decision, by number: "1 2 3"
inline std::string on_way(std::vector<TreeRow> const& rows) {
   std::string s{};
   for (std::size_t i{}; i < rows.size(); i += 1) {
      if (rows[i].on_way) { s += (s.empty() ? "" : " ") + std::to_string(i + 1); }
   }
   return s.empty() ? "ninguna" : s;
}
