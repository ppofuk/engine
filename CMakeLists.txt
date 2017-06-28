CMAKE_MINIMUM_REQUIRED(VERSION 3.5.0)
if(UNIX AND NOT APPLE)
  set(LINUX TRUE)
endif()

PROJECT(ent)

add_subdirectory(app/ app)
