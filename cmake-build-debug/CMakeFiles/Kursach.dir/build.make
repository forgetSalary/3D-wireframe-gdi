# CMAKE generated file: DO NOT EDIT!
# Generated by "NMake Makefiles" Generator, CMake Version 3.17

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


.SUFFIXES: .hpux_make_needs_suffix_list


# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE
NULL=nul
!ENDIF
SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\JetBrains\CLion 2020.2.5\bin\cmake\win\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\JetBrains\CLion 2020.2.5\bin\cmake\win\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\code\CPrctc\Kursach

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\code\CPrctc\Kursach\cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles\Kursach.dir\depend.make

# Include the progress variables for this target.
include CMakeFiles\Kursach.dir\progress.make

# Include the compile flags for this target's objects.
include CMakeFiles\Kursach.dir\flags.make

CMakeFiles\Kursach.dir\main.cpp.obj: CMakeFiles\Kursach.dir\flags.make
CMakeFiles\Kursach.dir\main.cpp.obj: ..\main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\code\CPrctc\Kursach\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Kursach.dir/main.cpp.obj"
	C:\PROGRA~2\MICROS~2\2019\COMMUN~1\VC\Tools\MSVC\1428~1.293\bin\Hostx86\x86\cl.exe @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoCMakeFiles\Kursach.dir\main.cpp.obj /FdCMakeFiles\Kursach.dir\ /FS -c C:\code\CPrctc\Kursach\main.cpp
<<

CMakeFiles\Kursach.dir\main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Kursach.dir/main.cpp.i"
	C:\PROGRA~2\MICROS~2\2019\COMMUN~1\VC\Tools\MSVC\1428~1.293\bin\Hostx86\x86\cl.exe > CMakeFiles\Kursach.dir\main.cpp.i @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\code\CPrctc\Kursach\main.cpp
<<

CMakeFiles\Kursach.dir\main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Kursach.dir/main.cpp.s"
	C:\PROGRA~2\MICROS~2\2019\COMMUN~1\VC\Tools\MSVC\1428~1.293\bin\Hostx86\x86\cl.exe @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoNUL /FAs /FaCMakeFiles\Kursach.dir\main.cpp.s /c C:\code\CPrctc\Kursach\main.cpp
<<

CMakeFiles\Kursach.dir\3D.cpp.obj: CMakeFiles\Kursach.dir\flags.make
CMakeFiles\Kursach.dir\3D.cpp.obj: ..\3D.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\code\CPrctc\Kursach\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/Kursach.dir/3D.cpp.obj"
	C:\PROGRA~2\MICROS~2\2019\COMMUN~1\VC\Tools\MSVC\1428~1.293\bin\Hostx86\x86\cl.exe @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoCMakeFiles\Kursach.dir\3D.cpp.obj /FdCMakeFiles\Kursach.dir\ /FS -c C:\code\CPrctc\Kursach\3D.cpp
<<

CMakeFiles\Kursach.dir\3D.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Kursach.dir/3D.cpp.i"
	C:\PROGRA~2\MICROS~2\2019\COMMUN~1\VC\Tools\MSVC\1428~1.293\bin\Hostx86\x86\cl.exe > CMakeFiles\Kursach.dir\3D.cpp.i @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\code\CPrctc\Kursach\3D.cpp
<<

CMakeFiles\Kursach.dir\3D.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Kursach.dir/3D.cpp.s"
	C:\PROGRA~2\MICROS~2\2019\COMMUN~1\VC\Tools\MSVC\1428~1.293\bin\Hostx86\x86\cl.exe @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoNUL /FAs /FaCMakeFiles\Kursach.dir\3D.cpp.s /c C:\code\CPrctc\Kursach\3D.cpp
<<

