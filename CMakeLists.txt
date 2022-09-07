cmake_minimum_required(VERSION 3.16)

# set the project name

project(Ati C)

add_subdirectory(test)
# add the executable
add_library(Ati
        ImplicitKeyThreadedATI.c
        ImplicitKeyThreadedATI.h
        ThreadedATI.h
        ThreadedATI.c
        )

target_include_directories(Ati PUBLIC .)

set_property(TARGET Ati PROPERTY C_STANDARD 11)
#set_target_properties(Ati PROPERTIES COMPILE_FLAGS    /FA  $<$<CONFIG:RELEASE>:O2>  )
#target_compile_options(Ati  PRIVATE /FA  $<$<CONFIG:RELEASE>:/O2>)
#
#add_executable(threaded_avl_tree_app
#		main.c
#		)