cmake_minimum_required(VERSION 3.16)

# set the project name

project(Test_Ati C)


add_executable(Test_ThreadedATI
        test_ThreadedATI.c
        test_ati_helper.h)

target_link_libraries(Test_ThreadedATI Ati)
set_property(TARGET Test_ThreadedATI PROPERTY C_STANDARD 11)


add_executable(Test_ImplicitKeyThreadedATI
        test_ImplicitKeyThreadedATI.c
        test_ati_helper.h test_ImplicitKeyThreadedATI.c)

target_link_libraries(Test_ImplicitKeyThreadedATI Ati)
set_property(TARGET Test_ImplicitKeyThreadedATI PROPERTY C_STANDARD 11)