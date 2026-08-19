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
elseif(${PLATFORM} STREQUAL "SIMULATOR64COMBINED" OR
       ${PLATFORM} STREQUAL "SIMULATORARM64" OR
       ${PLATFORM} STREQUAL "SIMULATOR64")
    set(IOS_PLATFORM simulator)
endif()

##### PLUGIN NAME #####
set(pluginname gipIOS)


##### PLUGIN PATHS #####
set(IOS_PLUGIN_DIR ${CMAKE_CURRENT_LIST_DIR})

set(IOS_PLUGIN_LIBS_DIR ${IOS_PLUGIN_DIR}/libs)

# MoltenVK is too large to keep in the repository, so it is fetched once per
# machine into libs/moltenvk/<version> - see the Vulkan section of the README.
# An explicit cache value or an installed SDK in the environment wins over it, and
# when nothing is found the engine simply builds without the Vulkan backend.
set(GLIST_MOLTENVK_LIBRARY "" CACHE FILEPATH "Path to the matching MoltenVK library")
set(GLIST_MOLTENVK_INCLUDE_DIR "" CACHE PATH "Directory containing vulkan/vulkan.h")
if(IOS_PLATFORM STREQUAL "simulator")
    set(GLIST_MOLTENVK_SLICE "ios-arm64_x86_64-simulator")
else()
    set(GLIST_MOLTENVK_SLICE "ios-arm64")
endif()
file(GLOB GLIST_MOLTENVK_BUNDLED "${IOS_PLUGIN_LIBS_DIR}/moltenvk/*")
list(SORT GLIST_MOLTENVK_BUNDLED)
list(REVERSE GLIST_MOLTENVK_BUNDLED)
if(NOT GLIST_MOLTENVK_INCLUDE_DIR)
    # find_path does nothing while the cache entry holds a value, and the empty
    # default above is one, so the entry has to go before the search can run. The
    # iOS toolchain restricts the find commands to the SDK, which is why these look
    # outside the find root as well.
    unset(GLIST_MOLTENVK_INCLUDE_DIR CACHE)
    find_path(GLIST_MOLTENVK_INCLUDE_DIR vulkan/vulkan.h
        HINTS ENV VULKAN_SDK ENV MOLTENVK_HOME ${GLIST_MOLTENVK_BUNDLED}
        PATH_SUFFIXES include MoltenVK/include
        NO_CMAKE_FIND_ROOT_PATH)
endif()
if(NOT GLIST_MOLTENVK_LIBRARY)
    unset(GLIST_MOLTENVK_LIBRARY CACHE)
    find_library(GLIST_MOLTENVK_LIBRARY NAMES MoltenVK
        HINTS ENV VULKAN_SDK ENV MOLTENVK_HOME ${GLIST_MOLTENVK_BUNDLED}
        PATH_SUFFIXES
            "lib/${GLIST_MOLTENVK_SLICE}"
            lib lib/MoltenVK
            "MoltenVK/MoltenVK.xcframework/${GLIST_MOLTENVK_SLICE}"
            "MoltenVK.xcframework/${GLIST_MOLTENVK_SLICE}"
        NO_CMAKE_FIND_ROOT_PATH)
endif()
if(GLIST_MOLTENVK_LIBRARY AND GLIST_MOLTENVK_INCLUDE_DIR)
    if(NOT TARGET MoltenVK)
        add_library(MoltenVK UNKNOWN IMPORTED)
        set_target_properties(MoltenVK PROPERTIES IMPORTED_LOCATION "${GLIST_MOLTENVK_LIBRARY}")
    endif()
    list(APPEND IOS_PLUGIN_INCLUDES "${GLIST_MOLTENVK_INCLUDE_DIR}")
    list(APPEND IOS_PLUGIN_LINKLIBS MoltenVK)
    set(Lib_Vulkan MoltenVK)
    set(GLIST_IOS_HAS_VULKAN ON)
    message(STATUS "iOS Vulkan enabled with MoltenVK: ${GLIST_MOLTENVK_LIBRARY}")
else()
    message(STATUS "iOS MoltenVK not configured; Vulkan backend disabled")
endif()

##### PLUGIN SOURCES #####
list(APPEND IOS_PLUGIN_SRCS
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
			#/Library/Developer/CommandLineTools/usr/include/c++/v1
)

add_library(assimp    STATIC IMPORTED)
add_library(freetype  STATIC IMPORTED)
add_library(libssl    STATIC IMPORTED)
add_library(libcrypto STATIC IMPORTED)
add_library(libcurl   STATIC IMPORTED)

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
add_apple_library(Metal)
add_apple_library(QuartzCore)
add_apple_library(CoreGraphics)
add_apple_library(IOSurface)
add_apple_library(AVFAudio)
add_apple_library(AudioToolbox)
add_apple_library(AVKit)
add_apple_library(AVFoundation)

##### PLUGIN DEPENDENCY LINKING (IF ANY) #####					
list(APPEND IOS_PLUGIN_LINKLIBS
	assimp
	freetype
	libssl
	libcrypto
	libcurl
	z
)

if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Darwin")
	set(SHADERTOHEADER_EXEC_PATH "${IOS_PLUGIN_DIR}/tools/GlistShaderToHeader-macos-universal")
else()
	message(FATAL_ERROR "iOS builds require a macOS host")
endif()
