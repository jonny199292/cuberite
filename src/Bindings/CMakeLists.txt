set(API_DOCS_GENERATED "${CMAKE_CURRENT_SOURCE_DIR}/ApiDocs_Generated.cpp")

add_custom_command(
	OUTPUT "${API_DOCS_GENERATED}"
	COMMAND "${Python3_EXECUTABLE}" GenerateApiDocs.py "${API_DOCS_GENERATED}"
	WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/docs"
	DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/docs/GenerateApiDocs.py"
	COMMENT "Generating API documentation metadata..."
)

target_sources(
	${CMAKE_PROJECT_NAME} PRIVATE

	Bindings.cpp
	ApiDocs.h
	ApiDocs_Generated.cpp
	DeprecatedBindings.cpp
	LuaChunkStay.cpp
	LuaJson.cpp
	LuaNameLookup.cpp
	LuaServerHandle.cpp
	LuaState.cpp
	LuaState_Implementation.cpp
	LuaTCPLink.cpp
	LuaUDPEndpoint.cpp
	LuaWindow.cpp
	ManualBindings.cpp
	ManualBindings_BlockArea.cpp
	ManualBindings_Network.cpp
	ManualBindings_RankManager.cpp
	ManualBindings_World.cpp
	Plugin.cpp
	PluginLua.cpp
	PluginManager.cpp

	Bindings.h
	DeprecatedBindings.h
	LuaChunkStay.h
	LuaFunctions.h
	LuaJson.h
	LuaNameLookup.h
	LuaServerHandle.h
	LuaState.h
	LuaState_Declaration.inc
	LuaState_Typedefs.inc
	LuaTCPLink.h
	LuaUDPEndpoint.h
	LuaWindow.h
	ManualBindings.h
	Plugin.h
	PluginLua.h
	PluginManager.h
	tolua++.h
)
