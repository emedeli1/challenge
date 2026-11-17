#pragma once
// What every test of the exercises uses: how a test says what it checks, and a way to
// look inside the scene (Probe). Do not change these files: they are the tests.
//
// Every case has two tests:
//   P1  what usually happens
//   P2  the limits: the smallest, the largest, the equal, none at all...
#include <cmath>
#include <format>
#include <optional>
#include <print>
#include <string>
#include <string_view>
#include <utility>
#include <ai/decision.hpp>
#include <scn/scene_game.hpp>

// The inside of the scene, for the tests: SceneGame is its friend
struct Probe {
   static EntityManager& EM(SceneGame& s)     { return s.EM_; }
   static int&           alarm(SceneGame& s)  { return s.alarm_; }
   static int&           guards(SceneGame& s) { return s.guards_; }
   static void projectiles(SceneGame& s)      { s.projectile_update(); }
   static void physics(SceneGame& s)          { s.physics_update(); }
   static void noises(SceneGame& s)           { s.noise_update(); }
};

// How a value is shown in a message
inline std::string show(bool b)        { return b ? "sí" : "no"; }
inline std::string show(int n)         { return std::to_string(n); }
inline std::string show(std::size_t n) { return std::to_string(n); }
inline std::string show(float f)       { return std::format("{:.2f}", f); }
inline std::string show(Action a) {
   return std::string{ ACTION_NAME[std::to_underlying(a)] };
}
inline std::string show(Vec2 v) {
   return std::format("({:.2f}, {:.2f})", v.x(), v.y());
}
inline std::string show(std::string const& s) { return s.empty() ? "ninguno" : s; }
inline std::string show(std::optional<std::size_t> n) {
   return n ? std::to_string(*n) : "nada (nullopt)";
}

// Which entity a pointer points to: its slot in the EntityManager
struct Slot {
   int i;                                   // -1: nullptr
   bool operator==(Slot const&) const = default;
};
inline Slot slot_of(EntityManager const& EM, Entity const* e) {
   if (!e) { return { -1 }; }
   return { static_cast<int>(e - EM.entities().data()) };
}
inline std::string show(Slot s) {
   return s.i < 0 ? "nullptr" : std::format("la entidad del hueco {}", s.i);
}

inline bool near(float a, float b) { return std::abs(a - b) < 0.001f; }
inline bool near(Vec2 a, Vec2 b)   { return near(a.x(), b.x()) && near(a.y(), b.y()); }

// The tests of one case. It prints each check and, at the end, P1 and P2
struct Prueba {
   explicit Prueba(std::string_view title) { std::println("\n== {}", title); }
   Prueba(Prueba const&) = delete;
   // A test that did not get to run (the program stopped before) is not passed
   ~Prueba() {
      for (int i{}; i < 2; i += 1) {
         if (!ran_[i]) { std::println("P{} no ha llegado a ejecutarse", i + 1); }
      }
      auto mark = [this](int i) { return ok_[i] && ran_[i] ? "✅" : "❌"; };
      std::println("Resultado: P1 {}  P2 {}", mark(0), mark(1));
   }

   // From now on, the checks are of P1 or of P2
   void p(int n, std::string_view what) {
      p_       = n - 1;
      ran_[p_] = true;
      std::println("P{} ({})", n, what);
   }
   // The check: what your code gives must be what is expected
   template <typename T>
   void expect(std::string_view what, T const& got, T const& want) {
      check(what, got == want, show(want), show(got));
   }
   // The same, for numbers with decimals (at most 0.001 apart)
   void expect_near(std::string_view what, float got, float want) {
      check(what, near(got, want), show(want), show(got));
   }
   void expect_near(std::string_view what, Vec2 got, Vec2 want) {
      check(what, near(got, want), show(want), show(got));
   }

private:
   void check(std::string_view what, bool ok, std::string const& want
            , std::string const& got) {
      if (ok) {
         std::println("   ✅ {}", what);
         return;
      }
      ok_[p_] = false;
      std::println("   ❌ {}: se espera {}, tu código da {}", what, want, got);
   }

   bool ok_[2]{ true, true };
   bool ran_[2]{ false, false };
   int  p_{ 0 };
};
