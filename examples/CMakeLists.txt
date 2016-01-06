if(EXAMPLES)
######################################################################
# Building
######################################################################

# Example lamp1
add_executable(example_lamp1 example_lamp1.cpp)
target_link_libraries(example_lamp1 Fisa-${FISA_VERSION_MAJOR}.${FISA_VERSION_MINOR}.${FISA_VERSION_PATCH})

# Example computer
add_executable(example_computer example_computer.cpp)
target_link_libraries(example_computer Fisa-${FISA_VERSION_MAJOR}.${FISA_VERSION_MINOR}.${FISA_VERSION_PATCH})

# Example lamp2
add_executable(example_lamp2 example_lamp2.cpp)
target_link_libraries(example_lamp2 Fisa-${FISA_VERSION_MAJOR}.${FISA_VERSION_MINOR}.${FISA_VERSION_PATCH})

# Example car
add_executable(example_car example_car.cpp)
target_link_libraries(example_car Fisa-${FISA_VERSION_MAJOR}.${FISA_VERSION_MINOR}.${FISA_VERSION_PATCH})

######################################################################
# Installation
######################################################################

install(TARGETS example_lamp1 example_computer example_lamp2 example_car DESTINATION "bin")
endif(EXAMPLES)
