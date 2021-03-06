CMAKE_MINIMUM_REQUIRED(VERSION 3.5.0)
if(UNIX AND NOT APPLE)
  set(LINUX TRUE)
endif()

PROJECT(glad)
find_package(OpenGL REQUIRED)

set(glad_src "src/glad.c")
add_library(glad STATIC ${glad_src})
target_link_libraries(glad PUBLIC ${OPENGL_LIBRARIES} ${CMAKE_DL_LIBS})
target_include_directories(glad PUBLIC "include/")
target_compile_definitions(glad PUBLIC "GLAD_STATIC")

if(LINUX)
  target_compile_definitions(glad PUBLIC "GL_GLEXT_PROTOTYPES")
  find_package(X11 REQUIRED)
  target_link_libraries(glad PUBLIC ${X11_LIBRARIES})
  target_include_directories(glad PUBLIC ${X11_INCLUDE_DIR})
endif()
