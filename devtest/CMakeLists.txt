

add_executable( devprog.app devtest.h  devtest.cc )

target_link_libraries(devprog.app logbook chrtools)

