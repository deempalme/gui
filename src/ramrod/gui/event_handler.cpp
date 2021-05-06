#include "ramrod/gui/event_handler.h"

#include <SDL2/SDL_keyboard.h>                     // for SDL_Keysym
#include <SDL2/SDL_keycode.h>                      // for SDLK_DOWN, SDLK_LEFT
#include <SDL2/SDL_mouse.h>                        // for SDL_BUTTON_LEFT
#include <SDL2/SDL_video.h>                        // for SDL_WINDOWEVENT_CLOSE
#include "SDL_events.h"                            // for SDL_Event, SDL_Win...
#include "SDL_rect.h"                              // for SDL_Rect
#include "SDL_stdinc.h"                            // for SDL_free
#include "ramrod/console.h"                        // for formatted
#include "ramrod/console/attention.h"              // for attention, attenti...
#include "ramrod/console/types.h"                  // for attention
#include "ramrod/gui/enumerators.h"                // for ctrl, not_none
#include "ramrod/gui/window.h"                     // for window

namespace ramrod {
  namespace gui {
    event_handler::event_handler() :
      event_()
    {
    }

    void event_handler::poll_events(){
      while(SDL_PollEvent(&event_) > 0) analyze_event();
    }

    void event_handler::wait_for_events(){
      if(SDL_WaitEvent(&event_) > 0) analyze_event();
      poll_events();
    }

    void event_handler::wait_for_events(const std::uint32_t milliseconds){
      if(SDL_WaitEventTimeout(&event_, milliseconds) > 0) analyze_event();
      poll_events();
    }

    // ::::::::::::::::::::::::::::::::::: PROTECTED FUNCTIONS :::::::::::::::::::::::::::::::::::

    void event_handler::finger_down_event(const finger_event::finger &/*event*/){}
    void event_handler::finger_move_event(const finger_event::finger &/*event*/){}
    void event_handler::finger_up_event(const finger_event::finger &/*event*/){}
    void event_handler::key_down_event(const keyboard_event::key &/*event*/){}
    void event_handler::key_up_event(const keyboard_event::key &/*event*/){}
    void event_handler::click_event(const mouse_event::button &/*event*/){}
    void event_handler::mouse_down_event(const mouse_event::button &/*event*/){}
    void event_handler::mouse_move_event(const mouse_event::move &/*event*/){}
    void event_handler::mouse_up_event(const mouse_event::button &/*event*/){}
    void event_handler::mouse_wheel_event(const mouse_event::wheel &/*event*/){}
    void event_handler::close_event(const window_event::window &/*event*/){}
    void event_handler::hide_event(const window_event::window &/*event*/){}
    void event_handler::maximize_event(const window_event::window &/*event*/){}
    void event_handler::minimize_event(const window_event::window &/*event*/){}
    void event_handler::move_event(const window_event::move &/*event*/){}
    void event_handler::resize_event(const window_event::resize &/*event*/){}
    void event_handler::restore_event(const window_event::resize &/*event*/){}
    void event_handler::show_event(const window_event::window &/*event*/){}
    void event_handler::quit_event(const window_event::window &/*event*/){}

    void event_handler::drop_text_event(const SDL_DropEvent &event){
      if(event.type == SDL_DROPBEGIN){
      }else if(event.type == SDL_DROPCOMPLETE){
      }else if(event.type == SDL_DROPFILE){
        SDL_free(event.file);
      }else if(event.type == SDL_DROPTEXT){
        SDL_free(event.file);
      }
    }

    void event_handler::text_input_event(const SDL_TextInputEvent &event){
      rr::formatted("pressed key: %s\n", rr::message::attention, event.text);
    }

    void event_handler::text_edit_event(const SDL_TextEditingEvent &event){
      rr::formatted("text edited: %s\n", rr::message::attention, event.text);
    }

    void event_handler::close(){}

    void event_handler::hide(){}

    void event_handler::show(){}

    void event_handler::update_position(){}

    void event_handler::update_size(){}

    // :::::::::::::::::::::::::::::::::::: PRIVATE FUNCTIONS ::::::::::::::::::::::::::::::::::::

