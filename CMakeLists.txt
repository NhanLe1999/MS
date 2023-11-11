#/****************************************************************************
# Copyright (c) 2013-2014 cocos2d-x.org
# Copyright (c) 2015-2017 Chukong Technologies Inc.
#
# http://www.cocos2d-x.org
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:

# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
# ***************************************************************************/


cmake_minimum_required(VERSION 3.6)

string(FIND "${CMAKE_CURRENT_BINARY_DIR}" "vmonkey" VMONKEY_POS_IN_PATH)

MESSAGE("------------Get the firebase path from build.config--------------------------")

if( WIN32 )
	file(READ "build_win.config" buildconfig)
	MESSAGE("------------Windows Environments: build_win.config--------------------------")
else()
	file(READ "build.config" buildconfig)
endif()

string(REGEX MATCH "(\n.*firebase.*)" firebasePathStr ${buildconfig})

string(REPLACE "\n" "" firebasePath ${firebasePathStr})

MESSAGE("Firebase path:" ${firebasePath})

set(FIREBASE_CPP_SDK_DIR "${firebasePath}")

MESSAGE("------------firebase build.config--------------------------")

set(MSVC_RUNTIME_MODE "MD")

if (MACOSX)
	set(CMAKE_OSX_SYSROOT macosx10.15)
	set(CMAKE_OSX_DEPLOYMENT_TARGET "10.15")
	set(OPENSSL_ROOT_DIR "/usr/local/opt/openssl")
	set(OPENSSL_LIBRARIES "/usr/local/opt/openssl/lib")
endif()



if (${VMONKEY_POS_IN_PATH} EQUAL "-1")
    message("making Monkey Stories...")
  if (CMAKE_BUILD_TYPE MATCHES Debug)
    set(APP_NAME stories_debug)
  else()
    set(APP_NAME MonkeyStories)
  endif()
    set(APP_EXEC_NAME "MonkeyStories")
    set(IS_VMONKEY OFF)
else()
  message("making VMonkey...")
  if (CMAKE_BUILD_TYPE MATCHES Debug)
  set(APP_NAME vmonkey_debug)
else()
  set(APP_NAME VMonkey)
endif()
  set(APP_EXEC_NAME "VMonkey")
  add_definitions(-DVMONKEY)
  set(IS_VMONKEY ON)
endif()

project(${APP_NAME})

set(COCOS2DX_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cocos2d)
set(CMAKE_MODULE_PATH ${COCOS2DX_ROOT_PATH}/cmake/Modules/)
set(PROJECT_WIN_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/proj.win32)

SET(CMAKE_C_USE_RESPONSE_FILE_FOR_OBJECTS 1)
SET(CMAKE_CXX_USE_RESPONSE_FILE_FOR_OBJECTS 1)

SET(CMAKE_C_RESPONSE_FILE_LINK_FLAG "@")
SET(CMAKE_CXX_RESPONSE_FILE_LINK_FLAG "@")

include(CocosBuildSet)
add_subdirectory(${COCOS2DX_ROOT_PATH}/cocos ${ENGINE_BINARY_PATH}/cocos/core)

# record sources, headers, resources...
set(GAME_SOURCE)
set(GAME_HEADER)

FILE(GLOB_RECURSE USER_HEADER "Classes/*.hpp" "Classes/*.h" "${FIREBASE_CPP_SDK_DIR}/include/*.h" "project.thirdparty/Spine/spine-cocos2dx/*.h" "project.thirdparty/Spine/spine-cpp/*.h")
FILE(GLOB_RECURSE USER_CPP "Classes/*.cpp" "Classes/*.c" "project.thirdparty/Spine/spine-cocos2dx/*.cpp" "project.thirdparty/Spine/spine-cpp/*.cpp")

MESSAGE("---------Classes-------")
FOREACH(FILE_PATH ${USER_HEADER})
	if(${dir_path} MATCHES "^(SQLiteCpp)")
		MESSAGE("continue....")
		continue()
	endif()
    MESSAGE(${FILE_PATH})
ENDFOREACH(FILE_PATH)

MESSAGE("<start>List Files:---------------")
FOREACH(FILE_PATH ${USER_CPP})
	if(${dir_path} MATCHES "^(SQLiteCpp)")
		MESSAGE("continue....")
		continue()
	endif()
    MESSAGE(${FILE_PATH})
ENDFOREACH(FILE_PATH)
MESSAGE("<end>List Files:---------------")

list(APPEND GAME_SOURCE
        ${USER_CPP}
    )
	
