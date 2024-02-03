cmake_minimum_required(VERSION 3.25)
project(cat1)
add_executable(${PROJECT_NAME})
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 23 CXX_STANDARD_REQUIRED TRUE)

add_subdirectory(src)
target_include_directories(${PROJECT_NAME} PRIVATE include)

include(FetchContent)
FetchContent_Declare(
    fmt
    GIT_REPOSITORY git@github.com:fmtlib/fmt.git
    GIT_TAG e69e5f977d458f2650bb346dadf2ad30c5320281
)
FetchContent_MakeAvailable(fmt)
target_link_libraries(${PROJECT_NAME} PRIVATE fmt::fmt)
