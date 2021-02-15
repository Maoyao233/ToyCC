cmake_minimum_required(VERSION 3.0.0)

project(ToyCC VERSION 0.1.0)

#include(CTest)
#enable_testing()

add_executable(tcc 
    Codegenerator.cpp
    Decl.cpp
    Expr.cpp
    lexer.cpp
    Parser.cpp
    Stmt.cpp
    token.cpp
    main.cpp)

find_package(nlohmann_json CONFIG REQUIRED)

find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

add_definitions(${LLVM_DEFINITIONS})

target_include_directories(tcc
    PUBLIC
        ${LLVM_INCLUDE_DIRS}
)

llvm_map_components_to_libnames(llvm_libs CORE)

target_link_libraries(tcc
    PRIVATE
        nlohmann_json nlohmann_json::nlohmann_json
        ${llvm_libs}
)

#set(CPACK_PROJECT_NAME ${PROJECT_NAME})
#set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
#include(CPack)
