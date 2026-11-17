// Pruebas del ejercicio 8: build_tree, sin preguntas inútiles
#include <ejercicios/pruebas/p08.hpp>

// The tree of guards.hpp, written as if/else
Action guard_rules(Percept const& p, Profile const& prof) {
   if (p.saw_intruder) {
      if (p.alarm) { return Action::Chase; }
      if (p.bell_from_me <= prof.bell_near) { return Action::RaiseAlarm; }
      return Action::Chase;
   }
   if (p.sees_chase) { return Action::Chase; }
   if (p.has_clue) {
      if (p.alarm) { return Action::Investigate; }
      if (p.clue_from_post <= prof.zone) { return Action::Investigate; }
      return Action::Patrol;
   }
   return Action::Patrol;
}

// A chain of questions: row k (k >= 2) asks and goes on with row k - 1 (depth k)
std::vector<NodeDesc> chain(std::size_t rows) {
   std::vector<NodeDesc> t{ leaf(Action::Patrol), leaf(Action::Chase) };
   for (std::size_t k{ 2 }; k < rows; k += 1) {
      t.push_back(ask(0, static_cast<std::uint8_t>(k - 1), 0));
   }
   return t;
}

int main() {
   Prueba t{ "Ejercicio 8: de una tabla a un árbol, sin preguntas inútiles" };

   t.p(1, "lo normal");
   {
      auto const r{ build_tree(GUARD_TREE, QUESTIONS) };
      t.expect("GUARD_TREE", result(r), std::string{ "árbol de 11 nodos" });
      t.expect("su raíz es la última fila (nodo 10)"
             , r && r->root() ? int{ r->root()->id() } : -1, 10);
      t.expect("decide como sus reglas, en las 64 situaciones con los 4 perfiles"
             , r ? agree(*r, guard_rules) : 0, ALL_SITUATIONS);
   }
   {
      std::array const table{ leaf(Action::Patrol), leaf(Action::Chase), ask(0, 1, 0)
                            , ask(3, 2, 2) };
      t.expect("una pregunta con sus dos hijos en la fila 2"
             , result(build_tree(table, QUESTIONS))
             , std::string{ "error en la fila 3: pregunta con los dos hijos iguales" });
   }

   t.p(2, "los límites");
   t.expect("una tabla vacía", result(build_tree({}, QUESTIONS))
          , std::string{ "error en la fila 0: tabla vacía" });
   {
      std::array const table{ leaf(Action::Patrol), ask(0, 0, 0) };
      t.expect("la pregunta de la fila 1 con sus dos hijos en la fila 0"
             , result(build_tree(table, QUESTIONS))
             , std::string{ "error en la fila 1: pregunta con los dos hijos iguales" });
   }
   {
      std::array const table{ leaf(Action::Patrol), ask(6, 0, 0) };
      t.expect("pregunta 6 y los dos hijos iguales: antes, la pregunta"
             , result(build_tree(table, QUESTIONS))
             , std::string{ "error en la fila 1: pregunta que no existe" });
   }
   {
      std::array const table{ leaf(Action::Patrol), ask(0, 1, 1) };
      t.expect("una pregunta que es sus dos hijos: antes, el hijo"
             , result(build_tree(table, QUESTIONS))
             , std::string{ "error en la fila 1: hijo que no es una fila anterior" });
   }
   t.expect("profundidad 8 justa (MAX_DEPTH)", result(build_tree(chain(9), QUESTIONS))
          , std::string{ "árbol de 9 nodos" });
   t.expect("profundidad 9", result(build_tree(chain(10), QUESTIONS))
          , std::string{ "error en la fila 9: árbol demasiado profundo" });
}
