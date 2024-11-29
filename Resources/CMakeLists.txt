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
# ****************************************************************************/

# CMakeLists.txt
cmake_minimum_required(VERSION 3.6)

set(APP_NAME HearthStone)
project(${APP_NAME})

# 设置 Cocos2d-x 根目录
set(COCOS2DX_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cocos2d)
set(CMAKE_MODULE_PATH ${COCOS2DX_ROOT_PATH}/cmake/Modules/)

include(CocosBuildSet)
add_subdirectory(${COCOS2DX_ROOT_PATH}/cocos ${ENGINE_BINARY_PATH}/cocos/core)

# 记录源文件
set(GAME_SOURCE
    Classes/AppDelegate.cpp
    Classes/AppDelegate.h
    
    # Card System
    Classes/Card/Card.cpp
    Classes/Card/Card.h
    Classes/Card/MinionCard.cpp
    Classes/Card/MinionCard.h
    Classes/Card/SpellCard.cpp
    Classes/Card/SpellCard.h
    
    # Effect System
    Classes/Effect/EffectManager.cpp
    Classes/Effect/EffectManager.h
    Classes/Effect/IEffect.h
    Classes/Effect/Effects/DamageEffect.cpp
    Classes/Effect/Effects/DamageEffect.h
    Classes/Effect/Effects/HealEffect.cpp
    Classes/Effect/Effects/HealEffect.h
    
    # Game System
    Classes/Game/TurnSystem.cpp
    Classes/Game/TurnSystem.h
    Classes/Game/GameManager.cpp
    Classes/Game/GameManager.h
    
    # Battle System
    Classes/Battle/CombatManager.cpp
    Classes/Battle/CombatManager.h
    
    # Animation System
    Classes/Animation/AnimationManager.cpp
    Classes/Animation/AnimationManager.h
    
    # Network System
    Classes/Network/NetworkManager.cpp
    Classes/Network/NetworkManager.h
    Classes/Network/GameAction.h
    Classes/Network/WebSocketClient.h
    
    # Scene System
    Classes/Scene/GameScene.cpp
    Classes/Scene/GameScene.h
    Classes/Scene/MenuScene.cpp
    Classes/Scene/MenuScene.h
    
    # Utils
    Classes/Utils/Constants.h
    Classes/Utils/Utils.h
)

# 记录资源文件
set(GAME_RESOURCE
    Resources/fonts
    Resources/images
    Resources/particles
    Resources/audio
)

if(WINDOWS)
    list(APPEND GAME_SOURCE
        proj.win32/main.cpp
        proj.win32/game.rc
        ${GAME_RESOURCE}
    )
elseif(MACOSX OR IOS)
    set(APP_UI_RES
        proj.ios_mac/ios/LaunchScreen.storyboard
        proj.ios_mac/ios/LaunchScreenBackground.png
        proj.ios_mac/ios/Images.xcassets
    )
    list(APPEND GAME_SOURCE
        proj.ios_mac/mac/main.cpp
        ${APP_UI_RES}
        ${GAME_RESOURCE}
    )
endif()

# 设置目标
if(ANDROID)
    add_library(${APP_NAME} SHARED ${GAME_SOURCE})
    IF(CMAKE_BUILD_TYPE MATCHES RELEASE)
        ADD_CUSTOM_COMMAND(TARGET ${APP_NAME} POST_BUILD COMMAND ${CMAKE_STRIP} lib${APP_NAME}.so)
    ENDIF()
else()
    add_executable(${APP_NAME} ${GAME_SOURCE})
endif()

target_link_libraries(${APP_NAME} cocos2d)
target_include_directories(${APP_NAME} PRIVATE Classes)

# 设置预处理器宏
target_compile_definitions(${APP_NAME} PRIVATE
    $<$<CONFIG:Debug>:COCOS2D_DEBUG=1>
)

if(MSVC)
    target_compile_definitions(${APP_NAME} PRIVATE
        _CRT_SECURE_NO_WARNINGS
        _SCL_SECURE_NO_WARNINGS
    )
endif()

# 复制资源文件
if(APPLE)
    set_target_properties(${APP_NAME} PROPERTIES RESOURCE "${GAME_RESOURCE}")
    if(MACOSX)
        set_target_properties(${APP_NAME} PROPERTIES
            MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/proj.ios_mac/mac/Info.plist"
        )
    elseif(IOS)
        cocos_pak_xcode(${APP_NAME} INFO_PLIST "iOSBundleInfo.plist.in")
        set_xcode_property(${APP_NAME} ASSETCATALOG_COMPILER_APPICON_NAME "AppIcon")
        set_xcode_property(${APP_NAME} DEVELOPMENT_TEAM "")
        set_xcode_property(${APP_NAME} CODE_SIGN_IDENTITY "iPhone Developer")
    endif()
elseif(WINDOWS)
    cocos_copy_target_dll(${APP_NAME})
endif()

# 复制资源
if(WINDOWS OR LINUX)
    cocos_copy_target_res(${APP_NAME} COPY_TO ${CMAKE_CURRENT_BINARY_DIR}/bin/${APP_NAME} FOLDERS ${GAME_RESOURCE})
endif()
# Additional source files
set(GAME_SRC
    ${GAME_SRC}
    Classes/Utils/ConfigManager.cpp
    Classes/Utils/GameLogger.cpp
    Classes/Utils/ResourceManager.cpp
    Classes/Player/Player.cpp
)

# Additional header files
set(GAME_HEADERS
    ${GAME_HEADERS}
    Classes/Utils/ConfigManager.h
    Classes/Utils/GameLogger.h
    Classes/Utils/ResourceManager.h
    Classes/Player/Player.h
)
