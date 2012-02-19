cmake_minimum_required (VERSION 2.6)
project (sus107-dt)

# wxWidgets
find_package(wxWidgets COMPONENTS core base adv stc richtext xml REQUIRED)
include( "${wxWidgets_USE_FILE}" )

#find_package(GTK2 COMPONENTS gtk REQUIRED)
#include_directories(${GTK2_INCLUDE_DIRS})

#find_package(X11 REQUIRED)
#include_directories(${X11_INCLUDE_DIR})

#include_directories(
#	"/home/mirek/madedit/charset-detector/include/"
#	"/home/mirek/madedit/charset-detector/src/"
#	"/home/mirek/madedit/charset-detector/src/tables/"
#)

add_executable(sus107-dt
	src/main.cpp
	src/Emulator.cpp
	src/Debugger.cpp
	src/DebuggerView.cpp
	src/RegistersView.cpp
	src/DebuggerCodeGui.cpp
	
	src/Machine.cpp
	src/Cpu.cpp
	src/Ula.cpp
	src/Memory.cpp
	src/PortDevice.cpp
	src/Ports.cpp
	src/TapeRecorder.cpp
	src/util/Timer.cpp
	src/Keyboard.cpp
	
	src/widgets/wxHexCtrl/wxHexCtrl.cpp
	src/widgets/wxHexCtrl/Tag.cpp
	src/widgets/wxHexCtrl/TagDialogGui.cpp
	
	src/widgets/HexEditorCtrl/HexEditorCtrl.cpp
	src/widgets/HexEditorCtrl/HexEditorCtrlGui.cpp
	
#	src/widgets/MadEdit/MadEdit.cpp
#	src/widgets/MadEdit/MadSyntax.cpp
#	src/widgets/MadEdit/MadEncoding.cpp
#	src/widgets/MadEdit/MadLines.cpp
#	src/widgets/MadEdit/MadUndo.cpp
#	src/widgets/MadEdit/MadEditBasic.cpp
#	src/widgets/MadEdit/MadEdit_gtk.cpp
#	src/widgets/MadEdit/clipbrd_gtk.cpp
#	src/widgets/MadEdit/MadEditAdvanced.cpp
#	src/widgets/MadEdit/caret_new.cpp
#	src/widgets/MadEdit/MadEditCommand.cpp
#	src/widgets/MadEdit/MadEncoding.cpp
#	src/widgets/MadEdit/charset-detector/src/entry/impl.cpp
#	src/widgets/MadEdit/MadEditSearch.cpp
#	src/widgets/MadEdit/TradSimp.cpp
#	src/widgets/MadEdit/charset-detector/src/nsUniversalDetector.cpp
#	src/widgets/MadEdit/charset-detector/src/CharDistribution.cpp
#	src/widgets/MadEdit/charset-detector/src/JpCntx.cpp
#	src/widgets/MadEdit/charset-detector/src/LangBulgarianModel.cpp
#	src/widgets/MadEdit/charset-detector/src/LangCyrillicModel.cpp
#	src/widgets/MadEdit/charset-detector/src/LangGreekModel.cpp
#	src/widgets/MadEdit/charset-detector/src/LangHebrewModel.cpp
#	src/widgets/MadEdit/charset-detector/src/LangHungarianModel.cpp
#	src/widgets/MadEdit/charset-detector/src/LangThaiModel.cpp
#	src/widgets/MadEdit/charset-detector/src/nsBig5Prober.cpp
#	src/widgets/MadEdit/charset-detector/src/nsEscCharsetProber.cpp
#	src/widgets/MadEdit/charset-detector/src/nsEscSM.cpp
#	src/widgets/MadEdit/charset-detector/src/nsEUCJPProber.cpp
#	src/widgets/MadEdit/charset-detector/src/nsEUCKRProber.cpp
#	src/widgets/MadEdit/charset-detector/src/nsEUCTWProber.cpp
#	src/widgets/MadEdit/charset-detector/src/nsGB2312Prober.cpp
#	src/widgets/MadEdit/charset-detector/src/nsHebrewProber.cpp
#	src/widgets/MadEdit/charset-detector/src/nsCharSetProber.cpp
#	src/widgets/MadEdit/charset-detector/src/nsLatin1Prober.cpp
#	src/widgets/MadEdit/charset-detector/src/nsMBCSGroupProber.cpp
#	src/widgets/MadEdit/charset-detector/src/nsMBCSSM.cpp
#	src/widgets/MadEdit/charset-detector/src/nsSBCSGroupProber.cpp
#	src/widgets/MadEdit/charset-detector/src/nsSBCharSetProber.cpp
#	src/widgets/MadEdit/charset-detector/src/nsSJISProber.cpp
#	src/widgets/MadEdit/charset-detector/src/nsUniversalDetector.cpp
#	src/widgets/MadEdit/charset-detector/src/nsUTF8Prober.cpp
	
)
target_link_libraries (sus107-dt
	spectrum
	${wxWidgets_LIBRARIES}
	
#	${GTK2_LIBRARIES}
#	${X11_LIBRARIES}
)

## Compiler flags
if(CMAKE_BUILD_TYPE MATCHES "Release")
    set(CMAKE_CXX_FLAGS "-O2")
endif()

set (Z80EX_PATH "${PROJECT_SOURCE_DIR}/z80ex"
	CACHE PATH "Path to Z80Ex library")
include_directories("${Z80EX_PATH}/include")
set (Z80EX_BINARY_DIR "${PROJECT_BINARY_DIR}/lib_z80ex")
make_directory (${Z80EX_BINARY_DIR})
set (Z80EX_STATIC_ONLY true)
add_subdirectory (${Z80EX_PATH} ${Z80EX_BINARY_DIR})
link_directories (${Z80EX_BINARY_DIR})
target_link_libraries (sus107-dt z80ex-static z80ex_dasm-static)
