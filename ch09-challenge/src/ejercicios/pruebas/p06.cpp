// Pruebas del ejercicio 6: SceneGame::new_dart
#include <ejercicios/pruebas/prueba.hpp>
#include <scn/rules.hpp>

// A guard of the courtyard, at pos and with that colour
Entity& guard_of(SceneGame& game, Vec2 pos, std::uint8_t color) {
   Entity* g{ Probe::EM(game).first_matching(CMP_GUARD) };
   g->phy.pos   = pos;
   g->ren.color = color;
   return *g;
}

int main() {
   Prueba t{ "Ejercicio 6: crear un dardo de aviso" };

   t.p(1, "lo normal");
   {
      SceneGame game{ 2026, false };
      game.new_dart(guard_of(game, { 300, 560 }, 2), { 420, 720 });
      Entity const* d{ game.entities().first_matching(CMP_PROJECTILE) };
      t.expect("hay un dardo (una entidad con CMP_PROJECTILE)", d != nullptr, true);
      if (!d) { return 1; }
      t.expect("sus componentes: CMP_RENDER, CMP_PHYSICS y CMP_PROJECTILE"
             , d->cmp == (CMP_RENDER | CMP_PHYSICS | CMP_PROJECTILE), true);
      t.expect_near("sale de donde está el guardia", d->phy.pos, { 300, 560 });
      t.expect_near("hacia (420, 720), a 200 px: su velocidad", d->phy.vel, { 6, 8 });
      t.expect("se dibuja como una piedra", d->ren.shape == Shape::Stone, true);
      t.expect("con el color del guardia", int{ d->ren.color }, 2);
      t.expect("vuela 200 / DART_SPEED ticks", d->proj.ticks, 20);
      t.expect_near("al caer, un ruido de radio DART_NOISE", d->proj.noise, DART_NOISE);
   }

   t.p(2, "los límites");
   {
      SceneGame game{ 2026, false };
      game.new_dart(guard_of(game, { 300, 560 }, 5), { 405, 560 });
      Entity const* d{ game.entities().first_matching(CMP_PROJECTILE) };
      t.expect("a 105 px: 10,5 ticks, sin decimales", d ? d->proj.ticks : -1, 10);
      t.expect("con el color del guardia", d ? int{ d->ren.color } : -1, 5);
   }
   {
      SceneGame game{ 2026, false };
      game.new_dart(guard_of(game, { 300, 560 }, 1), { 300, 560 });
      Entity const* d{ game.entities().first_matching(CMP_PROJECTILE) };
      t.expect_near("hacia donde está él: no se mueve"
                  , d ? d->phy.vel : Vec2{ 99, 99 }, { 0, 0 });
      t.expect("y vuela 0 ticks", d ? d->proj.ticks : -1, 0);
   }
}
