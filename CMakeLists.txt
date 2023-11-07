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

# Find the array module
if ( NOT "${HAS_ARRAY}")
    
    # Log
    message("[g10] Adding array project")
    
    # Add the array directory
    add_subdirectory(${CMAKE_SOURCE_DIR}/extern/array ${CMAKE_SOURCE_DIR}/build/array)

    # Let any parent project know that this project has array
    set(HAS_ARRAY true PARENT_SCOPE)
endif()

# Find the dict module
if ( NOT "${HAS_DICT}")
    
    # Log
    message("[g10] Adding dict project")
    
    # Add the dict directory
    add_subdirectory(${CMAKE_SOURCE_DIR}/extern/dict ${CMAKE_SOURCE_DIR}/build/dict)

    # Let any parent project know that this project has dict
    set(HAS_DICT true PARENT_SCOPE)
endif()

# Find the tuple module
if ( NOT "${HAS_TUPLE}")
    
    # Log
    message("[g10] Adding tuple project")
    
    # Add the tuple directory
    add_subdirectory(${CMAKE_SOURCE_DIR}/extern/tuple ${CMAKE_SOURCE_DIR}/build/tuple)

    # Let any parent project know that this project has tuple
    set(HAS_TUPLE true PARENT_SCOPE)
endif()

# Find the stack module
if ( NOT "${HAS_STACK}")
    
    # Log
    message("[g10] Adding stack project")
    
    # Add the stack directory
    add_subdirectory(${CMAKE_SOURCE_DIR}/extern/stack ${CMAKE_SOURCE_DIR}/build/stack)

    # Let any parent project know that this project has stack
    set(HAS_STACK true PARENT_SCOPE)
endif()

# Find the queue module
if ( NOT "${HAS_QUEUE}")
    
    # Log
    message("[g10] Adding queue project")
    
    # Add the queue directory
    add_subdirectory(${CMAKE_SOURCE_DIR}/extern/queue ${CMAKE_SOURCE_DIR}/build/queue)

    # Let any parent project know that this project has queue
    set(HAS_QUEUE true PARENT_SCOPE)
endif()

# Find the json module
if ( NOT "${HAS_JSON}")
    
    # Log
    message("[g10] Adding json project")
    
    # Add the json directory
    add_subdirectory(${CMAKE_SOURCE_DIR}/extern/json ${CMAKE_SOURCE_DIR}/build/json)

    # Let any parent project know that this project has json
    set(HAS_JSON true PARENT_SCOPE)
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

## Add source to the tester
add_executable (g10_test "g10_test.c")
add_dependencies(g10_test g10)
target_include_directories(g10_test PUBLIC include include/g10)
target_link_libraries(g10_test g10)

# g10 library
add_library (g10 SHARED "g10.c" "linear.c" "user_code.c") 
add_dependencies(g10 log sync crypto array dict tuple stack queue json)
target_include_directories(g10 PUBLIC include include/g10) #${CMAKE_SOURCE_DIR}/extern/json/include/ ${CMAKE_SOURCE_DIR}/extern/array/include/ ${CMAKE_SOURCE_DIR}/extern/dict/include/ ${CMAKE_SOURCE_DIR}/extern/stack/include/ ${CMAKE_SOURCE_DIR}/extern/queue/include/ ${CMAKE_SOURCE_DIR}/extern/sync/include/) 
target_link_libraries(g10 PUBLIC log sync crypto array dict tuple stack queue json m)