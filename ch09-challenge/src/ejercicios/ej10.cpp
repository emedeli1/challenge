//========================================================================================
// Ejercicio 10: tomar huecos desde el final, y liberarlos
//
// TIENES QUE COMPLETAR ALGO: EntityManager::new_entity y destroy_entity, en
// ❂❂ Your code ❂❂.
//
// 1. Pasa aquí tu código del papel, tal cual. Borra el
//    `return entities_[0];`, que solo está para que compile.
// 2. Ejecuta `make ej10`: las pruebas P1 y P2.
// 3. Si algo falla, corrígelo y explica cada cambio con su comentario:
//    `// FIX <tipo><nº>: <motivo>` (N, E, S o F, como dice la rúbrica).
// 4. Juega con tu versión: `make run CON=ej10`.
//========================================================================================
#include <exception>
#include <print>
#include <man/man_entity.hpp>

Entity&
EntityManager::new_entity(CmpMask cmp) {
   // ❂❂ Your code ❂❂ (1)
   return entities_[0];
}

void
EntityManager::destroy_entity(Entity& e) {
   // ❂❂ Your code ❂❂ (2)
}
