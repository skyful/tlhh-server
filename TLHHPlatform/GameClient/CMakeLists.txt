cmake_minimum_required(VERSION 3.5)

#Enable c++ 14
set(CMAKE_CXX_STANDARD 14)
set(PROJECT_NAME game_client)
#set project name
set(${PROJECT_NAME})

# Create a sources variable with a link to all cpp files to compile
set(SOURCES
    src/main.cpp
    src/attemper_sink.cpp
)

#create excute file
add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(game_server
	PRIVATE
		${PROJECT_SOURCE_DIR}/include

)
target_link_libraries( ${PROJECT_NAME} client_work_library )