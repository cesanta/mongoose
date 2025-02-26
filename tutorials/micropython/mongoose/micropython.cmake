# Create an INTERFACE library for our C module.
add_library(usermod_mongoose INTERFACE)

# Add our source files to the lib
target_sources(usermod_mongoose INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/module.c
    ${CMAKE_CURRENT_LIST_DIR}/mongoose.c
    ${CMAKE_CURRENT_LIST_DIR}/main.c
)

# Add the current directory as an include directory.
target_include_directories(usermod_mongoose INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}
)

# Link our INTERFACE library to the usermod target.
target_link_libraries(usermod INTERFACE usermod_mongoose)
