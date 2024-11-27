cmake_minimum_required(VERSION 3.20.0)
project(r-type
        VERSION 1.0
        DESCRIPTION "R-Type"
        LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_SHARED_LIBRARY_PREFIX "")
set(CMAKE_SHARED_LIBRARY_SUFFIX ".so")

# Core
file(GLOB_RECURSE SERVER_SRC "./server/*.cpp")
file(GLOB_RECURSE COMMON_SRC "./common/*.cpp")
file(GLOB_RECURSE CLIENT_SRC "./client/*.cpp")

# Includes
include_directories(client)
include_directories(common)
include_directories(server)

# Executables
add_library(${PROJECT_NAME}_engine SHARED ${COMMON_SRC})
add_executable(${PROJECT_NAME}_server ${SERVER_SRC})
add_executable(${PROJECT_NAME}_client ${SERVER_SRC})

# Libraries
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)
add_subdirectory("./submodules/nlohmann-json")
add_subdirectory("./submodules/luau")
add_subdirectory("./submodules/raylib")
add_subdirectory("./submodules/raylib-cpp")

# Linking
target_link_libraries(${PROJECT_NAME}_engine PRIVATE
        ${NLOHMANN_JSON_TARGET_NAME}
        raylib
        raylib_cpp
        Luau.Compiler
        Luau.VM
)
target_include_directories(${PROJECT_NAME}_engine PRIVATE "./submodules/rlImGui/rlImGui.h")
target_link_libraries(${PROJECT_NAME}_client PRIVATE ${PROJECT_NAME}_engine)
target_link_libraries(${PROJECT_NAME}_server PRIVATE ${PROJECT_NAME}_engine)
set_target_properties(${PROJECT_NAME}_engine PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
)
set_target_properties(${PROJECT_NAME}_server PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
)
set_target_properties(${PROJECT_NAME}_client PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
)

add_compile_options(${PROJECT_NAME}_client PRIVATE -Wall -Wextra -Wformat=2
        -Wswitch-default -Wcast-align -Wpointer-arith -Wundef -Wcast-qual
        -Wshadow -Wwrite-strings -Wconversion -Wunreachable-code -Wstrict-aliasing=2
        -fno-common -fstrict-aliasing -Wunused-macros #-fno-gnu-unique
)
add_compile_options(${PROJECT_NAME}_server PRIVATE -Wall -Wextra -Wformat=2
        -Wswitch-default -Wcast-align -Wpointer-arith -Wundef -Wcast-qual
        -Wshadow -Wwrite-strings -Wconversion -Wunreachable-code -Wstrict-aliasing=2
        -fno-common -fstrict-aliasing -Wunused-macros #-fno-gnu-unique
)
add_compile_options(${PROJECT_NAME}_engine PRIVATE -Wall -Wextra -Wformat=2
        -Wswitch-default -Wcast-align -Wpointer-arith -Wundef -Wcast-qual
        -Wshadow -Wwrite-strings -Wconversion -Wunreachable-code -Wstrict-aliasing=2
        -fno-common -fstrict-aliasing -Wunused-macros #-fno-gnu-unique
)
add_compile_definitions(MallocNanoZone=0)

if (CMAKE_BUILD_TYPE MATCHES RelWithDebInfo)
    target_compile_options(${PROJECT_NAME}_server PRIVATE -O2)
    target_compile_options(${PROJECT_NAME}_client PRIVATE -O2)
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-g3")
endif()

if(CMAKE_BUILD_TYPE MATCHES Release)
    target_compile_options(${PROJECT_NAME}_client PRIVATE -O2)
    target_compile_options(${PROJECT_NAME}_server PRIVATE -O2)
    set(CMAKE_CXX_FLAGS_RELEASE "-g3")
else()
    target_compile_options(${PROJECT_NAME}_client PRIVATE -O0)
    target_compile_options(${PROJECT_NAME}_server PRIVATE -O0)
    set(CMAKE_CXX_FLAGS_DEBUG "-g3")
    set(CMAKE_CXX_FLAGS_DEBUG "-fomit-frame-pointer -fsanitize=address")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
endif()
