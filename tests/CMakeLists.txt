if(NON_REGRESSION_TESTS)
######################################################################
# Building
######################################################################

# datetime_test1
add_executable(datetime_test1 datetime_test1.cpp)
target_link_libraries(datetime_test1 Fisa-${FISA_VERSION_MAJOR}.${FISA_VERSION_MINOR}.${FISA_VERSION_PATCH})

# transitions_test1
add_executable(transitions_test1 transitions_test1.cpp)
target_link_libraries(transitions_test1 Fisa-${FISA_VERSION_MAJOR}.${FISA_VERSION_MINOR}.${FISA_VERSION_PATCH})

# machine_test1
add_executable(machine_test1 machine_test1.cpp)
target_link_libraries(machine_test1 Fisa-${FISA_VERSION_MAJOR}.${FISA_VERSION_MINOR}.${FISA_VERSION_PATCH})

# machine_test2
add_executable(machine_test2 machine_test2.cpp)
target_link_libraries(machine_test2 Fisa-${FISA_VERSION_MAJOR}.${FISA_VERSION_MINOR}.${FISA_VERSION_PATCH})

# machine_test3
add_executable(machine_test3 machine_test3.cpp)
target_link_libraries(machine_test3 Fisa-${FISA_VERSION_MAJOR}.${FISA_VERSION_MINOR}.${FISA_VERSION_PATCH})

######################################################################
# Tests
######################################################################

enable_testing()
add_test(DatetimeTest1 datetime_test1)
add_test(TransitionsTest1 transitions_test1)
add_test(MachineTest1 machine_test1)
add_test(MachineTest2 machine_test2)
add_test(MachineTest3 machine_test3)
endif(NON_REGRESSION_TESTS)
