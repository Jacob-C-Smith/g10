# Need CMake 3.0
cmake_minimum_required (VERSION 3.28.0)

# This is the name of the repository
project ("g10")

# Set the correct paths
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
set(CMAKE_PREFIX_PATH ${CMAKE_SOURCE_DIR}/extern/)

# Comment out for Debug mode
set(IS_DEBUG_BUILD CMAKE_BUILD_TYPE STREQUAL "Debug")

# Set debug mode
if (${IS_DEBUG_BUILD})
    add_compile_definitions(NDEBUG)
endif()
add_compile_options(-gdwarf-4 -Wall -Wextra -Wpedantic -Wpointer-arith -Wstrict-prototypes -Wformat-security -Wfloat-equal -Wshadow -Wconversion -Wlogical-not-parentheses -Wnull-dereference)

add_compile_definitions(BUILD_G10_WITH_SDL2)

find_package(SDL2 REQUIRED CONFIG REQUIRED COMPONENTS SDL2)
find_package(SDL2 REQUIRED CONFIG COMPONENTS SDL2main)

IF(UNIX AND NOT APPLE)
	set(LINUX TRUE)
ENDIF()

IF(WIN32)
	IF (NOT Vulkan_FOUND)
		find_library(Vulkan_LIBRARY NAMES vulkan-1 vulkan PATHS ${CMAKE_SOURCE_DIR}/libs/vulkan)
		IF (Vulkan_LIBRARY)
			set(Vulkan_FOUND ON)
			MESSAGE("Using bundled Vulkan library version")
		ENDIF()
	ENDIF()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DVK_USE_PLATFORM_WIN32_KHR")
ELSEIF(LINUX)
	IF (NOT Vulkan_FOUND)
		find_library(Vulkan_LIBRARY NAMES vulkan HINTS "$ENV{VULKAN_SDK}/lib" "${CMAKE_SOURCE_DIR}/libs/vulkan" REQUIRED)
		IF (Vulkan_LIBRARY)
			set(Vulkan_FOUND ON)
			MESSAGE("Using bundled Vulkan library version")
		ENDIF()
	ENDIF()
	find_package(Threads REQUIRED)
	IF(USE_D2D_WSI)
		MESSAGE("Using direct to display extension...")
		add_definitions(-D_DIRECT2DISPLAY)
	ELSEIF(USE_DIRECTFB_WSI)
		find_package(DirectFB REQUIRED)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DVK_USE_PLATFORM_DIRECTFB_EXT")
		include_directories(${DIRECTFB_INCLUDE_DIR})
	ELSEIF(USE_WAYLAND_WSI)
		find_program(PKG_CONFIG pkg-config)
		if (NOT PKG_CONFIG)
			message(FATAL_ERROR "pkg-config binary not found")
		endif ()
		find_package(Wayland REQUIRED)
		if (NOT WAYLAND_FOUND)
			message(FATAL_ERROR "Wayland development package not found")
		endif ()
		pkg_check_modules(WAYLAND_PROTOCOLS REQUIRED wayland-protocols)
		if (NOT WAYLAND_PROTOCOLS_FOUND)
			message(FATAL_ERROR "Wayland protocols package not found")
		endif ()
		find_program(WAYLAND_SCANNER wayland-scanner)
		if (NOT WAYLAND_SCANNER)
			message(FATAL_ERROR "wayland-scanner binary not found")
		endif ()
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DVK_USE_PLATFORM_WAYLAND_KHR")
		include_directories(${WAYLAND_INCLUDE_DIR})
		pkg_get_variable(protocol_dir wayland-protocols pkgdatadir)
		execute_process(COMMAND ${WAYLAND_SCANNER} client-header ${protocol_dir}/stable/xdg-shell/xdg-shell.xml ${CMAKE_BINARY_DIR}/xdg-shell-client-protocol.h
				COMMAND ${WAYLAND_SCANNER} private-code ${protocol_dir}/stable/xdg-shell/xdg-shell.xml ${CMAKE_BINARY_DIR}/xdg-shell-protocol.c)
		include_directories(${CMAKE_BINARY_DIR})
	ELSEIF(USE_HEADLESS)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DVK_USE_PLATFORM_HEADLESS_EXT")
	ELSE(USE_D2D_WSI)
		find_package(XCB REQUIRED)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DVK_USE_PLATFORM_XCB_KHR")
	ENDIF(USE_D2D_WSI)
