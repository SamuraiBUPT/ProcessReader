# require cmake version
cmake_minimum_required(VERSION 3.16)

# declare project
project(ProcessReader)

# set c++ standard
set(CMAKE_CXX_STANDARD 11)

# set output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_library(D3D9_LIBRARY NAMES d3d9 PATHS "C:\\Program Files (x86)\\Windows Kits\\10\\Lib\\10.0.22621.0\\um\\x64")
find_library(DWMAPI_LIBRARY NAMES dwmapi PATHS "C:\\Program Files (x86)\\Windows Kits\\10\\Lib\\10.0.22621.0\\um\\x64")
find_library(D3DX9_LIBRARY NAMES d3dx9 PATHS "C:\\Program Files (x86)\\Windows Kits\\Microsoft-DirectX-SDK-June-2010--master\\Lib\\x64")

set(SOURCE
    main.cpp
    processbinder.cpp
    utils.cpp
    datareader.cpp 
    windowbinder.cpp windowbinder.h 
    dxrender.h dxrender.cpp)

# add main executable
add_executable(csgocheat ${SOURCE})

target_link_libraries(csgocheat ${D3D9_LIBRARY} ${DWMAPI_LIBRARY} ${D3DX9_LIBRARY})