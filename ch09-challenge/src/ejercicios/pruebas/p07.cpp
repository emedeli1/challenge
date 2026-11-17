// Pruebas del ejercicio 7: level_rows
#include <ejercicios/pruebas/rows.hpp>

std::vector<TreeRow> rows_of(DecisionTree const& tree, TreePath const& path
                           , std::size_t max_level) {
   std::vector<TreeRow> rows{};
   level_rows(*tree.root(), path, 0, "", true, max_level, rows);
   return rows;
}

// A leaf shared by two questions: root yes -> Chase or Patrol; no -> Investigate or
// the same Patrol. Its nodes: 0 Patrol, 1 Chase, 2 Investigate, 3 and 4 questions,
// 5 the root
DecisionTree shared_tree() {
   DecisionTree t{};
   Node const& patrol     { t.action(Action::Patrol) };
   Node const& chase      { t.action(Action::Chase) };
   Node const& investigate{ t.action(Action::Investigate) };
   Node const& yes        { t.question(QUESTIONS[1], chase, patrol) };
   Node const& no         { t.question(QUESTIONS[4], investigate, patrol) };
   t.set_root(t.question(QUESTIONS[0], yes, no));
   return t;
}

int main() {
   Prueba t{ "Ejercicio 7: el árbol del panel, hasta un nivel" };
   DecisionTree const guards{ guard_tree() };

   t.p(1, "lo normal");
   expect_rows(t, "hasta el nivel 2, decide Investigar"
             , rows_of(guards, TO_INVESTIGATE, 2), {
        R(0, "",   Q(1), true)
      , R(1, "sí", Q(2))
      , R(2, "sí", A(Action::Chase))
      , R(2, "no", Q(3), false, true)
      , R(1, "no", Q(4), true)
      , R(2, "sí", A(Action::Chase))
      , R(2, "no", Q(5), true, true)
   });
   expect_rows(t, "hasta el nivel 3, decide Perseguir", rows_of(guards, TO_CHASE, 3), {
        R(0, "",   Q(1), true)
      , R(1, "sí", Q(2), true)
      , R(2, "sí", A(Action::Chase), true)
      , R(2, "no", Q(3))
      , R(3, "sí", A(Action::RaiseAlarm))
      , R(3, "no", A(Action::Chase))
      , R(1, "no", Q(4))
      , R(2, "sí", A(Action::Chase))
      , R(2, "no", Q(5))
      , R(3, "sí", Q(2), false, true)
      , R(3, "no", A(Action::Patrol))
   });

   t.p(2, "los límites");
   expect_rows(t, "hasta el nivel 0", rows_of(guards, TO_CHASE, 0)
             , { R(0, "", Q(1), true, true) });
   DecisionTree const leaf{ leaf_tree(Action::Patrol) };
   expect_rows(t, "solo una hoja, hasta el nivel 0"
             , rows_of(leaf, decision({ { 0, true } }), 0)
             , { R(0, "", A(Action::Patrol), true) });
   DecisionTree const same{ same_leaf_tree(Action::Chase) };
   expect_rows(t, "la misma hoja en sí y en no, hasta el nivel 1, responde no"
             , rows_of(same, decision({ { 1, false }, { 0, true } }), 1), {
        R(0, "",   Q(1), true)
      , R(1, "sí", A(Action::Chase))
      , R(1, "no", A(Action::Chase), true)
   });
   DecisionTree const shared{ shared_tree() };
   expect_rows(t, "una hoja en dos preguntas, decide la del sí, hasta el nivel 2"
             , rows_of(shared, decision({ { 5, true }, { 3, false }, { 0, true } }), 2), {
        R(0, "",   Q(1), true)
      , R(1, "sí", Q(2), true)
      , R(2, "sí", A(Action::Chase))
      , R(2, "no", A(Action::Patrol), true)
      , R(1, "no", Q(5))
      , R(2, "sí", A(Action::Investigate))
      , R(2, "no", A(Action::Patrol))
   });
   std::vector<TreeRow> whole{};
   tree_rows(*guards.root(), TO_INVESTIGATE, 0, "", true, whole);
   std::vector<TreeRow> const deep{ rows_of(guards, TO_INVESTIGATE, MAX_DEPTH) };
   t.expect("hasta el nivel 8: cuántas filas (el árbol entero)", deep.size()
          , whole.size());
   t.expect("hasta el nivel 8: la última fila", row(deep, deep.size() - 1)
          , row(whole, whole.size() - 1));
   t.expect("hasta el nivel 8: las filas del camino", on_way(deep), on_way(whole));
}
