#pragma once
// A 2D vector of floats: positions and velocities in the arena, in pixels.
// Only the render system includes raylib.h: this is our own type (render converts it
// to Vector2).
#include <cmath>

struct Vec2 {
   constexpr Vec2() = default;
   constexpr Vec2(float x, float y) : x_{ x }, y_{ y } {}

   [[nodiscard]] constexpr float x() const { return x_; }
   [[nodiscard]] constexpr float y() const { return y_; }

   [[nodiscard]] constexpr Vec2 operator+(Vec2 const& r) const {
      return { x_ + r.x_, y_ + r.y_ };
   }
   [[nodiscard]] constexpr Vec2 operator-() const {
      return { -x_, -y_ };
   }
   [[nodiscard]] constexpr Vec2 operator-(Vec2 const& r) const {
      return *this + (-r);
   }
   [[nodiscard]] constexpr Vec2 operator*(float k) const {
      return { x_ * k, y_ * k };
   }
   constexpr Vec2& operator+=(Vec2 const& r) {
      x_ += r.x_;
      y_ += r.y_;
      return *this;
   }

   [[nodiscard]] constexpr float dot(Vec2 const& r) const {
      return x_ * r.x_ + y_ * r.y_;
   }
   // Its length squared: to compare distances without a square root
   [[nodiscard]] constexpr float length2() const {
      return dot(*this);
   }
   [[nodiscard]] float length() const {
      return std::sqrt(length2());
   }

   // The same direction, with length 1 (or {0, 0} if it has no length)
   [[nodiscard]] Vec2 normalized() const {
      float len{ length() };
      return len > 0.0f ? *this * (1.0f / len) : Vec2{};
   }

private:
   float x_{};
   float y_{};
};
