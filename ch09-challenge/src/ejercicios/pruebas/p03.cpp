// Pruebas del ejercicio 3: schedule, si volver a patrullar es inmediato
#include <ejercicios/pruebas/p03.hpp>

int main() {
   Prueba t{ "Ejercicio 3: qué hace con una decisión, si patrullar es inmediato" };
   using enum Action;

   t.p(1, "lo normal");
   expect(t, "patrulla y decide investigar"
        , guard(Patrol, Patrol, 0), Investigate, Patrol, Investigate, 12);
   expect(t, "investiga y decide patrullar"
        , guard(Investigate, Investigate, 0), Patrol, Patrol, Patrol, 0);
   expect(t, "espera investigar (faltan 5) y lo decide otra vez"
        , guard(Patrol, Investigate, 5), Investigate, Patrol, Investigate, 5);

   t.p(2, "los límites");
   expect(t, "persigue, espera investigar (faltan 5) y decide patrullar"
        , guard(Chase, Investigate, 5), Patrol, Patrol, Patrol, 0);
   expect(t, "patrulla, espera investigar (faltan 5) y decide patrullar"
        , guard(Patrol, Investigate, 5), Patrol, Patrol, Patrol, 0);
   expect(t, "investiga, espera patrullar (faltan 5) y decide patrullar"
        , guard(Investigate, Patrol, 5), Patrol, Patrol, Patrol, 0);
   expect(t, "R = 0 y decide perseguir (al menos 1 tick)"
        , guard(Patrol, Patrol, 0, 0), Chase, Patrol, Chase, 1);
   expect(t, "espera investigar (faltan 5) y decide perseguir"
        , guard(Patrol, Investigate, 5), Chase, Patrol, Chase, 12);
}
