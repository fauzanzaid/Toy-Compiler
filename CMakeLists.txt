cmake_minimum_required(VERSION 3.5)
project( valangLexicalAnalyzer VERSION 0.1.1 )

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -pedantic")

add_executable(valangLexicalAnalyzer ${PROJECT_SOURCE_DIR}/src/valangLexicalAnalyzer.c)

target_include_directories( valangLexicalAnalyzer PUBLIC ${PROJECT_SOURCE_DIR}/include )
target_sources( valangLexicalAnalyzer PRIVATE ${PROJECT_SOURCE_DIR}/src/Token_Data )
target_sources( valangLexicalAnalyzer PRIVATE ${PROJECT_SOURCE_DIR}/src/Lexer_init )
target_sources( valangLexicalAnalyzer PRIVATE ${PROJECT_SOURCE_DIR}/src/Dfa_init )
target_sources( valangLexicalAnalyzer PRIVATE ${PROJECT_SOURCE_DIR}/src/Parser_init )
target_sources( valangLexicalAnalyzer PRIVATE ${PROJECT_SOURCE_DIR}/src/ParseTree_init )

set_target_properties(valangLexicalAnalyzer
	PROPERTIES
	ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib"
	LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib"
	RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin"
)

if(NOT TARGET Dfa)
	add_subdirectory(${CMAKE_SOURCE_DIR}/ext/Dfa ${CMAKE_SOURCE_DIR}/ext/Dfa/build/bin)
endif(NOT TARGET Dfa)
target_link_libraries(valangLexicalAnalyzer Dfa)

if(NOT TARGET LinkedList)
	add_subdirectory(${CMAKE_SOURCE_DIR}/ext/LinkedList ${CMAKE_SOURCE_DIR}/ext/LinkedList/build/bin)
endif(NOT TARGET LinkedList)
target_link_libraries(valangLexicalAnalyzer LinkedList)

if(NOT TARGET Lexer)
	add_subdirectory(${CMAKE_SOURCE_DIR}/ext/Lexer ${CMAKE_SOURCE_DIR}/ext/Lexer/build/bin)
endif(NOT TARGET Lexer)
target_link_libraries(valangLexicalAnalyzer Lexer)

if(NOT TARGET Token)
	add_subdirectory(${CMAKE_SOURCE_DIR}/ext/Token ${CMAKE_SOURCE_DIR}/ext/Token/build/bin)
endif(NOT TARGET Token)
target_link_libraries(valangLexicalAnalyzer Token)

if(NOT TARGET ParserLL1)
	add_subdirectory(${CMAKE_SOURCE_DIR}/ext/ParserLL1 ${CMAKE_SOURCE_DIR}/ext/ParserLL1/build/bin)
endif(NOT TARGET ParserLL1)
target_link_libraries(valangLexicalAnalyzer ParserLL1)

if(NOT TARGET ParseTree)
	add_subdirectory(${CMAKE_SOURCE_DIR}/ext/ParseTree ${CMAKE_SOURCE_DIR}/ext/ParseTree/build/bin)
endif(NOT TARGET ParseTree)
target_link_libraries(valangLexicalAnalyzer ParseTree)
