# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list

# Suppress display of executed commands.
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
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/gabriel/gr-cwradar

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/gabriel/gr-cwradar/build

# Include any dependencies generated for this target.
include lib/CMakeFiles/test-cwradar.dir/depend.make

# Include the progress variables for this target.
include lib/CMakeFiles/test-cwradar.dir/progress.make

# Include the compile flags for this target's objects.
include lib/CMakeFiles/test-cwradar.dir/flags.make

lib/CMakeFiles/test-cwradar.dir/test_cwradar.cc.o: lib/CMakeFiles/test-cwradar.dir/flags.make
lib/CMakeFiles/test-cwradar.dir/test_cwradar.cc.o: ../lib/test_cwradar.cc
	$(CMAKE_COMMAND) -E cmake_progress_report /home/gabriel/gr-cwradar/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object lib/CMakeFiles/test-cwradar.dir/test_cwradar.cc.o"
	cd /home/gabriel/gr-cwradar/build/lib && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/test-cwradar.dir/test_cwradar.cc.o -c /home/gabriel/gr-cwradar/lib/test_cwradar.cc

lib/CMakeFiles/test-cwradar.dir/test_cwradar.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test-cwradar.dir/test_cwradar.cc.i"
	cd /home/gabriel/gr-cwradar/build/lib && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/gabriel/gr-cwradar/lib/test_cwradar.cc > CMakeFiles/test-cwradar.dir/test_cwradar.cc.i

lib/CMakeFiles/test-cwradar.dir/test_cwradar.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test-cwradar.dir/test_cwradar.cc.s"
	cd /home/gabriel/gr-cwradar/build/lib && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/gabriel/gr-cwradar/lib/test_cwradar.cc -o CMakeFiles/test-cwradar.dir/test_cwradar.cc.s

lib/CMakeFiles/test-cwradar.dir/test_cwradar.cc.o.requires:
.PHONY : lib/CMakeFiles/test-cwradar.dir/test_cwradar.cc.o.requires

lib/CMakeFiles/test-cwradar.dir/test_cwradar.cc.o.provides: lib/CMakeFiles/test-cwradar.dir/test_cwradar.cc.o.requires
	$(MAKE) -f lib/CMakeFiles/test-cwradar.dir/build.make lib/CMakeFiles/test-cwradar.dir/test_cwradar.cc.o.provides.build
.PHONY : lib/CMakeFiles/test-cwradar.dir/test_cwradar.cc.o.provides

lib/CMakeFiles/test-cwradar.dir/test_cwradar.cc.o.provides.build: lib/CMakeFiles/test-cwradar.dir/test_cwradar.cc.o

lib/CMakeFiles/test-cwradar.dir/qa_cwradar.cc.o: lib/CMakeFiles/test-cwradar.dir/flags.make
lib/CMakeFiles/test-cwradar.dir/qa_cwradar.cc.o: ../lib/qa_cwradar.cc
	$(CMAKE_COMMAND) -E cmake_progress_report /home/gabriel/gr-cwradar/build/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object lib/CMakeFiles/test-cwradar.dir/qa_cwradar.cc.o"
	cd /home/gabriel/gr-cwradar/build/lib && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/test-cwradar.dir/qa_cwradar.cc.o -c /home/gabriel/gr-cwradar/lib/qa_cwradar.cc

lib/CMakeFiles/test-cwradar.dir/qa_cwradar.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test-cwradar.dir/qa_cwradar.cc.i"
	cd /home/gabriel/gr-cwradar/build/lib && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/gabriel/gr-cwradar/lib/qa_cwradar.cc > CMakeFiles/test-cwradar.dir/qa_cwradar.cc.i

lib/CMakeFiles/test-cwradar.dir/qa_cwradar.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test-cwradar.dir/qa_cwradar.cc.s"
	cd /home/gabriel/gr-cwradar/build/lib && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/gabriel/gr-cwradar/lib/qa_cwradar.cc -o CMakeFiles/test-cwradar.dir/qa_cwradar.cc.s

