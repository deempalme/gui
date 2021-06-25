#cmake version
cmake_minimum_required(VERSION 3.5...3.15)

if(NOT TARGET ${RamRodGUI_LIBRARIES})
  project(RamRodGUI
    VERSION 1.0.0
    DESCRIPTION "Ram-Rod's graphical user interface"
    LANGUAGES CXX C
  )

  list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

  # Loading SDL2
  find_package(SDL2 REQUIRED)

  #header files
  set(HPP_FILES
    include/ramrod/gui/button.h
    include/ramrod/gui/constants.h
    include/ramrod/gui/element.h
    include/ramrod/gui/enumerators.h
    include/ramrod/gui/event_handler.h
    include/ramrod/gui/file_manager.h
    include/ramrod/gui/font_loader.h
    include/ramrod/gui/gui_manager.h
    include/ramrod/gui/image.h
    include/ramrod/gui/image_loader.h
    include/ramrod/gui/image_stream.h
    include/ramrod/gui/input.h
    include/ramrod/gui/input_text.h
    include/ramrod/gui/texture.h
    include/ramrod/gui/time_slider.h
    include/ramrod/gui/type_events.h
    include/ramrod/gui/types.h
    include/ramrod/gui/video_yuyv.h
    include/ramrod/gui/video_yv12.h
    include/ramrod/gui/window.h
  )

  #source files
  set(CPP_FILES
    src/ramrod/gui/button.cpp
    src/ramrod/gui/element.cpp
    src/ramrod/gui/event_handler.cpp
    src/ramrod/gui/file_manager.cpp
    src/ramrod/gui/font_loader.cpp
    src/ramrod/gui/gui_manager.cpp
    src/ramrod/gui/image.cpp
    src/ramrod/gui/image_loader.cpp
    src/ramrod/gui/image_stream.cpp
    src/ramrod/gui/input.cpp
    src/ramrod/gui/input_text.cpp
    src/ramrod/gui/texture.cpp
    src/ramrod/gui/time_slider.cpp
    src/ramrod/gui/video_yuyv.cpp
    src/ramrod/gui/video_yv12.cpp
    src/ramrod/gui/window.cpp
  )

  # // ------------------------------------------------------------------------------------ //
  # // ------------------------------------ START LIBS ------------------------------------ //
  # // ------------------------------------------------------------------------------------ //

  # +++++++++++++++++++++++++++++++++++++ Console printer ++++++++++++++++++++++++++++++++++++
  # adding the root directory of torero source tree to your project
  add_subdirectory(lib/ramrod/console_printer)

  # ++++++++++++++++++++++++++++++++++*+++++++ OPEN GL +++++++++++++++++++++++++++++++++++++++
  add_subdirectory(lib/ramrod/open_gl)

  # ++++++++++++++++++++++++++++++++++++++++ STB_IMAGE +++++++++++++++++++++++++++++++++++++++
  # adding the root directory of the stb library source tree to your project
  set(STB_FILES
    lib/stb/stb_image.h
  #  lib/stb/stb_image_write.h
  )

  # // ------------------------------------------------------------------------------------ //
  # // ------------------------------------ END LIBS -------------------------------------- //
  # // ------------------------------------------------------------------------------------ //

  set(${PROJECT_NAME}_INCLUDE_DIRS
    ${PROJECT_SOURCE_DIR}/include
    ${SDL2_INCLUDE_DIRS}
    lib/stb
    CACHE INTERNAL ""
  )

  set(${PROJECT_NAME}_LIBRARIES
    ${PROJECT_NAME}
    CACHE INTERNAL ""
  )

  #add every file to the executable
  add_library(${PROJECT_NAME})

  target_sources(${PROJECT_NAME}
    PRIVATE
      ${CPP_FILES}
      ${HPP_FILES}
      ${STB_FILES}
  )

  target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

  target_compile_definitions(${PROJECT_NAME}
    PUBLIC
      RAMROD_GUI_DIR="${PROJECT_SOURCE_DIR}"
    PRIVATE
      # TODO: change to binary dir at the end
#      RAMROD_GUI_DIR="${PROJECT_BINARY_DIR}"
      STB_IMAGE_STATIC=1
      STB_IMAGE_IMPLEMENTATION=1
      STBI_NO_HDR=1
  )

  set_target_properties(${PROJECT_NAME}
    PROPERTIES
      CXX_STANDARD 17
      CXX_STANDARD_REQUIRED ON
      CXX_EXTENSIONS OFF
  )

  target_include_directories(${PROJECT_NAME}
    PRIVATE
      include
      # RamRod utilities
      ${RamRodConsole_INCLUDE_DIRS}
      ${RamRodGL_INCLUDE_DIRS}
      # stbi image loader
      lib/stb
      # SDL2 libraries
      ${SDL2_INCLUDE_DIRS}
  )

  #indicates which libraries to use in the executable
  target_link_libraries(${PROJECT_NAME}
    # RamRod utilities
    ${RamRodConsole_LIBRARIES}
    ${RamRodGL_LIBRARIES}
    # SDL2 libraries
    ${SDL2_LIBRARIES}
  )

  # Copying resource's folder into the bindary folder
  file(COPY resources DESTINATION ${PROJECT_BINARY_DIR})

endif(NOT TARGET ${RamRodGUI_LIBRARIES})
