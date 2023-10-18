cmake_minimum_required (VERSION 3.27)

macro(add_apple_library lib)
    set(LIB_NAME ${lib}_LIB)
    find_library(${LIB_NAME} ${lib})
    list(APPEND IOS_PLUGIN_LINKLIBS ${${LIB_NAME}})

    message("LIBRARY NAME: ${${LIB_NAME}}")
endmacro()

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    message(FATAL_ERROR "gipIOS cannot be built without a toolchain for iOS.")
endif()

include(CheckTypeSize)
check_type_size(time_t SIZEOF_TIME_T)

get_property(IOS_PLATFORM SOURCE ${CMAKE_TOOLCHAIN_FILE} PROPERTY PLATFORM)

if(${PLATFORM} STREQUAL "OS64")
    set(IOS_PLATFORM os)
elseif(${PLATFORM} STREQUAL "SIMULATOR64COMBINED")
    set(IOS_PLATFORM simulator)
endif()

##### PLUGIN NAME #####
set(pluginname gipIOS)


##### PLUGIN PATHS #####
set(IOS_PLUGIN_DIR ${CMAKE_CURRENT_LIST_DIR})

set(IOS_PLUGIN_LIBS_DIR ${IOS_PLUGIN_DIR}/libs)

##### PLUGIN SOURCES #####
list(APPEND IOS_PLUGIN_SRCS
	${IOS_PLUGIN_LIBS_DIR}/sqlite3.c
	${IOS_PLUGIN_DIR}/src/gIOSMain.mm
	${IOS_PLUGIN_DIR}/src/gIOSWindow.cpp
	${IOS_PLUGIN_DIR}/src/gIOSApp.cpp
	${IOS_PLUGIN_DIR}/src/gIOSCanvas.cpp
	${IOS_PLUGIN_DIR}/src/gIOSViewController.mm
	${IOS_PLUGIN_DIR}/src/gIOSAppDelegate.mm
	${IOS_PLUGIN_DIR}/src/gIOSInterface.cpp
	${IOS_PLUGIN_DIR}/src/gIOSUtil.cpp
	${IOS_PLUGIN_DIR}/src/gIOSUtil.mm
)

set(IOS_PLUGIN_HEADERS
	${IOS_PLUGIN_DIR}/src/gIOSApp.h
	${IOS_PLUGIN_DIR}/src/gIOSAppDelegate.h
	${IOS_PLUGIN_DIR}/src/gIOSCanvas.h
	${IOS_PLUGIN_DIR}/src/gIOSInterface.h
	${IOS_PLUGIN_DIR}/src/gIOSMain.h
	${IOS_PLUGIN_DIR}/src/gIOSViewController.h
	${IOS_PLUGIN_DIR}/src/gIOSWindow.h
	${IOS_PLUGIN_DIR}/src/gIOSUtil.h
)

##### PLUGIN HEADERS #####
list(APPEND IOS_PLUGIN_INCLUDES
			${IOS_PLUGIN_DIR}/src
			${IOS_PLUGIN_LIBS_DIR}/include
			${IOS_PLUGIN_LIBS_DIR}/include/freetype2
			${BREW_PATH}/llvm/include/c++/v1
			#/Library/Developer/CommandLineTools/usr/include/c++/v1
)

add_library(fmod      STATIC IMPORTED)
add_library(fmodL     STATIC IMPORTED)
add_library(assimp    STATIC IMPORTED)
add_library(freetype  STATIC IMPORTED)
add_library(libssl    STATIC IMPORTED)
add_library(libcrypto STATIC IMPORTED)
add_library(libcurl   STATIC IMPORTED)

set_target_properties(fmod      PROPERTIES IMPORTED_LOCATION ${IOS_PLUGIN_LIBS_DIR}/lib/ios/${IOS_PLATFORM}/libfmod.a)
set_target_properties(fmodL     PROPERTIES IMPORTED_LOCATION ${IOS_PLUGIN_LIBS_DIR}/lib/ios/${IOS_PLATFORM}/libfmodL.a)
set_target_properties(assimp    PROPERTIES IMPORTED_LOCATION ${IOS_PLUGIN_LIBS_DIR}/lib/ios/${IOS_PLATFORM}/libassimp.a)
set_target_properties(freetype  PROPERTIES IMPORTED_LOCATION ${IOS_PLUGIN_LIBS_DIR}/lib/ios/${IOS_PLATFORM}/libfreetype.a)
set_target_properties(libssl    PROPERTIES IMPORTED_LOCATION ${IOS_PLUGIN_LIBS_DIR}/lib/ios/${IOS_PLATFORM}/libssl.a)
set_target_properties(libcrypto PROPERTIES IMPORTED_LOCATION ${IOS_PLUGIN_LIBS_DIR}/lib/ios/${IOS_PLATFORM}/libcrypto.a)
set_target_properties(libcurl   PROPERTIES IMPORTED_LOCATION ${IOS_PLUGIN_LIBS_DIR}/lib/ios/${IOS_PLATFORM}/libcurl.a)

# APPLE FRAMEWORKS
add_apple_library(UIKit)
add_apple_library(Foundation)
add_apple_library(MobileCoreServices)
#add_apple_library(CFNetwork)
#add_apple_library(SystemConfiguration)
add_apple_library(OpenGLES)
add_apple_library(GLKit)
add_apple_library(AVFAudio)
add_apple_library(AudioToolbox)
add_apple_library(AVKit)
add_apple_library(AVFoundation)

##### PLUGIN DEPENDENCY LINKING (IF ANY) #####					
list(APPEND IOS_PLUGIN_LINKLIBS
	fmod
	fmodL
	assimp
	freetype
	libssl
	libcrypto
	libcurl
	z
)