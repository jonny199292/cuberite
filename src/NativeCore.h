#pragma once

#include "Bindings/PluginManager.h"

/** Implementation of essential server commands in native C++.
This component serves as a robust replacement for the missing Lua Core plugin. */
class cNativeCore :
	public cPluginManager::cCommandHandler
{
public:
	cNativeCore();
	virtual ~cNativeCore() override;

	/** Registers all core commands to the plugin manager. */
	void Initialize();

	/** Executes the core commands. */
	virtual bool ExecuteCommand(
		const AStringVector & a_Split,
		cPlayer * a_Player,
		const AString & a_Command,
		cCommandOutputCallback * a_Output = nullptr
	) override;

private:
	/** Handles the /help command using ApiDocs. */
	bool HandleHelp(const AStringVector & a_Split, cPlayer * a_Player);

	/** Handles the /tp command. */
	bool HandleTeleport(const AStringVector & a_Split, cPlayer * a_Player);

	/** Handles the /rank command. */
	bool HandleRank(const AStringVector & a_Split, cPlayer * a_Player);
};
