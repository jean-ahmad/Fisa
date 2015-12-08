######################################################################
# Building
######################################################################

# test_datetime
add_executable(test_datetime test_datetime.cpp)
target_link_libraries(test_datetime Fisa-${FISA_VERSION_MAJOR}.${FISA_VERSION_MINOR}.${FISA_VERSION_PATCH})

# test_transitions
add_executable(test_transitions test_transitions.cpp)
target_link_libraries(test_transitions Fisa-${FISA_VERSION_MAJOR}.${FISA_VERSION_MINOR}.${FISA_VERSION_PATCH})

# test_machine1
add_executable(test_machine1 test_machine1.cpp)
target_link_libraries(test_machine1 Fisa-${FISA_VERSION_MAJOR}.${FISA_VERSION_MINOR}.${FISA_VERSION_PATCH})

# test_machine2
add_executable(test_machine2 test_machine2.cpp)
target_link_libraries(test_machine2 Fisa-${FISA_VERSION_MAJOR}.${FISA_VERSION_MINOR}.${FISA_VERSION_PATCH})

######################################################################
# Tests
######################################################################

enable_testing()
add_test(TestDatetime test_datetime)
add_test(TestTransitions test_transitions)
add_test(TestMachine1 test_machine1)
add_test(TestMachine2 test_machine2)
