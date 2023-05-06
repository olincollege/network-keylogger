# Some compilers warn that the return value of cr_assert gets converted from an
# int to a unsigned char:1. To avoid this error in those compilers, turn off
# conversion warnings for test code.

# If you need to test a library called foo, use lines like the following.
# add_executable(test_foo test_foo.c)
# target_link_libraries(test_foo
#     PRIVATE foo
#     PUBLIC ${CRITERION}
# )
# add_test(
#     NAME test_foo
#     COMMAND test_foo ${CRITERION_FLAGS}
# )

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-conversion -Wno-int-conversion")

list(APPEND CMAKE_CTEST_ARGUMENTS --verbose)

add_executable(test_keylogger test_keylogger.c)
target_link_libraries(test_keylogger
     PRIVATE keylogger
     ${LIBEVDEV_LIBRARY} evdev
     PUBLIC ${CRITERION}
 )
 add_test(
     NAME test_keylogger
     COMMAND test_keylogger ${CRITERION_FLAGS}
 )

add_executable(test_server test_server.c)
target_link_libraries(test_server
   PRIVATE server server_utils
   ${LIBEVDEV_LIBRARY} evdev
   PUBLIC ${CRITERION}
)
add_test(
   NAME test_server
   COMMAND test_server ${CRITERION_FLAGS}
)


add_executable(test_server_utils test_server_utils.c)
target_link_libraries(test_server_utils
   PRIVATE server_utils
   ${LIBEVDEV_LIBRARY} evdev
   PUBLIC ${CRITERION}
)
add_test(
   NAME test_server_utils
   COMMAND test_server_utils ${CRITERION_FLAGS}
)
