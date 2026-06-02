cmake_minimum_required (VERSION 3.13)
project (lsqlite C)

add_library(lsqlite lsqlite3.c)
target_link_libraries(lsqlite PUBLIC sqlite3 lualib)

# FreeBSD requires us to define this to get POSIX 2001 standard
if (${CMAKE_SYSTEM_NAME} STREQUAL "FreeBSD")
	target_compile_definitions(lsqlite PRIVATE _XOPEN_SOURCE=600)
endif()

if (UNIX)
	target_link_libraries(lsqlite PRIVATE ${DYNAMIC_LOADER})
endif()
