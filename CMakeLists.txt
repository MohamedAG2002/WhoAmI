cmake_minimum_required(VERSION 3.27)
project(WhoAmI)

find_library(raylib raylib REQUIRED)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(PROJ_SRC_DIR ${CMAKE_SOURCE_DIR}/src)

set(SOURCES 
  # Core 
  ${PROJ_SRC_DIR}/main.cpp
  ${PROJ_SRC_DIR}/core/game.cpp

  # Managers 
  ${PROJ_SRC_DIR}/managers/dialogue_manager.cpp
  ${PROJ_SRC_DIR}/managers/audio_manager.cpp
  
  # Collision 
  ${PROJ_SRC_DIR}/collision/collision_space.cpp
  ${PROJ_SRC_DIR}/collision/collider.cpp

  # Scenes
  ${PROJ_SRC_DIR}/scenes/main_scene.cpp

  # Entities 
  ${PROJ_SRC_DIR}/entities/player.cpp
  ${PROJ_SRC_DIR}/entities/platform.cpp
  ${PROJ_SRC_DIR}/entities/trigger.cpp

  # UI
  ${PROJ_SRC_DIR}/ui/text.cpp

  # Level
  ${PROJ_SRC_DIR}/level/level_editor.cpp
  ${PROJ_SRC_DIR}/level/levelizer.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

file(COPY assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

target_compile_definitions(${PROJECT_NAME} PRIVATE NO_DEFS)
target_compile_options(${PROJECT_NAME} PRIVATE -lm)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

target_include_directories(${PROJECT_NAME} BEFORE PRIVATE ${PROJ_SRC_DIR})
target_link_libraries(${PROJECT_NAME} PUBLIC ${raylib})

# For windows builds
add_definitions(-DSDL_MAIN_HANDELED)
