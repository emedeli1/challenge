// Pruebas del ejercicio 5: SceneGame::investigate, sin mirar alrededor con la alarma
#include <ejercicios/pruebas/p05.hpp>

int main() {
   SceneGame game{ 2026, false };
   Prueba t{ "Ejercicio 5: investigar, sin mirar alrededor si suena la alarma" };
   auto alarm = [&game](int ticks) { Probe::alarm(game) = ticks; };

   t.p(1, "lo normal");
   {
      alarm(0);
      Entity g{ walker({ 450, 560 }) };
      g.guard.has_clue = true;
      g.guard.clue     = { 450, 660 };
      game.investigate(g);
      t.expect_near("pista a 100 px, sin alarma: su velocidad", g.phy.vel, { 0, 1.5f });
      t.expect_near("y mira hacia la pista", g.guard.facing, { 0, 1 });
   }
   {
      alarm(0);
      Entity g{ walker({ 450, 560 }) };
      g.guard.has_clue = true;
      g.guard.clue     = { 455, 560 };
      game.investigate(g);
      t.expect("sin alarma, llega a la pista (a 5 px): search_for"
             , g.guard.search_for, SEARCH_TICKS);
   }
   {
      alarm(300);
      Entity g{ walker({ 450, 560 }) };
      g.guard.has_clue = true;
      g.guard.clue     = { 455, 560 };
      g.phy.vel        = { 1, 0 };
      game.investigate(g);
      t.expect("con la alarma, llega a la pista: no mira alrededor"
             , g.guard.search_for, 0);
      t.expect("olvida la pista", g.guard.has_clue, false);
      t.expect_near("y se para", g.phy.vel, { 0, 0 });
   }

   t.p(2, "los límites");
   {
      alarm(300);
      Entity g{ walker({ 450, 560 }) };
      g.guard.has_clue = true;
      g.guard.clue     = { 450, 660 };
      game.investigate(g);
      t.expect_near("pista a 100 px, con la alarma: su velocidad", g.phy.vel, { 0, 2 });
   }
   {
      alarm(300);
      Entity g{ walker({ 450, 560 }) };
      g.guard.has_clue   = true;
      g.guard.clue       = { 450, 560 };
      g.guard.search_for = 50;
      game.investigate(g);
      t.expect("ya miraba alrededor (faltan 50) y suena la alarma: sigue mirando"
             , g.guard.search_for, 49);
      t.expect_near("gira TURN_STEP", angle_of(g.guard.facing), TURN_STEP);
   }
   {
      alarm(0);
      Entity g{ walker({ 450, 560 }) };
      g.guard.has_clue   = true;
      g.guard.clue       = { 450, 560 };
      g.guard.search_for = 1;
      game.investigate(g);
      t.expect("el último tick de mirar: la olvida", g.guard.has_clue, false);
   }
   {
      alarm(300);
      Entity g{ walker({ 450, 560 }) };
      g.guard.has_clue = true;
      g.guard.clue     = { 450 + ARRIVE_DIST, 560 };
      game.investigate(g);
      t.expect("con la alarma, la pista a ARRIVE_DIST justos: la olvida"
             , g.guard.has_clue, false);
   }
   {
      alarm(0);
      Entity g{ walker({ 450, 560 }) };
      g.phy.vel = { 1, 0 };
      game.investigate(g);
      t.expect_near("sin pista: se para", g.phy.vel, { 0, 0 });
   }
}
