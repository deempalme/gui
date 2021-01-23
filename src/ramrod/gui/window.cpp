#include "ramrod/gui/window.h"

#include <cstdlib>

#include <cstdlib>                                  // for exit, EXIT_SUCCESS
#include "SDL.h"                                    // for SDL_Init, SDL_Quit
#include "SDL_endian.h"                             // for SDL_BIG_ENDIAN
#include "SDL_error.h"                              // for SDL_GetError
#include "SDL_main.h"                               // for SDL_SetMainReady
#include "SDL_mouse.h"                              // for SDL_GetGlobalMous...
#include "SDL_rect.h"                               // for SDL_Rect
#include "SDL_stdinc.h"                             // for SDL_TRUE
#include "SDL_surface.h"                            // for SDL_FreeSurface
#include "SDL_timer.h"                              // for SDL_Delay, SDL_Ge...
#include "glad/glad.h"                              // for glDisable, gladLo...
#include "ramrod/console.h"
#include "ramrod/gl/texture.h"                      // for texture
#include "ramrod/gui/enumerators.h"                 // for GLAD_not_loaded
#include "ramrod/gui/file_manager.h"                // for file_manager
#include "ramrod/gui/image_loader.h"                // for image_loader

namespace ramrod {
  namespace gui {
    window::window(const int width, const int height, const std::string &title) :
      event_handler(this),
      gui_manager(this),
      window_(nullptr),
      window_id_{0},
      context_(),
      title_(title),
      icon_path_(),
      max_filtering_{0.0f},
      diagonal_dpi_{0.0f},
      horizontal_dpi_{0.0f},
      vertical_dpi_{0.0f},
      updated_time_{0},
      initialized_{false},
      error_{false},
      error_log_{0}
    {
      // Avoiding main() redefinition
      SDL_SetMainReady();
      // SDL: initialize and configure
      // ------------------------------
      if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0){
        rr::error("SDL initialization failed");
        exit(ramrod::gui::error::SDL2_not_loaded);
      }

      sdl_attribute_setting();

      // SDL window creation
      // --------------------
      const int number_of_monitors{SDL_GetNumVideoDisplays()};
      SDL_DisplayMode monitor_mode;
      int mouse_x, mouse_y, i;
      SDL_GetGlobalMouseState(&mouse_x, &mouse_y);

      for(i = 0; i < number_of_monitors; ++i){
        if(SDL_GetDisplayBounds(i, &display_properties_) != 0) sdl_error();

        if(mouse_x > display_properties_.x
           && mouse_x < (display_properties_.x + display_properties_.w)
           && mouse_y > display_properties_.y
           && mouse_y < (display_properties_.y + display_properties_.h)){
          SDL_GetDesktopDisplayMode(i, &monitor_mode);
          break;
        }
      }
      window_ = SDL_CreateWindow(title_.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                 width, height, SDL_WINDOW_HIDDEN | SDL_WINDOW_OPENGL
                                 | SDL_WINDOW_RESIZABLE);

      initial_window_properties_.w = window_properties_.w = width;
      initial_window_properties_.h = window_properties_.h = height;

      SDL_GetDisplayDPI(i, &diagonal_dpi_, &horizontal_dpi_, &vertical_dpi_);

      if(!window_){
        rr::error("SDL failed creating a window");
        exit(ramrod::gui::error::window_not_loaded);
      }

      window_id_ = SDL_GetWindowID(window_);

      SDL_SetWindowDisplayMode(window_, &monitor_mode);
      SDL_SetWindowBordered(window_, SDL_TRUE);

      if(!(context_ = SDL_GL_CreateContext(window_))){
        rr::error("OpenGL context creation failed");
        exit(ramrod::gui::error::OpenGL_not_loaded);
      }

      SDL_GL_MakeCurrent(window_, context_);

      // glad: load all OpenGL function pointers
      // ---------------------------------------
      if(!gladLoadGLLoader(static_cast<GLADloadproc>(SDL_GL_GetProcAddress))){
        rr::error("Failed to initialize GLAD");
        exit(ramrod::gui::error::GLAD_not_loaded);
      }

      SDL_GL_SetSwapInterval(1);

      SDL_StopTextInput();
      main_initialize();
    }

