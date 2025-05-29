cmake_minimum_required(VERSION 3.24.2)

project(site-generator VERSION 0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(FetchContent)
include("${CMAKE_SOURCE_DIR}/external/md4c.cmake")
include_directories(SYSTEM "${CMAKE_BINARY_DIR}/_deps/md4c-src/src")

set(SOURCES
	src/md4c-html-process.cpp
	src/formatter.cpp
	src/GRY_mdTohtml.cpp
	src/GRY_SSG.cpp
	src/GRY_MdMetadata.cpp
	src/main.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_sources(${PROJECT_NAME} PUBLIC
	${MD4C_SOURCE_DIR}/src/md4c.c
	${MD4C_SOURCE_DIR}/src/entity.c
	${MD4C_SOURCE_DIR}/src/md4c-html.c
)

target_include_directories(${PROJECT_NAME}
	PRIVATE include
	PUBLIC ${MD4C_INCLUDE_DIR}
)
