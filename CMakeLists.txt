cmake_minimum_required(VERSION 3.5)
project( toycompiler VERSION 0.1.1 )

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -pedantic")

add_executable(toycompiler ${PROJECT_SOURCE_DIR}/src/driver.c)

target_include_directories( toycompiler PUBLIC ${PROJECT_SOURCE_DIR}/include )
target_sources( toycompiler PRIVATE ${PROJECT_SOURCE_DIR}/src/Token_Data )
target_sources( toycompiler PRIVATE ${PROJECT_SOURCE_DIR}/src/Lexer_init )
target_sources( toycompiler PRIVATE ${PROJECT_SOURCE_DIR}/src/Dfa_init )
target_sources( toycompiler PRIVATE ${PROJECT_SOURCE_DIR}/src/Parser_init )
target_sources( toycompiler PRIVATE ${PROJECT_SOURCE_DIR}/src/Type_init )
target_sources( toycompiler PRIVATE ${PROJECT_SOURCE_DIR}/src/Ast_init )
target_sources( toycompiler PRIVATE ${PROJECT_SOURCE_DIR}/src/Semantic_init )
target_sources( toycompiler PRIVATE ${PROJECT_SOURCE_DIR}/src/Icg_init )
target_sources( toycompiler PRIVATE ${PROJECT_SOURCE_DIR}/src/CodeGen_init )
target_sources( toycompiler PRIVATE ${PROJECT_SOURCE_DIR}/src/driver_init )

set_target_properties(toycompiler
	PROPERTIES
	ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib"
	LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib"
	RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin"
)

if(NOT TARGET Dfa)
	add_subdirectory(${CMAKE_SOURCE_DIR}/ext/Dfa ${CMAKE_SOURCE_DIR}/ext/Dfa/build/bin)
endif(NOT TARGET Dfa)
target_link_libraries(toycompiler Dfa)

if(NOT TARGET LinkedList)
	add_subdirectory(${CMAKE_SOURCE_DIR}/ext/LinkedList ${CMAKE_SOURCE_DIR}/ext/LinkedList/build/bin)
endif(NOT TARGET LinkedList)
target_link_libraries(toycompiler LinkedList)

if(NOT TARGET Lexer)
	add_subdirectory(${CMAKE_SOURCE_DIR}/ext/Lexer ${CMAKE_SOURCE_DIR}/ext/Lexer/build/bin)
endif(NOT TARGET Lexer)
target_link_libraries(toycompiler Lexer)

if(NOT TARGET Token)
	add_subdirectory(${CMAKE_SOURCE_DIR}/ext/Token ${CMAKE_SOURCE_DIR}/ext/Token/build/bin)
endif(NOT TARGET Token)
target_link_libraries(toycompiler Token)

if(NOT TARGET ParserLL1)
	add_subdirectory(${CMAKE_SOURCE_DIR}/ext/ParserLL1 ${CMAKE_SOURCE_DIR}/ext/ParserLL1/build/bin)
endif(NOT TARGET ParserLL1)
target_link_libraries(toycompiler ParserLL1)

if(NOT TARGET ParseTree)
	add_subdirectory(${CMAKE_SOURCE_DIR}/ext/ParseTree ${CMAKE_SOURCE_DIR}/ext/ParseTree/build/bin)
endif(NOT TARGET ParseTree)
target_link_libraries(toycompiler ParseTree)

if(NOT TARGET SymbolEnv)
	add_subdirectory(${CMAKE_SOURCE_DIR}/ext/SymbolEnv ${CMAKE_SOURCE_DIR}/ext/SymbolEnv/build/bin)
endif(NOT TARGET SymbolEnv)
target_link_libraries(toycompiler SymbolEnv)

if(NOT TARGET Quad)
	add_subdirectory(${CMAKE_SOURCE_DIR}/ext/Quad ${CMAKE_SOURCE_DIR}/ext/Quad/build/bin)
endif(NOT TARGET Quad)
target_link_libraries(toycompiler Quad)
