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

add_library(server_utils server_utils.c server_utils.h)
add_library(keylogger keylogger.c keylogger.h)
add_library(client client.c client.h)
add_library(server server.c server.h)

target_link_libraries(client PRIVATE server_utils)

target_link_libraries(server PRIVATE server_utils)
add_executable(main main.c)
target_link_libraries(main PRIVATE client keylogger server server_utils)
add_executable(run_server run_server.c)
target_link_libraries(run_server PRIVATE client keylogger server server_utils)
