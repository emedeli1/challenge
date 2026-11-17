#pragma once
// The entity manager: it owns the entities and lends them to the systems. Every slot
// exists from the start: an Entity object lives as long as the manager. A slot is in
// use from new_entity to destroy_entity, and then it is free for another entity.
// The same ideas as man_entity of the Game Boy: new_entity = man_entity_alloc,
// destroy_entity = man_entity_free, for_each_matching = man_entity_for_each_matching.
#include <cstddef>
#include <vector>
#include <cmp/entity.hpp>

inline constexpr std::size_t MAX_ENTITIES{ 64 };

struct EntityManager {
   // All the slots, free. The vector never grows (man/man_entity.cpp)
   [[nodiscard]] explicit EntityManager(std::size_t max_entities);

   // A free slot with these components. There is always one: MAX_ENTITIES is enough
   // for the game (static_assert in scene_game.cpp). If not, the program stops
   // (man/man_entity_alloc.cpp, exercise 10)
   [[nodiscard]] Entity& new_entity(CmpMask cmp);
   // Its slot is free again: the next new_entity can reuse it for something else
   void destroy_entity(Entity& e);

   // Every entity in use, always in the same order. It walks with an index (not with
   // iterators) because a system may create entities while it walks
   void for_each(auto&& process) {
      for (std::size_t i{}; i < entities_.size(); i += 1) {
         if (!entities_[i].cmp.empty()) { process(entities_[i]); }
      }
   }
   void for_each(auto&& process) const {
      for (std::size_t i{}; i < entities_.size(); i += 1) {
         if (!entities_[i].cmp.empty()) { process(entities_[i]); }
      }
   }
   // Only the entities that have ALL the components of mask
   void for_each_matching(CmpMask mask, auto&& process) {
      for_each([&](Entity& e) {
         if (e.cmp.has(mask)) { process(e); }
      });
   }
   void for_each_matching(CmpMask mask, auto&& process) const {
      for_each([&](Entity const& e) {
         if (e.cmp.has(mask)) { process(e); }
      });
   }
   // The first entity in use with ALL the components of mask, or nullptr. Two
   // versions: a const EntityManager only gives Entity const*
   // (man/man_entity_first_matching.cpp)
   [[nodiscard]] Entity*       first_matching(CmpMask mask);
   [[nodiscard]] Entity const* first_matching(CmpMask mask) const;

   [[nodiscard]] std::vector<Entity> const& entities() const { return entities_; }

private:
   std::vector<Entity> entities_{};
};
