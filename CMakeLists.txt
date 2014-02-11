##### THIS WILL NEED SOME HELP WITH  NON-Linux compiles

## CMake compatibility issues: don't modify this, please!
CMAKE_MINIMUM_REQUIRED( VERSION 2.4.6 )

if(COMMAND cmake_policy)
     cmake_policy(SET CMP0003 NEW)
	 cmake_policy(SET CMP0015 NEW)  # to supress relative library directory warnings
endif(COMMAND cmake_policy)

SET(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

IF (UNIX)
	EXECUTE_PROCESS(COMMAND sh -c "echo `uname`-`uname -m`" OUTPUT_VARIABLE VARIANT_DIRECTORY OUTPUT_STRIP_TRAILING_WHITESPACE)
	SET(BUILD_PLATFORM "${VARIANT_DIRECTORY}")
	MESSAGE(STATUS "Build Platform: ${BUILD_PLATFORM}")
ELSEIF(WIN32)
	SET( BUILD_PLATFORM "windows_x86_64")
ENDIF()

find_package(Maya)

if (MAYA_FOUND)


IF ( UNIX AND NOT APPLE )
	ADD_DEFINITIONS(-DLINUX)
ENDIF()
if( APPLE )
	ADD_DEFINITIONS(-DMAC_PLUGIN)
	ADD_DEFINITIONS(-DOSMac_)
	ADD_DEFINITIONS(-DOSMac_MachO)
ENDIF()

SET(CMAKE_CXX_WARNING_LEVEL 4)


MARK_AS_ADVANCED(CMAKE_BACKWARDS_COMPATIBILITY)
## allow more human readable "if then else" constructs
SET( CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS TRUE )
## Use verbose make file
SET ( CMAKE_VERBOSE_MAKEFILE TRUE )

set (PLUGIN_NAME "DisplaceD")
set (CMAKE_PROJECT_NAME "DisplaceD")

if(APPLE)
	set (PLUGIN_SUFFIX ".bundle")
	set (CMAKE_SHARED_LIBRARY_SUFFIX ".bundle")
	set (SUFFIX ".bundle")
ENDIF()

if(WIN32)
	set (PLUGIN_SUFFIX ".mll")
	set (CMAKE_SHARED_LIBRARY_SUFFIX ".mll")
	set (SUFFIX ".mll")
ENDIF()

if (UNIX)
	set (PLUGIN_SUFFIX ".so")
	set (CMAKE_SHARED_LIBRARY_SUFFIX ".so")
	set (SUFFIX ".so")
ENDIF()


set (STD_INCLUDES "." ".." "/usr/X11R6/include" "/usr/include" "/usr/include/CC")
set (FOOBAR "/usr/lib64")

include_directories (${STD_INCLUDES}  ${MAYA_INCLUDE_DIRS})
link_directories  ( ${FOOBAR} ${MAYA_LIBRARY_DIR})

add_definitions(-D_BOOL)
add_definitions(-DREQUIRE_IOSTREAM)
if (!WIN32)
	add_definitions(-fPIC)
	add_definitions(-O3)
	add_definitions(-pthread)
endif()


FILE(GLOB displaceDCPP "src/*.cpp")
FILE(GLOB displaceDINCLUDES "src/*.h")

SET(TARGET_H ${displaceDINCLUDES})
SET(TARGET_SRC ${displaceDCPP})

find_package(OpenGL REQUIRED)
if (OPENGL_FOUND)

if(APPLE)
	SET(OPENGL_gl_LIBRARY "-framework OpenGL")
	SET(OPENGL_glu_LIBRARY "-framework GLUT")
elseif(WIN32)

	SET(OPENGL_gl_LIBRARY "openGL32")
	SET(OPENGL_glu_LIBRARY "glu32")
else()
	SET(OPENGL_gl_LIBRARY "GL")
	SET(OPENGL_glu_LIBRARY "GLU")
endif()
endif(OPENGL_FOUND)

SET(MAYA_LIBS OpenMaya Foundation OpenMayaAnim OpenMayaFX OpenMayaUI OpenMayaRender)


ADD_LIBRARY(${PLUGIN_NAME} SHARED ${TARGET_H} ${TARGET_SRC})
set_target_properties(${PLUGIN_NAME} PROPERTIES PREFIX "")

## need these to build for windows or else the plugin won't load
IF(WIN32)
	set_target_properties(${PLUGIN_NAME} PROPERTIES LINK_FLAGS "/export:initializePlugin /export:uninitializePlugin")
ENDIF()

TARGET_LINK_LIBRARIES(${PLUGIN_NAME} ${MAYA_LIBS} ${OPENGL_gl_LIBRARY} ${OPENGL_glu_LIBRARY})

SET (DISPLACED_OUTPUT_PATH  ${CMAKE_SOURCE_DIR}/outputs/${BUILD_PLATFORM}/maya-${MAYA_VERSION}/plug-ins)
SET (DISPLACED_OUTPUTSCRIPTS_PATH  ${CMAKE_SOURCE_DIR}/outputs/${BUILD_PLATFORM}/maya-${MAYA_VERSION}/scripts)
SET (DISPLACED_OUTPUTICONS_PATH  ${CMAKE_SOURCE_DIR}/outputs/${BUILD_PLATFORM}/maya-${MAYA_VERSION}/icons)


INSTALL( TARGETS ${PLUGIN_NAME}  DESTINATION  ${DISPLACED_OUTPUT_PATH})

FILE(GLOB DISPLACED_SCRIPTS "scripts/*.mel")
INSTALL( FILES ${DISPLACED_SCRIPTS} DESTINATION ${DISPLACED_OUTPUTSCRIPTS_PATH})

FILE(GLOB DISPLACED_ICONS "icons/*")
INSTALL( FILES ${DISPLACED_ICONS} DESTINATION ${DISPLACED_OUTPUTICONS_PATH})


ENDIF(MAYA_FOUND)

