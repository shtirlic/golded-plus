set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(gcfg_SOURCES
  gedacfg.cpp
  gxareas.cpp
  gxcrash.cpp
  gxezy102.cpp
  gxezy110.cpp
  gxfd.cpp
  gxfecho4.cpp
  gxfecho5.cpp
  gxfecho6.cpp
  gxgecho.cpp
  gxhpt.cpp
  gxlora.cpp
  gxmax3.cpp
  gxme2.cpp
  gxopus.cpp
  gxqfront.cpp
  gxqecho.cpp
  gxspace.cpp
  gxsquish.cpp
  gxsync.cpp
  gxtmail.cpp
)
include_directories(../gall ../glibc)
ADD_LIBRARY(gcfg STATIC ${gcfg_SOURCES})
