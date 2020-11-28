#ifndef RAMROD_GUI_WINDOW_H
#define RAMROD_GUI_WINDOW_H

#define SDL_MAIN_HANDLED
#include <stdint.h>                    // for uint32_t
#include <string>                      // for string, allocator

#include "SDL_video.h"                 // for SDL_GLContext
#include "ramrod/gui/event_handler.h"  // for event_handler
#include "ramrod/gui/gui_manager.h"    // for gui_manager
#include "ramrod/gui/types.h"          // for position, size

namespace ramrod {
  namespace gui {
    class window :
        public gui::event_handler, public gui::gui_manager
    {
    public:
      explicit window(const int width = 1024, const int height = 512,
                      const std::string &title = "Untitled");
      ~window();
      /**
       * @brief Getting the battery status
       *
       * This will give you information about the battery status and how much
       * is left in seconds and percentage, or -1 if they cannot be determined
       * or is not running on battery.
       *
       * @return Structure with power state, seconds left and percentage left
       */
      gui::battery::status battery_status();
      /**
       * @brief Indicates if there are changes that should be repainted
       * @return `true` if there are changes in openGL and the screen should be repainted
       */
      bool changed();
      /**
       * @brief Closes this window
       */
      void close();
      /**
       * @brief Cheking if window is closing
       *
       * This function returns if the window is closing (which may not close the program
       * execution), if you are utilizing an infinite loop, this function will be very
       * useful to break that loop and also close the program.
       *
       * @return `true` is window is closed/closing
       */
      bool closing();
      /**
       * @brief Execute window
       *
       * This is the main function; it will open a new window with predefined properties
       * or properties defined by the user suchlike, width, height, title, etc. If
       * `infinite_loop = true` then, it will maintain the window open until a close
       * event is triggered.
       *
       * @param infinite_loop If set to `true` it will maintain the window open
       *                      until a close event is triggered.
       * @param sleep_time Time between frames in milliseconds and only works if
       *                   `infinite_time` is `true`.
       *
       * @return This will return the type of error if the window was not created
       *         properly: `EXIT_SUCCESS`, `GLFW_NOT_LOADED`, `WINDOW_NOT_LOADED`,
       *         `GLAD_NOT_LOADED` or `EXISTING_WINDOW`.
       */
      int execute(const bool infinite_loop = true, int sleep_time = 1000);
      /**
       * @brief Execute window with extra parameters
       *
       * **Width** and **height** represent the window resolution, if `full_screen` is `true`
       * then, your screen resolution will change, to have the native resolution of your system,
       * set **width** and **height** to *zero*, also, if **width** and **height** is equal to
       * *zero* and `full_screen` is `false` then, **width** and **height** will change to
       * initial values: 1500 and 800 respectively.
       *
       * @param width Window's width in pixels.
       * @param height Window's height in pixels.
       * @param title The UTF-8 encoded window's title.
       * @param full_screen If set to `true` it will make the window full screen.
       * @param maximized If set to `true` it will maximize the window, ignored if
       *                  `full_screen = true`.
       * @param infinite_loop If set to `true` it will maintain the window open until
       *                      close event.
       * @param sleep_time Time between frames in milliseconds and only works if
       *                   `infinite_time` is `true`.
       *
       * @return This will return the type of error if the window was not created
       *         properly: `EXIT_SUCCESS`, `GLFW_NOT_LOADED`, `WINDOW_NOT_LOADED`,
       *         `GLAD_NOT_LOADED` or `EXISTING_WINDOW`.
       */
      int execute(int width, int height, const std::string title = "Untitled",
                  const bool full_screen = false, const bool maximized = false,
                  const bool infinite_loop = true, int sleep_time = 1000);
      /**
       * @brief Making or exiting the full screen mode
       *
       * Makes the window full screen or resizes back the window to its defined width
       * and height. **This will hide the title and launcher bar**.
       *
       * @param make_full `true` to make window full screen
       */
      void full_screen(const bool make_full = true);
      /**
       * @brief Gets the time in milliseconds since the execution's starting
       * @return Number of milliseconds since start
       */
      std::uint32_t get_time();
      /**
       * @brief Getting the window's height
       * @return Window's height in pixels
       */
      int height();
      /**
       * @brief Maximizes or restores the window
       * @param maximize `true` to maximize, `false` to restore its size
       */
      void maximize(const bool maximize = true);
      /**
       * @brief Minimizes or restores the window
       * @param minimize `true` to minimize, `false` to restore its size
       */
      void minimize(const bool minimize = true);
      /**
       * @brief Getting the window's position
       * @return A 2D point structure with x and y coordinates
       */
      gui::position<int> position();
      /**
       * @brief Changing the window's position
       *
       * Changes the window's position in screen, The origin of the coordinate system
       * is at the top-left corner of the monitor.
       *
       * @param x Position from the left side of the monitor in pixels.
       * @param y Position from the top side of the monitor in pixels.
       */
      void position(const int x, const int y);
      /**
       * @brief Process all pending events
       *
       * Such events like key, mouse, window, etc.
       */
      void process_pending_events();
      /**
       * @brief Restarts the openGL viewport to its initial size (window's size)
       */
      void restart_viewport();
      /**
       * @brief Printing the latest SDL error
       */
      void sdl_error();
      /**
       * @brief Clears the openGL's color and depth buffers
       */
      void screen_clear();
      /**
       * @brief Getting the maximum anisotropic filtering
       *
       * This functions return the maximum anisotropic filtering than your computer
       * could handle, it is very useful when defining your own textures.
       *
       * @return Maximum anisotropic filtering value, is 8.0f by default for performance.
       */
      float screen_max_filtering();
      /**
       * @brief Changing the maximum anisotropic filtering
       * @param new_max_filtering_value New maximum anisotropic filtering value
       * @return The new max filtering value
       */
      float screen_max_filtering(const float new_max_filtering_value);
      /**
       * @brief Paint the screen without cleaning screen
       */
      void screen_paint();
      /**
       * @brief Clears, paints the screen and swaps the openGL's buffers
       */
      void screen_redraw();
      /**
       * @brief Forcing a openGL's screen repainting
       * @param change `true` to force a repaint or `false` to dismiss all changes
       *               if they have not been yet been painted
       */
      void screen_reload(const bool change = true);
      /**
       * @brief Swaps the front and back openGL's buffers
       */
      void screen_swap_buffers();
      /**
       * @brief Getting the window's size
       * @return A size structure with width and height
       */
      gui::size<int> size();
      /**
       * @brief Changing the window's size
       *
       * This function changes the size of the window, use negative values to equalize window
       * and monitor width or/and height. For full screen windows, the specified size becomes
       * the new resolution of the window. The window is resized to fit the resolution of the
       * set video mode.
       *
       * @param width Size in pixels of the window. Use any negative value to set the
       *              window's width equal to the monitor's resolution.
       * @param height Size in pixels of the window. Use any negative value to set the
       *               window's height equal to the monitor's resolution.
       */
      void size(const int width, const int height);
      /**
       * @brief Sleeping during specified milliseconds
       * @param milliseconds Number of milliseconds to sleep
       */
      void sleep(const uint32_t milliseconds);
      /**
       * @brief Getting the window's title
       * @return String containing the window's title
       */
      const std::string &title();
      /**
       * @brief Changing the window's title
       *
       * To change the window title (which appears at the bar in the top side of the window) use:
       *
       * @param title The UTF-8 encoded window title.
       */
      void title(const std::string &title);
      /**
       * @brief Checking window's visility
       *
       * This function returns the window's visibility; `true` if is not hidden or
       * `false` if is hidden.
       *
       * @return `true` if is not hidden or `false` if is hidden.
       */
      bool visibility();
      /**
       * @brief Changin the window's visibility
       * @param hide `true` to hide window, `false` to show window
       */
      void visibility(const bool hide);
      /**
       * @brief Getting the window's width
       * @return Window's width in pixels
       */
      int width();
      /**
       * @brief Getting the path of the window's icon
       * @return Absolute path of the window's icon
       */
      const std::string window_icon();
      /**
       * @brief Setting the path of the window's icon
       * @param icon_path Relative or absolute path towards the window's icon
       * @return `false` if file was not found
       */
      bool window_icon(const std::string &icon_path);
      /**
       * @brief Getting this window's ID
       * @return Id numeric value
       */
      std::uint32_t window_id();

    protected:
      /**
       * @brief OpenGL clearing the screen
       *
       * This function is triggered in each frame if you are using
       * `execute(..., const bool infinite_loop = true, ...)` to clear
       * the openGL screen.
       */
      virtual void clear();
      /**
       * @brief Initializes OpenGL capabilities
       *
       * This function will be triggered after the window creation
       */
      virtual void initialize();
      /**
       * @brief OpenGL painting of the screen
       *
       * This function is triggered in each frame if you are using
       * `execute(..., const bool infinite_loop = true, ...)` to paint
       * the openGL screen.
       */
      virtual void paint();
      /**
       * @brief Setting the SDL attributes
       *
       * This function is called when this object is constructed, before creating the window,
       * if you want to change the default SDL properties, you could overload this function.
       */
      virtual void sdl_attribute_setting();
      /**
       * @brief Update function defined to be overriden
       *
       * This function will be called after the screen paint in every frame, you
       * could override it to make data updates in each frame.
       */
      virtual void update();

    private:
      void main_initialize();

      bool load_icon();

      SDL_Window *window_;
      std::uint32_t window_id_;
      SDL_GLContext context_;
      std::string title_;
      std::string icon_path_;

      float max_filtering_;
      float diagonal_dpi_, horizontal_dpi_, vertical_dpi_;

      bool error_;
      int error_log_;
    };
  } // namespace: gui
} // namespace: ramrod

#endif // RAMROD_GUI_WINDOW_H
