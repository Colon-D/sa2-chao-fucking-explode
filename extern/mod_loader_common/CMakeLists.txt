cmake_minimum_required(VERSION 3.14)
include(FetchContent)

FetchContent_Declare(
  "mod_loader_common"
  GIT_REPOSITORY "https://github.com/sonicretro/mod-loader-common.git"
  GIT_TAG        "59fbefbbf2c3ca5e6ec7a355ce70a7a5db2db5c1"
)
FetchContent_MakeAvailable("mod_loader_common")

add_library("mod_loader_common" STATIC
    "${mod_loader_common_SOURCE_DIR}/ModLoaderCommon/CodeParser.cpp"
    "${mod_loader_common_SOURCE_DIR}/ModLoaderCommon/FileSystem.cpp"
    "${mod_loader_common_SOURCE_DIR}/ModLoaderCommon/IniFile.cpp"
    "${mod_loader_common_SOURCE_DIR}/ModLoaderCommon/stdafx.cpp"
    "${mod_loader_common_SOURCE_DIR}/ModLoaderCommon/TextConv.cpp"
)
target_include_directories("mod_loader_common" PUBLIC
    "${mod_loader_common_SOURCE_DIR}/ModLoaderCommon"
)