ELSEIF(APPLE)
	IF (CMAKE_GENERATOR MATCHES "Xcode")
		# Suppress regeneration for Xcode since environment variables will be lost if not set in Xcode locations/custom paths
		set(CMAKE_SUPPRESS_REGENERATION ON)
		set(CMAKE_XCODE_GENERATE_SCHEME ON)
		IF (Vulkan_FOUND)
			# If the Vulkan loader was found and its environment variables are defined, make them available within Xcode schemes
			IF (DEFINED ENV{VK_ADD_LAYER_PATH})
				set(CMAKE_XCODE_SCHEME_ENVIRONMENT "${CMAKE_XCODE_SCHEME_ENVIRONMENT};VK_ADD_LAYER_PATH=$ENV{VK_ADD_LAYER_PATH}")
			ENDIF()
			IF (DEFINED ENV{VK_ICD_FILENAMES})
				set(CMAKE_XCODE_SCHEME_ENVIRONMENT "${CMAKE_XCODE_SCHEME_ENVIRONMENT};VK_ICD_FILENAMES=$ENV{VK_ICD_FILENAMES}")
			ENDIF()
			IF (DEFINED ENV{VK_DRIVER_FILES})
				set(CMAKE_XCODE_SCHEME_ENVIRONMENT "${CMAKE_XCODE_SCHEME_ENVIRONMENT};VK_DRIVER_FILES=$ENV{VK_DRIVER_FILES}")
			ENDIF()
		ENDIF()
	ENDIF()
	IF (NOT Vulkan_FOUND)
		# If the Vulkan loader was not found, then look for libMoltenVK.dylib in the Vulkan SDK and legacy MoltenVK locations
		set(CMAKE_FIND_FRAMEWORK NEVER)
		find_library(Vulkan_LIBRARY NAMES MoltenVK HINTS "$ENV{VULKAN_SDK}/lib" "$ENV{VULKAN_SDK}/dylib/macOS" REQUIRED)
		IF (Vulkan_LIBRARY)
			set(Vulkan_FOUND ON)
			MESSAGE(STATUS "Using MoltenVK Vulkan Portability library")
		ENDIF()
	ENDIF()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DVK_USE_PLATFORM_METAL_EXT -DVK_EXAMPLE_XCODE_GENERATED")
	# Todo : android?
ENDIF(WIN32)

IF (NOT Vulkan_FOUND)
	message(FATAL_ERROR "Could not find Vulkan library!")
ELSE()
	message(STATUS ${Vulkan_LIBRARY})
ENDIF()

# Find the log module
if ( NOT "${HAS_LOG}")
    
    # Add the log directory
    add_subdirectory(${CMAKE_SOURCE_DIR}/extern/log ${CMAKE_SOURCE_DIR}/build/log)

    # Let any parent project know that this project has log
    if(NOT PROJECT_IS_TOP_LEVEL)
        set(HAS_LOG true PARENT_SCOPE)
        set(LOG_INCLUDE_DIR ${LOG_INCLUDE_DIR} PARENT_SCOPE)
    endif()
endif()

# Find the socket module
if ( NOT "${HAS_SOCKET}")
    
    # Add the socket directory
    add_subdirectory(${CMAKE_SOURCE_DIR}/extern/socket ${CMAKE_SOURCE_DIR}/build/socket)

    # Let any parent project know that this project has socket
    if(NOT PROJECT_IS_TOP_LEVEL)
        set(HAS_SOCKET true PARENT_SCOPE)
        set(SOCKET_INCLUDE_DIR ${SOCKET_INCLUDE_DIR} PARENT_SCOPE)
    endif()
endif()

# Find the hash cache module
if ( NOT "${HAS_HASH_CACHE}")
    
    # Add the hash cache directory
    add_subdirectory(${CMAKE_SOURCE_DIR}/extern/hash-cache ${CMAKE_SOURCE_DIR}/build/hash-cache)

    # Let any parent project know that this project has hash cache
    if(NOT PROJECT_IS_TOP_LEVEL)
        set(HAS_HASH_CACHE true PARENT_SCOPE)
        set(HASH_CACHE_INCLUDE_DIR ${HASH_CACHE_INCLUDE_DIR} PARENT_SCOPE)
    endif()
