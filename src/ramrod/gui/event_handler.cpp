#include "ramrod/gui/event_handler.h"

#include <SDL2/SDL_keyboard.h>          // for SDL_Keysym
#include <SDL2/SDL_keycode.h>           // for SDLK_DOWN, SDLK_LEFT
#include <SDL2/SDL_mouse.h>             // for SDL_BUTTON_LEFT
#include <SDL2/SDL_video.h>             // for SDL_WINDOWEVENT_CLOSE
#include <SDL2/SDL_events.h>            // for SDL_Event, SDL_Win...
#include <SDL2/SDL_rect.h>              // for SDL_Rect
#include <SDL2/SDL_stdinc.h>            // for SDL_free
#include "ramrod/gui/enumerators.h"     // for ctrl, not_none
#include "ramrod/gui/window.h"          // for window

namespace ramrod::gui {
  event_handler::event_handler() :
    event_()
  {}

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

  void ramrod::gui::event_handler::finger_down_event(const ramrod::event::finger &/*event*/){}

  void ramrod::gui::event_handler::finger_move_event(const ramrod::event::finger &/*event*/){}

  void ramrod::gui::event_handler::finger_up_event(const ramrod::event::finger &/*event*/){}

  void ramrod::gui::event_handler::multigesture_event(const ramrod::event::multigesture &/*event*/){}

  void ramrod::gui::event_handler::click_event(const ramrod::event::mouse_button &/*event*/){}

  void ramrod::gui::event_handler::close_event(const ramrod::event::window &/*event*/){}

  void ramrod::gui::event_handler::hide_event(const ramrod::event::window &/*event*/){}

  void ramrod::gui::event_handler::maximize_event(const ramrod::event::window &/*event*/){}

  void ramrod::gui::event_handler::minimize_event(const ramrod::event::window &/*event*/){}

  void ramrod::gui::event_handler::move_event(const ramrod::event::window_move &/*event*/){}

  void ramrod::gui::event_handler::resize_event(const ramrod::event::window_resize &/*event*/){}

  void ramrod::gui::event_handler::restore_event(const ramrod::event::window_resize &/*event*/){}

  void ramrod::gui::event_handler::show_event(const ramrod::event::window &/*event*/){}

  void ramrod::gui::event_handler::quit_event(const ramrod::event::window &/*event*/){}

  void event_handler::drop_text_event(const SDL_DropEvent &event){
    if(event.type == SDL_DROPBEGIN){
    }else if(event.type == SDL_DROPCOMPLETE){
    }else if(event.type == SDL_DROPFILE){
      SDL_free(event.file);
    }else if(event.type == SDL_DROPTEXT){
      SDL_free(event.file);
    }
  }

  void ramrod::gui::event_handler::text_input_event(const SDL_TextInputEvent &/*event*/){}

  void ramrod::gui::event_handler::text_edit_event(const SDL_TextEditingEvent &/*event*/){}

  void ramrod::gui::event_handler::close(){}

  void ramrod::gui::event_handler::hide(){}

  void ramrod::gui::event_handler::show(){}

  void ramrod::gui::event_handler::update_position(){}

  void ramrod::gui::event_handler::update_size(){}

  // :::::::::::::::::::::::::::::::::::: PRIVATE FUNCTIONS ::::::::::::::::::::::::::::::::::::

