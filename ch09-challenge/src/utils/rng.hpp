#pragma once
// Pseudo-random numbers with xorshift, like the one of the Game Boy (Challenge 5),
// but with 32 bits.
// The same seed always gives the same numbers: the same seed, the same match.
// That is why the game never uses rand(), std::random_device or <random>.
#include <cstdint>

struct Xorshift32 {
   std::uint32_t state{ 1 };        // never 0: from 0 it would only give zeros

   std::uint32_t next() {
      state ^= state << 13;
      state ^= state >> 17;
      state ^= state << 5;
      return state;
   }

   // A number from 0 to n - 1
   std::uint32_t below(std::uint32_t n) { return next() % n; }
};

