# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.21

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
CMAKE_COMMAND = /Applications/CMake.app/Contents/bin/cmake

# The command to remove a file.
RM = /Applications/CMake.app/Contents/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/mac_yfj/Desktop/nori_PT

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/mac_yfj/Desktop/nori_PT/build

# Include any dependencies generated for this target.
include CMakeFiles/warptest.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/warptest.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/warptest.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/warptest.dir/flags.make

CMakeFiles/warptest.dir/src/warp.cpp.o: CMakeFiles/warptest.dir/flags.make
CMakeFiles/warptest.dir/src/warp.cpp.o: ../src/warp.cpp
CMakeFiles/warptest.dir/src/warp.cpp.o: CMakeFiles/warptest.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/mac_yfj/Desktop/nori_PT/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/warptest.dir/src/warp.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/warptest.dir/src/warp.cpp.o -MF CMakeFiles/warptest.dir/src/warp.cpp.o.d -o CMakeFiles/warptest.dir/src/warp.cpp.o -c /Users/mac_yfj/Desktop/nori_PT/src/warp.cpp

CMakeFiles/warptest.dir/src/warp.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/warptest.dir/src/warp.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/mac_yfj/Desktop/nori_PT/src/warp.cpp > CMakeFiles/warptest.dir/src/warp.cpp.i

CMakeFiles/warptest.dir/src/warp.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/warptest.dir/src/warp.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/mac_yfj/Desktop/nori_PT/src/warp.cpp -o CMakeFiles/warptest.dir/src/warp.cpp.s

CMakeFiles/warptest.dir/src/warptest.cpp.o: CMakeFiles/warptest.dir/flags.make
CMakeFiles/warptest.dir/src/warptest.cpp.o: ../src/warptest.cpp
CMakeFiles/warptest.dir/src/warptest.cpp.o: CMakeFiles/warptest.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/mac_yfj/Desktop/nori_PT/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/warptest.dir/src/warptest.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/warptest.dir/src/warptest.cpp.o -MF CMakeFiles/warptest.dir/src/warptest.cpp.o.d -o CMakeFiles/warptest.dir/src/warptest.cpp.o -c /Users/mac_yfj/Desktop/nori_PT/src/warptest.cpp

CMakeFiles/warptest.dir/src/warptest.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/warptest.dir/src/warptest.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/mac_yfj/Desktop/nori_PT/src/warptest.cpp > CMakeFiles/warptest.dir/src/warptest.cpp.i

CMakeFiles/warptest.dir/src/warptest.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/warptest.dir/src/warptest.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/mac_yfj/Desktop/nori_PT/src/warptest.cpp -o CMakeFiles/warptest.dir/src/warptest.cpp.s

CMakeFiles/warptest.dir/src/microfacet.cpp.o: CMakeFiles/warptest.dir/flags.make
CMakeFiles/warptest.dir/src/microfacet.cpp.o: ../src/microfacet.cpp
CMakeFiles/warptest.dir/src/microfacet.cpp.o: CMakeFiles/warptest.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/mac_yfj/Desktop/nori_PT/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/warptest.dir/src/microfacet.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/warptest.dir/src/microfacet.cpp.o -MF CMakeFiles/warptest.dir/src/microfacet.cpp.o.d -o CMakeFiles/warptest.dir/src/microfacet.cpp.o -c /Users/mac_yfj/Desktop/nori_PT/src/microfacet.cpp

CMakeFiles/warptest.dir/src/microfacet.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/warptest.dir/src/microfacet.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/mac_yfj/Desktop/nori_PT/src/microfacet.cpp > CMakeFiles/warptest.dir/src/microfacet.cpp.i

CMakeFiles/warptest.dir/src/microfacet.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/warptest.dir/src/microfacet.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/mac_yfj/Desktop/nori_PT/src/microfacet.cpp -o CMakeFiles/warptest.dir/src/microfacet.cpp.s

CMakeFiles/warptest.dir/src/object.cpp.o: CMakeFiles/warptest.dir/flags.make
CMakeFiles/warptest.dir/src/object.cpp.o: ../src/object.cpp
CMakeFiles/warptest.dir/src/object.cpp.o: CMakeFiles/warptest.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/mac_yfj/Desktop/nori_PT/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/warptest.dir/src/object.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/warptest.dir/src/object.cpp.o -MF CMakeFiles/warptest.dir/src/object.cpp.o.d -o CMakeFiles/warptest.dir/src/object.cpp.o -c /Users/mac_yfj/Desktop/nori_PT/src/object.cpp

