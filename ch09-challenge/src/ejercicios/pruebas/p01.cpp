// Pruebas del ejercicio 1: sees, con el cono más ancho si suena la alarma
#include <ejercicios/pruebas/guardias.hpp>
#include <ejercicios/pruebas/prueba.hpp>

int main() {
   SceneGame game{ 2026, false };                    // the courtyard, with its walls
   Prueba t{ "Ejercicio 1: ver, con el cono más ancho si suena la alarma" };
   // In the open part of the courtyard, below the keep. Sight 200 px, cone 90°
   Vec2 const   me{ 450, 560 };
   Vec2 const   right{ 1, 0 };
   Entity const g{ guard_at(me, right) };
   auto alarm = [&game](int ticks) { Probe::alarm(game) = ticks; };

   t.p(1, "lo normal");
   alarm(0);
   t.expect("sin alarma: delante, a 100 px", game.sees(g, { 550, 560 }), true);
   t.expect("sin alarma: a 60° de su frente, a 150 px"
          , game.sees(g, at_angle(me, right, 60, 150)), false);
   alarm(300);
   t.expect("con la alarma: a 60° de su frente, a 150 px"
          , game.sees(g, at_angle(me, right, 60, 150)), true);
   t.expect("con la alarma: detrás, a 50 px", game.sees(g, { 400, 560 }), false);
   Entity const east{ guard_at({ 580, 360 }, { -1, 0 }) };   // the keep in between
   t.expect("con la alarma: delante y a 180 px, pero con un muro en medio"
          , game.sees(east, { 400, 360 }), false);

   t.p(2, "los límites");
   alarm(300);
   t.expect("con la alarma: a 89°, a 100 px", game.sees(g, at_angle(me, right, 89, 100))
          , true);
   t.expect("con la alarma: a 91°, a 100 px", game.sees(g, at_angle(me, right, 91, 100))
          , false);
   t.expect("con la alarma: delante, a 201 px", game.sees(g, { 651, 560 }), false);
   t.expect("con la alarma: en el mismo punto que el guardia", game.sees(g, me), true);
   alarm(1);
   t.expect("la alarma suena su último tick: a 60°, a 150 px"
          , game.sees(g, at_angle(me, right, 60, 150)), true);
   alarm(0);
   t.expect("sin alarma: a 60° de su frente, a 80 px"
          , game.sees(g, at_angle(me, right, 60, 80)), false);
   t.expect("sin alarma: en el mismo punto que el guardia", game.sees(g, me), true);
   t.expect("sin alarma: a 44°, a 100 px", game.sees(g, at_angle(me, right, 44, 100))
          , true);
}
