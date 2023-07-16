cmake_minimum_required (VERSION 3.26)

project ("g10")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)
set(CMAKE_PREFIX_PATH ${CMAKE_SOURCE_DIR}/extern/)

IF(WIN32)
    find_package(SDL2_image REQUIRED CONFIG REQUIRED COMPONENTS SDL2_image)
ELSEIF(LINUX)
    set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
    find_package(SDL2_image REQUIRED)
ELSEIF(APPLE)

ENDIF(WIN32)

# Find the array module
if ( NOT "${HAS_ARRAY}")
    
    # Log
    message("[G10] Adding array project")
    
    # Add the array directory
    add_subdirectory(${CMAKE_SOURCE_DIR}/extern/array ${CMAKE_SOURCE_DIR}/build/array)

    # Let any parent project know that this project has array
    set(HAS_ARRAY true PARENT_SCOPE)
endif()

# Find the dict module
if ( NOT "${HAS_DICT}")
    
    # Log
    message("[G10] Adding dict project")
    
    # Add the dict directory
    add_subdirectory(${CMAKE_SOURCE_DIR}/extern/dict ${CMAKE_SOURCE_DIR}/build/dict)

    # Let any parent project know that this project has dict
    set(HAS_DICT true PARENT_SCOPE)
endif()

# Find the queue module
if ( NOT "${HAS_QUEUE}")
    
    # Log
    message("[G10] Adding queue project")
    
    # Add the queue directory
    add_subdirectory(${CMAKE_SOURCE_DIR}/extern/queue ${CMAKE_SOURCE_DIR}/build/queue)

    # Let any parent project know that this project has queue
    set(HAS_QUEUE true PARENT_SCOPE)
endif()

# Find the stack module
if ( NOT "${HAS_STACK}")
    
    # Log
    message("[G10] Adding stack project")
    
    # Add the stack directory
    add_subdirectory(${CMAKE_SOURCE_DIR}/extern/stack ${CMAKE_SOURCE_DIR}/build/stack)

    # Let any parent project know that this project has stack
    set(HAS_STACK true PARENT_SCOPE)
endif()

# Find the sync module
if ( NOT "${HAS_SYNC}")

    # Log
    message("[G10] Adding sync project")

    # Add the sync directory
    add_subdirectory(${CMAKE_SOURCE_DIR}/extern/sync ${CMAKE_SOURCE_DIR}/build/sync)

    # Let any parent project know that this project has sync
    set(HAS_SYNC true PARENT_SCOPE)
endif()

# Find the JSON module
if ( NOT "${HAS_JSON}")
    
    # Log
    message("[G10] Adding JSON project")
    
    # Add the JSON directory
    add_subdirectory(${CMAKE_SOURCE_DIR}/extern/json ${CMAKE_SOURCE_DIR}/build/json)

    # Let any parent project know that this project has json
    set(HAS_JSON true PARENT_SCOPE)
endif()

if ( NOT "${HAS_G10}")
    set(HAS_G10 true PARENT_SCOPE)
endif()

option(AUTO_LOCATE_VULKAN "AUTO_LOCATE_VULKAN" ON)

find_package(SDL2 REQUIRED CONFIG REQUIRED COMPONENTS SDL2)
find_package(SDL2 REQUIRED CONFIG COMPONENTS SDL2main)
find_package(SDL2_image REQUIRED CONFIG REQUIRED COMPONENTS SDL2_image)
find_package(SDL2_net REQUIRED CONFIG REQUIRED COMPONENTS SDL2_net)

add_definitions(-D_CRT_SECURE_NO_WARNINGS)

if(AUTO_LOCATE_VULKAN)
	message(STATUS "Attempting auto locate Vulkan using CMake......")
	
	# Find Vulkan Path using CMake's Vulkan Module
	# This will return Boolean 'Vulkan_FOUND' indicating the status of find as success(ON) or fail(OFF).
	# Include directory path - 'Vulkan_INCLUDE_DIRS' and 'Vulkan_LIBRARY' with required libraries.
	find_package(Vulkan)
	
	# Try extracting VulkanSDK path from ${Vulkan_INCLUDE_DIRS}
	if (NOT ${Vulkan_INCLUDE_DIRS} STREQUAL "")
		set(VULKAN_PATH ${Vulkan_INCLUDE_DIRS})
		STRING(REGEX REPLACE "/Include" "" VULKAN_PATH ${VULKAN_PATH})
	endif()
		 
	if(NOT Vulkan_FOUND)
		# CMake may fail to locate the libraries but could be able to 
		# provide some path in Vulkan SDK include directory variable
		# 'Vulkan_INCLUDE_DIRS', try to extract path from this.
		message(STATUS "Failed to locate Vulkan SDK, retrying again...")
		if(EXISTS "${VULKAN_PATH}")
			message(STATUS "Successfully located the Vulkan SDK: ${VULKAN_PATH}")
		else()
			message("Error: Unable to locate Vulkan SDK. Please turn off auto locate option by specifying 'AUTO_LOCATE_VULKAN' as 'OFF'")
			message("and specify manually path using 'VULKAN_SDK' and 'VULKAN_VERSION' variables in the CMakeLists.txt.")
			return()
		endif()
	endif()
