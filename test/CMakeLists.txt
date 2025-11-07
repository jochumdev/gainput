cmake_minimum_required(VERSION 3.25.0 FATAL_ERROR)

project(
	gainputtest
	LANGUAGES CXX
	DESCRIPTION "Gainput test suite"
)

set(TEST_SOURCES
	"${CMAKE_CURRENT_SOURCE_DIR}/test_inputdevice.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/test_inputmanager.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/test_inputmap.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/test_inputrecording.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/test_inputstate.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/test.cpp"
)

# Define cmake target for the test project
if(ANDROID)
	add_library(${PROJECT_NAME} SHARED ${TEST_SOURCES})
	add_library(gainput::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
else()
	add_executable(${PROJECT_NAME} 
		$<$<BOOL:${WIN32}>:WIN32>
		$<$<BOOL:${APPLE}>:MACOSX_BUNDLE>
		${TEST_SOURCES}
	)
	add_executable(gainput::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
endif()

if(APPLE AND NOT IOS)
	find_library(APPKIT AppKit)
else()
	set(APPKIT "")
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
	target_compile_options(${PROJECT_NAME} PRIVATE "-std=c++0x")
endif()

target_link_libraries(${PROJECT_NAME} 
	PRIVATE 
	gainput::requirements
	gainput::gainput
	$<$<BOOL:${WIN32}>:${XINPUT} ws2_32>
	$<$<BOOL:${LINUX}>:X11 GL rt>
	$<$<BOOL:${APPLE}>:${FOUNDATION} ${IOKIT} ${APPKIT}>
)

if(MSVC)
	set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "/SUBSYSTEM:CONSOLE")
endif(MSVC)

target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/lib/include/")
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/extern/catch/")
