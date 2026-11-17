#pragma once
// A box aligned with the axes (AABB): what collides in the arena.
// Two boxes overlap if they overlap in x AND in y: the same test as the Game Boy.
#include <utils/vec2.hpp>

struct Box {
   constexpr Box(Vec2 min, Vec2 max) : min_{ min }, max_{ max } {}

   [[nodiscard]] constexpr Vec2 min() const { return min_; }    // top left corner
   [[nodiscard]] constexpr Vec2 max() const { return max_; }    // bottom right corner
   [[nodiscard]] constexpr Vec2 size() const { return max_ - min_; }

   [[nodiscard]] constexpr bool overlaps(Box const& b) const {
      return   min_.x() < b.max_.x() && b.min_.x() < max_.x()
            && min_.y() < b.max_.y() && b.min_.y() < max_.y();
   }
   // All of this box is inside b
   [[nodiscard]] constexpr bool inside(Box const& b) const {
      return   min_.x() >= b.min_.x() && max_.x() <= b.max_.x()
            && min_.y() >= b.min_.y() && max_.y() <= b.max_.y();
   }

private:
   Vec2 min_{};
   Vec2 max_{};
};
