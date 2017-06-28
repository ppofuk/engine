CMAKE_MINIMUM_REQUIRED(VERSION 3.5.0)
if(UNIX AND NOT APPLE)
  set(LINUX TRUE)
endif()

PROJECT(ent_app)
set (CMAKE_CXX_STANDARD 14)

file(GLOB ent_app_test_src "test/*.cc")

if(LINUX)
  file(GLOB ent_app_test_platform_src "test/linux/*.cc")
endif()

if(WIN32)
  file(GLOB ent_app_test_platform_src "test/win32/*.cc")
endif()

add_executable(app_test
  ${ent_app_test_src}
  ${ent_app_test_platform_src})

target_include_directories(app_test PUBLIC "test/")

add_subdirectory("../library" "lib")
target_link_libraries(app_test
  ent_core
  ent_cef)
