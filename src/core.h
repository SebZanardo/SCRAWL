#ifndef CORE_H
#define CORE_H

// SYSTEM INCLUDES ////////////////////////////////////////////////////////////
#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// EXTERNAL INCLUDES //////////////////////////////////////////////////////////
#include "include/raylib.h"
#include "include/raymath.h"
#include "include/rlgl.h"

// BASE ///////////////////////////////////////////////////////////////////////
// Units
#define ByteBits 8
#define KiB(n) (((u64)(n)) << 10)
#define MiB(n) (((u64)(n)) << 20)
#define GiB(n) (((u64)(n)) << 30)
#define Thousand(n) (((u64)(n)) * 1000)
#define Million(n)  (((u64)(n)) * 1000000)
#define Billion(n)  (((u64)(n)) * 1000000000)

// Helper macros
#define Min(a, b) (((a)<(b))?(a):(b))
#define Max(a, b) (((a)>(b))?(a):(b))
#define Clamp(v, a, b) (((v)<(a))?(a):((v)>(b))?(b):(v))

#define Mod(x, n) ((((x) % (n)) + (n)) % (n))

#define ArrayCount(a) (sizeof(a) / sizeof((a)[0]))

#define Bit(n)              (1ull << (n))
#define FlagGet(n, flag)    ((n) & (flag))
#define FlagSet(n, flag)    ((n) |= (flag))
#define FlagClear(n, flag)  ((n) &= ~(flag))
#define FlagToggle(n, flag) ((n) ^= (flag))

#define AlignUpPow2(n, p) (((u64)(n) + ((u64)(p) - 1)) & (~((u64)(p) - 1)))

// Logging macros
#define CleanErrno() (errno == 0 ? "None" : strerror(errno))
#define LogErrno(M, ...) fprintf(stderr, "[ERROR] (%s:%d: errno: %s) " M "\n", __FILE__, __LINE__, CleanErrno(), ##__VA_ARGS__)
#define Assert(A, M, ...) ((A) ? (void)0 : (LogErrno(M, ##__VA_ARGS__), assert(A)))
#define StaticAssert(A, M) _Static_assert(A, M)

// Unsigned integer types
typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

// Signed integer types
typedef int8_t   i8;
typedef int16_t  i16;
typedef int32_t  i32;
typedef int64_t  i64;

// Boolean types
typedef i8       b8;
typedef i16      b16;
typedef i32      b32;
typedef i64      b64;

// Floating point types
typedef float    f32;
typedef double   f64;

// Enum types
#define enum8(type)  u8
#define enum16(type) u16
#define enum32(type) u32
#define enum64(type) u64

// Constants
#define MAX_U64 0xFFFFFFFFFFFFFFFFull
#define MAX_U32 0xFFFFFFFFu
#define MAX_U16 0xFFFFu
#define MAX_U8  0xFFu

#define MAX_I64 ((i64)0x7FFFFFFFFFFFFFFFll)
#define MAX_I32 ((i32)0x7FFFFFFF)
#define MAX_I16 ((i16)0x7FFF)
#define MAX_I8  ((i8) 0x7F)

#define MIN_I64 ((i64)0x8000000000000000ll)
#define MIN_I32 ((i32)0x80000000)
#define MIN_I16 ((i16)0x8000)
#define MIN_I8  ((i8) 0x80)

#define MAX_F32 3.402823466e+38f
#define MAX_F64 1.7976931348623158e+308

typedef struct {
    i32 x;
    i32 y;
} IntVector2;

// ARENA //////////////////////////////////////////////////////////////////////
typedef struct {
    u64 reserve_size;
    u64 commit_size;
    u64 base;
    u64 base_next;
} Arena;

extern Arena *global_arena;

Arena* ArenaCreate(u64 reserve_size, u64 commit_size);
void ArenaDestroy(Arena* arena);
void* ArenaPush(Arena* arena, u64 size, b8 should_zero);
void ArenaPop(Arena* arena, u64 size);
void ArenaPopTo(Arena* arena, u64 pos);
void ArenaClear(Arena* arena);

#define ArenaPushStruct(arena, T) (T*)ArenaPush((arena), sizeof(T), true)
#define ArenaPushStructNonZero(arena, T) (T*)ArenaPush((arena), sizeof(T), false)
#define ArenaPushArray(arena, T, n) (T*)ArenaPush((arena), sizeof(T) * (n), true)
#define ArenaPushArrayNonZero(arena, T, n) (T*)ArenaPush((arena), sizeof(T) * (n), false)

// MATH ///////////////////////////////////////////////////////////////////////
#define abs_i64(v)    (i64) llabs(v)

#define sqrt_f32(v)   sqrtf(v)
#define log_f32(v)    logf(v)
#define cbrt_f32(v)   cbrtf(v)
#define mod_f32(a, b) fmodf((a), (b))
#define pow_f32(b, e) powf((b), (e))
#define ceil_f32(v)   ceilf(v)
#define floor_f32(v)  floorf(v)
#define round_f32(v)  roundf(v)
#define abs_f32(v)    fabsf(v)
#define sin_f32(v)    sinf(v)
#define cos_f32(v)    cosf(v)
#define tan_f32(v)    tanf(v)
#define exp_f32(v)    expf(v)

// RANDOM /////////////////////////////////////////////////////////////////////
// Based on the PCG random number generator (https://www.pcg-random.org/)
// Licensed under Apache License 2.0 (NO WARRANTY, etc. see website)
typedef struct {
    u64 state;
    u64 inc;
} RNG;

#define PCG32_INITIALIZER { 0x853c49e6748fea9bULL, 0xda3e39cb94b95bdbULL }

extern RNG global_rng;

void RandomSetSeed(u64 initstate, u64 initseq);
u32 RandomU32(void);
f32 RandomF32(void);
f32 RandomNormBetween(f32 min, f32 max);
Vector2 RandomCircle(f32 radius);

void RNGRandomSetSeed(RNG* rng, u64 initstate, u64 initseq);
u32 RNGRandomU32(RNG* rng);
f32 RNGRandomF32(RNG* rng);
f32 RNGRandomNormBetween(RNG* rng, f32 min, f32 max);
Vector2 RNGRandomCircle(RNG* rng, f32 radius);

#endif // CORE_H