    window::~window(){
      rr::regular("Destroying window");

      if(context_) SDL_GL_DeleteContext(context_);
      if(window_) SDL_DestroyWindow(window_);
      SDL_Quit();
    }

    battery::status window::battery_status(){
      battery::status status;
      status.status = static_cast<battery::power_state>(SDL_GetPowerInfo(&status.seconds_left,
                                                                         &status.percentage_left));
      return status;
    }

    bool window::changed(){
      if(hidden_) return false;
      return has_changed_;
    }

    void window::close(){
      closing_ = true;
    }

    bool window::closing(){
      return closing_;
    }

    int window::execute(const bool infinite_loop, std::uint32_t sleep_time){
      if(!error_){
        SDL_ShowWindow(window_);
        if(infinite_loop)
          while(!closing_){
            if(sleep_time == 0 && initialized_){
              wait_for_events();
            }else if(initialized_ && (get_time() - updated_time_) < sleep_time){
              sleep(5);
              continue;
            }
            // Gets new update's time
            updated_time_ = get_time();
            // Polls all events that happened between frames
            poll_events();

            // Checks if the windows is visible and there are at least a change
            if(!hidden_/* && has_changed_*/){
              has_changed_ = false;

              clear();
              paint();
              SDL_GL_SwapWindow(window_);
            }
            update();

            initialized_ = true;
          }
        return EXIT_SUCCESS;
      }else
        return error_log_;
    }

    int window::execute(int width, int height, const std::string &title,
                        const bool full_screen, const bool maximized,
                        const bool infinite_loop, std::uint32_t sleep_time){
      if(!error_){
        SDL_SetWindowTitle(window_, title.c_str());

        width = (width <= 0)? display_properties_.w : width;
        height = (height <= 0)? display_properties_.h : height;

        const int x = display_properties_.x + (display_properties_.w - width)/2;
        const int y = display_properties_.y + (display_properties_.h - height)/2;

        SDL_SetWindowSize(window_, width, height);
        SDL_SetWindowPosition(window_, x, y);

        if(full_screen){
          SDL_SetWindowFullscreen(window_, SDL_WINDOW_FULLSCREEN);
        }else if(maximized){
          SDL_MaximizeWindow(window_);
        }

        return execute(infinite_loop, sleep_time);
      }else
        return error_log_;
    }

    void window::full_screen(const bool make_full){
      if(make_full){
        SDL_SetWindowFullscreen(window_, SDL_WINDOW_FULLSCREEN);
      }else{
        SDL_SetWindowFullscreen(window_, 0);
        SDL_RestoreWindow(window_);
      }
    }

    std::uint32_t window::get_time(){
      return SDL_GetTicks();
    }

    int window::height(){
      return window_properties_.h;
    }

    void window::maximize(const bool maximize){
      if(maximize)
        SDL_MaximizeWindow(window_);
      else
        SDL_RestoreWindow(window_);
    }

    void window::minimize(const bool minimize){
      if(minimize)
        SDL_MinimizeWindow(window_);
      else
        SDL_RestoreWindow(window_);
    }

    gui::position<int> window::position(){
      return gui::position<int>{ window_properties_.x, window_properties_.y };
    }

    void window::position(const int x, const int y){
      SDL_SetWindowPosition(window_, x, y);
    }

    void window::process_pending_events(){
      poll_events();
    }

    void window::restart_viewport(){
      glViewport(0, 0, window_properties_.w, window_properties_.h);
      resize();
    }

    void window::sdl_error(){
      rr::error() << "SDL -> " << SDL_GetError() << rr::endl;
    }

    void window::screen_clear(){
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    float window::screen_max_filtering(){
      return max_filtering_;
    }

    float window::screen_max_filtering(const float new_max_filtering_value){
      float maximum;
      glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maximum);
      max_filtering_ = (new_max_filtering_value > maximum)? maximum : new_max_filtering_value;
      ramrod::gl::texture::set_max_filtering(max_filtering_);
      return max_filtering_;
    }

    void window::screen_paint(){
      paint();
      has_changed_ = false;
    }

    void window::screen_redraw(){
      clear();
      paint();
      SDL_GL_SwapWindow(window_);
    }

