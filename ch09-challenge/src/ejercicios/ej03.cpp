//========================================================================================
// Ejercicio 3: qué hace con una decisión, si volver a patrullar es inmediato
//
// TIENES QUE COMPLETAR ALGO: schedule, en ❂❂ Your code ❂❂.
//
// 1. Pasa aquí tu código del papel, tal cual.
// 2. Ejecuta `make ej03`: las pruebas P1 y P2.
// 3. Si algo falla, corrígelo y explica cada cambio con su comentario:
//    `// FIX <tipo><nº>: <motivo>` (N, E, S o F, como dice la rúbrica).
// 4. Juega con tu versión: `make run CON=ej03`.
//========================================================================================
#include <algorithm>
#include <sys/ai.hpp>

void schedule(GuardComponent& g, Action decided) {
   // ❂❂ Your code ❂❂
   g.pending  = decided;
   g.react_in = std::max(1, g.traits.react);
}
