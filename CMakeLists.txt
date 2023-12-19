# Need CMake 3.0
cmake_minimum_required (VERSION 3.28.0)

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
add_compile_options(-gdwarf-4 -Wall -Wextra -Wpedantic -Wpointer-arith -Wstrict-prototypes -Wformat-security -Wfloat-equal -Wshadow -Wconversion -Wlogical-not-parentheses -Wnull-dereference)

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

# Find the crypto module
if ( NOT "${HAS_CRYPTO}")

    # Add the crypto directory
    add_subdirectory(${CMAKE_SOURCE_DIR}/extern/crypto ${CMAKE_SOURCE_DIR}/build/crypto)

    # Let any parent project know that this project has crypto
    if(NOT PROJECT_IS_TOP_LEVEL)
        set(HAS_CRYPTO true PARENT_SCOPE)
        set(CRYPTO_INCLUDE_DIR ${CRYPTO_INCLUDE_DIR} PARENT_SCOPE)
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

# Find the web module
if ( NOT "${HAS_WEB}")
    
    # Add the web directory
    add_subdirectory(${CMAKE_SOURCE_DIR}/extern/web ${CMAKE_SOURCE_DIR}/build/web)

    # Let any parent project know that this project has web
    if(NOT PROJECT_IS_TOP_LEVEL)
        set(HAS_WEB true PARENT_SCOPE)
        set(WEB_INCLUDE_DIR ${WEB_INCLUDE_DIR} PARENT_SCOPE)
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
target_include_directories(g10_example PUBLIC ${G10_INCLUDE_DIR} ${ARRAY_INCLUDE_DIR} ${BASE64_INCLUDE_DIR} ${CRYPTO_INCLUDE_DIR} ${DICT_INCLUDE_DIR} ${HTTP_INCLUDE_DIR} ${JSON_INCLUDE_DIR} ${LOG_INCLUDE_DIR} ${PARALLEL_INCLUDE_DIR} ${QUEUE_INCLUDE_DIR} ${SOCKET_INCLUDE_DIR} ${STACK_INCLUDE_DIR} ${SYNC_INCLUDE_DIR} ${TUPLE_INCLUDE_DIR} ${WEB_INCLUDE_DIR}) 
target_link_libraries(g10_example g10)

## Add source to the tester
add_executable (g10_test "g10_test.c")
add_dependencies(g10_test g10)
target_include_directories(g10_test PUBLIC ${G10_INCLUDE_DIR} ${ARRAY_INCLUDE_DIR} ${BASE64_INCLUDE_DIR} ${CRYPTO_INCLUDE_DIR} ${DICT_INCLUDE_DIR} ${HTTP_INCLUDE_DIR} ${JSON_INCLUDE_DIR} ${LOG_INCLUDE_DIR} ${PARALLEL_INCLUDE_DIR} ${QUEUE_INCLUDE_DIR} ${SOCKET_INCLUDE_DIR} ${STACK_INCLUDE_DIR} ${SYNC_INCLUDE_DIR} ${TUPLE_INCLUDE_DIR} ${WEB_INCLUDE_DIR})
target_link_libraries(g10_test g10)

# g10 library
add_library (g10 SHARED "g10.c" "linear.c" "server.c" "user_code.c") 
add_dependencies(g10 log sync crypto array dict tuple stack queue json http web socket)
target_include_directories(g10 PUBLIC ${G10_INCLUDE_DIR} ${ARRAY_INCLUDE_DIR} ${BASE64_INCLUDE_DIR} ${CRYPTO_INCLUDE_DIR} ${DICT_INCLUDE_DIR} ${HTTP_INCLUDE_DIR} ${JSON_INCLUDE_DIR} ${LOG_INCLUDE_DIR} ${PARALLEL_INCLUDE_DIR} ${QUEUE_INCLUDE_DIR} ${SOCKET_INCLUDE_DIR} ${STACK_INCLUDE_DIR} ${SYNC_INCLUDE_DIR} ${TUPLE_INCLUDE_DIR} ${WEB_INCLUDE_DIR}) 
target_link_libraries(g10 PUBLIC log sync crypto array dict tuple stack queue json http parallel web socket m)