  void event_handler::analyze_event(){
    switch(event_.type){
      case SDL_QUIT:
        close();
      break;
      case SDL_MOUSEBUTTONDOWN:
        mouse_down_event(event::mouse_button{event_.button.timestamp, event_.button.windowID,
                                             event_.button.which, event_.button.button,
                                             event_.button.clicks, event_.button.x,
                                             event_.button.y});
      break;
      case SDL_MOUSEBUTTONUP:
        mouse_up_event(event::mouse_button{event_.button.timestamp, event_.button.windowID,
                                           event_.button.which, event_.button.button,
                                           event_.button.clicks, event_.button.x,
                                           event_.button.y});
      break;
      case SDL_MOUSEMOTION:
        mouse_move_event(event::mouse_move{event_.motion.timestamp, event_.motion.windowID,
                                           event_.motion.which, event_.motion.state,
                                           event_.motion.x, event_.motion.y,
                                           event_.motion.xrel, event_.motion.yrel});
      break;
      case SDL_MOUSEWHEEL:
        mouse_wheel_event(event::mouse_wheel{event_.wheel.timestamp, event_.wheel.windowID,
                                             event_.wheel.which, event_.wheel.x,
                                             event_.wheel.y, event_.wheel.direction});
      break;
      case SDL_KEYDOWN:
        key_down_event(event::key{event_.key.timestamp, event_.key.windowID,
                                  event_.key.repeat, event_.key.keysym.scancode,
                                  event_.key.keysym.sym, event_.key.keysym.mod});
      break;
      case SDL_KEYUP:
        key_up_event(event::key{event_.key.timestamp, event_.key.windowID,
                                event_.key.repeat, event_.key.keysym.scancode,
                                event_.key.keysym.sym, event_.key.keysym.mod});
      break;
      case SDL_FINGERDOWN:
        finger_down_event(event::finger{event_.tfinger.timestamp, event_.tfinger.windowID,
                                        event_.tfinger.touchId, event_.tfinger.fingerId,
                                        event_.tfinger.x, event_.tfinger.y,
                                        event_.tfinger.dx, event_.tfinger.dy,
                                        event_.tfinger.pressure});
      break;
      case SDL_FINGERUP:
        finger_up_event(event::finger{event_.tfinger.timestamp, event_.tfinger.windowID,
                                      event_.tfinger.touchId, event_.tfinger.fingerId,
                                      event_.tfinger.x, event_.tfinger.y,
                                      event_.tfinger.dx, event_.tfinger.dy,
                                      event_.tfinger.pressure});
      break;
      case SDL_FINGERMOTION:
        finger_move_event(event::finger{event_.tfinger.timestamp, event_.tfinger.windowID,
                                        event_.tfinger.touchId, event_.tfinger.fingerId,
                                        event_.tfinger.x, event_.tfinger.y,
                                        event_.tfinger.dx, event_.tfinger.dy,
                                        event_.tfinger.pressure});
      break;
      case SDL_MULTIGESTURE:
        multigesture_event(event::multigesture{event_.mgesture.timestamp, event_.mgesture.touchId,
                                               event_.mgesture.numFingers, event_.mgesture.padding,
                                               event_.mgesture.dTheta, event_.mgesture.dDist,
                                               event_.mgesture.x, event_.mgesture.y});
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
        close_event(event::window{event.timestamp, event.windowID});
      break;
      case SDL_WINDOWEVENT_SHOWN:
        show();
        show_event(event::window{event.timestamp, event.windowID});
      break;
      case SDL_WINDOWEVENT_HIDDEN:
        hide();
        hide_event(event::window{event.timestamp, event.windowID});
      break;
      case SDL_WINDOWEVENT_MINIMIZED:
        hide();
        minimize_event(event::window{event.timestamp, event.windowID});
      break;
      case SDL_WINDOWEVENT_MAXIMIZED:
        show();
        maximize_event(event::window{event.timestamp, event.windowID});
      break;
      case SDL_WINDOWEVENT_RESTORED:
        show();
        restore_event(event::window_resize{event.timestamp, event.windowID,
                                           event.data1, event.data2});
      break;
      case SDL_WINDOWEVENT_SIZE_CHANGED:
        if(event.data1 > 0 && event.data2 > 0) show();
        update_size();
        resize_event(event::window_resize{event.timestamp, event.windowID,
                                          event.data1, event.data2});
      break;
      case SDL_WINDOWEVENT_MOVED:
        update_position();
        move_event(event::window_move{event.timestamp, event.windowID,
                                      event.data1, event.data2});
      break;
      default: break;
    }
  }
} // namespace ramrod::gui
