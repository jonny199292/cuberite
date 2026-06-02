# lib/faster/CMakeLists.txt
set (FASTER_HEADERS
  cc/src/core/address.h
  cc/src/core/alloc.h
  cc/src/core/async.h
  cc/src/core/async_result_types.h
  cc/src/core/auto_ptr.h
  cc/src/core/checkpoint_locks.h
  cc/src/core/checkpoint_state.h
  cc/src/core/checkpoint_state_f2.h
  cc/src/core/config.h
  cc/src/core/constants.h
  cc/src/core/f2.h
  cc/src/core/faster.h
  cc/src/core/gc_state.h
  cc/src/core/grow_state.h
  cc/src/core/guid.h
  cc/src/core/internal_contexts.h
  cc/src/core/internal_contexts_f2.h
  cc/src/core/key_hash.h
  cc/src/core/light_epoch.h
  cc/src/core/lss_allocator.h
  cc/src/core/malloc_fixed_page_size.h
  cc/src/core/native_buffer_pool.h
  cc/src/core/persistent_memory_malloc.h
  cc/src/core/phase.h
  cc/src/core/read_cache.h
  cc/src/core/read_cache_utils.h
  cc/src/core/record.h
  cc/src/core/recovery_status.h
  cc/src/core/state_transitions.h
  cc/src/core/status.h
  cc/src/core/thread.h
  cc/src/core/utility.h
  cc/src/device/file_system_disk.h
  cc/src/device/null_disk.h
  cc/src/environment/file.h
  cc/src/environment/file_common.h
  cc/src/index/cold_index.h
  cc/src/index/cold_index_contexts.h
  cc/src/index/hash_bucket.h
  cc/src/index/hash_table.h
  cc/src/index/index.h
  cc/src/index/mem_index.h
)

if (MSVC)
set (FASTER_HEADERS ${FASTER_HEADERS}
  cc/src/environment/file_windows.h
)
else()
set (FASTER_HEADERS ${FASTER_HEADERS}
  cc/src/environment/file_linux.h
)
endif()

set (FASTER_SOURCES
  cc/src/core/address.cc
  cc/src/core/lss_allocator.cc
  cc/src/core/thread.cc
)

if (MSVC)
set (FASTER_SOURCES ${FASTER_SOURCES}
  cc/src/environment/file_windows.cc
)
else()
set (FASTER_SOURCES ${FASTER_SOURCES}
  cc/src/environment/file_linux.cc
)
endif()

add_library(faster STATIC ${FASTER_SOURCES} ${FASTER_HEADERS})

target_include_directories(faster PUBLIC cc/src)

if (NOT MSVC)
    target_link_libraries(faster PRIVATE uuid aio tbb)
endif()
