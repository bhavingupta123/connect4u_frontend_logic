# /****************************************************************************
# Copyright (c) 2013-2014 cocos2d-x.org
# Copyright (c) 2019-present Axmol Engine contributors (see AUTHORS.md).
# https://axmol.dev/
# ****************************************************************************/

cmake_minimum_required(VERSION 3.20)

set(APP_NAME Connect4Game)
project(${APP_NAME})

# ------------------ Platform Compatibility ------------------
# Ensure the project runs on your macOS version (15.3.2)
set(CMAKE_OSX_DEPLOYMENT_TARGET "15.3")

# ------------------ Debugging Options ------------------
set(CMAKE_VERBOSE_MAKEFILE ON)

# ------------------ Axmol Engine Setup ------------------
if(XCODE)
  set(CMAKE_XCODE_GENERATE_TOP_LEVEL_PROJECT_ONLY TRUE)
endif()

set(_is_axmol_embed FALSE)
set(_AX_USE_PREBUILT FALSE)

# Add Axmol CMake modules
set(CMAKE_MODULE_PATH
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules
)

# Order of includes matters
include(AXGameEngineOptions)
include(AXGameEngineSetup)

# ------------------ Game Source ------------------
file(GLOB_RECURSE GAME_HEADER
  Source/*.h Source/*.hpp
)

file(GLOB_RECURSE GAME_SOURCE
  Source/*.cpp Source/*.c
)

set(GAME_INC_DIRS
  "${CMAKE_CURRENT_SOURCE_DIR}/Source"
)

# ------------------ Game Assets ------------------
set(content_folder "${CMAKE_CURRENT_SOURCE_DIR}/Content")

if(APPLE)
  ax_mark_multi_resources(common_content_files RES_TO "Resources" FOLDERS ${content_folder})
elseif(WINDOWS)
  ax_mark_multi_resources(common_content_files RES_TO "Content" FOLDERS ${content_folder})
endif()

# ------------------ Build Targets ------------------
include(AXGameSourceSetup)

set(APP_SOURCES
  ${GAME_HEADER}
  ${GAME_SOURCE}
)

include(AXGameTargetSetup)

# ------------------ App Config & Resources ------------------
ax_setup_app_config(${APP_NAME})

# ------------------ Platform-specific Build Setup ------------------
include(AXGamePlatformSetup)

# ------------------ Final Setup (Required) ------------------
include(AXGameFinalSetup)
