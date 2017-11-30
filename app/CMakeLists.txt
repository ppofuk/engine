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

if(NOT WIN32)
add_executable(app_test
  ${ent_app_test_src}
  ${ent_app_test_platform_src})
else()
  add_executable(app_test WIN32
  ${ent_app_test_src}
  ${ent_app_test_platform_src})
endif()

target_include_directories(app_test PUBLIC "test/")

add_subdirectory("../library" "lib")
target_link_libraries(app_test
  ent_core)

PROJECT(mesh_convert)
set (CMAKE_CXX_STANDARD 14)
file(GLOB mesh_convert_src "mesh-convert/*.cc")

if(LINUX)
  file(GLOB mesh_convert_platform_src "mesh-convert/linux/*.cc")
endif()

if(WIN32)
  file(GLOB mesh_convert_platform_src "mesh-convert/win32/*.cc")
endif()

if(NOT WIN32)
add_executable(mesh_convert
  ${mesh_convert_src}
  ${mesh_convert_platform_src})
else()
  add_executable(mesh_convert WIN32
  ${mesh_convert_src}
  ${mesh_convert_platform_src})
endif()

target_include_directories(mesh_convert PUBLIC "mesh-convert/")

target_link_libraries(mesh_convert
  ent_core)