CMakeFiles\Kursach.dir\arena.cpp.obj: CMakeFiles\Kursach.dir\flags.make
CMakeFiles\Kursach.dir\arena.cpp.obj: ..\arena.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\code\CPrctc\Kursach\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/Kursach.dir/arena.cpp.obj"
	C:\PROGRA~2\MICROS~2\2019\COMMUN~1\VC\Tools\MSVC\1428~1.293\bin\Hostx86\x86\cl.exe @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoCMakeFiles\Kursach.dir\arena.cpp.obj /FdCMakeFiles\Kursach.dir\ /FS -c C:\code\CPrctc\Kursach\arena.cpp
<<

CMakeFiles\Kursach.dir\arena.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Kursach.dir/arena.cpp.i"
	C:\PROGRA~2\MICROS~2\2019\COMMUN~1\VC\Tools\MSVC\1428~1.293\bin\Hostx86\x86\cl.exe > CMakeFiles\Kursach.dir\arena.cpp.i @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\code\CPrctc\Kursach\arena.cpp
<<

CMakeFiles\Kursach.dir\arena.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Kursach.dir/arena.cpp.s"
	C:\PROGRA~2\MICROS~2\2019\COMMUN~1\VC\Tools\MSVC\1428~1.293\bin\Hostx86\x86\cl.exe @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoNUL /FAs /FaCMakeFiles\Kursach.dir\arena.cpp.s /c C:\code\CPrctc\Kursach\arena.cpp
<<

# Object files for target Kursach
Kursach_OBJECTS = \
"CMakeFiles\Kursach.dir\main.cpp.obj" \
"CMakeFiles\Kursach.dir\3D.cpp.obj" \
"CMakeFiles\Kursach.dir\arena.cpp.obj"

# External object files for target Kursach
Kursach_EXTERNAL_OBJECTS =

Kursach.exe: CMakeFiles\Kursach.dir\main.cpp.obj
Kursach.exe: CMakeFiles\Kursach.dir\3D.cpp.obj
Kursach.exe: CMakeFiles\Kursach.dir\arena.cpp.obj
Kursach.exe: CMakeFiles\Kursach.dir\build.make
Kursach.exe: CMakeFiles\Kursach.dir\objects1.rsp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\code\CPrctc\Kursach\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable Kursach.exe"
	"C:\Program Files\JetBrains\CLion 2020.2.5\bin\cmake\win\bin\cmake.exe" -E vs_link_exe --intdir=CMakeFiles\Kursach.dir --rc=C:\PROGRA~2\WI3CF2~1\10\bin\100183~1.0\x86\rc.exe --mt=C:\PROGRA~2\WI3CF2~1\10\bin\100183~1.0\x86\mt.exe --manifests  -- C:\PROGRA~2\MICROS~2\2019\COMMUN~1\VC\Tools\MSVC\1428~1.293\bin\Hostx86\x86\link.exe /nologo @CMakeFiles\Kursach.dir\objects1.rsp @<<
 /out:Kursach.exe /implib:Kursach.lib /pdb:C:\code\CPrctc\Kursach\cmake-build-debug\Kursach.pdb /version:0.0  /machine:X86 /debug /INCREMENTAL /subsystem:windows  kernel32.lib user32.lib gdi32.lib winspool.lib shell32.lib ole32.lib oleaut32.lib uuid.lib comdlg32.lib advapi32.lib 
<<

# Rule to build all files generated by this target.
CMakeFiles\Kursach.dir\build: Kursach.exe

.PHONY : CMakeFiles\Kursach.dir\build

CMakeFiles\Kursach.dir\clean:
	$(CMAKE_COMMAND) -P CMakeFiles\Kursach.dir\cmake_clean.cmake
.PHONY : CMakeFiles\Kursach.dir\clean

CMakeFiles\Kursach.dir\depend:
	$(CMAKE_COMMAND) -E cmake_depends "NMake Makefiles" C:\code\CPrctc\Kursach C:\code\CPrctc\Kursach C:\code\CPrctc\Kursach\cmake-build-debug C:\code\CPrctc\Kursach\cmake-build-debug C:\code\CPrctc\Kursach\cmake-build-debug\CMakeFiles\Kursach.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles\Kursach.dir\depend

