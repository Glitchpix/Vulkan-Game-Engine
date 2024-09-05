#include <cstdint>
#include <climits>

// Make sure sizeof(char) = 1 = 8-bits = 1-byte
static_assert(CHAR_BIT == 8);

#define DLL_EXPORT __declspec(dllexport)

typedef std::int8_t i8;
typedef std::int16_t i16;
typedef std::int32_t i32;
typedef std::int64_t i64;

typedef std::uint8_t  u8;
typedef std::uint16_t u16;
typedef std::uint32_t u32;
typedef std::uint64_t u64;

static_assert(sizeof(float) == 4);
static_assert(sizeof(double) == 8);
typedef float f32;
typedef double f64;