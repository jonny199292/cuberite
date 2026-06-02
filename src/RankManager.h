
#pragma once

#include "Globals.h"

class cUUID;
class cMojangAPI;

namespace FasterDB
{
	class cFasterRankManager;
}

class cRankManager
{
public:
	/** Acquire this lock to perform mass changes. 
	Wraps operations into a consistent state for FASTER sessions. */
	class cMassChangeLock
	{
	public:
		cMassChangeLock(cRankManager & a_RankManager) :
			m_Lock(a_RankManager.m_CS)
		{
		}

		~cMassChangeLock()
		{
		}

	protected:
		cCSLock m_Lock;
	};


	/** Creates the rank manager. Needs to be initialized before other use. */
	cRankManager(void);

	~cRankManager();

	/** Initializes the rank manager. Performs migration or default-setting if no data is found. */
	void Initialize(cMojangAPI & a_MojangAPI);

	/** Returns the name of the rank assigned to the specified player.
	Returns empty string if player not found. */
	AString GetPlayerRankName(const cUUID & a_PlayerUUID);

	/** Returns the name of the player with the specified UUID, as stored in the DB.
	Returns empty string if player not found. */
	AString GetPlayerName(const cUUID & a_PlayerUUID);

	/** Returns the names of all groups that are assigned to the specified player.
	Returns an empty vector if the player is not found. */
	AStringVector GetPlayerGroups(const cUUID & a_PlayerUUID);

	/** Returns all permissions assigned to the specified player through their rank and groups.
	Returns an empty vector if the player is not found. */
	AStringVector GetPlayerPermissions(const cUUID & a_PlayerUUID);

	/** Returns all restrictions assigned to the specified player through their rank and groups.
	Returns an empty vector if the player is not found. */
	AStringVector GetPlayerRestrictions(const cUUID & a_PlayerUUID);

	/** Returns the names of all groups that are assigned to the specified rank.
	Returns an empty vector if the rank is not found. */
	AStringVector GetRankGroups(const AString & a_RankName);

	/** Returns all permissions assigned to the specified group.
	Returns an empty vector if the group is not found. */
	AStringVector GetGroupPermissions(const AString & a_GroupName);

	/** Returns all restrictions assigned to the specified group.
	Returns an empty vector if the group is not found. */
	AStringVector GetGroupRestrictions(const AString & a_GroupName);

	/** Returns all permissions assigned to the specified rank.
	Returns an empty vector if the rank is not found. */
	AStringVector GetRankPermissions(const AString & a_RankName);

	/** Returns all restrictions assigned to the specified rank.
	Returns an empty vector if the rank is not found. */
	AStringVector GetRankRestrictions(const AString & a_RankName);

	/** Returns the UUIDs of all players that have a rank assigned to them. */
	std::vector<cUUID> GetAllPlayerUUIDs(void);

	/** Returns the names of all ranks. */
	AStringVector GetAllRanks(void);

	/** Returns the names of all groups. */
	AStringVector GetAllGroups(void);

	/** Returns all permissions that are assigned to any group. */
	AStringVector GetAllPermissions(void);

	/** Returns all restrictions that are assigned to any group. */
	AStringVector GetAllRestrictions(void);

	/** Returns all permissions and restrictions that are assigned to any group. */
	AStringVector GetAllPermissionsRestrictions(void);

	/** Returns the message visuals (prefix, suffix, color) for the specified player.
	Returns true if the visuals were read, false if the player not found. */
	bool GetPlayerMsgVisuals(const cUUID & a_PlayerUUID, AString & a_MsgPrefix, AString & a_MsgSuffix, AString & a_MsgNameColorCode);

	/** Adds a new rank. Does nothing if a rank of the same name already exists. */
	void AddRank(const AString & a_RankName, const AString & a_MsgPrefix, const AString & a_MsgSuffix, const AString & a_MsgNameColorCode);

	/** Adds a new group. Does nothing if a group of the same name already exists. */
	void AddGroup(const AString & a_GroupName);

	/** Adds the specified groups. Does nothing if any of the groups already exist. */
	void AddGroups(const AStringVector & a_GroupNames);

	/** Adds the specified group to the specified rank.
	Returns true if successful, false if no such rank or group, or if already present. */
	bool AddGroupToRank(const AString & a_GroupName, const AString & a_RankName);

	/** Adds the specified permission to the specified group.
	Returns true if successful, false if no such group or if already present. */
	bool AddPermissionToGroup(const AString & a_Permission, const AString & a_GroupName);

