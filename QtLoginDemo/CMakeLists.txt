cmake_minimum_required(VERSION 3.16)
project(QtLoginDemo VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Test)

# --- 主程序 ---
add_executable(QtLoginDemo
    src/main.cpp
    src/LoginWindow.cpp
    src/AuthValidator.cpp
)
target_link_libraries(QtLoginDemo PRIVATE Qt6::Widgets)
target_include_directories(QtLoginDemo PRIVATE src)

# --- 单元测试 ---
enable_testing()

add_executable(test_auth_validator
    tests/test_auth_validator.cpp
    src/AuthValidator.cpp
)
target_link_libraries(test_auth_validator PRIVATE Qt6::Test Qt6::Widgets)
target_include_directories(test_auth_validator PRIVATE src)

add_test(NAME AuthValidator COMMAND test_auth_validator)
