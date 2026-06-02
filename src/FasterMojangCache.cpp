
#include "Globals.h"
#include "FasterMojangCache.h"
#include <filesystem>

namespace FasterDB
{

static void MojangCache_IndexPersistCallback(FASTER::core::Status result) {}
static void MojangCache_HybridLogPersistCallback(FASTER::core::Status result, uint64_t persistent_serial_num) {}

cFasterMojangCache::cFasterMojangCache(const std::string & a_Path)
{
	// Ensure dedicated directory exists to avoid file/directory naming conflicts in FASTER
	std::filesystem::create_directories("FasterData/" + a_Path);
	std::string basePath = "FasterData/" + a_Path + "/";

	uint64_t kIndexSize = 1L << 13; // 8192 slots
	uint64_t kLogSize = 1L << 30;   // 1GB to satisfy page requirements
	double kMutableFraction = 0.5;

	m_NameToUUIDStore = std::make_unique<NameToUUIDStore>(NameToUUIDStore::IndexConfig{ kIndexSize }, kLogSize, basePath + "name2uuid", kMutableFraction);
	m_UUIDToProfileStore = std::make_unique<UUIDToProfileStore>(UUIDToProfileStore::IndexConfig{ kIndexSize }, kLogSize, basePath + "uuid2profile", kMutableFraction);
}

cFasterMojangCache::~cFasterMojangCache()
{
	try
	{
		Checkpoint();
	}
	catch (...) {}
	m_NameToUUIDStore.reset();
	m_UUIDToProfileStore.reset();
}

void cFasterMojangCache::PutNameToUUID(const AString & a_Name, const cUUID & a_UUID)
{
	NameKey key(a_Name);
	UUIDKey value(a_UUID);

	m_NameToUUIDStore->StartSession();
	SyncUpsertContext<NameKey, UUIDKey> context(key, value);
	m_NameToUUIDStore->Upsert(context, AsyncCallback, 0);
	m_NameToUUIDStore->CompletePending(true);
	m_NameToUUIDStore->StopSession();
}

bool cFasterMojangCache::GetUUIDByName(const AString & a_Name, cUUID & a_UUID)
{
	NameKey key(a_Name);
	UUIDKey value;

	m_NameToUUIDStore->StartSession();
	SyncReadContext<NameKey, UUIDKey> context(key, &value);
	auto status = m_NameToUUIDStore->Read(context, AsyncCallback, 0);
	
	if (status == FASTER::core::Status::Pending)
	{
		m_NameToUUIDStore->CompletePending(true);
	}
	
	bool found = context.found;
	m_NameToUUIDStore->StopSession();

	if (found)
	{
		a_UUID.FromRaw(value.uuid);
	}
	return found;
}

void cFasterMojangCache::PutProfile(const cUUID & a_UUID, const AString & a_Name, Int64 a_DateTime)
{
	UUIDKey key(a_UUID);
	MojangProfileValue value;
	std::strncpy(value.playerName, a_Name.c_str(), 15);
	value.uuid = a_UUID.ToRaw();
	value.dateTime = a_DateTime;

	m_UUIDToProfileStore->StartSession();
	SyncUpsertContext<UUIDKey, MojangProfileValue> context(key, value);
	m_UUIDToProfileStore->Upsert(context, AsyncCallback, 0);
	m_UUIDToProfileStore->CompletePending(true);
	m_UUIDToProfileStore->StopSession();
}

bool cFasterMojangCache::GetProfile(const cUUID & a_UUID, AString & a_Name, Int64 & a_DateTime)
{
	UUIDKey key(a_UUID);
	MojangProfileValue value;

	m_UUIDToProfileStore->StartSession();
	SyncReadContext<UUIDKey, MojangProfileValue> context(key, &value);
	auto status = m_UUIDToProfileStore->Read(context, AsyncCallback, 0);

	if (status == FASTER::core::Status::Pending)
	{
		m_UUIDToProfileStore->CompletePending(true);
	}

	bool found = context.found;
	m_UUIDToProfileStore->StopSession();

	if (found)
	{
		a_Name = value.playerName;
		a_DateTime = value.dateTime;
	}
	return found;
}

void cFasterMojangCache::Checkpoint()
{
	FASTER::core::Guid token;
	m_NameToUUIDStore->Checkpoint(MojangCache_IndexPersistCallback, MojangCache_HybridLogPersistCallback, token);
	m_UUIDToProfileStore->Checkpoint(MojangCache_IndexPersistCallback, MojangCache_HybridLogPersistCallback, token);
}

} // namespace FasterDB
