cmake_minimum_required(VERSION 3.16)
project(GhostCloak VERSION 1.0.0 LANGUAGES CXX)

# Set CMake policy
cmake_policy(SET CMP0115 NEW)

# Set C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add cmake module path
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Find required packages
find_package(gdiplus REQUIRED)

# Source files
set(SOURCES
    src/main.cpp
    src/ConfigManager.cpp
    src/BackgroundManager.cpp
    src/UIManager.cpp
    src/AutoClickManager.cpp
    src/GDIPlusHelper.cpp
    src/WindowHelper.cpp
    src/DragManager.cpp
    src/res/app_icon.rc
)

# Header files
set(HEADERS
    include/AppState.h
    include/ConfigManager.h
    include/BackgroundManager.h
    include/UIManager.h
    include/AutoClickManager.h
    include/GDIPlusHelper.h
    include/WindowHelper.h
    include/DragManager.h
)

# Create executable
add_executable(GhostCloak ${SOURCES} ${HEADERS})

# Include directories
target_include_directories(GhostCloak PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Link libraries
target_link_libraries(GhostCloak PRIVATE
    GDIPlus::GDIPlus
    user32
    gdi32
    shlwapi
    shell32
)

# Compiler flags for MSVC
if(MSVC)
    # Set compiler options for all configurations
    target_compile_options(GhostCloak PRIVATE /W3 /EHsc)
    
    # Debug configuration - no optimization
    target_compile_options(GhostCloak PRIVATE $<$<CONFIG:Debug>:/Od>)
    # Release configuration - optimization
    target_compile_options(GhostCloak PRIVATE $<$<CONFIG:Release>:/O2>)
    
    target_compile_definitions(GhostCloak PRIVATE _UNICODE UNICODE)
    set_target_properties(GhostCloak PROPERTIES
        WIN32_EXECUTABLE ON
        SUBSYSTEM WINDOWS
    )
endif()

# Compiler flags for other compilers
if(NOT MSVC)
    target_compile_options(GhostCloak PRIVATE -Wall -Wextra)
endif()

# Print build info
message(STATUS "GhostCloak Project configured successfully")
message(STATUS "  Source Directory: ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "  Build Directory: ${CMAKE_BINARY_DIR}")
message(STATUS "  C++ Standard: ${CMAKE_CXX_STANDARD}")