CMakeFiles/warptest.dir/src/object.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/warptest.dir/src/object.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/mac_yfj/Desktop/nori_PT/src/object.cpp > CMakeFiles/warptest.dir/src/object.cpp.i

CMakeFiles/warptest.dir/src/object.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/warptest.dir/src/object.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/mac_yfj/Desktop/nori_PT/src/object.cpp -o CMakeFiles/warptest.dir/src/object.cpp.s

CMakeFiles/warptest.dir/src/proplist.cpp.o: CMakeFiles/warptest.dir/flags.make
CMakeFiles/warptest.dir/src/proplist.cpp.o: ../src/proplist.cpp
CMakeFiles/warptest.dir/src/proplist.cpp.o: CMakeFiles/warptest.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/mac_yfj/Desktop/nori_PT/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/warptest.dir/src/proplist.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/warptest.dir/src/proplist.cpp.o -MF CMakeFiles/warptest.dir/src/proplist.cpp.o.d -o CMakeFiles/warptest.dir/src/proplist.cpp.o -c /Users/mac_yfj/Desktop/nori_PT/src/proplist.cpp

CMakeFiles/warptest.dir/src/proplist.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/warptest.dir/src/proplist.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/mac_yfj/Desktop/nori_PT/src/proplist.cpp > CMakeFiles/warptest.dir/src/proplist.cpp.i

CMakeFiles/warptest.dir/src/proplist.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/warptest.dir/src/proplist.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/mac_yfj/Desktop/nori_PT/src/proplist.cpp -o CMakeFiles/warptest.dir/src/proplist.cpp.s

CMakeFiles/warptest.dir/src/common.cpp.o: CMakeFiles/warptest.dir/flags.make
CMakeFiles/warptest.dir/src/common.cpp.o: ../src/common.cpp
CMakeFiles/warptest.dir/src/common.cpp.o: CMakeFiles/warptest.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/mac_yfj/Desktop/nori_PT/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/warptest.dir/src/common.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/warptest.dir/src/common.cpp.o -MF CMakeFiles/warptest.dir/src/common.cpp.o.d -o CMakeFiles/warptest.dir/src/common.cpp.o -c /Users/mac_yfj/Desktop/nori_PT/src/common.cpp

CMakeFiles/warptest.dir/src/common.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/warptest.dir/src/common.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/mac_yfj/Desktop/nori_PT/src/common.cpp > CMakeFiles/warptest.dir/src/common.cpp.i

CMakeFiles/warptest.dir/src/common.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/warptest.dir/src/common.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/mac_yfj/Desktop/nori_PT/src/common.cpp -o CMakeFiles/warptest.dir/src/common.cpp.s

# Object files for target warptest
warptest_OBJECTS = \
"CMakeFiles/warptest.dir/src/warp.cpp.o" \
"CMakeFiles/warptest.dir/src/warptest.cpp.o" \
"CMakeFiles/warptest.dir/src/microfacet.cpp.o" \
"CMakeFiles/warptest.dir/src/object.cpp.o" \
"CMakeFiles/warptest.dir/src/proplist.cpp.o" \
"CMakeFiles/warptest.dir/src/common.cpp.o"

# External object files for target warptest
warptest_EXTERNAL_OBJECTS =

warptest: CMakeFiles/warptest.dir/src/warp.cpp.o
warptest: CMakeFiles/warptest.dir/src/warptest.cpp.o
warptest: CMakeFiles/warptest.dir/src/microfacet.cpp.o
warptest: CMakeFiles/warptest.dir/src/object.cpp.o
warptest: CMakeFiles/warptest.dir/src/proplist.cpp.o
warptest: CMakeFiles/warptest.dir/src/common.cpp.o
warptest: CMakeFiles/warptest.dir/build.make
warptest: ext_build/tbb/libtbb_static.a
warptest: ext_build/nanogui/libnanogui.a
warptest: CMakeFiles/warptest.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/mac_yfj/Desktop/nori_PT/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Linking CXX executable warptest"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/warptest.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/warptest.dir/build: warptest
.PHONY : CMakeFiles/warptest.dir/build

CMakeFiles/warptest.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/warptest.dir/cmake_clean.cmake
.PHONY : CMakeFiles/warptest.dir/clean

CMakeFiles/warptest.dir/depend:
	cd /Users/mac_yfj/Desktop/nori_PT/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/mac_yfj/Desktop/nori_PT /Users/mac_yfj/Desktop/nori_PT /Users/mac_yfj/Desktop/nori_PT/build /Users/mac_yfj/Desktop/nori_PT/build /Users/mac_yfj/Desktop/nori_PT/build/CMakeFiles/warptest.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/warptest.dir/depend

