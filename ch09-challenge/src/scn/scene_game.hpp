#pragma once
// The game scene: the entities, the guards' decision tree, the seed and the systems.
// Each system is a member function of the scene, implemented in its own file of src/sys/
// (sys/ai.cpp implements ai_update, and so on), like the sys_*.asm files of the Game
// Boy. Most of them are compiled inside the library (lib/libarena.a). The library was
// compiled with this class: do not change its data members.
#include <cstdint>
#include <ai/dt.hpp>
#include <man/man_entity.hpp>
#include <utils/rng.hpp>

// The canvas: render always draws at this size, and main scales it to the window
inline constexpr int SCREEN_W{ 1920 };
inline constexpr int SCREEN_H{ 1080 };

// What the player does this tick (main reads it from the keyboard)
struct Input {
   Vec2 dir{};            // where the intruder walks ({0, 0}: it stays)
   bool run{};            // it runs: faster, but its steps are heard
   bool stone{};          // it throws a stone ahead: a noise far from it
};

// What the player is looking at. It is not part of the game: the systems never change it
struct Ui {
   int  selected{ 0 };    // the guard whose tree the HUD shows
   bool paused{ false };
   bool step_once{ false };
   bool folded{ false };  // the HUD shows the tree folded (key F)
};

struct SceneGame {
   // varied: every guard is its profile with a little variation from the seed
   [[nodiscard]] explicit SceneGame(std::uint32_t seed, bool varied = true);

   void update(Input const& in);           // one tick: every system, in the same order
   void render(Ui const& ui) const;        // render system (src/sys/render.cpp)

   // To experiment from the keyboard (they change the guard with that number)
   void next_profile(int guard_id);        // the next personality of PROFILES
   void think_slower(int guard_id);        // it thinks half as often
   void think_faster(int guard_id);        // it thinks twice as often
   void autopilot();                       // the intruder walks its route again

   [[nodiscard]] EntityManager const& entities() const { return EM_; }
   [[nodiscard]] DecisionTree const&  tree()     const { return tree_; }
   [[nodiscard]] std::uint32_t        tick()     const { return tick_; }
   [[nodiscard]] std::uint32_t        seed()     const { return seed_; }
   [[nodiscard]] int                  guards()   const { return guards_; }
   [[nodiscard]] int                  alarm()    const { return alarm_; }
   [[nodiscard]] bool                 varied()   const { return varied_; }
   [[nodiscard]] Entity const*        intruder() const;

   // What can be seen (src/sys/ai_sight.cpp). They only read: the HUD uses them too
   [[nodiscard]] bool line_of_sight(Vec2 from, Vec2 to) const;
   [[nodiscard]] bool solid_at(Box const& box) const;

   // The parts of the AI system, each one in its own file of src/sys/ (ai_*.cpp)
   void guard_tick(Entity& guard);                // one guard, one tick   (ex. 4)
   void think(Entity& guard);                     // perceive, decide, schedule
   void perceive(Entity& guard);                  // its memory and its Percept
   void act(Entity& guard);                       // what it does with its action
   void investigate(Entity& guard);               // Action::Investigate   (ex. 5)
   void raise_alarm(Entity& guard);               // Action::RaiseAlarm
   [[nodiscard]] bool sees(Entity const& guard, Vec2 target) const;      // (ex. 1)
   [[nodiscard]] Entity const* nearest_chaser(Entity const& guard) const; // (ex. 2)
   [[nodiscard]] Entity const* loudest_noise(Vec2 at) const;

   // New entities, each one in its own file of src/sys/ (new_*.cpp)
   void new_noise(Vec2 pos, float radius);        // a noise that the guards hear
   void new_stone(Vec2 from, Vec2 dir);           // a stone that flies and falls

   // The game does not use them
   void return_to_post(Entity& guard);
   void new_reinforcement();
   void new_dart(Entity const& guard, Vec2 target);   // a dart of a guard (ex. 6)

private:
   friend struct Probe;                    // the tests of the exercises look inside

   // The systems, in the order update() calls them. Each one in src/sys/
   void intruder_update(Input const& in);  // intruder.cpp    keys or autopilot
   void ai_update();                       // ai.cpp          every guard: guard_tick
   void path_update();                     // path.cpp        who has a route, follows it
   void projectile_update();               // projectile.cpp  stones fall: a noise
   void physics_update();                  // physics.cpp     everything that moves, moves
   void noise_update();                    // noise.cpp       noises fade away
   void rules_update();                    // rules.cpp       loot, bell, caught, escaped

   EntityManager EM_{ MAX_ENTITIES };
   DecisionTree  tree_{};                  // one tree for every guard: they share it
   Xorshift32    rng_{};
   std::uint32_t tick_{};
   std::uint32_t seed_{};
   int           guards_{};                // guards created
   int           alarm_{};                 // > 0: ticks the bell keeps ringing
   bool          varied_{};                // the guards vary from their profiles
};
