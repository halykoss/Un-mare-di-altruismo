cmake_minimum_required(VERSION 3.0)
project(Altruismo)

if (NOT VCPKG_TOOLCHAIN)
  find_package(PkgConfig)
  pkg_check_modules(GTKMM gtkmm-3.0)
  include_directories(${GTKMM_INCLUDE_DIRS})
  link_directories(${GTKMM_LIBRARY_DIRS})
  link_libraries(${GTKMM_LIBRARIES})
else()
  find_library(ATKMM_LIBRARY atkmm)
  link_libraries(${ATKMM_LIBRARY})
  find_library(CAIROMM_LIBRARY cairomm-1.0)
  link_libraries(${CAIROMM_LIBRARY})
  find_library(GDKMM_LIBRARY gdkmm)
  link_libraries(${GDKMM_LIBRARY})
  find_library(GIOMM_LIBRARY giomm)
  link_libraries(${GIOMM_LIBRARY})
  find_library(GLIBMM_LIBRARY glibmm)
  link_libraries(${GLIBMM_LIBRARY})
  find_library(GTKMM_LIBRARY gtkmm)
  link_libraries(${GTKMM_LIBRARY})
  find_library(PANGOMM_LIBRARY pangomm)
  link_libraries(${PANGOMM_LIBRARY})
  find_library(SIGC_LIBRARY sigc-2.0)
  link_libraries(${SIGC_LIBRARY})
  find_path(GTKMM_INCLUDE_DIRS gtkmm.h)
  include_directories(${GTKMM_INCLUDE_DIRS})
endif()

if (MSVC)
  set(GUI_OPTION WIN32)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4250")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /ENTRY:mainCRTStartup")
elseif (APPLE)
  set(GUI_OPTION MACOSX_BUNDLE)
endif ()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(THREADS_PREFER_PTHREAD_FLAG ON)
set(JSON_BuildTests OFF CACHE INTERNAL "")
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
execute_process (
    COMMAND bash -c "pip3 install .."
    OUTPUT_VARIABLE outVar
)

add_subdirectory(${nlohmann_json_source}
   src/json)

FIND_PACKAGE( Boost 1.40 COMPONENTS program_options REQUIRED )
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )

add_executable(
  a.out ${GUI_OPTION} 
  src/area/area.cc 
  src/field/field.cc 
  src/tile/tile.cc 
  src/tile/food/food.cc 
  src/tile/fish/fish.cc 
  src/utils/utils.cc
  src/main.cpp
  )
target_link_libraries(a.out PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(a.out PRIVATE Threads::Threads)
TARGET_LINK_LIBRARIES(a.out PRIVATE ${Boost_LIBRARIES})
