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

FetchContent_Declare(
  glew
  URL     https://sourceforge.net/projects/glew/files/glew/2.1.0/glew-2.1.0-win32.zip/download
  
  SOURCE_DIR ${CMAKE_SOURCE_DIR}/lib/glew
)


FetchContent_Declare(
  glm
  URL     https://github.com/g-truc/glm/releases/download/1.0.1/glm-1.0.1-light.7z

  SOURCE_DIR ${CMAKE_SOURCE_DIR}/lib/glm
)

FetchContent_MakeAvailable(glew)
FetchContent_MakeAvailable(glfw)
FetchContent_MakeAvailable(glm)