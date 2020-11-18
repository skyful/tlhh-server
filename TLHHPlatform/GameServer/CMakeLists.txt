cmake_minimum_required(VERSION 3.5)

#Enable c++ 14
set(CMAKE_CXX_STANDARD 14)
#set project name
set(game_server)

#find boost install with library
set(BOOST_INCLUDE_DIR /usr/local/include/boost_1_73_0)
set(BOOST_LINRARYP_DIR /usr/local/libs)
find_package(Boost 1.73.0 REQUIRED COMPONENTS)

if(Boost_FOUND)
	message("boost found")
	include_directories(${BOOST_INCLUDE_DIR})
	link_directories(${BOOST_LINRARYP_DIR})
else()
	message(FATAL_ERROR,"canot find boost")
endif()

include_directories(${MYSQL_INCLUDE_DIR})


# Create a sources variable with a link to all cpp files to compile
set(SOURCES
    src/test.cpp
    src/attemper_sink.cpp
	src/attemper_db.cpp
)

#create excute file
add_executable(game_server ${SOURCES})

target_include_directories(game_server
	PRIVATE
		${PROJECT_SOURCE_DIR}/include

)
target_link_libraries( game_server ${Boost_LIBRARIES} -lmysqlclient )
target_link_libraries( game_server server_work_library sql_manager_library )