list(APPEND GAME_HEADER
        ${USER_HEADER}
    )
	 

set(GAME_RES_FOLDER
    "${CMAKE_CURRENT_SOURCE_DIR}/Resources"
)

if(APPLE OR WINDOWS)
    cocos_mark_multi_resources(common_res_files RES_TO "Resources" FOLDERS ${GAME_RES_FOLDER})
endif()


MACRO(HEADER_DIRECTORIES return_list)
    SET(dir_list "")
	MESSAGE("<start>List Folders:---------------")
    FOREACH(file_path ${USER_HEADER})
        GET_FILENAME_COMPONENT(dir_path ${file_path} PATH)
		
		if(${dir_path} MATCHES "^(SQLiteCpp)")
			MESSAGE("continue....")
			continue()
		endif()
		
		MESSAGE(${dir_path})
        SET(dir_list ${dir_list} ${dir_path})
    ENDFOREACH()
	MESSAGE("<end>List Folders:---------------")
    LIST(REMOVE_DUPLICATES dir_list)
    SET(${return_list} ${dir_list})
ENDMACRO()

# add cross-platforms source files and header files
list(APPEND GAME_SOURCE
    ${AP_SRC}
)

list(APPEND GAME_HEADER	
    ${AP_HEADER}
)

if(ANDROID)
    # change APP_NAME to the share library name for Android, it's value depend on AndroidManifest.xml
    set(APP_NAME MyGame)
    list(APPEND GAME_SOURCE
        build-android/app/jni/hellocpp/main.cpp
    )

elseif(LINUX)
    list(APPEND GAME_SOURCE
        proj.linux/main.cpp
         )
elseif(WINDOWS)
  if(IS_VMONKEY)
    list(APPEND GAME_SOURCE
    proj.win32/res/vm/game.rc
    )
  else()
  list(APPEND GAME_SOURCE

  proj.win32/res/ms/game.rc
  )
  endif()

    list(APPEND GAME_HEADER
        proj.win32/main.h
        proj.win32/resource.h
        proj.win32/Win32DeviceInfo.h
    )
    list(APPEND GAME_SOURCE
        proj.win32/main.cpp
		     proj.win32/Win32DeviceInfo.cpp
         ${common_res_files}
         )
  string(REGEX REPLACE "/Z[iI7]" ""
         CMAKE_CXX_FLAGS_RELEASE
         "${CMAKE_CXX_FLAGS_RELEASE}")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Z7")
  string(REGEX REPLACE "/Z[iI7]" ""
         CMAKE_CXX_FLAGS_DEBUG
         "${CMAKE_CXX_FLAGS_DEBUG}")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Z7")
elseif(APPLE)
    if(IOS)
        list(APPEND GAME_HEADER
            proj.ios_mac/ios/AppController.h
            proj.ios_mac/ios/RootViewController.h
        )
        set(APP_UI_RES
            proj.ios_mac/ios/LaunchScreen.storyboard
            proj.ios_mac/ios/LaunchScreenBackground.png
            proj.ios_mac/ios/Images.xcassets
        )
        list(APPEND GAME_SOURCE
            proj.ios_mac/ios/main.m
            proj.ios_mac/ios/AppController.mm
            proj.ios_mac/ios/RootViewController.mm
            proj.ios_mac/ios/Prefix.pch
            ${APP_UI_RES}
        )
    elseif(MACOSX)
        message( "is_vmonkey : ${IS_VMONKEY}")
        if (${VMONKEY_POS_IN_PATH} EQUAL "-1")
          set(APP_UI_RES
              proj.ios_mac/mac/ms/Icon.icns
              proj.ios_mac/mac/ms/Info.plist
              proj.ios_mac/mac/ms/google-services.json
          )
        else()
            set(APP_UI_RES
                proj.ios_mac/mac/vm/Icon.icns
                proj.ios_mac/mac/vm/Info.plist
                proj.ios_mac/mac/vm/google-services.json
            )
        endif()

        list(APPEND GAME_HEADER
            proj.ios_mac/ios/Reachability.h
            proj.ios_mac/RMStore/RMStore.h
        )
        list(APPEND GAME_SOURCE
            proj.ios_mac/ios/Reachability.m
            proj.ios_mac/RMStore/RMStore.mm
            Classes/mjlib/MacCppBridge.mm
            Classes/mjlib/MacBridge.mm
            Classes/lib/MJPlatformConfig-Mac.mm
            Classes/recorder/MSScreenRecorder.mm
            Classes/rating/RatingPopup.mm
            Classes/story/homescene/subscriptions/MJInApp-Apple.mm
            Classes/story/homescene/subscriptions/StoreHelper.mm
            Classes/games/phonics/MSRecord-Apple.mm
            proj.ios_mac/mac/main.cpp
            proj.ios_mac/mac/Prefix.pch
            ${APP_UI_RES}
        )
    endif()
    list(APPEND GAME_SOURCE ${common_res_files})
