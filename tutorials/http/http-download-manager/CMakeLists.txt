cmake_minimum_required(VERSION 3.10)

# Set some basic project attributes
project (http-file-downloader
	VERSION 0.1
	DESCRIPTION "A Http sample Download Manager")

# This project will output an executable file
add_executable(${PROJECT_NAME} http-file-downloader.c mongoose.c)

# Create a simple configuration header
configure_file(config.h.in config.h)

# Include the configuration header in the build
target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_BINARY_DIR}")
