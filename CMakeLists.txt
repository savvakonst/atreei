cmake_minimum_required(VERSION 3.16)

# set the project name

project(threaded_avl_tree C)

# add the executable
add_executable(threaded_avl_tree_
        ThreadedAvlTree.h
        ThreadedAvlTree.c
        main.c
        )
set_property(TARGET threaded_avl_tree_ PROPERTY C_STANDARD 11)
#set_target_properties(threaded_avl_tree_ PROPERTIES COMPILE_FLAGS    /FA  $<$<CONFIG:RELEASE>:O2>  )
#target_compile_options(threaded_avl_tree_  PRIVATE /FA  $<$<CONFIG:RELEASE>:/O2>)
#
#add_executable(threaded_avl_tree_app
#		main.c
#		)