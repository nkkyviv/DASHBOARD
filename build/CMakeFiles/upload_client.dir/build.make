# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/vivian/Desktop/av22tr-team-b

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/vivian/Desktop/av22tr-team-b/build

# Utility rule file for upload_client.

# Include any custom commands dependencies for this target.
include CMakeFiles/upload_client.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/upload_client.dir/progress.make

CMakeFiles/upload_client:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/vivian/Desktop/av22tr-team-b/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building and uploading the project to the ESP32 for client"
	cd /home/vivian/Desktop/av22tr-team-b/client/esp32 && platformio run --target upload

upload_client: CMakeFiles/upload_client
upload_client: CMakeFiles/upload_client.dir/build.make
.PHONY : upload_client

# Rule to build all files generated by this target.
CMakeFiles/upload_client.dir/build: upload_client
.PHONY : CMakeFiles/upload_client.dir/build

CMakeFiles/upload_client.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/upload_client.dir/cmake_clean.cmake
.PHONY : CMakeFiles/upload_client.dir/clean

CMakeFiles/upload_client.dir/depend:
	cd /home/vivian/Desktop/av22tr-team-b/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/vivian/Desktop/av22tr-team-b /home/vivian/Desktop/av22tr-team-b /home/vivian/Desktop/av22tr-team-b/build /home/vivian/Desktop/av22tr-team-b/build /home/vivian/Desktop/av22tr-team-b/build/CMakeFiles/upload_client.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/upload_client.dir/depend