lib/CMakeFiles/test-cwradar.dir/qa_cwradar.cc.o.requires:
.PHONY : lib/CMakeFiles/test-cwradar.dir/qa_cwradar.cc.o.requires

lib/CMakeFiles/test-cwradar.dir/qa_cwradar.cc.o.provides: lib/CMakeFiles/test-cwradar.dir/qa_cwradar.cc.o.requires
	$(MAKE) -f lib/CMakeFiles/test-cwradar.dir/build.make lib/CMakeFiles/test-cwradar.dir/qa_cwradar.cc.o.provides.build
.PHONY : lib/CMakeFiles/test-cwradar.dir/qa_cwradar.cc.o.provides

lib/CMakeFiles/test-cwradar.dir/qa_cwradar.cc.o.provides.build: lib/CMakeFiles/test-cwradar.dir/qa_cwradar.cc.o

# Object files for target test-cwradar
test__cwradar_OBJECTS = \
"CMakeFiles/test-cwradar.dir/test_cwradar.cc.o" \
"CMakeFiles/test-cwradar.dir/qa_cwradar.cc.o"

# External object files for target test-cwradar
test__cwradar_EXTERNAL_OBJECTS =

lib/test-cwradar: lib/CMakeFiles/test-cwradar.dir/test_cwradar.cc.o
lib/test-cwradar: lib/CMakeFiles/test-cwradar.dir/qa_cwradar.cc.o
lib/test-cwradar: lib/CMakeFiles/test-cwradar.dir/build.make
lib/test-cwradar: /usr/local/lib/libgnuradio-runtime.so
lib/test-cwradar: /usr/local/lib/libgnuradio-pmt.so
lib/test-cwradar: /usr/lib/x86_64-linux-gnu/libboost_filesystem.so
lib/test-cwradar: /usr/lib/x86_64-linux-gnu/libboost_system.so
lib/test-cwradar: /usr/lib/x86_64-linux-gnu/libcppunit.so
lib/test-cwradar: lib/libgnuradio-cwradar.so
lib/test-cwradar: /usr/local/lib/libgnuradio-runtime.so
lib/test-cwradar: /usr/local/lib/libgnuradio-pmt.so
lib/test-cwradar: /usr/lib/x86_64-linux-gnu/libboost_filesystem.so
lib/test-cwradar: /usr/lib/x86_64-linux-gnu/libboost_system.so
lib/test-cwradar: lib/CMakeFiles/test-cwradar.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable test-cwradar"
	cd /home/gabriel/gr-cwradar/build/lib && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test-cwradar.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
lib/CMakeFiles/test-cwradar.dir/build: lib/test-cwradar
.PHONY : lib/CMakeFiles/test-cwradar.dir/build

lib/CMakeFiles/test-cwradar.dir/requires: lib/CMakeFiles/test-cwradar.dir/test_cwradar.cc.o.requires
lib/CMakeFiles/test-cwradar.dir/requires: lib/CMakeFiles/test-cwradar.dir/qa_cwradar.cc.o.requires
.PHONY : lib/CMakeFiles/test-cwradar.dir/requires

lib/CMakeFiles/test-cwradar.dir/clean:
	cd /home/gabriel/gr-cwradar/build/lib && $(CMAKE_COMMAND) -P CMakeFiles/test-cwradar.dir/cmake_clean.cmake
.PHONY : lib/CMakeFiles/test-cwradar.dir/clean

lib/CMakeFiles/test-cwradar.dir/depend:
	cd /home/gabriel/gr-cwradar/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/gabriel/gr-cwradar /home/gabriel/gr-cwradar/lib /home/gabriel/gr-cwradar/build /home/gabriel/gr-cwradar/build/lib /home/gabriel/gr-cwradar/build/lib/CMakeFiles/test-cwradar.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : lib/CMakeFiles/test-cwradar.dir/depend
