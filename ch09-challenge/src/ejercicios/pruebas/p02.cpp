// Pruebas del ejercicio 2: SceneGame::nearest_chaser, si se oye
#include <vector>
#include <ejercicios/pruebas/prueba.hpp>
#include <scn/rules.hpp>

struct Courtyard {
   SceneGame            game{ 2026, false };
   std::vector<Entity*> g{};                    // its three guards

   Courtyard() {
      Probe::EM(game).for_each_matching(CMP_GUARD, [&](Entity& e) { g.push_back(&e); });
      place(0, { 450, 560 });                   // the one who asks, to the right
      g[0]->guard.facing = { 1, 0 };
      place(1, { 550, 560 });
      place(2, { 600, 560 });
   }
   void place(std::size_t i, Vec2 pos) { g[i]->phy.pos = pos; }
   void chase(std::size_t i, int memory) {
      g[i]->guard.action = Action::Chase;
      g[i]->guard.memory = memory;
   }
   Slot nearest_chaser() { return slot_of(game.entities(), game.nearest_chaser(*g[0])); }
   Slot guard(std::size_t i) { return slot_of(game.entities(), g[i]); }
};

int main() {
   Prueba t{ "Ejercicio 2: el compañero más cercano que persigue, si se oye" };

   t.p(1, "lo normal");
   {
      Courtyard c{};
      c.chase(1, 50);
      c.chase(2, 50);
      t.expect("dos persiguen, a 100 y 150 px", c.nearest_chaser(), c.guard(1));
   }
   {
      Courtyard c{};
      c.chase(2, 50);
      t.expect("el de 100 px patrulla; el de 150 px persigue"
             , c.nearest_chaser(), c.guard(2));
   }
   {
      Courtyard c{};
      c.place(1, { 350, 560 });                 // behind it: it does not see it
      c.chase(1, 50);
      c.chase(2, 50);
      t.expect("el más cercano persigue detrás de él, a 100 px: lo oye"
             , c.nearest_chaser(), c.guard(1));
   }

   t.p(2, "los límites");
   {
      Courtyard c{};
      t.expect("nadie persigue", c.nearest_chaser(), Slot{ -1 });
   }
   {
      Courtyard c{};
      c.chase(0, 50);
      t.expect("solo persigue él mismo", c.nearest_chaser(), Slot{ -1 });
   }
   {
      Courtyard c{};
      c.chase(1, 0);
      c.chase(2, 50);
      t.expect("el de 100 px persigue sin haberlo visto (memory 0)"
             , c.nearest_chaser(), c.guard(2));
   }
   {
      Courtyard c{};
      c.place(2, { 350, 560 });                 // behind it, at 100 px too
      c.chase(1, 50);
      c.chase(2, 50);
      t.expect("dos persiguen a 100 px: el primero que se recorre"
             , c.nearest_chaser(), c.guard(1));
   }
   {
      Courtyard c{};
      c.place(1, { 450 + CALL_DIST, 560 });
      c.chase(1, 50);
      t.expect("persigue a CALL_DIST justos", c.nearest_chaser(), c.guard(1));
   }
   {
      Courtyard c{};
      c.place(1, { 450 + CALL_DIST + 1, 560 });
      c.chase(1, 50);
      t.expect("persigue a CALL_DIST + 1 px", c.nearest_chaser(), Slot{ -1 });
   }
}
