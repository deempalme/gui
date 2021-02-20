#include "ramrod/gui/font_loader.h"

#include <fstream>
#include <vector>

#include "ramrod/console.h"
#include "ramrod/gui/enumerators.h"
#include "ramrod/gui/file_manager.h"
#include "ramrod/gui/image_loader.h"

namespace ramrod {
  namespace gui {
    font_loader::font_loader(const std::string &font_distance_path,
                             const std::string &font_info_path) :
      font_name_("untitled"),
      distance_path_(font_distance_path),
      info_path_(font_info_path),
      file_exists_{false},
      is_loaded_{false},
      texture_(false, gui::texture_unit::albedo),
      characters_(),
      error_{true}
    {
      if(gui::file_manager::check_path(font_distance_path)
      && gui::file_manager::check_path(font_info_path)){
        file_exists_ = true;
        error_ = false;
        load_distance();
        load_info();
      }else
        rr::error("Folder path does not exist");
    }

    bool font_loader::use(){
      if(!is_loaded_ || error_) return false;
      texture_.activate();
      texture_.bind();
      return true;
    }

    std::string font_loader::decimal_to_unicode(std::uint32_t number){
      // Supports 32 bit number
      char hexadecimal[5] = "000\0";
      std::uint32_t temporal;
      std::size_t i{3};

      while(number != 0){
        temporal = number % 16;

        if(temporal < 10)
          hexadecimal[i--] = static_cast<char>(temporal + 48);
        else
          hexadecimal[i--] = static_cast<char>(temporal + 55);

        number /= 16;
      }
      hexadecimal[4] = '\0';

      return std::string(hexadecimal);
    }

    std::string font_loader::unicode_to_utf8(std::uint32_t code){
      char chars[5];
      if(code <= 0x7F){
        chars[0] = (code & 0x7F); chars[1] = '\0';
      }else if (code <= 0x7FF){
        // one continuation byte
        chars[1] = 0x80 | (code & 0x3F); code = (code >> 6);
        chars[0] = 0xC0 | (code & 0x1F); chars[2] = '\0';
      }else if (code <= 0xFFFF){
        // two continuation bytes
        chars[2] = 0x80 | (code & 0x3F); code = (code >> 6);
        chars[1] = 0x80 | (code & 0x3F); code = (code >> 6);
        chars[0] = 0xE0 | (code & 0xF); chars[3] = '\0';
      }else if (code <= 0x10FFFF){
        // three continuation bytes
        chars[3] = 0x80 | (code & 0x3F); code = (code >> 6);
        chars[2] = 0x80 | (code & 0x3F); code = (code >> 6);
        chars[1] = 0x80 | (code & 0x3F); code = (code >> 6);
        chars[0] = 0xF0 | (code & 0x7); chars[4] = '\0';
      }else{
        // unicode replacement character
        chars[2] = 0xEF; chars[1] = 0xBF; chars[0] = 0xBD;
        chars[3] = '\0';
      }
      return std::string(chars);
    }

    const std::map<const std::string, text::character> &font_loader::characters(){
      return characters_;
    }

    const std::string &font_loader::font_name(){
      return font_name_;
    }

    // :::::::::::::::::::::::::::::::::::: PRIVATE FUNCTIONS ::::::::::::::::::::::::::::::::::::

    void font_loader::load_distance(){
      gui::image_loader image(distance_path_, false);

      if(!image.data()){
        error_ = true;
        is_loaded_ = false;
        rr::error() << "Font displacement was not successfully loaded: "
                    << image.failure_reason() << rr::endl;
        return;
      }

      texture_.generate();
      texture_.activate();
      texture_.bind();
      texture_.allocate(image.width(), image.height(), image.data(), image.format());
      texture_.release();

      is_loaded_ = true;
      error_ = false;
    }

    void font_loader::load_info(){
      std::ifstream file;
      std::string line;
      std::size_t position(0u);
      gui::text::character letter;
      std::uint32_t ascii;
      float scale_w, scale_h, font_size;
      float padding_top, padding_right, padding_bottom, padding_left;
      float space_x, space_y;

      file.open(info_path_);

      if(file.is_open()){
        while(std::getline(file, line)){
          if(position == 0u){
            std::size_t column{line.find("padding") + 8u};
            std::string sub_line(line.substr(column));
            const char *padding = sub_line.c_str();

            std::sscanf(padding, "%f,%f,%f,%f %*8s%f,%f",
                        &padding_top, &padding_right, &padding_bottom, &padding_left,
                        &space_x, &space_y);

            // Removing the font's name
            std::size_t last{0};
            std::vector<std::size_t> positions;

            while((last = line.find('"', last + 1)) != std::string::npos)
              positions.push_back(last);

            if(positions.size() > 2){
              positions.resize(positions.size() - 2);
              font_name_ = line.substr(positions[0] + 1, positions.back() - positions[0] - 1);
            }

            sub_line = line.substr(line.find("size=") + 5u);
            const char *size = sub_line.c_str();
            std::sscanf(size, "%f %*s", &font_size);

            ++position;
          }else if(position == 1u){
            std::size_t column{line.find("base") + 5u};
            std::string sub_line(line.substr(column));
            const char *scale = sub_line.c_str();

            std::sscanf(scale, "%*f %*7s%f %*7s%f", &scale_w, &scale_h);
            ++position;
          }else if(position > 4u){
            if(line[0] != 'c') continue;

            const char *line_c = line.c_str();
            float x{-2.0f}, y{-2.0f};
            std::sscanf(line_c, "%*s %*3s%u %*2s%f %*2s%f %*6s%f %*7s%f %*8s%f %*8s%f %*9s%f",
                        &ascii, &x, &y, &letter.texture.map.width,
                        &letter.texture.map.height, &letter.position.offset.x,
                        &letter.position.offset.y, &letter.position.offset.next);

            letter.texture.map.u1 = x / scale_w;
            letter.texture.map.v1 = y / scale_h;
            letter.texture.map.u2 = (x + letter.texture.map.width) / scale_w;
            letter.texture.map.v2 = (y + letter.texture.map.height) / scale_h;

            letter.texture.map.width /= font_size;
            letter.texture.map.height /= font_size;
            letter.position.offset.next = (letter.position.offset.next - padding_left
                                           - padding_right) / font_size;
            letter.position.offset.x = (letter.position.offset.x - padding_left) / font_size;
            letter.position.offset.y = (letter.position.offset.y - padding_top) / font_size;

            characters_[unicode_to_utf8(ascii)] = letter;
          }else
            ++position;
        }

        file.close();
      }else{
        error_ = true;
        rr::error() << "Font information was not successfully open: " << info_path_ << rr::endl;
      }
    }
  } // namespace: gui
} // namespace: ramrod
