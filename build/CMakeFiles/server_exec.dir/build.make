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
CMAKE_SOURCE_DIR = /home/mefathim-tech-41/ccccccccc/all

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/mefathim-tech-41/ccccccccc/all/build

# Include any dependencies generated for this target.
include CMakeFiles/server_exec.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/server_exec.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/server_exec.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/server_exec.dir/flags.make

CMakeFiles/server_exec.dir/server_main.cpp.o: CMakeFiles/server_exec.dir/flags.make
CMakeFiles/server_exec.dir/server_main.cpp.o: ../server_main.cpp
CMakeFiles/server_exec.dir/server_main.cpp.o: CMakeFiles/server_exec.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/mefathim-tech-41/ccccccccc/all/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/server_exec.dir/server_main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/server_exec.dir/server_main.cpp.o -MF CMakeFiles/server_exec.dir/server_main.cpp.o.d -o CMakeFiles/server_exec.dir/server_main.cpp.o -c /home/mefathim-tech-41/ccccccccc/all/server_main.cpp

CMakeFiles/server_exec.dir/server_main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/server_exec.dir/server_main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/mefathim-tech-41/ccccccccc/all/server_main.cpp > CMakeFiles/server_exec.dir/server_main.cpp.i

CMakeFiles/server_exec.dir/server_main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/server_exec.dir/server_main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/mefathim-tech-41/ccccccccc/all/server_main.cpp -o CMakeFiles/server_exec.dir/server_main.cpp.s

# Object files for target server_exec
server_exec_OBJECTS = \
"CMakeFiles/server_exec.dir/server_main.cpp.o"

# External object files for target server_exec
server_exec_EXTERNAL_OBJECTS =

server_exec: CMakeFiles/server_exec.dir/server_main.cpp.o
server_exec: CMakeFiles/server_exec.dir/build.make
server_exec: CMakeFiles/server_exec.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/mefathim-tech-41/ccccccccc/all/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable server_exec"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/server_exec.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/server_exec.dir/build: server_exec
.PHONY : CMakeFiles/server_exec.dir/build

CMakeFiles/server_exec.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/server_exec.dir/cmake_clean.cmake
.PHONY : CMakeFiles/server_exec.dir/clean

CMakeFiles/server_exec.dir/depend:
	cd /home/mefathim-tech-41/ccccccccc/all/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/mefathim-tech-41/ccccccccc/all /home/mefathim-tech-41/ccccccccc/all /home/mefathim-tech-41/ccccccccc/all/build /home/mefathim-tech-41/ccccccccc/all/build /home/mefathim-tech-41/ccccccccc/all/build/CMakeFiles/server_exec.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/server_exec.dir/depend

