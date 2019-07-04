cmake_minimum_required(VERSION 3.9)
project(tgdpwdgen C)

set(CMAKE_C_STANDARD 11)

FIND_PACKAGE(PkgConfig REQUIRED)
PKG_CHECK_MODULES(GTK3 REQUIRED gtk+-3.0)
PKG_CHECK_MODULES(LIBNOTIFY REQUIRED libnotify)
PKG_CHECK_MODULES(LIBQRENCODE REQUIRED libqrencode)
PKG_CHECK_MODULES(LIBPNG REQUIRED libpng)


# Setup CMake to use GTK+, tell the compiler where to look for headers
# and to the linker where to look for libraries
INCLUDE_DIRECTORIES(${GTK3_INCLUDE_DIRS} ${LIBNOTIFY_INCLUDE_DIRS} ${LIBQRENCODE_INCLUDE_DIRS} ${LIBPNG_INCLUDE_DIRS})
LINK_DIRECTORIES(${GTK3_LIBRARY_DIRS} ${LIBNOTIFY_LIBRARY_DIRS} ${LIBQRENCODE_LIBRARY_DIRS} ${LIBPNG_LIBRARY_DIRS})

# Add other flags to the compiler
ADD_DEFINITIONS(${GTK3_CFLAGS_OTHER})

add_executable(tgdpwdgen main.c tgdpwdgen.c tgdpwdgen.h libtaggerd.c libtaggerd.h libtaggerdgui.c libtaggerdgui.h)

TARGET_LINK_LIBRARIES(tgdpwdgen ${GTK3_LIBRARIES} ${LIBNOTIFY_LIBRARIES} ${LIBQRENCODE_LIBRARIES} ${LIBPNG_LIBRARIES})