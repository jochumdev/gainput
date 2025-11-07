#
# CMake Requirements
#
cmake_minimum_required(VERSION 3.25.0 FATAL_ERROR)

#
# Project Definition
#
project(gainput
	VERSION 2.1.0
	LANGUAGES CXX
	DESCRIPTION "C++ gaming input library"
)

message(STATUS "GAINPUT version ${PROJECT_VERSION}")

set(CMAKE_MACOSX_RPATH 1)

# Configure the gainput.cpp with the correct project version from CMake
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/source/gainput/gainput.cpp.in" "${CMAKE_BINARY_DIR}/configured_files/gainput.cpp")

set(GAINPUT_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/source/gainput/GainputAllocator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/gainput/GainputInputDeltaState.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/gainput/GainputInputDevice.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/gainput/GainputInputManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/gainput/GainputInputMap.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/gainput/GainputInputState.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/gainput/GainputMapFilters.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/gainput/builtin/GainputInputDeviceBuiltIn.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/gainput/dev/GainputDev.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/gainput/dev/GainputMemoryStream.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/gainput/dev/GainputNetAddress.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/gainput/dev/GainputNetConnection.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/gainput/dev/GainputNetListener.cpp
    ${CMAKE_BINARY_DIR}/configured_files/gainput.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/gainput/gestures/GainputButtonStickGesture.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/gainput/gestures/GainputDoubleClickGesture.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/gainput/gestures/GainputHoldGesture.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/gainput/gestures/GainputPinchGesture.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/gainput/gestures/GainputRotateGesture.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/gainput/gestures/GainputSimultaneouslyDownGesture.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/gainput/gestures/GainputTapGesture.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/gainput/keyboard/GainputInputDeviceKeyboard.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/gainput/keyboard/GainputInputDeviceKeyboardMac.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/gainput/mouse/GainputInputDeviceMouse.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/gainput/pad/GainputInputDevicePad.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/gainput/pad/GainputInputDevicePadMac.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/gainput/recorder/GainputInputPlayer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/gainput/recorder/GainputInputRecorder.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/gainput/recorder/GainputInputRecording.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/gainput/touch/GainputInputDeviceTouch.cpp
)

set(GAINPUT_APPLE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/source/gainput/GainputIos.mm
    ${CMAKE_CURRENT_SOURCE_DIR}/source/gainput/GainputMac.mm
    ${CMAKE_CURRENT_SOURCE_DIR}/source/gainput/builtin/GainputInputDeviceBuiltInIos.mm
    ${CMAKE_CURRENT_SOURCE_DIR}/source/gainput/mouse/GainputInputDeviceMouseMac.mm
    ${CMAKE_CURRENT_SOURCE_DIR}/source/gainput/mouse/GainputInputDeviceMouseMacRaw.mm
    ${CMAKE_CURRENT_SOURCE_DIR}/source/gainput/pad/GainputInputDevicePadIos.mm
)

if(("${GAINPUT_TARGET_PLATFORM}" MATCHES "QuestVR") OR ANDROID )
  set(GAINPUT_FILES
      ${GAINPUT_FILES}
      ${CMAKE_CURRENT_SOURCE_DIR}/source/gainput/touch/GainputInputDeviceGestureDetectorAndroid.cpp
  )
elseif(LINUX)
  if(GAINPUT_ENABLE_HID)
    set(GAINPUT_FILES
        ${GAINPUT_FILES}
        ${CMAKE_CURRENT_SOURCE_DIR}/source/gainput/hid/timer/GainputTimer.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/source/gainput/hid/timer/GainputTimerLinux.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/source/gainput/hid/GainputHID.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/source/gainput/hid/GainputHIDWhitelist.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/source/gainput/hid/hidparsers/HIDParserPS4Controller.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/source/gainput/hid/hidparsers/HIDParserPS5Controller.cpp
    )
  endif()
elseif("${GAINPUT_TARGET_PLATFORM}" MATCHES "NX64")
  # Nintendo 64