    void event_handler::analyze_event(){
      switch(event_.type){
        case SDL_QUIT:
          close();
        break;
        case SDL_MOUSEBUTTONDOWN:
          mouse_down_event(gui::mouse_event::button{event_.button.timestamp, event_.button.windowID,
                                                    event_.button.which, event_.button.button,
                                                    event_.button.clicks, event_.button.x,
                                                    event_.button.y});
        break;
        case SDL_MOUSEBUTTONUP:
          mouse_up_event(gui::mouse_event::button{event_.button.timestamp, event_.button.windowID,
                                                  event_.button.which, event_.button.button,
                                                  event_.button.clicks, event_.button.x,
                                                  event_.button.y});
        break;
        case SDL_MOUSEMOTION:
          mouse_move_event(gui::mouse_event::move{event_.motion.timestamp, event_.motion.windowID,
                                                  event_.motion.which, event_.motion.state,
                                                  event_.motion.x, event_.motion.y,
                                                  event_.motion.xrel, event_.motion.yrel});
        break;
        case SDL_MOUSEWHEEL:
          mouse_wheel_event(gui::mouse_event::wheel{event_.wheel.timestamp, event_.wheel.windowID,
                                                    event_.wheel.which, event_.wheel.x,
                                                    event_.wheel.y, event_.wheel.direction});
        break;
        case SDL_KEYDOWN:
          key_down_event(gui::keyboard_event::key{event_.key.timestamp, event_.key.windowID,
                                                  event_.key.repeat, event_.key.keysym.scancode,
                                                  event_.key.keysym.sym, event_.key.keysym.mod});
        break;
        case SDL_KEYUP:
          key_up_event(gui::keyboard_event::key{event_.key.timestamp, event_.key.windowID,
                                                event_.key.repeat, event_.key.keysym.scancode,
                                                event_.key.keysym.sym, event_.key.keysym.mod});
        break;
        case SDL_FINGERDOWN:
          finger_down_event(gui::finger_event::finger{event_.tfinger.timestamp, event_.tfinger.windowID,
                                                      event_.tfinger.touchId, event_.tfinger.fingerId,
                                                      event_.tfinger.x, event_.tfinger.y,
                                                      event_.tfinger.dx, event_.tfinger.dy,
                                                      event_.tfinger.pressure});
        break;
        case SDL_FINGERUP:
          finger_up_event(gui::finger_event::finger{event_.tfinger.timestamp, event_.tfinger.windowID,
                                                    event_.tfinger.touchId, event_.tfinger.fingerId,
                                                    event_.tfinger.x, event_.tfinger.y,
                                                    event_.tfinger.dx, event_.tfinger.dy,
                                                    event_.tfinger.pressure});
        break;
        case SDL_FINGERMOTION:
          finger_move_event(gui::finger_event::finger{event_.tfinger.timestamp, event_.tfinger.windowID,
                                                      event_.tfinger.touchId, event_.tfinger.fingerId,
                                                      event_.tfinger.x, event_.tfinger.y,
                                                      event_.tfinger.dx, event_.tfinger.dy,
                                                      event_.tfinger.pressure});
        break;
        case SDL_WINDOWEVENT:
          window_event(event_.window);
        break;
        case SDL_TEXTINPUT:
          text_input_event(event_.text);
        break;
        case SDL_TEXTEDITING:
          text_edit_event(event_.edit);
        break;
        case SDL_DROPBEGIN:
          drop_text_event(event_.drop);
        break;
        case SDL_DROPCOMPLETE:
          drop_text_event(event_.drop);
        break;
        case SDL_DROPFILE:
          drop_text_event(event_.drop);
        break;
        case SDL_DROPTEXT:
          drop_text_event(event_.drop);
        break;
        default: break;
      }
    }

    void event_handler::window_event(const SDL_WindowEvent &event){
      switch(event.event){
        case SDL_WINDOWEVENT_CLOSE:
          close();
          close_event(gui::window_event::window{event.timestamp, event.windowID});
        break;
        case SDL_WINDOWEVENT_SHOWN:
          show();
          show_event(gui::window_event::window{event.timestamp, event.windowID});
        break;
        case SDL_WINDOWEVENT_HIDDEN:
          hide();
          hide_event(gui::window_event::window{event.timestamp, event.windowID});
        break;
        case SDL_WINDOWEVENT_MINIMIZED:
          hide();
          minimize_event(gui::window_event::window{event.timestamp, event.windowID});
        break;
        case SDL_WINDOWEVENT_MAXIMIZED:
          show();
          maximize_event(gui::window_event::window{event.timestamp, event.windowID});
        break;
        case SDL_WINDOWEVENT_RESTORED:
          show();
          restore_event(gui::window_event::resize{event.timestamp, event.windowID,
                                                  event.data1, event.data2});
        break;
        case SDL_WINDOWEVENT_SIZE_CHANGED:
          if(event.data1 > 0 && event.data2 > 0) show();
          update_size();
          resize_event(gui::window_event::resize{event.timestamp, event.windowID,
                                                 event.data1, event.data2});
        break;
        case SDL_WINDOWEVENT_MOVED:
          update_position();
          move_event(gui::window_event::move{event.timestamp, event.windowID,
                                             event.data1, event.data2});
        break;
        default: break;
      }
    }
  } // namespace: gui
} // namespace: ramrod
