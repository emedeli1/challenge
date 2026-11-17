// Pruebas del ejercicio 10: new_entity desde el final, y destroy_entity
#include <ejercicios/pruebas/prueba.hpp>

int main() {
   Prueba t{ "Ejercicio 10: tomar huecos desde el final, y liberarlos" };

   t.p(1, "lo normal");
   {
      EntityManager EM{ 4 };
      auto slot = [&EM](Entity const& e) { return slot_of(EM, &e); };
      Entity& a{ EM.new_entity(CMP_RENDER) };
      t.expect("la primera, en el hueco 3 (el último)", slot(a), Slot{ 3 });
      t.expect("con sus componentes", a.cmp == CMP_RENDER, true);
      Entity& b{ EM.new_entity(CMP_NOISE) };
      t.expect("la segunda, en el hueco 2", slot(b), Slot{ 2 });
      EM.destroy_entity(a);
      t.expect("destruida la primera, su hueco queda libre", EM.entities()[3].cmp.empty()
             , true);
      t.expect("la siguiente reutiliza el hueco 3", slot(EM.new_entity(CMP_PHYSICS))
             , Slot{ 3 });
   }

   t.p(2, "los límites");
   {
      EntityManager EM{ 4 };
      Entity& a{ EM.new_entity(CMP_RENDER) };
      a.phy.pos     = { 5, 5 };
      a.noise.ticks = 7;
      EM.destroy_entity(a);
      t.expect_near("destruida: sus datos vuelven a cero", EM.entities()[3].phy.pos
                  , { 0, 0 });
      a.phy.pos = { 5, 5 };                     // dirty, but free
      Entity& b{ EM.new_entity(CMP_NOISE) };
      t.expect_near("una nueva en ese hueco empieza limpia", b.phy.pos, { 0, 0 });
      t.expect("con sus componentes, no los de antes", b.cmp == CMP_NOISE, true);
   }
   {
      EntityManager EM{ 4 };
      Entity* last{ nullptr };
      for (int i{}; i < 4; i += 1) { last = &EM.new_entity(CMP_RENDER); }
      EM.destroy_entity(*last);
      t.expect("lleno y liberada la última que se creó: la nueva va al hueco 0"
             , slot_of(EM, &EM.new_entity(CMP_NOISE)), Slot{ 0 });
   }
   {
      EntityManager EM{ 4 };
      std::vector<Entity*> e{};
      for (int i{}; i < 4; i += 1) { e.push_back(&EM.new_entity(CMP_RENDER)); }
      EM.destroy_entity(*e[2]);                 // slot 1
      EM.destroy_entity(*e[1]);                 // slot 2
      t.expect("libres los huecos 1 y 2: la nueva va al 2"
             , slot_of(EM, &EM.new_entity(CMP_NOISE)), Slot{ 2 });
   }
}
