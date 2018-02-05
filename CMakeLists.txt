cmake_minimum_required(VERSION 3.5)
project( valangLexicalAnalyzer VERSION 0.1.1 )

add_executable(valangLexicalAnalyzer ${PROJECT_SOURCE_DIR}/src/valangLexicalAnalyzer.c)

target_include_directories( valangLexicalAnalyzer PUBLIC ${PROJECT_SOURCE_DIR}/include )
#target_sources( valangLexicalAnalyzer PRIVATE ${PROJECT_SOURCE_DIR}/src/valangLexicalAnalyzer )

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