elseif(WIN32)
  set(GAINPUT_FILES
      ${GAINPUT_FILES}
      ${CMAKE_CURRENT_SOURCE_DIR}/source/gainput/hid/timer/GainputTimer.cpp
      ${CMAKE_CURRENT_SOURCE_DIR}/source/gainput/hid/timer/GainputTimerWindows.cpp
      ${CMAKE_CURRENT_SOURCE_DIR}/source/gainput/hid/GainputHID.cpp
      ${CMAKE_CURRENT_SOURCE_DIR}/source/gainput/hid/GainputHIDWhitelist.cpp
      ${CMAKE_CURRENT_SOURCE_DIR}/source/gainput/hid/hidparsers/HIDParserPS4Controller.cpp
      ${CMAKE_CURRENT_SOURCE_DIR}/source/gainput/hid/hidparsers/HIDParserPS5Controller.cpp
  )
elseif( APPLE OR ("${GAINPUT_TARGET_PLATFORM}" MATCHES "Darwin") )
  set(GAINPUT_FILES ${GAINPUT_FILES} ${GAINPUT_APPLE_FILES})
elseif("${GAINPUT_TARGET_PLATFORM}" MATCHES "ORBIS")
  # PS4
  set(GAINPUT_FILES
      ${GAINPUT_FILES}
      ${CMAKE_CURRENT_SOURCE_DIR}/source/gainput/hid/hidparsers/HIDParserPS4Controller.cpp
      ${CMAKE_CURRENT_SOURCE_DIR}/source/gainput/hid/hidparsers/HIDParserPS5Controller.cpp
  )

elseif("${GAINPUT_TARGET_PLATFORM}" MATCHES "PROSPERO")
  # PS5
  set(GAINPUT_FILES
      ${GAINPUT_FILES}
      ${CMAKE_CURRENT_SOURCE_DIR}/source/gainput/hid/hidparsers/HIDParserPS4Controller.cpp
      ${CMAKE_CURRENT_SOURCE_DIR}/source/gainput/hid/hidparsers/HIDParserPS5Controller.cpp
  )
endif()

add_library(${PROJECT_NAME} ${GAINPUT_FILES})
add_library(gainput::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  target_compile_options(${PROJECT_NAME} PRIVATE -std=gnu++14 -Wshadow -Wno-variadic-macros)
endif()

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/include)

target_link_libraries(${PROJECT_NAME} 
  PRIVATE $<$<BOOL:${GAINPUT_ENABLE_HID}>:hidapi::hidapi>
          $<$<BOOL:${WIN32}>:winmm.lib> # Needed otherwise timeGetTime has no reference
          $<$<BOOL:${LINUX}>:X11> # Needed oterhwise XkbKeycodeToKeysym gets missing reference
)

target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/")
set(install_libs ${install_libs} gainput)

if("${GAINPUT_TARGET_PLATFORM}" MATCHES "QuestVR")
  set(${PROJECT_NAME}_DEFINES ${GAINPUT_DEFINES} GAINPUT_PLATFORM_QUEST)
  target_link_libraries(${PROJECT_NAME} PUBLIC native_app_glue log android)

elseif(ANDROID)
  set(${PROJECT_NAME}_DEFINES ${GAINPUT_DEFINES} GAINPUT_PLATFORM_ANDROID)
  target_link_libraries(${PROJECT_NAME} PUBLIC native_app_glue log android)

elseif(LINUX)
  set(${PROJECT_NAME}_DEFINES ${GAINPUT_DEFINES} GAINPUT_PLATFORM_LINUX)

  if(GAINPUT_ENABLE_HID)
    target_link_libraries(${PROJECT_NAME} PUBLIC hidapi::hidapi)
  endif()

elseif("${GAINPUT_TARGET_PLATFORM}" MATCHES "NX64")
  # Nintendo 64
  set(${PROJECT_NAME}_DEFINES ${GAINPUT_DEFINES} GAINPUT_PLATFORM_NX64 "GAINPUT_LIB_DYNAMIC=1")
  target_link_libraries(${PROJECT_NAME} PUBLIC Xinput9_1_0 ws2_32)