endif()

# Find the array module
if ( NOT "${HAS_ARRAY}")

    # Add the array directory
    add_subdirectory(${CMAKE_SOURCE_DIR}/extern/array ${CMAKE_SOURCE_DIR}/build/array)

    # Let any parent project know that this project has array
    if(NOT PROJECT_IS_TOP_LEVEL)
        set(HAS_ARRAY true PARENT_SCOPE)
        set(ARRAY_INCLUDE_DIR ${ARRAY_INCLUDE_DIR} PARENT_SCOPE)
    endif()
endif()


# Find the base64 module
if ( NOT "${HAS_BASE64}")

    # Add the base64 directory
    add_subdirectory(${CMAKE_SOURCE_DIR}/extern/base64 ${CMAKE_SOURCE_DIR}/build/base64)

    # Let any parent project know that this project has base64
    if(NOT PROJECT_IS_TOP_LEVEL)
        set(HAS_BASE64 true PARENT_SCOPE)
        set(BASE64_INCLUDE_DIR ${BASE64_INCLUDE_DIR} PARENT_SCOPE)
    endif()
endif()

# Find the dict module
if ( NOT "${HAS_DICT}")

    # Add the dict directory
    add_subdirectory(${CMAKE_SOURCE_DIR}/extern/dict ${CMAKE_SOURCE_DIR}/build/dict)

    # Let any parent project know that this project has dict
    if(NOT PROJECT_IS_TOP_LEVEL)
        set(HAS_DICT true PARENT_SCOPE)
        set(DICT_INCLUDE_DIR ${DICT_INCLUDE_DIR} PARENT_SCOPE)
    endif()
endif()

# Find the tuple module
if ( NOT "${HAS_TUPLE}")
    
    # Add the tuple directory
    add_subdirectory(${CMAKE_SOURCE_DIR}/extern/tuple ${CMAKE_SOURCE_DIR}/build/tuple)

    # Let any parent project know that this project has tuple
    if(NOT PROJECT_IS_TOP_LEVEL)
        set(HAS_TUPLE true PARENT_SCOPE)
        set(TUPLE_INCLUDE_DIR ${TUPLE_INCLUDE_DIR} PARENT_SCOPE)
    endif()
endif()
 
# Find the stack module
if ( NOT "${HAS_STACK}")

    # Add the stack directory
    add_subdirectory(${CMAKE_SOURCE_DIR}/extern/stack ${CMAKE_SOURCE_DIR}/build/stack)

    # Let any parent project know that this project has stack
    if(NOT PROJECT_IS_TOP_LEVEL)
        set(HAS_STACK true PARENT_SCOPE)
        set(STACK_INCLUDE_DIR ${STACK_INCLUDE_DIR} PARENT_SCOPE)
    endif()
endif()

# Find the queue module
if ( NOT "${HAS_QUEUE}")
    
    # Add the queue directory
    add_subdirectory(${CMAKE_SOURCE_DIR}/extern/queue ${CMAKE_SOURCE_DIR}/build/queue)

    # Let any parent project know that this project has queue
    if(NOT PROJECT_IS_TOP_LEVEL)
        set(HAS_QUEUE true PARENT_SCOPE)
        set(QUEUE_INCLUDE_DIR ${QUEUE_INCLUDE_DIR} PARENT_SCOPE)
    endif()
endif()

# Find the json module
if ( NOT "${HAS_JSON}")
    
    # Add the json directory
    add_subdirectory(${CMAKE_SOURCE_DIR}/extern/json ${CMAKE_SOURCE_DIR}/build/json)

    # Let any parent project know that this project has json
    if(NOT PROJECT_IS_TOP_LEVEL)
        set(HAS_JSON true PARENT_SCOPE)
        set(JSON_INCLUDE_DIR ${JSON_INCLUDE_DIR} PARENT_SCOPE)
    endif()
endif()

# Find the http module
if ( NOT "${HAS_HTTP}")

    # Add the http directory
    add_subdirectory(${CMAKE_SOURCE_DIR}/extern/http ${CMAKE_SOURCE_DIR}/build/http)

    # Let any parent project know that this project has http
    if(NOT PROJECT_IS_TOP_LEVEL)
        set(HAS_HTTP true PARENT_SCOPE)
        set(HTTP_INCLUDE_DIR ${HTTP_INCLUDE_DIR} PARENT_SCOPE)
    endif()
