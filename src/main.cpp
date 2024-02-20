// Dear ImGui: standalone example application for SDL2 + OpenGL
// (SDL is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan/Metal graphics context creation, etc.)

// Learn about Dear ImGui:
// - FAQ                  https://dearimgui.com/faq
// - Getting Started      https://dearimgui.com/getting-started
// - Documentation        https://dearimgui.com/docs (same as your local docs/ folder).
// - Introduction, links and more at the top of imgui.cpp

#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#include <SDL.h>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL_opengles2.h>
#else
#include <SDL_opengl.h>
#endif

#include "internal/dockspace.h"
#include "archive/wad/wad.h"
#include "archive/wad/thing.h"
#include "domain/complevel.h"
#include <iostream>

#include "archive/wad/vertex.h"
#include "internal/diff/dmp_diff.hpp"

// This example can also compile and run with Emscripten! See 'Makefile.emscripten' for details.
#ifdef __EMSCRIPTEN__
#include "../libs/emscripten/emscripten_mainloop_stub.h"
#endif

using namespace std;
using namespace ade_internal;

// Main code
int main(int, char**)
{
  // Setup SDL
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
  {
    printf("Error: %s\n", SDL_GetError());
    return -1;
  }

  // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
  // GL ES 2.0 + GLSL 100
  const char* glsl_version = "#version 100";
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#elif defined(__APPLE__)
  // GL 3.2 Core + GLSL 150
  const char* glsl_version = "#version 150";
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG); // Always required on Mac
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
#else
  // GL 3.0 + GLSL 130
  const char* glsl_version = "#version 130";
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#endif

  // From 2.0.18: Enable native IME.
#ifdef SDL_HINT_IME_SHOW_UI
  SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");
#endif

  // Create window with graphics context
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
  SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
  SDL_Window* window = SDL_CreateWindow("ADE - Abstract Doom Editor", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, window_flags);
  SDL_GLContext gl_context = SDL_GL_CreateContext(window);
  SDL_GL_MakeCurrent(window, gl_context);
  SDL_GL_SetSwapInterval(0); // Enable vsync

  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
  io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
  //io.ConfigViewportsNoAutoMerge = true;
  //io.ConfigViewportsNoTaskBarIcon = true;

  // Setup Dear ImGui style
  //ImGui::StyleColorsDark();
  ImGui::StyleColorsLight();

  // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
  ImGuiStyle& style = ImGui::GetStyle();
  if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
  {
    style.WindowRounding = 0.0f;
    style.Colors[ImGuiCol_WindowBg].w = 1.0f;
  }

  // Setup Platform/Renderer backends
  ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
  ImGui_ImplOpenGL3_Init(glsl_version);

  // Load Fonts
  // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
  // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
  // - If the file cannot be loaded, the function will return a nullptr. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
  // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
  // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
  // - Read 'docs/FONTS.md' for more instructions and details.
  // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
  // - Our Emscripten build process allows embedding fonts to be accessible at runtime from the "fonts/" folder. See Makefile.emscripten for details.
  //io.Fonts->AddFontDefault();
  io.Fonts->AddFontFromFileTTF("../assets/fonts/Roboto-Regular.ttf", 18.0f);
  //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
  //IM_ASSERT(font != nullptr);

  // Our state
  bool show_demo_window = true;
  ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

  // Preparing work

  ade::Vertex a{ 1, 0 }, b{ 0, 1 }, c{ 2, 2 }, d{ 3, 4 };
  vector<ade::Vertex> prev{ a, b, c }, curr{a, d};
  auto diffs = MyersDiff<vector<ade::Vertex>>(prev, curr);
  for (auto diff : diffs)
  {
    cout << (int)diff.operation << endl;
    cout << (int)diff.text.from->x << endl;
    cout << (int)diff.text.from->y << endl;
  }
  cout << endl;


  ade::Wad wad;
  bool success = wad.open("../assets/wads/Haste.wad");
  if (!success) cout << "Wad not found\n";

  auto range = wad.get_map_range(1);
  auto table = wad.get_map_table<ade::Thing>(range, "THING");

  cout << "\nTotal: " << table.size() << "\n\n";

  for (int i = 0; i < table.size() && i < 100; i++)
  {
    auto row = table[i];
    cout << row.x << ", " << row.y << " | " << row.angle << "\n";
  }

  auto image = wad.get_image("TALLGRS4");
  cout << "\nTALLGRS4 - " << image.get_w() << ", " << image.get_h() << "\n";
  cout << image.get_spans(19).size() << "\n";

  // Main loop
  bool done = false;
