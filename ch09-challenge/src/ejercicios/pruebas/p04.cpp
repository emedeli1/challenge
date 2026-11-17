// Pruebas del ejercicio 4: guard_tick, que con la alarma piensa más
#include <ejercicios/pruebas/p04.hpp>

int main() {
   SceneGame game{ 2026, false };
   Prueba t{ "Ejercicio 4: el tick de un guardia, que con la alarma piensa más" };
   auto alarm = [&game](int ticks) { Probe::alarm(game) = ticks; };

   t.p(1, "lo normal");
   {
      alarm(0);
      Entity g{ tick_guard(3, 1) };
      t.expect("sin alarma, P = 3: los ticks en que piensa (de 7)"
             , run(game, g, 7), std::string{ "1 4 7" });
   }
   {
      alarm(300);
      Entity g{ tick_guard(4, 1) };
      t.expect("con la alarma, P = 4: los ticks en que piensa (de 7)"
             , run(game, g, 7), std::string{ "1 3 5 7" });
   }
   {
      alarm(0);
      Entity g{ tick_guard(3, 2, 5) };
      run(game, g, 3);
      t.expect("memory = 5, tras 3 ticks", g.guard.memory, 2);
   }

   t.p(2, "los límites");
   {
      alarm(300);
      Entity g{ tick_guard(3, 1) };
      t.expect("con la alarma, P = 3: P / 2 es 1 (de 4)"
             , run(game, g, 4), std::string{ "1 2 3 4" });
   }
   {
      alarm(300);
      Entity g{ tick_guard(1, 1) };
      t.expect("con la alarma, P = 1: P / 2 es 0, pero al menos 1 (de 3)"
             , run(game, g, 3), std::string{ "1 2 3" });
   }
   {
      alarm(300);
      Entity g{ tick_guard(4, 1) };
      g.guard.pending  = Action::Chase;
      g.guard.react_in = 1;
      run(game, g, 1);
      t.expect("en el tick 1 pasa a perseguir antes de pensar", call(0)
             , std::string{ "think en el tick 1, Perseguir" });
      t.expect("y después actúa", call(1), std::string{ "act en el tick 1, Perseguir" });
   }
}
