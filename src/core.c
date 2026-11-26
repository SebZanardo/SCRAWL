#include "core.h"

// OS /////////////////////////////////////////////////////////////////////////
#ifdef _WIN32
#include <windows.h>

static u32 OSPageSize(void) {
    SYSTEM_INFO sysinfo = { 0 };
    GetSystemInfo(&sysinfo);
    return sysinfo.dwPageSize;
}

static void* OSMemoryReserve(u64 size) {
    return VirtualAlloc(NULL, size, MEM_RESERVE, PAGE_READWRITE);
}

static b32 OSMemoryCommit(void* ptr, u64 size) {
    return VirtualAlloc(ptr, size, MEM_COMMIT, PAGE_READWRITE) != NULL;
}

static b32 OSMemoryRelease(void* ptr, u64 size) {
    (void)size;
    return VirtualFree(ptr, 0, MEM_RELEASE);
}

#else
#include <unistd.h>
#include <sys/mman.h>

static u32 OSPageSize(void) {
    return (u32)sysconf(_SC_PAGESIZE);
}

static void* OSMemoryReserve(u64 size) {
    void* ptr = mmap(NULL, size, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    return (ptr == MAP_FAILED) ? NULL : ptr;
}

static b32 OSMemoryCommit(void* ptr, u64 size) {
    return mprotect(ptr, size, PROT_READ | PROT_WRITE) == 0;
}

static b32 OSMemoryRelease(void* ptr, u64 size) {
    return munmap(ptr, size) == 0;
}

#endif

// ARENA //////////////////////////////////////////////////////////////////////
Arena *global_arena = NULL;

static const u64 ARENA_BASE_POS = sizeof(Arena);
static const u64 ARENA_ALIGN = sizeof(void*);

Arena* ArenaCreate(u64 reserve_size, u64 commit_size) {
    u32 pagesize = OSPageSize();

    reserve_size = AlignUpPow2(reserve_size, pagesize);
    commit_size = AlignUpPow2(commit_size, pagesize);

    Arena* arena = OSMemoryReserve(reserve_size);

    Assert(OSMemoryCommit(arena, commit_size), "Unable to allocate arena");

    arena->reserve_size = reserve_size;
    arena->commit_size = commit_size;
    arena->base = ARENA_BASE_POS;
    arena->base_next = commit_size;

    return arena;
}

void ArenaDestroy(Arena* arena) {
    OSMemoryRelease(arena, arena->reserve_size);
}

void* ArenaPush(Arena* arena, u64 size, b8 should_zero) {
    u64 pos_aligned = AlignUpPow2(arena->base, ARENA_ALIGN);
    u64 new_pos = pos_aligned + size;

    if (new_pos > arena->reserve_size) { return NULL; }

    if (new_pos > arena->base_next) {
        u64 new_commit_pos = new_pos;
        new_commit_pos += arena->commit_size - 1;
        new_commit_pos -= new_commit_pos % arena->commit_size;
        new_commit_pos = Min(new_commit_pos, arena->reserve_size);

        u8* mem = (u8*)arena + arena->base_next;
        u64 commit_size = new_commit_pos - arena->base_next;

        if (!OSMemoryCommit(mem, commit_size)) { return NULL; }

        arena->base_next = new_commit_pos;
    }

    arena->base = new_pos;

    u8* out = (u8*)arena + pos_aligned;
    if (should_zero) { memset(out, 0, size); }
    return out;
}

void ArenaPop(Arena* arena, u64 size) {
    size = Min(size, arena->base - ARENA_BASE_POS);
    arena->base -= size;
}

void ArenaPopTo(Arena* arena, u64 pos) {
    u64 size = pos < arena->base ? arena->base - pos : 0;
    ArenaPop(arena, size);
}

void ArenaClear(Arena* arena) {
    ArenaPopTo(arena, ARENA_BASE_POS);
}

// RANDOM /////////////////////////////////////////////////////////////////////
RNG global_rng = PCG32_INITIALIZER;

void RandomSetSeed(u64 initstate, u64 initseq) {
    global_rng.state = 0;
    global_rng.inc = (initseq << 1) | 1;
    RandomU32();
    global_rng.state += initstate;
    RandomU32();
}

u32 RandomU32(void) {
    u64 oldstate = global_rng.state;
    global_rng.state = oldstate * 6364136223846793005ULL + global_rng.inc;
    u32 xorshifted = ((oldstate >> 18u) ^ oldstate) >> 27u;
    u32 rot = oldstate >> 59u;
    return (xorshifted >> rot) | (xorshifted << ((-rot) & 31));
}

f32 RandomF32(void) {
    return (f32)RandomU32() / (f32)MAX_U32;
}

static f32 RandomNorm(void) {
    f32 u1 = 0.0f;
    do { u1 = RandomF32(); } while (u1 == 0.0f);
    f32 u2 = RandomF32();

    f32 magnitude = sqrt_f32(-2.0f * log_f32(u1));
    f32 value = magnitude * cos_f32(2.0f * PI * u2);

    return value;
}

f32 RandomNormBetween(f32 min, f32 max) {
    f32 center = (min + max) / 2.0f;
    f32 stddev = (max - min) / 6.0f;

    f32 value;
    do { value = center + RandomNorm() * stddev;
    } while (value < min || value > max);

    return value;
}

Vector2 RandomCircle(f32 radius) {
    f32 r = radius * sqrt_f32(RandomF32());
    f32 theta = RandomF32() * 2 * PI;
    return (Vector2) {
        r * cos_f32(theta),
        r * sin_f32(theta)
    };
}

void RNGRandomSetSeed(RNG* rng, u64 initstate, u64 initseq) {
    rng->state = 0;
    rng->inc = (initseq << 1) | 1;
    RNGRandomU32(rng);
    rng->state += initstate;
    RNGRandomU32(rng);
}

u32 RNGRandomU32(RNG* rng) {
    u64 oldstate = rng->state;
    rng->state = oldstate * 6364136223846793005ULL + rng->inc;
    u32 xorshifted = ((oldstate >> 18u) ^ oldstate) >> 27u;
    u32 rot = oldstate >> 59u;
    return (xorshifted >> rot) | (xorshifted << ((-rot) & 31));
}

f32 RNGRandomF32(RNG* rng) {
    return (f32)RNGRandomU32(rng) / (f32)MAX_U32;
}

static f32 RNGRandomNorm(RNG* rng) {
    f32 u1 = 0.0f;
    do { u1 = RNGRandomF32(rng); } while (u1 == 0.0f);
    f32 u2 = RNGRandomF32(rng);

    f32 magnitude = sqrt_f32(-2.0f * log_f32(u1));
    f32 value = magnitude * cos_f32(2.0f * PI * u2);

    return value;
}

f32 RNGRandomNormBetween(RNG* rng, f32 min, f32 max) {
    f32 center = (min + max) / 2.0f;
    f32 stddev = (max - min) / 6.0f;

    f32 value;
    do { value = center + RNGRandomNorm(rng) * stddev;
    } while (value < min || value > max);

    return value;
}

Vector2 RNGRandomCircle(RNG* rng, f32 radius) {
    f32 r = radius * sqrt_f32(RNGRandomF32(rng));
    f32 theta = RNGRandomF32(rng) * 2 * PI;
    return (Vector2) {
        r * cos_f32(theta),
        r * sin_f32(theta)
    };
}
