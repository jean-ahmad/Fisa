######################################################################
# Building
######################################################################

# Example lamp1
add_executable(example_lamp1 example_lamp1.cpp)
target_link_libraries(example_lamp1 Fisa-${FISA_VERSION_MAJOR}.${FISA_VERSION_MINOR}.${FISA_VERSION_PATCH})

# Example machine1
add_executable(example_machine1 example_machine1.cpp)
target_link_libraries(example_machine1 Fisa-${FISA_VERSION_MAJOR}.${FISA_VERSION_MINOR}.${FISA_VERSION_PATCH})

# Example lamp2
add_executable(example_lamp2 example_lamp2.cpp)
target_link_libraries(example_lamp2 Fisa-${FISA_VERSION_MAJOR}.${FISA_VERSION_MINOR}.${FISA_VERSION_PATCH})

######################################################################
# Installation
######################################################################

install(TARGETS example_lamp1 example_machine1 example_lamp2 DESTINATION "bin")

