
#include "Globals.h"
#include "FasterRankManager.h"
#include "StringUtils.h"
#include <filesystem>
#include <algorithm>

namespace FasterDB
{

static void RankManager_IndexPersistCallback(FASTER::core::Status result) {}
static void RankManager_HybridLogPersistCallback(FASTER::core::Status result, uint64_t persistent_serial_num) {}

cFasterRankManager::cFasterRankManager(const std::string & a_Path) :
	m_BasePath("FasterData/" + a_Path + "/")
{
	// Ensure dedicated directory exists to avoid file/directory naming conflicts in FASTER
	std::filesystem::create_directories("FasterData/" + a_Path);
	std::string basePath = m_BasePath;

	uint64_t kIndexSize = 1L << 13; // 8192 slots
	uint64_t kLogSize = 1L << 30;   // 1GB to satisfy page requirements
	double kMutableFraction = 0.5;

	m_PlayerRankStore = std::make_unique<PlayerRankStore>(PlayerRankStore::IndexConfig{ kIndexSize }, kLogSize, basePath + "playerrank", kMutableFraction);
	m_RankStore = std::make_unique<RankStore>(RankStore::IndexConfig{ kIndexSize }, kLogSize, basePath + "ranks", kMutableFraction);
	m_SettingsStore = std::make_unique<SettingsStore>(SettingsStore::IndexConfig{ kIndexSize }, kLogSize, basePath + "settings", kMutableFraction);
	m_RankGroupCache = std::make_unique<RelationalCacheStore>(RelationalCacheStore::IndexConfig{ kIndexSize }, kLogSize, basePath + "rankgroupcache", kMutableFraction);
	m_RankPermissionCache = std::make_unique<RelationalCacheStore>(RelationalCacheStore::IndexConfig{ kIndexSize }, kLogSize, basePath + "rankpermcache", kMutableFraction);
	m_RankRestrictionCache = std::make_unique<RelationalCacheStore>(RelationalCacheStore::IndexConfig{ kIndexSize }, kLogSize, basePath + "rankrestcache", kMutableFraction);
	m_GroupPermissionStore = std::make_unique<RelationalCacheStore>(RelationalCacheStore::IndexConfig{ kIndexSize }, kLogSize, basePath + "groupperms", kMutableFraction);
	m_GroupRestrictionStore = std::make_unique<RelationalCacheStore>(RelationalCacheStore::IndexConfig{ kIndexSize }, kLogSize, basePath + "grouprests", kMutableFraction);
	m_MasterListStore = std::make_unique<RelationalCacheStore>(RelationalCacheStore::IndexConfig{ kIndexSize }, kLogSize, basePath + "masterlists", kMutableFraction);
}

cFasterRankManager::~cFasterRankManager()
{
	try
	{
		Checkpoint();
	}
	catch (...) {}
}

void cFasterRankManager::SetPlayerRank(const cUUID & a_PlayerUUID, const AString & a_PlayerName, const AString & a_RankName)
{
	UUIDKey key(a_PlayerUUID);
	PlayerRankValue value;
	std::strncpy(value.playerName, a_PlayerName.c_str(), 15);
	std::strncpy(value.rankName, a_RankName.c_str(), 31);

	m_PlayerRankStore->StartSession();
	SyncUpsertContext<UUIDKey, PlayerRankValue> context(key, value);
	m_PlayerRankStore->Upsert(context, AsyncCallback, 0);
	m_PlayerRankStore->CompletePending(true);
	m_PlayerRankStore->StopSession();

	AddToMasterList("__all_player_uuids__", a_PlayerUUID.ToShortString());
}

bool cFasterRankManager::GetPlayerRank(const cUUID & a_PlayerUUID, AString & a_RankName, AString & a_PlayerName)
{
	UUIDKey key(a_PlayerUUID);
	PlayerRankValue value;

	m_PlayerRankStore->StartSession();
	SyncReadContext<UUIDKey, PlayerRankValue> context(key, &value);
	auto status = m_PlayerRankStore->Read(context, AsyncCallback, 0);
	if (status == FASTER::core::Status::Pending) m_PlayerRankStore->CompletePending(true);
	bool found = context.found;
	m_PlayerRankStore->StopSession();

	if (found)
	{
		a_RankName = value.rankName;
		a_PlayerName = value.playerName;
	}
	return found;
}

void cFasterRankManager::RemovePlayerRank(const cUUID & a_PlayerUUID)
{
	SetPlayerRank(a_PlayerUUID, "", "");
	RemoveFromMasterList("__all_player_uuids__", a_PlayerUUID.ToShortString());
}

void cFasterRankManager::SetRankDetails(const AString & a_RankName, const AString & a_Prefix, const AString & a_Suffix, const AString & a_Color)
{
	StringKey key(a_RankName);
	RankValue value;
	std::strncpy(value.rankName, a_RankName.c_str(), 31);
	std::strncpy(value.prefix, a_Prefix.c_str(), 63);
	std::strncpy(value.suffix, a_Suffix.c_str(), 63);
	std::strncpy(value.color, a_Color.c_str(), 7);

	m_RankStore->StartSession();
	SyncUpsertContext<StringKey, RankValue> context(key, value);
	m_RankStore->Upsert(context, AsyncCallback, 0);
	m_RankStore->CompletePending(true);
	m_RankStore->StopSession();

	AddToMasterList("__all_ranks__", a_RankName);
}

bool cFasterRankManager::GetRankDetails(const AString & a_RankName, AString & a_Prefix, AString & a_Suffix, AString & a_Color)
{
	StringKey key(a_RankName);
	RankValue value;

	m_RankStore->StartSession();
	SyncReadContext<StringKey, RankValue> context(key, &value);
	auto status = m_RankStore->Read(context, AsyncCallback, 0);
	if (status == FASTER::core::Status::Pending) m_RankStore->CompletePending(true);
	bool found = context.found;
	m_RankStore->StopSession();

	if (found)
	{
		a_Prefix = value.prefix;
		a_Suffix = value.suffix;
		a_Color = value.color;
	}
	return found;
}

void cFasterRankManager::RemoveRank(const AString & a_RankName)
{
	SetRankDetails(a_RankName, "", "", "");
	RemoveFromMasterList("__all_ranks__", a_RankName);
}

void cFasterRankManager::SetRankGroups(const AString & a_RankName, const AStringVector & a_Groups)
{
	StringKey key(a_RankName);
	LargeStringValue value(SerializeVector(a_Groups));

	m_RankGroupCache->StartSession();
	SyncUpsertContext<StringKey, LargeStringValue> context(key, value);
	m_RankGroupCache->Upsert(context, AsyncCallback, 0);
	m_RankGroupCache->CompletePending(true);
	m_RankGroupCache->StopSession();
}

bool cFasterRankManager::GetRankGroups(const AString & a_RankName, AStringVector & a_Groups)
{
	StringKey key(a_RankName);
	LargeStringValue value;

	m_RankGroupCache->StartSession();
	SyncReadContext<StringKey, LargeStringValue> context(key, &value);
	auto status = m_RankGroupCache->Read(context, AsyncCallback, 0);
	if (status == FASTER::core::Status::Pending) m_RankGroupCache->CompletePending(true);
	bool found = context.found;
	m_RankGroupCache->StopSession();

	if (found)
	{
		a_Groups = DeserializeVector(value.data);
	}
	return found;
}

void cFasterRankManager::SetRankPermissions(const AString & a_RankName, const AStringVector & a_Permissions)
{
	StringKey key(a_RankName);
	LargeStringValue value(SerializeVector(a_Permissions));

	m_RankPermissionCache->StartSession();
	SyncUpsertContext<StringKey, LargeStringValue> context(key, value);
	m_RankPermissionCache->Upsert(context, AsyncCallback, 0);
	m_RankPermissionCache->CompletePending(true);
	m_RankPermissionCache->StopSession();
}

bool cFasterRankManager::GetRankPermissions(const AString & a_RankName, AStringVector & a_Permissions)
{
	StringKey key(a_RankName);
	LargeStringValue value;

	m_RankPermissionCache->StartSession();
	SyncReadContext<StringKey, LargeStringValue> context(key, &value);
	auto status = m_RankPermissionCache->Read(context, AsyncCallback, 0);
	if (status == FASTER::core::Status::Pending) m_RankPermissionCache->CompletePending(true);
	bool found = context.found;
	m_RankPermissionCache->StopSession();

	if (found)
	{
		a_Permissions = DeserializeVector(value.data);
	}
	return found;
}

void cFasterRankManager::SetGroupPermissions(const AString & a_GroupName, const AStringVector & a_Permissions)
{
	StringKey key(a_GroupName);
	LargeStringValue value(SerializeVector(a_Permissions));

	m_GroupPermissionStore->StartSession();
	SyncUpsertContext<StringKey, LargeStringValue> context(key, value);
	m_GroupPermissionStore->Upsert(context, AsyncCallback, 0);
	m_GroupPermissionStore->CompletePending(true);
	m_GroupPermissionStore->StopSession();

	AddToMasterList("__all_groups__", a_GroupName);
}

bool cFasterRankManager::GetGroupPermissions(const AString & a_GroupName, AStringVector & a_Permissions)
{
	StringKey key(a_GroupName);
	LargeStringValue value;

	m_GroupPermissionStore->StartSession();
	SyncReadContext<StringKey, LargeStringValue> context(key, &value);
	auto status = m_GroupPermissionStore->Read(context, AsyncCallback, 0);
	if (status == FASTER::core::Status::Pending) m_GroupPermissionStore->CompletePending(true);
	bool found = context.found;
	m_GroupPermissionStore->StopSession();

	if (found)
	{
		a_Permissions = DeserializeVector(value.data);
	}
	return found;
}

void cFasterRankManager::SetGroupRestrictions(const AString & a_GroupName, const AStringVector & a_Restrictions)
{
	StringKey key(a_GroupName);
	LargeStringValue value(SerializeVector(a_Restrictions));

	m_GroupRestrictionStore->StartSession();
	SyncUpsertContext<StringKey, LargeStringValue> context(key, value);
	m_GroupRestrictionStore->Upsert(context, AsyncCallback, 0);
	m_GroupRestrictionStore->CompletePending(true);
	m_GroupRestrictionStore->StopSession();

	AddToMasterList("__all_groups__", a_GroupName);
}

bool cFasterRankManager::GetGroupRestrictions(const AString & a_GroupName, AStringVector & a_Restrictions)
{
	StringKey key(a_GroupName);
	LargeStringValue value;

	m_GroupRestrictionStore->StartSession();
	SyncReadContext<StringKey, LargeStringValue> context(key, &value);
	auto status = m_GroupRestrictionStore->Read(context, AsyncCallback, 0);
	if (status == FASTER::core::Status::Pending) m_GroupRestrictionStore->CompletePending(true);
	bool found = context.found;
	m_GroupRestrictionStore->StopSession();

	if (found)
	{
		a_Restrictions = DeserializeVector(value.data);
	}
	return found;
}

void cFasterRankManager::RemoveGroup(const AString & a_GroupName)
{
	SetGroupPermissions(a_GroupName, {});
	SetGroupRestrictions(a_GroupName, {});
	RemoveFromMasterList("__all_groups__", a_GroupName);
}

AStringVector cFasterRankManager::GetAllRanks()
{
	return GetMasterList("__all_ranks__");
}

AStringVector cFasterRankManager::GetAllGroups()
{
	return GetMasterList("__all_groups__");
}

std::vector<cUUID> cFasterRankManager::GetAllPlayerUUIDs()
{
	AStringVector raw = GetMasterList("__all_player_uuids__");
	std::vector<cUUID> res;
	res.reserve(raw.size());
	for (const auto & s : raw)
	{
		cUUID u;
		if (u.FromString(s)) res.push_back(u);
	}
	return res;
}

void cFasterRankManager::ClearCaches()
{
	Checkpoint();

	uint64_t kIndexSize = 1L << 13;
	uint64_t kLogSize = 1L << 25; // 32MB
	double kMutableFraction = 0.5;
	std::string basePath = m_BasePath;

	m_RankGroupCache.reset(new RelationalCacheStore(RelationalCacheStore::IndexConfig{ kIndexSize }, kLogSize, basePath + "rankgroupcache", kMutableFraction));
	m_RankPermissionCache.reset(new RelationalCacheStore(RelationalCacheStore::IndexConfig{ kIndexSize }, kLogSize, basePath + "rankpermcache", kMutableFraction));
	m_RankRestrictionCache.reset(new RelationalCacheStore(RelationalCacheStore::IndexConfig{ kIndexSize }, kLogSize, basePath + "rankrestcache", kMutableFraction));
	m_GroupPermissionStore.reset(new RelationalCacheStore(RelationalCacheStore::IndexConfig{ kIndexSize }, kLogSize, basePath + "groupperms", kMutableFraction));
	m_GroupRestrictionStore.reset(new RelationalCacheStore(RelationalCacheStore::IndexConfig{ kIndexSize }, kLogSize, basePath + "grouprests", kMutableFraction));
	m_MasterListStore.reset(new RelationalCacheStore(RelationalCacheStore::IndexConfig{ kIndexSize }, kLogSize, basePath + "masterlists", kMutableFraction));
}

void cFasterRankManager::SetDefaultRank(const AString & a_RankName)
{
	StringKey key("default_rank");
	StringKey value(a_RankName);

	m_SettingsStore->StartSession();
	SyncUpsertContext<StringKey, StringKey> context(key, value);
	m_SettingsStore->Upsert(context, AsyncCallback, 0);
	m_SettingsStore->CompletePending(true);
	m_SettingsStore->StopSession();
}

bool cFasterRankManager::GetDefaultRank(AString & a_RankName)
{
	StringKey key("default_rank");
	StringKey value;

	m_SettingsStore->StartSession();
	SyncReadContext<StringKey, StringKey> context(key, &value);
	auto status = m_SettingsStore->Read(context, AsyncCallback, 0);
	if (status == FASTER::core::Status::Pending) m_SettingsStore->CompletePending(true);
	bool found = context.found;
	m_SettingsStore->StopSession();

	if (found)
	{
		a_RankName = value.data;
	}
	return found;
}

void cFasterRankManager::Checkpoint()
{
	FASTER::core::Guid token;
	m_PlayerRankStore->Checkpoint(RankManager_IndexPersistCallback, RankManager_HybridLogPersistCallback, token);
	m_RankStore->Checkpoint(RankManager_IndexPersistCallback, RankManager_HybridLogPersistCallback, token);
	m_SettingsStore->Checkpoint(RankManager_IndexPersistCallback, RankManager_HybridLogPersistCallback, token);
	m_RankGroupCache->Checkpoint(RankManager_IndexPersistCallback, RankManager_HybridLogPersistCallback, token);
	m_RankPermissionCache->Checkpoint(RankManager_IndexPersistCallback, RankManager_HybridLogPersistCallback, token);
	m_RankRestrictionCache->Checkpoint(RankManager_IndexPersistCallback, RankManager_HybridLogPersistCallback, token);
	m_GroupPermissionStore->Checkpoint(RankManager_IndexPersistCallback, RankManager_HybridLogPersistCallback, token);
	m_GroupRestrictionStore->Checkpoint(RankManager_IndexPersistCallback, RankManager_HybridLogPersistCallback, token);
	m_MasterListStore->Checkpoint(RankManager_IndexPersistCallback, RankManager_HybridLogPersistCallback, token);
}

void cFasterRankManager::AddToMasterList(const AString & a_ListKey, const AString & a_Item)
{
	AStringVector list = GetMasterList(a_ListKey);
	if (std::find(list.begin(), list.end(), a_Item) == list.end())
	{
		list.push_back(a_Item);
		StringKey key(a_ListKey);
		LargeStringValue value(SerializeVector(list));

		m_MasterListStore->StartSession();
		SyncUpsertContext<StringKey, LargeStringValue> context(key, value);
		m_MasterListStore->Upsert(context, AsyncCallback, 0);
		m_MasterListStore->CompletePending(true);
		m_MasterListStore->StopSession();
	}
}

void cFasterRankManager::RemoveFromMasterList(const AString & a_ListKey, const AString & a_Item)
{
	AStringVector list = GetMasterList(a_ListKey);
	auto itr = std::find(list.begin(), list.end(), a_Item);
	if (itr != list.end())
	{
		list.erase(itr);
		StringKey key(a_ListKey);
		LargeStringValue value(SerializeVector(list));

		m_MasterListStore->StartSession();
		SyncUpsertContext<StringKey, LargeStringValue> context(key, value);
		m_MasterListStore->Upsert(context, AsyncCallback, 0);
		m_MasterListStore->CompletePending(true);
		m_MasterListStore->StopSession();
	}
}

AStringVector cFasterRankManager::GetMasterList(const AString & a_ListKey)
{
	StringKey key(a_ListKey);
	LargeStringValue value;

	m_MasterListStore->StartSession();
	SyncReadContext<StringKey, LargeStringValue> context(key, &value);
	auto status = m_MasterListStore->Read(context, AsyncCallback, 0);
	if (status == FASTER::core::Status::Pending) m_MasterListStore->CompletePending(true);
	bool found = context.found;
	m_MasterListStore->StopSession();

	if (found)
	{
		return DeserializeVector(value.data);
	}
	return {};
}

AString cFasterRankManager::SerializeVector(const AStringVector & a_Vector)
{
	return StringJoin(a_Vector, ",");
}

AStringVector cFasterRankManager::DeserializeVector(const AString & a_String)
{
	return StringSplit(a_String, ",");
}

} // namespace FasterDB