endif()

# mark app complie info and libs info
set(all_code_files
    ${GAME_HEADER}
    ${GAME_SOURCE}
)
if(NOT ANDROID)
    add_executable(${APP_NAME} ${all_code_files})
else()
    add_library(${APP_NAME} SHARED ${all_code_files})
    add_subdirectory(${COCOS2DX_ROOT_PATH}/cocos/platform/android ${ENGINE_BINARY_PATH}/cocos/platform)
    target_link_libraries(${APP_NAME} -Wl,--whole-archive cpp_android_spec -Wl,--no-whole-archive)
endif()

# list folders

HEADER_DIRECTORIES(header_dir_list)

list(LENGTH header_dir_list header_dir_list_count)

target_link_libraries(${APP_NAME} cocos2d)

if (ANDROID)
    target_include_directories(${APP_NAME}
        PRIVATE build-android/app/jni
    )
endif()

if (MACOSX)
    target_include_directories(${APP_NAME}
        PRIVATE proj.ios_mac/ios
        PRIVATE proj.ios_mac/RMStore
    )
endif()

target_include_directories(${APP_NAME}
	PRIVATE ${header_dir_list} # Recursive	
    PRIVATE ${COCOS2DX_ROOT_PATH}/cocos/audio/include
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/project.thirdparty/Spine/spine-cocos2dx/src
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/project.thirdparty/Spine/spine-cpp/spine-cpp/include
    ${AP_DIR}
)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/project.thirdparty/SQLiteCpp)
target_include_directories(${APP_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/project.thirdparty/SQLiteCpp/include/)
target_include_directories(${APP_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/project.thirdparty/SQLiteCpp/include/SQLiteCpp)

target_link_libraries(${APP_NAME} SQLiteCpp sqlite3)

add_subdirectory(${FIREBASE_CPP_SDK_DIR} bin/ EXCLUDE_FROM_ALL)
# The core Firebase library (firebase_app) is required to use any Firebase product,
# and it must always be listed last.
target_link_libraries(${APP_NAME} firebase_auth)
target_link_libraries(${APP_NAME} firebase_database)
target_link_libraries(${APP_NAME} firebase_remote_config)
target_link_libraries(${APP_NAME} firebase_dynamic_links)
target_link_libraries(${APP_NAME} firebase_messaging)
target_link_libraries(${APP_NAME} firebase_analytics)
target_link_libraries(${APP_NAME} firebase_firestore)
target_link_libraries(${APP_NAME} firebase_app)

# mark app resources
setup_cocos_app_config(${APP_NAME})

if(APPLE)

    set_target_properties(${APP_NAME} PROPERTIES RESOURCE "${APP_UI_RES}")
    set_source_files_properties(proj.ios_mac/RMStore/RMStore.mm PROPERTIES COMPILE_FLAGS -fobjc-arc)

    if(MACOSX)
      if (${VMONKEY_POS_IN_PATH} EQUAL "-1")
        set_target_properties(${APP_NAME} PROPERTIES
            MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/proj.ios_mac/mac/ms/Info.plist"
            )
      else()
          set_target_properties(${APP_NAME} PROPERTIES
              MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/proj.ios_mac/mac/vm/Info.plist"
            )
      endif()

		find_library(STORE_KIT StoreKit)
		find_library(SYSTEM_CONFIG SystemConfiguration)
		find_library(CORE_FOUNDATION CoreFoundation)
		find_library(FOUNDATION Foundation)
		find_library(APPKIT AppKit)
		find_library(SECURITY Security)
		#find_library(PTHREAD pthread)
		target_link_libraries(${PROJECT_NAME} "-framework GSS")
		target_link_libraries(${PROJECT_NAME} "-framework Kerberos")
		target_link_libraries(${APP_NAME} ${SYSTEM_CONFIG} ${STORE_KIT} ${CORE_FOUNDATION} ${FOUNDATION} ${SECURITY})

		target_link_libraries(${APP_NAME}
			${CMAKE_CURRENT_SOURCE_DIR}/proj.ios_mac/mac/boost/libboost_atomic.a
			${CMAKE_CURRENT_SOURCE_DIR}/proj.ios_mac/mac/boost/libboost_chrono.a
			${CMAKE_CURRENT_SOURCE_DIR}/proj.ios_mac/mac/boost/libboost_system.a
			${CMAKE_CURRENT_SOURCE_DIR}/proj.ios_mac/mac/boost/libboost_thread.a
			${CMAKE_CURRENT_SOURCE_DIR}/proj.ios_mac/mac/boost/libboost_date_time.a
		)
        #find_package(Boost COMPONENTS thread system chrono atomic date_time REQUIRED)
        #include_directories(${Boost_INCLUDE_DIR})
        #target_link_libraries(${APP_NAME} ${Boost_LIBRARIES})
 # CORE_FOUNDATION APPKIT SECURITY PTHREAD)
    elseif(IOS)
        cocos_pak_xcode(${APP_NAME} INFO_PLIST "iOSBundleInfo.plist.in")
        set_xcode_property(${APP_NAME} ASSETCATALOG_COMPILER_APPICON_NAME "AppIcon")
    endif()
elseif(WINDOWS)

	string(REPLACE "/Zi" "/Z7" CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG}")
	string(REPLACE "/Zi" "/Z7" CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
	string(REPLACE "/Zi" "/Z7" CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELWITHDEBINFO}")
	string(REPLACE "/Zi" "/Z7" CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
	
    set_target_properties(${APP_NAME} PROPERTIES OUTPUT_NAME ${APP_EXEC_NAME})
    cocos_copy_target_dll(${APP_NAME} COPY_TO ${APP_RES_DIR}/..)
	
	if (${VMONKEY_POS_IN_PATH} EQUAL "-1")
       add_custom_command(
		TARGET ${APP_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E 
		copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/proj.win32/res/services/vm" $<TARGET_FILE_DIR:${APP_NAME}>
		) 
	else()
        add_custom_command(
		TARGET ${APP_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E 
		copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/proj.win32/res/services/ms" $<TARGET_FILE_DIR:${APP_NAME}>
		) 	
    endif()
		
	#message(STATUS "[windows] copy dll files.")
	#runtime copy
	add_custom_command(
		TARGET ${APP_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E 
		copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/project.release/ms/dll" $<TARGET_FILE_DIR:${APP_NAME}>
	) 
		
	if(IS_VMONKEY)
		set(SERVICES_RES_FOLDER
			"${CMAKE_CURRENT_SOURCE_DIR}/proj.win32/res/services/vm/"
		)
	else()
		set(SERVICES_RES_FOLDER
				"${CMAKE_CURRENT_SOURCE_DIR}/proj.win32/res/services/ms/"
		)
	endif()
	
	#message(STATUS "[windows] copy dll files.")
	#runtime copy
	add_custom_command(
		TARGET ${APP_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E 
		copy_directory "${SERVICES_RES_FOLDER}" $<TARGET_FILE_DIR:${APP_NAME}>/Resources
	) 
	
	#message(STATUS "[windows] link support files")
	#runtime link
	target_link_libraries(${APP_NAME} 
		crypt32.lib 
		iphlpapi.lib 
		psapi.lib 
		userenv.lib  
	)
	
	find_package(cpprestsdk CONFIG REQUIRED)
	target_link_libraries(${APP_NAME} cpprestsdk::cpprest)
	
endif()


if(MACOSX)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/proj.ios_mac/mac/cpprestsdk)
    target_include_directories(${APP_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/proj.ios_mac/mac/cpprestsdk/include)
    target_link_libraries(${APP_NAME} cpprest)
  #  find_package(cpprestsdk REQUIRED)
  #  target_link_libraries(${APP_NAME} cpprestsdk::cpprest)
endif()

if(NOT WINDOWS)
	if(IS_VMONKEY)
		set(SERVICES_RES_FOLDER
			"${CMAKE_CURRENT_SOURCE_DIR}/proj.win32/res/services/vm/"
		)
	else()
		set(SERVICES_RES_FOLDER
				"${CMAKE_CURRENT_SOURCE_DIR}/proj.win32/res/services/ms/"
		)
	endif()
endif()

if(LINUX OR WINDOWS)
	add_custom_command(
        TARGET ${APP_NAME}
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E
        create_symlink ${CMAKE_CURRENT_SOURCE_DIR}/Resources $<TARGET_FILE_DIR:${APP_NAME}>/Resources)
endif()
