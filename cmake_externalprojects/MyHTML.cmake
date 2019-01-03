cmake_minimum_required(VERSION 3.11.4)

set(MyHTML_BUILD_WITHOUT_THREADS ON)
set(MyCORE_BUILD_WITHOUT_THREADS ON)
set(MyHTML_BUILD_SHARED OFF)

include(FetchContent)
FetchContent_Declare(
    myhtml
    GIT_REPOSITORY     https://github.com/lexborisov/myhtml.git
    GIT_TAG            v4.0.5
    SOURCE_DIR         "${CMAKE_SOURCE_DIR}/thirdparty/myhtml"
    BINARY_DIR         "${CMAKE_BINARY_DIR}/myhtml-bin"
)

FetchContent_GetProperties(myhtml)
if(NOT myhtml_POPULATED)
    FetchContent_Populate(myhtml)
    add_subdirectory(${myhtml_SOURCE_DIR} ${myhtml_BINARY_DIR})
    include_directories("${myhtml_SOURCE_DIR}/include")
endif()
