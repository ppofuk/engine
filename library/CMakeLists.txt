CMAKE_MINIMUM_REQUIRED(VERSION 3.5.0)
if(UNIX AND NOT APPLE)
  set(LINUX TRUE)
endif()

PROJECT(ent_lib)
set (CMAKE_CXX_STANDARD 14)

file(GLOB ent_core_src "core/*.cc")
file(GLOB ent_core_render_src "core/render/*.cc")
file(GLOB imgui_src "../third-party/imgui/*.cpp")

if(LINUX)
  file(GLOB ent_core_platform_src "core/linux/*.cc")
endif()

if(WIN32)
  file(GLOB ent_core_platform_src "core/win32/*.cc")
endif()

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
find_package(SDL2 REQUIRED)

add_library(ent_core
  ${ent_core_src}
  ${ent_core_render_src}
  ${ent_core_platform_src}
  ${imgui_src})

target_include_directories(ent_core PUBLIC "./")
target_include_directories(ent_core PUBLIC "core/")
target_include_directories(ent_core PUBLIC "../third-party/imgui/")
target_include_directories(ent_core PUBLIC "../third-party/gli/")
target_include_directories(ent_core PUBLIC "../third-party")

if(LINUX)
  target_compile_definitions(ent_core PUBLIC "OS_LINUX")
  find_package(Threads REQUIRED)
  target_link_libraries(ent_core PUBLIC pthread)
  
  find_package(X11 REQUIRED)
  target_link_libraries(ent_core PUBLIC ${X11_LIBRARIES})
  target_include_directories(ent_core PUBLIC ${X11_INCLUDE_DIR})
endif()

if(WIN32)
  target_compile_definitions(ent_core PUBLIC "OS_WINDOWS")
endif()

add_subdirectory("../third-party/glad" "glad")
target_link_libraries(ent_core PUBLIC glad)

target_include_directories(ent_core PUBLIC "../third-party/glm")

target_include_directories(ent_core PUBLIC ${SDL2_INCLUDE_DIR})
target_link_libraries(ent_core PUBLIC ${SDL2_LIBRARY})
