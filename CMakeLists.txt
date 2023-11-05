# Need CMake 3.0
cmake_minimum_required (VERSION 3.0)

# This is the name of the repository
project ("g10")

# Set the correct paths
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)
set(CMAKE_PREFIX_PATH ${CMAKE_SOURCE_DIR}/extern/)

# Comment out for Debug mode
set(IS_DEBUG_BUILD CMAKE_BUILD_TYPE STREQUAL "Debug")

# Set debug mode
if (${IS_DEBUG_BUILD})
    add_compile_definitions(NDEBUG)
endif()

# Add the g10 project
if ( NOT "${HAS_G10}")
    
    # Log
    message("[g10] Adding g10 project")

    # Let any parent project know that it has the g10 project
    set(HAS_G10 true PARENT_SCOPE)
endif()

# g10 example
add_executable (g10_internal_example "main.c") 
add_dependencies(g10_internal_example g10)
target_include_directories(g10_internal_example PUBLIC include include/g10) # ${CMAKE_SOURCE_DIR}/extern/json/include/ ${CMAKE_SOURCE_DIR}/extern/array/include/ ${CMAKE_SOURCE_DIR}/extern/dict/include/ ${CMAKE_SOURCE_DIR}/extern/stack/include/ ${CMAKE_SOURCE_DIR}/extern/queue/include/ ${CMAKE_SOURCE_DIR}/extern/sync/include/) 
target_link_libraries(g10_internal_example g10)

# g10 library
add_library (g10 SHARED "g10.c") 
#add_dependencies(g10 json array dict stack queue sync)
target_include_directories(g10 PUBLIC include include/g10) #${CMAKE_SOURCE_DIR}/extern/json/include/ ${CMAKE_SOURCE_DIR}/extern/array/include/ ${CMAKE_SOURCE_DIR}/extern/dict/include/ ${CMAKE_SOURCE_DIR}/extern/stack/include/ ${CMAKE_SOURCE_DIR}/extern/queue/include/ ${CMAKE_SOURCE_DIR}/extern/sync/include/) 
#target_link_libraries(g10 PUBLIC json array dict stack queue sync ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${SDL2_NET_INCLUDE_DIRS} ${VULKAN_LIB_LIST} PRIVATE SDL2_image::SDL2_image SDL2_net::SDL2_net )