	/** Adds the specified restriction to the specified group.
	Returns true if successful, false if no such group or if already present. */
	bool AddRestrictionToGroup(const AString & a_Restriction, const AString & a_GroupName);

	/** Adds the specified permissions to the specified group.
	Returns true if successful, false if no such group. If any permission is already present, it is ignored. */
	bool AddPermissionsToGroup(const AStringVector & a_Permissions, const AString & a_GroupName);

	/** Adds the specified restrictions to the specified group.
	Returns true if successful, false if no such group. If any restriction is already present, it is ignored. */
	bool AddRestrictionsToGroup(const AStringVector & a_Restrictions, const AString & a_GroupName);

	/** Removes the specified rank.
	If a_ReplacementRankName is specified, players assigned to the rank being removed are reassigned. */
	void RemoveRank(const AString & a_RankName, const AString & a_ReplacementRankName = "");

	/** Removes the specified group. */
	void RemoveGroup(const AString & a_GroupName);

	/** Removes the specified group from the specified rank. */
	void RemoveGroupFromRank(const AString & a_GroupName, const AString & a_RankName);

	/** Removes the specified permission from the specified group. */
	void RemovePermissionFromGroup(const AString & a_Permission, const AString & a_GroupName);

	/** Removes the specified restriction from the specified group. */
	void RemoveRestrictionFromGroup(const AString & a_Restriction, const AString & a_GroupName);

	/** Renames the specified rank. Returns true if successful. */
	bool RenameRank(const AString & a_OldRankName, const AString & a_NewRankName);

	/** Renames the specified group. Returns true if successful. */
	bool RenameGroup(const AString & a_OldGroupName, const AString & a_NewGroupName);

	/** Sets the rank for the specified player.
	The player name is only used for informational purposes in the DB. */
	void SetPlayerRank(const cUUID & a_PlayerUUID, const AString & a_PlayerName, const AString & a_RankName);

	/** Removes the player's rank assignment. */
	void RemovePlayerRank(const cUUID & a_PlayerUUID);

	/** Sets the message visuals for the specified rank. */
	void SetRankVisuals(const AString & a_RankName, const AString & a_MsgPrefix, const AString & a_MsgSuffix, const AString & a_MsgNameColorCode);

	/** Returns the message visuals for the specified rank.
	Returns true if successful, false if rank not found. */
	bool GetRankVisuals(const AString & a_RankName, AString & a_MsgPrefix, AString & a_MsgSuffix, AString & a_MsgNameColorCode);

	/** Returns true if the specified rank exists. */
	bool RankExists(const AString & a_RankName);

	/** Returns true if the specified group exists. */
	bool GroupExists(const AString & a_GroupName);

	/** Returns true if the specified player has a rank assigned. */
	bool IsPlayerRankSet(const cUUID & a_PlayerUUID);

	/** Returns true if the specified group is assigned to the specified rank. */
	bool IsGroupInRank(const AString & a_GroupName, const AString & a_RankName);

	/** Returns true if the specified group contains the specified permission. */
	bool IsPermissionInGroup(const AString & a_Permission, const AString & a_GroupName);

	/** Returns true if the specified group contains the specified restriction. */
	bool IsRestrictionInGroup(const AString & a_Restriction, const AString & a_GroupName);

	/** Called by cMojangAPI whenever the playername-uuid pairing is discovered. */
	void NotifyNameUUID(const AString & a_PlayerName, const cUUID & a_UUID);

	/** Sets the name of the default rank. Returns true if successful. */
	bool SetDefaultRank(const AString & a_RankName);

	/** Returns the name of the default rank. */
	AString GetDefaultRank(void);

	/** Clears all player rank assignments. */
	void ClearPlayerRanks(void);

	/** Updates the player name in the DB. Returns true if successful. */
	bool UpdatePlayerName(const cUUID & a_PlayerUUID, const AString & a_NewPlayerName);

private:
	/** Set to true once initialized. */
	bool m_IsInitialized;

	/** The name of the default rank. */
	AString m_DefaultRank;

	/** The FASTER-based storage engine. */
	std::unique_ptr<FasterDB::cFasterRankManager> m_FasterStore;

	/** The mutex protecting access to internal state. */
	cCriticalSection m_CS;

	/** Returns true if no data is present in the store. */
	bool AreTablesEmpty(void);

	/** Creates default ranks and groups. */
	void CreateDefaults(void);
};
