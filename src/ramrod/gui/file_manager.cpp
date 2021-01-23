#include "ramrod/gui/file_manager.h"

#include "ramrod/console.h"

#include <filesystem>

namespace ramrod {
  namespace gui {
    file_manager::file_manager(){}

    void file_manager::absolute_path(std::string *relative_path){
      if(relative_path->front() != '/')
        *relative_path = program_path() + '/' + *relative_path;
      else
        *relative_path = program_path() + *relative_path;
    }

    bool file_manager::check_path(std::string *path){
      const std::string initial_path(*path);
      if(path->front() != '/') *path = '/' + *path;

      if(!std::filesystem::exists(std::filesystem::path(*path))){
        *path = program_path() + *path;

        if(!std::filesystem::exists(std::filesystem::path(*path))){
          rr::error("The file: " + initial_path + " was not found.\n" +
                    "  Neither: " + *path + "\n");
          *path = initial_path;
          return false;
        }
      }
      return true;
    }

    bool file_manager::check_path(const std::string &path){
      std::string modifiable(path);
      if(path.front() != '/') modifiable = '/' + modifiable;

      if(!std::filesystem::exists(std::filesystem::path(modifiable))){
        modifiable = program_path() + modifiable;

        if(!std::filesystem::exists(std::filesystem::path(modifiable))){
          rr::error("The file: " + path + " was not found.\n" +
                    "  Neither: " + modifiable + "\n");
          return false;
        }
      }
      return true;
    }

    bool file_manager::create_directory(const std::string &path){
      return std::filesystem::create_directory(path);
    }

    bool file_manager::exists(const std::string &path){
      return std::filesystem::exists(std::filesystem::path(path));
    }

    bool file_manager::is_directory(const std::string &path){
      return std::filesystem::is_directory(std::filesystem::path(path));
    }

    const std::string &file_manager::program_path(){
      if(!initialized_) reset();
      return program_path_;
    }

    void file_manager::reset(){
      program_path_ = std::filesystem::current_path().string();
      initialized_ = true;
    }

    // ::::::::::::::::::::::::::::::: INITIALIZING STATIC VARIABLES ::::::::::::::::::::::::::::::::

    std::string file_manager::program_path_;
    bool file_manager::initialized_{false};

  } // namespace: gui
} // namespace: ramrod
