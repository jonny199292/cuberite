
#pragma once

#include "Globals.h"
#include "UUID.h"
#include "core/faster.h"
#include "device/file_system_disk.h"

namespace FasterDB
{

// --- Common Keys ---

struct UUIDKey
{
	std::array<uint8_t, 16> uuid;

	UUIDKey() { uuid.fill(0); }
	UUIDKey(const cUUID & a_UUID) : uuid(a_UUID.ToRaw()) {}

	inline static constexpr uint32_t size() { return 16; }
	inline FASTER::core::KeyHash GetHash() const
	{
		uint64_t hash = 0;
		std::memcpy(&hash, uuid.data(), 8);
		return FASTER::core::KeyHash{ hash };
	}
	inline bool operator==(const UUIDKey & other) const
	{
		return uuid == other.uuid;
	}
};

struct StringKey
{
	char data[64];

	StringKey() { std::memset(data, 0, 64); }
	StringKey(const AString & a_String)
	{
		std::memset(data, 0, 64);
		std::strncpy(data, a_String.c_str(), 63);
	}

	inline static constexpr uint32_t size() { return 64; }
	inline FASTER::core::KeyHash GetHash() const
	{
		uint64_t h = 5381;
		for (int i = 0; i < 64 && data[i]; ++i)
		{
			h = ((h << 5) + h) + data[i];
		}
		return FASTER::core::KeyHash{ h };
	}
	inline bool operator==(const StringKey & other) const
	{
		return std::strncmp(data, other.data, 64) == 0;
	}
};

struct NameKey : public StringKey
{
	using StringKey::StringKey;
};

// --- Values ---

struct RankValue
{
	char rankName[32];
	char prefix[64];
	char suffix[64];
	char color[8];

	RankValue()
	{
		std::memset(rankName, 0, 32);
		std::memset(prefix, 0, 64);
		std::memset(suffix, 0, 64);
		std::memset(color, 0, 8);
	}

	inline static constexpr uint32_t size() { return sizeof(RankValue); }
};

struct PlayerRankValue
{
	char rankName[32];
	char playerName[16];

	PlayerRankValue()
	{
		std::memset(rankName, 0, 32);
		std::memset(playerName, 0, 16);
	}

	inline static constexpr uint32_t size() { return sizeof(PlayerRankValue); }
};

struct MojangProfileValue
{
	char playerName[16];
	std::array<uint8_t, 16> uuid;
	Int64 dateTime;

	MojangProfileValue()
	{
		std::memset(playerName, 0, 16);
		uuid.fill(0);
		dateTime = 0;
	}

	inline static constexpr uint32_t size() { return sizeof(MojangProfileValue); }
};

struct LargeStringValue
{
	char data[1024];

	LargeStringValue() { std::memset(data, 0, 1024); }
	LargeStringValue(const AString & a_String)
	{
		std::memset(data, 0, 1024);
		std::strncpy(data, a_String.c_str(), 1023);
	}

	inline static constexpr uint32_t size() { return 1024; }
};

// --- Contexts ---

template <typename K, typename V>
struct SyncReadContext : public FASTER::core::IAsyncContext
{
	typedef K key_t;
	typedef V value_t;

	K key_val;
	V * output;
	bool found;

	SyncReadContext(const K& a_Key, V * out) : key_val(a_Key), output(out), found(false) {}
	SyncReadContext(const SyncReadContext& other) : key_val(other.key_val), output(other.output), found(other.found) {}

	inline const K& key() const { return key_val; }

	inline void Get(const V& a_Value)
	{
		if (output) *output = a_Value;
		found = true;
	}

	inline void GetAtomic(const V& a_Value)
	{
		if (output) *output = a_Value;
		found = true;
	}

protected:
	FASTER::core::Status DeepCopy_Internal(FASTER::core::IAsyncContext*& context_copy) override
	{
		context_copy = new SyncReadContext(*this);
		return FASTER::core::Status::Ok;
	}
};

template <typename K, typename V>
struct SyncUpsertContext : public FASTER::core::IAsyncContext
{
	typedef K key_t;
	typedef V value_t;

	K key_val;
	V value_val;

	SyncUpsertContext(const K& a_Key, const V& a_Value) : key_val(a_Key), value_val(a_Value) {}
	SyncUpsertContext(const SyncUpsertContext& other) : key_val(other.key_val), value_val(other.value_val) {}

	inline const K& key() const { return key_val; }
	inline uint32_t value_size() const { return V::size(); }
	inline void Put(V& a_Value) { a_Value = value_val; }
	inline bool PutAtomic(V& a_Value) { a_Value = value_val; return true; }

protected:
	FASTER::core::Status DeepCopy_Internal(FASTER::core::IAsyncContext*& context_copy) override
	{
		context_copy = new SyncUpsertContext(*this);
		return FASTER::core::Status::Ok;
	}
};

} // namespace FasterDB
