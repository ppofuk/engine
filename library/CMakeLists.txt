CMAKE_MINIMUM_REQUIRED(VERSION 3.5.0)
if(UNIX AND NOT APPLE)
  set(LINUX TRUE)
endif()

PROJECT(ent_lib)
set (CMAKE_CXX_STANDARD 14)

file(GLOB ent_core_src "core/*.cc")
file(GLOB ent_core_render_src "core/render/*.cc")

if(LINUX)
  file(GLOB ent_core_platform_src "core/linux/*.cc")
endif()

if(WIN32)
  file(GLOB ent_core_platform_src "core/win32/*.cc")
endif()

add_library(ent_core
  ${ent_core_src}
  ${ent_core_render_src}
  ${ent_core_platform_src})

target_include_directories(ent_core PUBLIC "./")
target_include_directories(ent_core PUBLIC "core/")

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

add_subdirectory("../third-party/glew" "glew")
target_link_libraries(ent_core PUBLIC glew)

target_include_directories(ent_core PUBLIC "../third-party/glm")


# CEF
file(GLOB ent_cef_src "cef/*.cc")
add_library(ent_cef ${ent_cef_src})
target_include_directories(ent_cef PUBLIC "cef/")

if(LINUX)
  add_subdirectory("../third-party/cef-linux" "cef/")
  target_link_libraries(ent_cef PUBLIC libcef_dll_wrapper)
  if (CMAKE_BUILD_TYPE EQUAL "DEBUG")
    target_link_libraries(ent_cef
      PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../third-party/cef-linux/Debug/libcef.so")
  else()
    target_link_libraries(ent_cef
      PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../third-party/cef-linux/Release/libcef.so")
  endif()
  
endif()

# if(WIN32)
#   list(REMOVE_ITEM CEF_COMPILER_FLAGS /WX)
# endif()

target_link_libraries(ent_cef PUBLIC ent_core)
