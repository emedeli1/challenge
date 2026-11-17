#pragma once
// Which components an entity has: a type of its own instead of a raw integer.
//
// In Challenge 7, cmp was a std::uint8_t and every system asked with an AND by hand:
// (e.cmp & mask) == mask. It worked, but the code said HOW (the bits), not WHAT
// ("does it have a collider?"). CmpMask keeps the same bits inside, private, and
// gives each question a name. It costs nothing: sizeof(CmpMask) == sizeof(std::uint16_t).
//
// Its constructor is explicit: a plain number is not a mask by accident
// (CmpMask m = 3; does not compile). The masks are the constants CMP_* and TAG_*.
#include <cstdint>

struct CmpMask {
   constexpr CmpMask() = default;
   constexpr explicit CmpMask(std::uint16_t bits) : bits_{ bits } {}

   // No component at all: a free slot
   [[nodiscard]] constexpr bool empty() const { return bits_ == 0; }
   // It has ALL the components of m
   [[nodiscard]] constexpr bool has(CmpMask m) const {
      return (bits_ & m.bits_) == m.bits_;
   }
   // It has AT LEAST ONE of the components of m
   [[nodiscard]] constexpr bool has_any(CmpMask m) const {
      return (bits_ & m.bits_) != 0;
   }

   constexpr void add(CmpMask m) {
      bits_ = static_cast<std::uint16_t>(bits_ | m.bits_);
   }
   constexpr void remove(CmpMask m) {
      bits_ = static_cast<std::uint16_t>(bits_ & ~m.bits_);
   }

   // CMP_RENDER | CMP_PHYSICS: both
   [[nodiscard]] constexpr CmpMask operator|(CmpMask m) const {
      return CmpMask{ static_cast<std::uint16_t>(bits_ | m.bits_) };
   }
   [[nodiscard]] constexpr bool operator==(CmpMask const&) const = default;

private:
   std::uint16_t bits_{};
};

// Components: each bit has its data in the Entity (cmp/entity.hpp)
inline constexpr CmpMask CMP_RENDER   { 1 << 0 };   // it is drawn             (ren)
inline constexpr CmpMask CMP_PHYSICS  { 1 << 1 };   // it moves                (phy.vel)
inline constexpr CmpMask CMP_COLLIDER { 1 << 2 };   // it collides             (col)
inline constexpr CmpMask CMP_PATH     { 1 << 3 };   // it follows a route      (path)
inline constexpr CmpMask CMP_GUARD    { 1 << 4 };   // a guard: it decides     (guard)
inline constexpr CmpMask CMP_INTRUDER { 1 << 5 };   // the intruder            (intr)
inline constexpr CmpMask CMP_NOISE    { 1 << 6 };   // a noise that is heard   (noise)
inline constexpr CmpMask CMP_PROJECTILE{ 1 << 7 };  // it flies and falls      (proj)

// Tags: components with no data. The bit alone says what the entity is
inline constexpr CmpMask TAG_BELL     { 1 << 8 };   // the alarm bell
inline constexpr CmpMask TAG_LOOT     { 1 << 9 };   // what the intruder comes to steal
inline constexpr CmpMask TAG_EXIT     { 1 << 10 };  // the way out
