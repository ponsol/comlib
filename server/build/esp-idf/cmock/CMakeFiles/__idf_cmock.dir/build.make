# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.20

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
CMAKE_SOURCE_DIR = /home/sjk/laptopdir/course/embedded/esp32/greenh/comlib/server

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/sjk/laptopdir/course/embedded/esp32/greenh/comlib/server/build

# Include any dependencies generated for this target.
include esp-idf/cmock/CMakeFiles/__idf_cmock.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include esp-idf/cmock/CMakeFiles/__idf_cmock.dir/compiler_depend.make

# Include the progress variables for this target.
include esp-idf/cmock/CMakeFiles/__idf_cmock.dir/progress.make

# Include the compile flags for this target's objects.
include esp-idf/cmock/CMakeFiles/__idf_cmock.dir/flags.make

esp-idf/cmock/CMakeFiles/__idf_cmock.dir/CMock/src/cmock.c.obj: esp-idf/cmock/CMakeFiles/__idf_cmock.dir/flags.make
esp-idf/cmock/CMakeFiles/__idf_cmock.dir/CMock/src/cmock.c.obj: /home/sjk/esp/esp-idf-4.4/components/cmock/CMock/src/cmock.c
esp-idf/cmock/CMakeFiles/__idf_cmock.dir/CMock/src/cmock.c.obj: esp-idf/cmock/CMakeFiles/__idf_cmock.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/sjk/laptopdir/course/embedded/esp32/greenh/comlib/server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object esp-idf/cmock/CMakeFiles/__idf_cmock.dir/CMock/src/cmock.c.obj"
	cd /home/sjk/laptopdir/course/embedded/esp32/greenh/comlib/server/build/esp-idf/cmock && /home/sjk/.espressif/tools/xtensa-esp32-elf/esp-2021r2-8.4.0/xtensa-esp32-elf/bin/xtensa-esp32-elf-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT esp-idf/cmock/CMakeFiles/__idf_cmock.dir/CMock/src/cmock.c.obj -MF CMakeFiles/__idf_cmock.dir/CMock/src/cmock.c.obj.d -o CMakeFiles/__idf_cmock.dir/CMock/src/cmock.c.obj -c /home/sjk/esp/esp-idf-4.4/components/cmock/CMock/src/cmock.c

esp-idf/cmock/CMakeFiles/__idf_cmock.dir/CMock/src/cmock.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/__idf_cmock.dir/CMock/src/cmock.c.i"
	cd /home/sjk/laptopdir/course/embedded/esp32/greenh/comlib/server/build/esp-idf/cmock && /home/sjk/.espressif/tools/xtensa-esp32-elf/esp-2021r2-8.4.0/xtensa-esp32-elf/bin/xtensa-esp32-elf-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/sjk/esp/esp-idf-4.4/components/cmock/CMock/src/cmock.c > CMakeFiles/__idf_cmock.dir/CMock/src/cmock.c.i

esp-idf/cmock/CMakeFiles/__idf_cmock.dir/CMock/src/cmock.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/__idf_cmock.dir/CMock/src/cmock.c.s"
	cd /home/sjk/laptopdir/course/embedded/esp32/greenh/comlib/server/build/esp-idf/cmock && /home/sjk/.espressif/tools/xtensa-esp32-elf/esp-2021r2-8.4.0/xtensa-esp32-elf/bin/xtensa-esp32-elf-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/sjk/esp/esp-idf-4.4/components/cmock/CMock/src/cmock.c -o CMakeFiles/__idf_cmock.dir/CMock/src/cmock.c.s

# Object files for target __idf_cmock
__idf_cmock_OBJECTS = \
"CMakeFiles/__idf_cmock.dir/CMock/src/cmock.c.obj"

# External object files for target __idf_cmock
__idf_cmock_EXTERNAL_OBJECTS =

esp-idf/cmock/libcmock.a: esp-idf/cmock/CMakeFiles/__idf_cmock.dir/CMock/src/cmock.c.obj
esp-idf/cmock/libcmock.a: esp-idf/cmock/CMakeFiles/__idf_cmock.dir/build.make
esp-idf/cmock/libcmock.a: esp-idf/cmock/CMakeFiles/__idf_cmock.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/sjk/laptopdir/course/embedded/esp32/greenh/comlib/server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C static library libcmock.a"
	cd /home/sjk/laptopdir/course/embedded/esp32/greenh/comlib/server/build/esp-idf/cmock && $(CMAKE_COMMAND) -P CMakeFiles/__idf_cmock.dir/cmake_clean_target.cmake
	cd /home/sjk/laptopdir/course/embedded/esp32/greenh/comlib/server/build/esp-idf/cmock && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/__idf_cmock.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
esp-idf/cmock/CMakeFiles/__idf_cmock.dir/build: esp-idf/cmock/libcmock.a
.PHONY : esp-idf/cmock/CMakeFiles/__idf_cmock.dir/build

esp-idf/cmock/CMakeFiles/__idf_cmock.dir/clean:
	cd /home/sjk/laptopdir/course/embedded/esp32/greenh/comlib/server/build/esp-idf/cmock && $(CMAKE_COMMAND) -P CMakeFiles/__idf_cmock.dir/cmake_clean.cmake
.PHONY : esp-idf/cmock/CMakeFiles/__idf_cmock.dir/clean

esp-idf/cmock/CMakeFiles/__idf_cmock.dir/depend:
	cd /home/sjk/laptopdir/course/embedded/esp32/greenh/comlib/server/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/sjk/laptopdir/course/embedded/esp32/greenh/comlib/server /home/sjk/esp/esp-idf-4.4/components/cmock /home/sjk/laptopdir/course/embedded/esp32/greenh/comlib/server/build /home/sjk/laptopdir/course/embedded/esp32/greenh/comlib/server/build/esp-idf/cmock /home/sjk/laptopdir/course/embedded/esp32/greenh/comlib/server/build/esp-idf/cmock/CMakeFiles/__idf_cmock.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : esp-idf/cmock/CMakeFiles/__idf_cmock.dir/depend

