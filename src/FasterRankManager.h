
#pragma once

#include "FasterDatabase.h"

#ifdef _WIN32
#include "environment/file_windows.h"
#else
#include "environment/file_linux.h"
#endif

namespace FasterDB
{

#ifdef _WIN32
typedef FASTER::environment::ThreadPoolIoHandler handler_t;
#else
typedef FASTER::environment::QueueIoHandler handler_t;
#endif

typedef FASTER::device::FileSystemDisk<handler_t, 1073741824ull> disk_t;

class cFasterRankManager
{
public:
	cFasterRankManager(const std::string & a_Path);
	~cFasterRankManager();

	// Player-Rank mappings
	void SetPlayerRank(const cUUID & a_PlayerUUID, const AString & a_PlayerName, const AString & a_RankName);
	bool GetPlayerRank(const cUUID & a_PlayerUUID, AString & a_RankName, AString & a_PlayerName);
	void RemovePlayerRank(const cUUID & a_PlayerUUID);

	// Rank details
	void SetRankDetails(const AString & a_RankName, const AString & a_Prefix, const AString & a_Suffix, const AString & a_Color);
	bool GetRankDetails(const AString & a_RankName, AString & a_Prefix, AString & a_Suffix, AString & a_Color);
	void RemoveRank(const AString & a_RankName);

	// Relational data caching
	void SetRankGroups(const AString & a_RankName, const AStringVector & a_Groups);
	bool GetRankGroups(const AString & a_RankName, AStringVector & a_Groups);

	void SetRankPermissions(const AString & a_RankName, const AStringVector & a_Permissions);
	bool GetRankPermissions(const AString & a_RankName, AStringVector & a_Permissions);

	// Group data
	void SetGroupPermissions(const AString & a_GroupName, const AStringVector & a_Permissions);
	bool GetGroupPermissions(const AString & a_GroupName, AStringVector & a_Permissions);
	void SetGroupRestrictions(const AString & a_GroupName, const AStringVector & a_Restrictions);
	bool GetGroupRestrictions(const AString & a_GroupName, AStringVector & a_Restrictions);
	void RemoveGroup(const AString & a_GroupName);

	// Master lists retrieval
	AStringVector GetAllRanks();
	AStringVector GetAllGroups();
	std::vector<cUUID> GetAllPlayerUUIDs();

	void ClearCaches();

	// Global settings
	void SetDefaultRank(const AString & a_RankName);
	bool GetDefaultRank(AString & a_RankName);

	void Checkpoint();

	// Master list management (internal helpers but public for ease of migration if needed)
	void AddToMasterList(const AString & a_ListKey, const AString & a_Item);
	void RemoveFromMasterList(const AString & a_ListKey, const AString & a_Item);

private:
	typedef FASTER::core::FasterKv<UUIDKey, PlayerRankValue, disk_t> PlayerRankStore;
	typedef FASTER::core::FasterKv<StringKey, RankValue, disk_t> RankStore;
	typedef FASTER::core::FasterKv<StringKey, StringKey, disk_t> SettingsStore;
	typedef FASTER::core::FasterKv<StringKey, LargeStringValue, disk_t> RelationalCacheStore;

	std::unique_ptr<PlayerRankStore> m_PlayerRankStore;
	std::unique_ptr<RankStore> m_RankStore;
	std::unique_ptr<SettingsStore> m_SettingsStore;
	std::unique_ptr<RelationalCacheStore> m_RankGroupCache;
	std::unique_ptr<RelationalCacheStore> m_RankPermissionCache;
	std::unique_ptr<RelationalCacheStore> m_RankRestrictionCache;
	std::unique_ptr<RelationalCacheStore> m_GroupPermissionStore;
	std::unique_ptr<RelationalCacheStore> m_GroupRestrictionStore;
	std::unique_ptr<RelationalCacheStore> m_MasterListStore;

	static void AsyncCallback(FASTER::core::IAsyncContext* ctx, FASTER::core::Status status) {}

	AString SerializeVector(const AStringVector & a_Vector);
	AStringVector DeserializeVector(const AString & a_String);

	AStringVector GetMasterList(const AString & a_ListKey);

	AString m_BasePath;
};

} // namespace FasterDB