    void window::screen_reload(const bool change){
      has_changed_ = change;
    }

    void window::screen_swap_buffers(){
      SDL_GL_SwapWindow(window_);
    }

    gui::size<int> window::size(){
      return gui::size<int>{ window_properties_.w, window_properties_.h };
    }

    void window::size(const int width, const int height){
      SDL_SetWindowSize(window_, width, height);
      SDL_RestoreWindow(window_);

      has_changed_ = true;
    }

    void window::sleep(const std::uint32_t milliseconds){
      SDL_Delay(milliseconds);
    }

    const std::string &window::title(){
      return title_;
    }

    void window::title(const std::string &title){
      title_ = title;
      SDL_SetWindowTitle(window_, title.c_str());
    }

    bool window::visibility(){
      return !hidden_;
    }

    void window::visibility(const bool hide){
      if(hide)
        SDL_HideWindow(window_);
      else
        SDL_ShowWindow(window_);
    }

    int window::width(){
      return window_properties_.w;
    }

    const std::string window::window_icon(){
      return icon_path_;
    }

    bool window::window_icon(const std::string &icon_path){
      if(!ramrod::gui::file_manager::check_path(icon_path)) return false;
      icon_path_ = icon_path;
      load_icon();
      return true;
    }

    std::uint32_t window::window_id(){
      return window_id_;
    }

    // ::::::::::::::::::::::::::::::::::: PROTECTED FUNCTIONS ::::::::::::::::::::::::::::::::::::

    void window::clear(){
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void window::initialize(){
    }

    void window::paint(){
      gui::gui_manager::paint();
    }

    void window::sdl_attribute_setting(){
      SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
      SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
      SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
      SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
      SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
      SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 0);
      SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
      SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
      SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, SDL_TRUE);
      SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, SDL_TRUE);
      SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
      SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
    }

    void window::update(){}

    // :::::::::::::::::::::::::::::::::::: PRIVATE FUNCTIONS :::::::::::::::::::::::::::::::::::::

    void window::main_initialize(){
      // configure global opengl state
      // -----------------------------
      // setting the background color
      glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
      // this line allows anti-aliasing to create fine edges
      //    glEnable(GL_MULTISAMPLE);
      // this line allows z-buffer to avoid rear objects to appear in front
      glDisable(GL_DEPTH_TEST);
      // enable seamless cubemap sampling for lower mip levels in the pre-filter map.
      glDisable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
      glDisable(GL_PROGRAM_POINT_SIZE);
      // these allow alpha transparency in the rendering
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      // Avoiding the rendering of all back faces
      glFrontFace(GL_CCW);
      glCullFace(GL_BACK);
      glEnable(GL_CULL_FACE);

      // detects the maximum anisotropic filtering samples
      glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &max_filtering_);
      max_filtering_ = (max_filtering_ > 8.0f)? 8.0f : max_filtering_;
      ramrod::gl::texture::set_max_filtering(max_filtering_);

      glPixelStorei(GL_PACK_ALIGNMENT, 1);
      glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

      gui::gui_manager::main_initialize();

      initialize();
    }

    bool window::load_icon(){
      gui::image_loader new_icon(icon_path_, false, 4);

      if(!new_icon.data()){
        rr::error("Loading icon failed: " + new_icon.failure_reason());
        return false;
      }

      uint32_t red_mask{0x000000ff}, green_mask{0x0000ff00};
      uint32_t blue_mask{0x00ff0000}, alpha_mask{0xff000000};

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
      red_mask = 0xff000000;
      green_mask = 0x00ff0000;
      blue_mask = 0x0000ff00;
      alpha_mask = 0x000000ff;
#endif

      SDL_Surface *icon =
          SDL_CreateRGBSurfaceFrom((void*)new_icon.data(), new_icon.width(), new_icon.height(),
                                   32, new_icon.number_of_components() * new_icon.width(),
                                   red_mask, green_mask, blue_mask, alpha_mask);

      if(icon)
        SDL_SetWindowIcon(window_, icon);
      else{
        rr::error("Creating icon failed");
        SDL_FreeSurface(icon);
        return false;
      }

      SDL_FreeSurface(icon);
      return true;
    }
  } // namespace: gui
} // namespace: ramrod
