#pragma once
// For the tests of exercise 3: a guard that does something, waits for something and
// reacts in R ticks; and what schedule leaves in it.
#include <sys/ai.hpp>
#include <ejercicios/pruebas/prueba.hpp>

inline GuardComponent guard(Action doing, Action waiting, int react_in, int r = 12) {
   GuardComponent g{};
   g.action       = doing;
   g.pending      = waiting;
   g.react_in     = react_in;
   g.traits.react = r;
   return g;
}

// It decides `decided`; what does it do, what does it wait for and in how many ticks?
inline void expect(Prueba& t, std::string_view what, GuardComponent g, Action decided
                 , Action doing, Action waiting, int react_in) {
   schedule(g, decided);
   t.expect(std::format("{}: hace", what), g.action, doing);
   if (react_in > 0) { t.expect(std::format("{}: espera", what), g.pending, waiting); }
   t.expect(std::format("{}: react_in", what), g.react_in, react_in);
}