#ifdef __EMSCRIPTEN__
  // For an Emscripten build we are disabling file-system access, so let's not attempt to do a fopen() of the imgui.ini file.
  // You may manually call LoadIniSettingsFromMemory() to load settings from your own storage.
  io.IniFilename = nullptr;
  EMSCRIPTEN_MAINLOOP_BEGIN
#else
  while (!done)
#endif
  {
    // Poll and handle events (inputs, window resize, etc.)
    // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
    // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
    // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
    // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
      ImGui_ImplSDL2_ProcessEvent(&event);
      if (event.type == SDL_QUIT)
          done = true;
      if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
          done = true;
    }

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    static bool enable_docking = false;
    static bool dockspace_open = true;
    static bool show_win_names = false;
    show_main_dockspace(&dockspace_open);

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_AlwaysAutoResize;

    if (!enable_docking) window_flags |= ImGuiWindowFlags_NoTitleBar
                                      |  ImGuiWindowFlags_NoMove
                                      |  ImGuiWindowFlags_NoNavFocus
                                      |  ImGuiWindowFlags_NoCollapse;

    ImGuiWindowClass window_class;
    if (!enable_docking) // Hiding buttons on tab bar
    {
      window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar;
    }

    // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
    if (show_demo_window)
    {
      ImGui::SetNextWindowClass(&window_class);
      ImGui::ShowDemoWindow(&show_demo_window);
    }

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);


    // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
    {
      static float f = 0.0f;
      static int counter = 0;

      ImGui::SetNextWindowClass(&window_class);
      ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

      ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
      ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
      ImGui::Checkbox("Enable docking", &enable_docking);     // Enable panels rearrange
      ImGui::Checkbox("Show panels names", &show_win_names);  // Show panels names

      ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
      ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

      if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
          counter++;
      ImGui::SameLine();
      ImGui::Text("counter = %d", counter);

      ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
      ImGui::End();
    }

        

    // 4. Main panel
    {
      ImGui::SetNextWindowClass(&window_class);
      ImGui::Begin("Main", NULL, window_flags);
      if (show_win_names) ImGui::Text("Main");

      auto sizes = io.DisplaySize;
      ImGui::Text("Sizes = %f %f", sizes.x, sizes.y);

      ImTextureID my_tex_id = io.Fonts->TexID;
      float my_tex_w = (float)image.get_w();
      float my_tex_h = (float)image.get_h();
      ImVec2 uv_min = ImVec2(0.0f, 0.0f);
      ImVec2 uv_max = ImVec2(1.0f, 1.0f);
      ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // No tint
      ImVec4 border_col = ImGui::GetStyleColorVec4(ImGuiCol_Border);
      ImGui::Image(my_tex_id, ImVec2(my_tex_w, my_tex_h), uv_min, uv_max, tint_col, border_col);

      ImGui::SetWindowSize(sizes);
      ImGui::SetWindowPos(ImVec2(.0f, .0f));

      ImGui::End();
    }

    // 5. Project panel
    {
      ImGui::SetNextWindowClass(&window_class);
      ImGui::Begin("Project", NULL, window_flags);
      if (show_win_names) ImGui::Text("Project");
      ImGui::End();
    }

    // 6. Props panel
    {
      ImGui::SetNextWindowClass(&window_class);
      ImGui::Begin("Props", NULL, window_flags);
      if (show_win_names) ImGui::Text("Props");
      ImGui::End();
    }

    // 7. Tools panel
    {
      ImGui::SetNextWindowClass(&window_class);
      ImGui::Begin("Tools", NULL, window_flags);
      if (show_win_names) ImGui::Text("Tools");
      ImGui::End();
    }

    // 8. Info panel
    {
      ImGui::SetNextWindowClass(&window_class);
      ImGui::Begin("Info", NULL, window_flags);
      if (show_win_names) ImGui::Text("Info");
      ImGui::End();
    }

    ImGui::PopStyleVar(2);

    // Rendering
    ImGui::Render();
    glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // Update and Render additional Platform Windows
    // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
    //  For this specific demo app we could also call SDL_GL_MakeCurrent(window, gl_context) directly)
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
      SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
      SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
      ImGui::UpdatePlatformWindows();
      ImGui::RenderPlatformWindowsDefault();
      SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
    }

    SDL_GL_SwapWindow(window);
  }
#ifdef __EMSCRIPTEN__
  EMSCRIPTEN_MAINLOOP_END;
#endif

  // Cleanup
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();

  SDL_GL_DeleteContext(gl_context);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
