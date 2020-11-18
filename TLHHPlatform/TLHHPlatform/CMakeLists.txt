cmake_minimum_required(VERSION 3.5)

# Enable C++14
set(CMAKE_CXX_STANDARD 14)
#set project name
project(net_work_library)

set(SOURCES
	src/server_work_engine.cpp
	src/client_work_engine.cpp
	src/server_user_manager.cpp
	src/tcp_socket.cpp
	src/tcp_server.cpp
	src/tcp_client.cpp
	src/mysql_client.cpp
	src/mysql_reset.cpp
	src/sql_manager.cpp
	src/sql_reset.cpp
)

#########################################
# create a  client_work_library library
#########################################
add_library(client_work_library 
	SHARED
		${SOURCES}
)
#alias library
#add_library(net_work::client ALIAS client_work_library)

#########################################
# create a  sql_manager_library library
#########################################
add_library(sql_manager_library 
	SHARED
		${SOURCES}
)
#alias library
#add_library(net_work::server ALIAS sql_manager_library)


#########################################
# create a  server_work_library library
#########################################
add_library(server_work_library 
	SHARED
		${SOURCES}
)
#alias library
#add_library(net_work::server ALIAS server_work_library)



target_include_directories(server_work_library
	PUBLIC
		${PROJECT_SOURCE_DIR}/include

)

target_link_libraries( client_work_library -pthread)
target_link_libraries( server_work_library -pthread )
target_link_libraries( sql_manager_library -pthread -lmysqlclient)