cmake_minimum_required (VERSION 2.6)
project (DP)

# wxWidgets
find_package(wxWidgets COMPONENTS core base REQUIRED)
include( "${wxWidgets_USE_FILE}" )

add_executable(dp
	src/main.cpp
	src/Emulator.cpp
	src/Machine.cpp
	src/Cpu.cpp
	src/Ula.cpp
	src/Memory.cpp
	src/PortDevice.cpp
	src/Ports.cpp
	src/TapeRecorder.cpp
	src/util/Timer.cpp
	src/Keyboard.cpp
)
target_link_libraries (dp
	spectrum
	opencv_core opencv_imgproc opencv_highgui opencv_ml opencv_video opencv_features2d opencv_calib3d opencv_objdetect opencv_contrib opencv_legacy opencv_flann
	${wxWidgets_LIBRARIES}
)

## Compiler flags
if(CMAKE_BUILD_TYPE MATCHES "Release")
    set(CMAKE_CXX_FLAGS "-O2")
endif()

set (Z80EX_PATH "${PROJECT_SOURCE_DIR}/../z80ex"
	CACHE PATH "Path to Z80Ex library")
include_directories("${Z80EX_PATH}/include")
set (Z80EX_BINARY_DIR "${PROJECT_BINARY_DIR}/lib_z80ex")
make_directory (${Z80EX_BINARY_DIR})
set (Z80EX_STATIC_ONLY true)
add_subdirectory (${Z80EX_PATH} ${Z80EX_BINARY_DIR})
link_directories (${Z80EX_BINARY_DIR})
target_link_libraries (dp z80ex-static z80ex_dasm-static)