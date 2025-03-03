set(CMAKE_SOURCE_DIR ${CMAKE_SOURCE_DIR})
include(FetchContent)
FetchContent_Declare(
  glfw
  URL     https://sourceforge.net/projects/glfw.mirror/files/latest/download
  SOURCE_DIR ${CMAKE_SOURCE_DIR}/lib/glfw
)

FetchContent_Declare(
  glew
  URL     https://sourceforge.net/projects/glew/files/glew/2.1.0/glew-2.1.0-win32.zip/download
  
  SOURCE_DIR ${CMAKE_SOURCE_DIR}/lib/glew
)

FetchContent_MakeAvailable(glew)
FetchContent_MakeAvailable(glfw)
