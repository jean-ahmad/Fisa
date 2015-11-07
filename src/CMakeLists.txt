######################################################################
# Building
######################################################################

file(GLOB FISA_INC "*.hpp")
file(GLOB FISA_SRC "*.cpp")

add_library(Fisa-${FISA_VERSION_MAJOR}.${FISA_VERSION_MINOR}.${FISA_VERSION_PATCH} ${FISA_INC} ${FISA_SRC})

######################################################################
# Installation
######################################################################

install(TARGETS Fisa-${FISA_VERSION_MAJOR}.${FISA_VERSION_MINOR}.${FISA_VERSION_PATCH} DESTINATION lib)
