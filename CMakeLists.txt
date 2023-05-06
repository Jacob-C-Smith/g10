cmake_minimum_required (VERSION 3.8)

project ("g10")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
set(CMAKE_PREFIX_PATH ${CMAKE_SOURCE_DIR}/extern/)

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

add_subdirectory(${PROJECT_SOURCE_DIR}/json)
add_subdirectory(${PROJECT_SOURCE_DIR}/queue)

find_library(array NAMES array HINTS ${PROJECT_SOURCE_DIR}/array/build/Debug/*)
find_library(queue NAMES queue HINTS ${PROJECT_SOURCE_DIR}/queue/build/Debug/*)
find_library(dict  NAMES dict HINTS  ${PROJECT_SOURCE_DIR}/dict/build/Debug/*)
find_library(json  NAMES json HINTS  ${PROJECT_SOURCE_DIR}/json/build/Debug/*)

include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS} ${SDL2_NET_INCLUDE_DIRS} ${VULKAN_INCLUDE_DIRS})

# Add source to this project's executable.
add_executable (g10_example "G10.c" "GXInput.c" "GXAI.c" "GXCamera.c" "GXScheduler.c" "GXLinear.c" "GXPart.c" "GXMaterial.c" "GXQuaternion.c" "GXUserCode.c" "GXTransform.c" "main.c") # "GXAudio.c" "GXBV.c" "GXCameraController.c" "GXCollider.c" "GXCollision.c" "GXDiscordIntegration.c" "GXEntity.c" "GXMaterial.c" "GXPLY.c" "GXPart.c" "GXPhysics.c" "GXQuaternion.c" "GXRenderer.c" "GXRigidbody.c"  "GXScheduler.c" "GXServer.c" "GXTexture.c" "GXTransform.c" "APS_3rdPersonCtrl.c"

# Add source to this project's 
add_library    (g10 SHARED  "G10.c" "GXInput.c" "GXAI.c" "GXCamera.c" "GXScheduler.c" "GXLinear.c" "GXPart.c" "GXMaterial.c" "GXQuaternion.c" "GXUserCode.c" "GXTransform.c") # "GXAudio.c" "GXBV.c" "GXCameraController.c" "GXCollider.c" "GXCollision.c" "GXDiscordIntegration.c" "GXEntity.c" "GXMaterial.c" "GXPLY.c" "GXPart.c" "GXPhysics.c" "GXQuaternion.c" "GXRenderer.c" "GXRigidbody.c"  "GXScheduler.c" "GXServer.c" "GXTexture.c" "GXTransform.c" "APS_3rdPersonCtrl.c" 

target_link_libraries(g10_example PUBLIC queue array dict json  ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${SDL2_NET_INCLUDE_DIRS} ${VULKAN_LIB_LIST})
target_link_libraries(g10 PUBLIC queue array dict json  ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${SDL2_NET_LIBRARIES} ${VULKAN_LIB_LIST})
target_link_libraries(g10_example PRIVATE SDL2_image::SDL2_image)
target_link_libraries(g10 PRIVATE SDL2_image::SDL2_image)
target_link_libraries(g10_example PRIVATE SDL2_net::SDL2_net)
target_link_libraries(g10 PRIVATE SDL2_net::SDL2_net)

# Define targets
target_include_directories(g10_example PUBLIC "include/" "json/include/")
target_include_directories(g10 PUBLIC "include/" "json/include/")