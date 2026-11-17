#pragma once
// Tracing a game with no window: what --trace prints, and the fingerprint of the state.
// The intruder walks its route on autopilot, so the same seed always gives the same game.
#include <bit>
#include <print>
#include <cstdint>
#include <cstdlib>
#include <utility>
#include <scn/scene_game.hpp>

namespace {

// The number in argv[i], or 2026 if there is none
std::uint32_t seed_arg(int argc, char** argv, int i) {
   if (argc <= i) { return 2026u; }
   return static_cast<std::uint32_t>(std::strtoul(argv[i], nullptr, 10));
}

// A "fingerprint" of the state: the same state, the same number.
// It mixes the data of the guards and the intruder like FNV-1a, but 4 bytes at a time
std::uint32_t fingerprint(SceneGame const& game) {
   std::uint32_t h{ 2166136261u };
   auto mix = [&h](std::uint32_t v) { h = (h ^ v) * 16777619u; };
   auto mix_pos = [&mix](Vec2 p) {
      mix(std::bit_cast<std::uint32_t>(p.x()));
      mix(std::bit_cast<std::uint32_t>(p.y()));
   };
   game.entities().for_each_matching(CMP_GUARD, [&](Entity const& e) {
      mix_pos(e.phy.pos);
      mix(std::to_underlying(e.guard.action));
      mix(e.guard.has_clue ? 1u : 0u);
   });
   if (Entity const* in{ game.intruder() }) {
      mix_pos(in->phy.pos);
      mix(in->intr.has_loot ? 1u : 0u);
      mix(in->intr.caught ? 2u : in->intr.escaped ? 3u : 0u);
   }
   mix(static_cast<std::uint32_t>(game.alarm()));
   return h;
}

int trace(int ticks, std::uint32_t seed) {
   SceneGame game{ seed };
   for (int t{ 1 }; t <= ticks; t += 1) {
      game.update(Input{});
      if (t % 60 != 0) { continue; }
      std::print("tick {:5}", game.tick());
      game.entities().for_each_matching(CMP_GUARD, [](Entity const& e) {
         Action const a{ e.guard.action };
         std::print("  | guardia {} {:13}", e.guard.id + 1
                   , ACTION_NAME[std::to_underlying(a)]);
      });
      if (Entity const* in{ game.intruder() }) {
         IntruderComponent const& i{ in->intr };
         char const* state{ i.caught ? "atrapado" : i.escaped ? "escapado"
                          : i.has_loot ? "con el botin" : "" };
         std::print("  | intruso {}", state);
      }
      std::println("");
   }
   std::println("huella del estado: {:08x}", fingerprint(game));
   return 0;
}

} // namespace
