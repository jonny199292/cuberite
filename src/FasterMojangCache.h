
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

class cFasterMojangCache
{
public:
	cFasterMojangCache(const std::string & a_Path);
	~cFasterMojangCache();

	void PutNameToUUID(const AString & a_Name, const cUUID & a_UUID);
	bool GetUUIDByName(const AString & a_Name, cUUID & a_UUID);

	void PutProfile(const cUUID & a_UUID, const AString & a_Name, Int64 a_DateTime);
	bool GetProfile(const cUUID & a_UUID, AString & a_Name, Int64 & a_DateTime);

	void Checkpoint();

private:
	typedef FASTER::core::FasterKv<NameKey, UUIDKey, disk_t> NameToUUIDStore;
	typedef FASTER::core::FasterKv<UUIDKey, MojangProfileValue, disk_t> UUIDToProfileStore;

	std::unique_ptr<NameToUUIDStore> m_NameToUUIDStore;
	std::unique_ptr<UUIDToProfileStore> m_UUIDToProfileStore;

	static void AsyncCallback(FASTER::core::IAsyncContext* ctx, FASTER::core::Status status)
	{
		// Context is usually managed by the caller in our sync wrappers
	}
};

} // namespace FasterDB