endif()

# Find the parallel module
if ( NOT "${HAS_PARALLEL}")
    
    # Add the parallel directory
    add_subdirectory(${CMAKE_SOURCE_DIR}/extern/parallel ${CMAKE_SOURCE_DIR}/build/parallel)

    # Let any parent project know that this project has parallel
    if(NOT PROJECT_IS_TOP_LEVEL)
        set(HAS_PARALLEL true PARENT_SCOPE)
        set(PARALLEL_INCLUDE_DIR ${PARALLEL_INCLUDE_DIR} PARENT_SCOPE)
    endif()
endif()

# Add the g10 project
if ( NOT "${HAS_G10}")
    
    # Log
    message("[g10] Adding g10 project")

    # Set the g10 include
    set(G10_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/include)

    # Let any parent project know that it has the g10 project
    if(NOT PROJECT_IS_TOP_LEVEL)
        set(HAS_G10 true PARENT_SCOPE)
        set(G10_INCLUDE_DIR ${G10_INCLUDE_DIR} PARENT_SCOPE)
    endif()
endif()

# g10 example
add_executable (g10_example "main.c") 
add_dependencies(g10_example g10)
target_include_directories(g10_example PUBLIC ${G10_INCLUDE_DIR} ${SDL2_INCLUDE_DIRS} ${ARRAY_INCLUDE_DIR} ${BASE64_INCLUDE_DIR} ${DICT_INCLUDE_DIR} ${HTTP_INCLUDE_DIR}/ ${JSON_INCLUDE_DIR} ${LOG_INCLUDE_DIR} ${PARALLEL_INCLUDE_DIR} ${QUEUE_INCLUDE_DIR} ${SOCKET_INCLUDE_DIR} ${STACK_INCLUDE_DIR} ${SYNC_INCLUDE_DIR} ${TUPLE_INCLUDE_DIR}) 
target_link_libraries(g10_example g10)

# g10 example
add_executable (g10_test "g10_test.c") 
add_dependencies(g10_test g10)
target_include_directories(g10_test PUBLIC ${G10_INCLUDE_DIR} ${SDL2_INCLUDE_DIRS} ${ARRAY_INCLUDE_DIR} ${BASE64_INCLUDE_DIR} ${DICT_INCLUDE_DIR} ${HTTP_INCLUDE_DIR}/ ${JSON_INCLUDE_DIR} ${LOG_INCLUDE_DIR} ${PARALLEL_INCLUDE_DIR} ${QUEUE_INCLUDE_DIR} ${SOCKET_INCLUDE_DIR} ${STACK_INCLUDE_DIR} ${SYNC_INCLUDE_DIR} ${TUPLE_INCLUDE_DIR}) 
target_link_libraries(g10_test g10)

# g10 library
add_library (g10 SHARED "g10.c" "ai.c" "linear.c" "renderer.c" "scene.c" "entity.c" "transform.c" "quaternion.c" "user_code.c" "3rd-party/g_sdl2.c" "3rd-party/g_vulkan.c") 
add_dependencies(g10 log sync array dict tuple stack queue json http socket hash_cache)
target_include_directories(g10 PUBLIC ${G10_INCLUDE_DIR} ${SDL2_INCLUDE_DIRS} ${ARRAY_INCLUDE_DIR} ${BASE64_INCLUDE_DIR} ${DICT_INCLUDE_DIR} ${HTTP_INCLUDE_DIR} ${JSON_INCLUDE_DIR} ${LOG_INCLUDE_DIR} ${PARALLEL_INCLUDE_DIR} ${QUEUE_INCLUDE_DIR} ${SOCKET_INCLUDE_DIR} ${STACK_INCLUDE_DIR} ${SYNC_INCLUDE_DIR} ${TUPLE_INCLUDE_DIR} ${HASH_CACHE_INCLUDE_DIR}) 
target_link_libraries(g10 PUBLIC ${SDL2_LIBRARIES} ${Vulkan_LIBRARY} log sync array dict tuple stack queue json http parallel socket m GL )
