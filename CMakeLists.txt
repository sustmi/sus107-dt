cmake_minimum_required (VERSION 2.6)
project (sus107-dt)

# wxWidgets library
find_package(wxWidgets COMPONENTS core base adv stc richtext xml REQUIRED)
include( "${wxWidgets_USE_FILE}" )

# Portaudio library
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
find_package(Portaudio REQUIRED)
include_directories(${PORTAUDIO_INCLUDE_DIRS})

add_executable(sus107-dt
	src/main.cpp
	
	src/Emulator.cpp
	src/EmulatorListener.cpp
	src/Debugger.cpp
	src/DebuggerListener.cpp
	
	src/Machine.cpp
	src/Cpu.cpp
	src/Ula.cpp
	src/Memory.cpp
	src/PortDevice.cpp
	src/Ports.cpp
	src/TapeRecorder.cpp
	src/Keyboard.cpp
	src/Joystick.cpp
	src/Speaker.cpp
	src/Clock.cpp
	
	src/ui/icons.cpp
	src/ui/GridCellBitmapRenderer.cpp
	
	src/ui/EmulatorView.cpp
	src/ui/DebuggerView.cpp
	src/ui/DebuggerRegistersView.cpp
	src/ui/DebuggerCodeGui.cpp
	src/ui/DebuggerHexGui.cpp
	
	src/ui/widgets/HexEditorCtrl/HexEditorCtrl.cpp
	src/ui/widgets/HexEditorCtrl/HexEditorCtrlGui.cpp
	src/ui/widgets/HexEditorCtrl/wxHexCtrl/wxHexCtrl.cpp
	src/ui/widgets/HexEditorCtrl/wxHexCtrl/Tag.cpp
	src/ui/widgets/HexEditorCtrl/wxHexCtrl/TagDialogGui.cpp
)

target_link_libraries (sus107-dt
	spectrum
	${wxWidgets_LIBRARIES}
	${PORTAUDIO_LIBRARIES}
)

## Compiler flags
if(CMAKE_BUILD_TYPE MATCHES "Release")
    set(CMAKE_CXX_FLAGS "-O2")
endif()

# z80ex library (static) 
set (Z80EX_PATH "${PROJECT_SOURCE_DIR}/lib/z80ex"
	CACHE PATH "Path to Z80Ex library")
include_directories("${Z80EX_PATH}/include")
set (Z80EX_BINARY_DIR "${PROJECT_BINARY_DIR}/lib_z80ex")
make_directory (${Z80EX_BINARY_DIR})
set (Z80EX_STATIC_ONLY true)
add_subdirectory (${Z80EX_PATH} ${Z80EX_BINARY_DIR})
link_directories (${Z80EX_BINARY_DIR})
target_link_libraries (sus107-dt z80ex-static z80ex_dasm-static)
