// Infiltration: the game loop with a fixed step.
//
//   ./arena [seed]                the game (default seed: 2026)
//   ./arena --trace TICKS [seed]  no window: simulates TICKS ticks and prints a summary.
//                                 Run it twice with the same seed: you get the same.
#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <string_view>
#include <raylib.h>
#include <scn/scene_game.hpp>
#include <utils/tracing.hpp>

//-----------------------------------------------------------------------------
// The keys to experiment: they change the selected guard (never during a system)
//-----------------------------------------------------------------------------
void handle_keys(SceneGame& game, Ui& ui) {
   for (int k{}; k < game.guards(); k += 1) {
      if (IsKeyPressed(KEY_ONE + k)) { ui.selected = k; }
   }
   if (IsKeyPressed(KEY_P))     { game.next_profile(ui.selected); }

   // - and + (also on the numeric keypad): it thinks slower or faster
   bool const slower{ IsKeyPressed(KEY_MINUS) || IsKeyPressed(KEY_KP_SUBTRACT) };
   bool const faster{ IsKeyPressed(KEY_EQUAL) || IsKeyPressed(KEY_KP_ADD) };
   if (slower)                  { game.think_slower(ui.selected); }
   if (faster)                  { game.think_faster(ui.selected); }

   if (IsKeyPressed(KEY_I))     { game.autopilot(); }
   if (IsKeyPressed(KEY_SPACE)) { ui.paused = !ui.paused; }
   if (IsKeyPressed(KEY_N))     { ui.step_once = true; }
   if (IsKeyPressed(KEY_F))     { ui.folded = !ui.folded; }
   if (IsKeyPressed(KEY_R))     { game = SceneGame{ game.seed() + 1, game.varied() }; }
   if (IsKeyPressed(KEY_V))     { game = SceneGame{ game.seed(), !game.varied() }; }
   if (IsKeyPressed(KEY_F11))   { ToggleBorderlessWindowed(); }
   if (IsKeyPressed(KEY_F12))   { TakeScreenshot("infiltracion.png"); }
}

//-----------------------------------------------------------------------------
// What the intruder does this tick: arrows or WASD to walk, SHIFT to run, T to
// throw a stone
//-----------------------------------------------------------------------------
Input read_input() {
   auto axis = [](int minus_a, int minus_b, int plus_a, int plus_b) {
      float v{};
      if (IsKeyDown(minus_a) || IsKeyDown(minus_b)) { v -= 1.0f; }
      if (IsKeyDown(plus_a)  || IsKeyDown(plus_b))  { v += 1.0f; }
      return v;
   };
   return {
        .dir   = { axis(KEY_LEFT, KEY_A, KEY_RIGHT, KEY_D)
                 , axis(KEY_UP, KEY_W, KEY_DOWN, KEY_S) }
      , .run   = IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)
      , .stone = IsKeyPressed(KEY_T)
   };
}

//-----------------------------------------------------------------------------
// The window: 1600 x 900, or smaller if the monitor is smaller. It can be resized
//-----------------------------------------------------------------------------
void open_window() {
   SetConfigFlags(FLAG_WINDOW_RESIZABLE);
   InitWindow(1600, 900, "V26 Challenge 8 - Infiltración");
   SetWindowMinSize(SCREEN_W / 4, SCREEN_H / 4);

   int const monitor{ GetCurrentMonitor() };
   int const max_w{ GetMonitorWidth(monitor) * 9 / 10 };
   int const max_h{ GetMonitorHeight(monitor) * 9 / 10 };
   if (GetScreenWidth() > max_w || GetScreenHeight() > max_h) {
      int const w{ std::min(max_w, max_h * 16 / 9) };
      SetWindowSize(w, w * 9 / 16);
   }
}

//-----------------------------------------------------------------------------
// The canvas, scaled to fit the window (black bands if its shape is different)
//-----------------------------------------------------------------------------
void draw_scaled(RenderTexture2D const& canvas) {
   auto const  sw{ static_cast<float>(GetScreenWidth()) };
   auto const  sh{ static_cast<float>(GetScreenHeight()) };
   float const w{ static_cast<float>(SCREEN_W) };
   float const h{ static_cast<float>(SCREEN_H) };
   float const k{ std::min(sw / w, sh / h) };

   Rectangle const source{ 0, 0, w, -h };     // render textures are upside down
   Rectangle const target{ (sw - w * k) / 2, (sh - h * k) / 2, w * k, h * k };
   DrawTexturePro(canvas.texture, source, target, { 0, 0 }, 0, WHITE);
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void run_game(std::uint32_t const seed) {
   open_window();
   SetTargetFPS(60);  // one frame = one tick of simulation

   // The game draws on the canvas, always at SCREEN_W x SCREEN_H
   RenderTexture2D canvas{ LoadRenderTexture(SCREEN_W, SCREEN_H) };
   SetTextureFilter(canvas.texture, TEXTURE_FILTER_BILINEAR);

   SceneGame game{ seed };
   Ui        ui{};
   while (!WindowShouldClose()) {
      handle_keys(game, ui);
      Input const in{ read_input() };

      // The logic advances in whole ticks: never GetFrameTime()
      if (!ui.paused || ui.step_once) {
         game.update(in);
         ui.step_once = false;
      }

      // Render: on the canvas, and the canvas on the window
      BeginTextureMode(canvas);
      game.render(ui);
      EndTextureMode();

      BeginDrawing();
      ClearBackground(BLACK);
      draw_scaled(canvas);
      EndDrawing();
   }
   UnloadRenderTexture(canvas);
   CloseWindow();
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
int main(int argc, char** argv) {
   // Trace a game with a given seed
   // To check it ends up in the same state (fingerprint)
   if (argc >= 3 && std::string_view{ argv[1] } == "--trace") {
      return trace(std::atoi(argv[2]), seed_arg(argc, argv, 3));
   }

   // Run a new game
   run_game( seed_arg(argc, argv, 1) );
   return 0;
}
