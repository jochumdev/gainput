
project(listenersample)

include_directories(../../lib/include/)
include_directories(../samplefw/)

file(GLOB_RECURSE sources *.cpp)
file(GLOB_RECURSE sfwsources ../samplefw/*.cpp)

if(ANDROID)
	add_library(listenersample SHARED ${sources} ${sfwsources})
else()
	add_executable(listenersample WIN32 ${sources} ${sfwsources})
endif()

target_link_libraries(listenersample gainput)

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	target_link_libraries(listenersample X11 GL rt)
elseif(WIN32)
	target_link_libraries(listenersample ${XINPUT} ws2_32)
elseif(APPLE)
	target_link_libraries(listenersample ${FOUNDATION} ${IOKIT} ${APPKIT})
endif()
