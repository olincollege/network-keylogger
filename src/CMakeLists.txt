# If you have a program that's just a .c file and it has a main method, define
# an executable. If it has both a .c and .h file, make it a library. Also, link
# libraries appropriately here. Targets should be added in dependency order - if
# executable bar needs library foo, then you should do
#   add_library(foo foo.c foo.h)
#   add_executable(bar bar.c)
#   target_link_libraries(bar PRIVATE foo)
# or similar. The PRIVATE/INTERFACE/PUBLIC keyword will depend on whether the
# library is used only in function bodies (PRIVATE), only in function
# signatures/types (INTERFACE), or both (PUBLIC).
SET(GCC_COVERAGE_COMPILE_FLAGS "-I/usr/include/libevdev-1.0 -levdev")


# Add the libevdev library
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBEVDEV REQUIRED libevdev)
include_directories(${LIBEVDEV_INCLUDE_DIRS})
link_directories(${LIBEVDEV_LIBRARY_DIRS})

add_library(server server.c server.h)
add_library(server_utils server_utils.c server_utils.h)
add_library(keylogger keylogger.c keylogger.h)
add_library(client client.c client.h)
add_executable(main main.c)
target_link_libraries(main PRIVATE client keylogger server server_utils ${LIBEVDEV_LIBRARY})


target_link_libraries(main PRIVATE client keylogger server)
