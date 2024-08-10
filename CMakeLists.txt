cmake_minimum_required(VERSION 3.20)

project("sa2_chao_fucking_explode")

add_library("${PROJECT_NAME}" SHARED
    "src/main.cpp"
    "src/extra_math.cpp"
    "src/chao_functions.cpp"
    "src/hooks.cpp"
    "src/globals.cpp"
    "src/config.cpp"
)
target_compile_features("${PROJECT_NAME}" PRIVATE cxx_std_20)

add_subdirectory(extern)
target_link_libraries("${PROJECT_NAME}" PRIVATE
    "mod_loader_common"
    "libmodutils"
)
target_include_directories("${PROJECT_NAME}" PRIVATE
	"include"
)

# if Sonic Adventure 2 directory is set, set the runtime output directory to 
# the appropriate mod directory
set("SONIC_ADVENTURE_2_DIRECTORY" "C:/Program Files (x86)/Steam/steamapps/common/Sonic Adventure 2/" CACHE PATH "")
# convert to forward slashes
cmake_path(SET "SONIC_ADVENTURE_2_DIRECTORY" "${SONIC_ADVENTURE_2_DIRECTORY}")
IF(EXISTS "${SONIC_ADVENTURE_2_DIRECTORY}")
    set(MOD_DESTINATION "${SONIC_ADVENTURE_2_DIRECTORY}/mods/sa2-chao-fucking-explode")

    set_target_properties("${PROJECT_NAME}" PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${MOD_DESTINATION}"
    )
ELSE()
    message(WARNING "SONIC_ADVENTURE_2_DIRECTORY not found")
ENDIF()