elseif(WIN32)
  set(${PROJECT_NAME}_DEFINES ${GAINPUT_DEFINES} GAINPUT_PLATFORM_NX64 "GAINPUT_LIB_DYNAMIC=1")
  target_link_libraries(${PROJECT_NAME} PUBLIC Xinput9_1_0 ws2_32 hidapi::hidapi)

elseif("${GAINPUT_TARGET_PLATFORM}" MATCHES "TVOS")
  # AppleTV
  set(${PROJECT_NAME}_DEFINES ${GAINPUT_DEFINES} GAINPUT_PLATFORM_TVOS)
  find_library(FOUNDATION Foundation)
  find_library(IOKIT IOKit)
  find_library(GAME_CONTROLLER GameController)
  find_library(UIKIT UIKit)
  find_library(COREMOTION CoreMotion)
  find_library(QUARTZCORE QuartzCore)
  target_link_libraries(
    ${PROJECT_NAME} PUBLIC ${FOUNDATION} ${IOKIT} ${GAME_CONTROLLER} ${UIKIT} ${COREMOTION}
  )

elseif(IOS)
  set(${PROJECT_NAME}_DEFINES ${GAINPUT_DEFINES} GAINPUT_PLATFORM_IOS)
  if(${PROJECT_NAME}_ENABLE_IOS_HAPTICS MATCHES ON)
    set(${PROJECT_NAME}_DEFINES ${GAINPUT_DEFINES} GAINPUT_IOS_HAPTICS)
  endif()

  find_library(FOUNDATION Foundation)
  find_library(IOKIT IOKit)
  find_library(GAME_CONTROLLER GameController)
  find_library(UIKIT UIKit)
  find_library(COREMOTION CoreMotion)
  find_library(QUARTZCORE QuartzCore)
  target_link_libraries(
    ${PROJECT_NAME} PUBLIC ${FOUNDATION} ${IOKIT} ${GAME_CONTROLLER} ${UIKIT} ${COREMOTION}
  )

elseif("${GAINPUT_TARGET_PLATFORM}" MATCHES "Darwin")
  set(${PROJECT_NAME}_DEFINES ${GAINPUT_DEFINES} GAINPUT_PLATFORM_MAC)

  find_library(FOUNDATION Foundation)
  find_library(IOKIT IOKit)
  find_library(GAME_CONTROLLER GameController)
  find_library(APPKIT AppKit)
  target_link_libraries(${PROJECT_NAME} PUBLIC ${FOUNDATION} ${IOKIT} ${GAME_CONTROLLER} ${APPKIT})

elseif("${GAINPUT_TARGET_PLATFORM}" MATCHES "ORBIS")
  # PS4
  set(${PROJECT_NAME}_DEFINES ${GAINPUT_DEFINES} GAINPUT_PLATFORM_ORBIS)

elseif("${GAINPUT_TARGET_PLATFORM}" MATCHES "PROSPERO")
  # PS5
  set(${PROJECT_NAME}_DEFINES ${GAINPUT_DEFINES} GAINPUT_PLATFORM_PROSPERO)
endif()

target_compile_definitions(${PROJECT_NAME} PUBLIC ${GAINPUT_DEFINES})

# Library installation directory
if(NOT DEFINED CMAKE_INSTALL_LIBDIR)
  set(CMAKE_INSTALL_LIBDIR lib)
endif(NOT DEFINED CMAKE_INSTALL_LIBDIR)
set(libdir ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR})

install(
  DIRECTORY "include/gainput"
  DESTINATION "include"
  FILES_MATCHING
  PATTERN "*.h"
)

install(
  TARGETS ${install_libs}
  LIBRARY DESTINATION "${libdir}"
  ARCHIVE DESTINATION "${libdir}"
  RUNTIME DESTINATION "bin"
)
