cmake_minimum_required( VERSION 3.0.2 )

#project name
get_filename_component( CURRENT_DIR_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME )
project( ${CURRENT_DIR_NAME} )

if(CMAKE_COMPILER_IS_GNUCXX)
    add_compile_options(-std=c++11)
    add_compile_options(-fPIC)
else()
    add_definitions(-DQ_COMPILER_INITIALIZER_LISTS)
endif(CMAKE_COMPILER_IS_GNUCXX)

if(UNIX)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -g -O0")
endif(UNIX)

#find packages
find_package( Qt5Gui           REQUIRED )
find_package( Qt5Widgets       REQUIRED )
find_package( Qt5Core          REQUIRED )
find_package( Qt5Network       REQUIRED )


#settings
set( ROOT_DIR "${CMAKE_CURRENT_LIST_DIR}" )
set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${ROOT_DIR}/build/cmake/" )
set( CURRENT_SRC_PATH "${CMAKE_CURRENT_LIST_DIR}/src" )
set(CMAKE_AUTORCC ON)

#includes
include( custom-tools )
include( cotire )

#load config
set( CONFIG_PATH "${ROOT_DIR}/config.in" )
set( CONFIG_TEMPLATE_PATH "${ROOT_DIR}/build/cmake/templates/config.in" )
load_config( ${CONFIG_PATH} ${CONFIG_TEMPLATE_PATH} )
message( "Platform found at: \"${ROOT_DIR}/${PLATFORM_PATH}\"" )

include_directories( "${CURRENT_SRC_PATH}" )
include_directories( "${CMAKE_CURRENT_LIST_DIR}/${PLATFORM_PATH}/common" )
include_directories( "${CMAKE_CURRENT_LIST_DIR}/${PLATFORM_PATH}/" )


#build file list
file( GLOB_RECURSE H_FILES "${CURRENT_SRC_PATH}/*.h" )
file( GLOB_RECURSE CPP_FILES "${CURRENT_SRC_PATH}/*.cpp" )
file( GLOB_RECURSE UI_FILES "${CURRENT_SRC_PATH}/*.ui" )
file( GLOB_RECURSE RC_FILES "${ROOT_DIR}/*.rc" )

set( USER_FILES ${H_FILES} ${CPP_FILES} ${UI_FILES} ${RC_FILES} )

#wrapping
unset( MOC_LIST )
qt5_wrap_cpp( MOC_LIST ${H_FILES} OPTIONS "--no-notes" )
qt5_wrap_ui( MOC_LIST ${UI_FILES} )

set( GENERATED_FILES ${MOC_LIST} )
set( ALL_FILES ${USER_FILES} ${GENERATED_FILES} )

apply_source_groups( USER_FILES ${CURRENT_SRC_PATH} "Sources" )
apply_source_groups( GENERATED_FILES ${CURRENT_SRC_PATH} "Generated" )

#compiling
add_executable( ${PROJECT_NAME} WIN32 ${ALL_FILES} )

#modules
add_subdirectory( "${CMAKE_CURRENT_LIST_DIR}/${PLATFORM_PATH}/utils"               "${CMAKE_CURRENT_BINARY_DIR}/utils_build" )
add_subdirectory( "${CMAKE_CURRENT_LIST_DIR}/${PLATFORM_PATH}/network-core"        "${CMAKE_CURRENT_BINARY_DIR}/network-core_build" )

#linking
target_link_libraries( ${PROJECT_NAME} Qt5::Gui )
target_link_libraries( ${PROJECT_NAME} Qt5::Widgets )
target_link_libraries( ${PROJECT_NAME} Qt5::Core )
target_link_libraries( ${PROJECT_NAME} Qt5::Network )

target_link_libraries( ${PROJECT_NAME} utils )
target_link_libraries( ${PROJECT_NAME} network-core )

#cotire
set_target_properties( ${PROJECT_NAME} PROPERTIES COTIRE_CXX_PREFIX_HEADER_INIT "src/Common.h" )
set_target_properties( ${PROJECT_NAME} PROPERTIES COTIRE_ADD_UNITY_BUILD FALSE )
cotire( ${PROJECT_NAME} )