else()
	message(STATUS "Attempting to locate Vulkan SDK using manual path......")
	set(VULKAN_SDK "C:/VulkanSDK")
	set(VULKAN_VERSION "1.0.33.0")
	set(VULKAN_PATH "${VULKAN_SDK}/${VULKAN_VERSION}")
	message(STATUS "Using manual specified path: ${VULKAN_PATH}")

	# Check if manual set path exists
	if(NOT EXISTS "${VULKAN_PATH}")
		message("Error: Unable to locate this Vulkan SDK path VULKAN_PATH: ${VULKAN_PATH}, please specify correct path.
		For more information on correct installation process, please refer to subsection 'Getting started with Lunar-G SDK'
		and 'Setting up first project with CMake' in Chapter 3, 'Shaking hands with the device' in this book 'Learning Vulkan', ISBN - 9781786469809.")
	   return()
	endif()
endif()

add_definitions(-DVK_USE_PLATFORM_WIN32_KHR)

# vulkan-1 library for build Vulkan application.
set(VULKAN_LIB_LIST "vulkan-1")

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	# Include Vulkan header files from Vulkan SDK
	include_directories(AFTER ${VULKAN_PATH}/Include)

	# Link directory for vulkan-1
	link_directories(${VULKAN_PATH}/Bin;${VULKAN_PATH}/Lib;)
endif()

# Define directories and the contained folder and files inside.
if(WIN32)
    source_group("include" REGULAR_EXPRESSION "include/*")
    source_group("source" REGULAR_EXPRESSION "source/*")
endif(WIN32)

# G10 executable
add_executable (g10_internal_example "G10.c" "GXAI.c" "GXBV.c" "GXCamera.c" "GXCameraController.c" "GXCollider.c" "GXCollision.c" "GXEntity.c" "GXInput.c" "GXLinear.c" "GXMaterial.c" "GXPart.c" "GXPhysics.c" "GXPLY.c" "GXQuaternion.c" "GXRenderer.c" "GXRigidbody.c" "GXScene.c" "GXScheduler.c" "GXServer.c" "GXShader.c" "GXTransform.c" "GXUserCode.c" "main.c") 
#add_executable (g10_internal_example "Resource.rc")
add_dependencies(g10_internal_example json array dict stack queue sync)
target_include_directories(g10_internal_example PUBLIC include ${CMAKE_SOURCE_DIR}/extern/json/include/ ${CMAKE_SOURCE_DIR}/extern/array/include/ ${CMAKE_SOURCE_DIR}/extern/dict/include/ ${CMAKE_SOURCE_DIR}/extern/stack/include/ ${CMAKE_SOURCE_DIR}/extern/queue/include/ ${CMAKE_SOURCE_DIR}/extern/sync/include/) 
target_link_libraries(g10_internal_example PUBLIC json array dict stack queue sync ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${SDL2_NET_INCLUDE_DIRS} ${VULKAN_LIB_LIST} PRIVATE SDL2_image::SDL2_image SDL2_net::SDL2_net )

# G10 library
add_library (g10 SHARED "G10.c" "GXAI.c" "GXBV.c" "GXCamera.c" "GXCameraController.c" "GXCollider.c" "GXCollision.c" "GXEntity.c" "GXInput.c" "GXLinear.c" "GXMaterial.c" "GXPart.c" "GXPhysics.c" "GXPLY.c" "GXQuaternion.c" "GXRenderer.c" "GXRigidbody.c" "GXScene.c" "GXScheduler.c" "GXServer.c" "GXShader.c" "GXTransform.c" "GXUserCode.c") 
add_dependencies(g10 json array dict stack queue sync)
target_include_directories(g10 PUBLIC include ${CMAKE_SOURCE_DIR}/extern/json/include/ ${CMAKE_SOURCE_DIR}/extern/array/include/ ${CMAKE_SOURCE_DIR}/extern/dict/include/ ${CMAKE_SOURCE_DIR}/extern/stack/include/ ${CMAKE_SOURCE_DIR}/extern/queue/include/ ${CMAKE_SOURCE_DIR}/extern/sync/include/) 
target_link_libraries(g10 PUBLIC json array dict stack queue sync ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${SDL2_NET_INCLUDE_DIRS} ${VULKAN_LIB_LIST} PRIVATE SDL2_image::SDL2_image SDL2_net::SDL2_net )