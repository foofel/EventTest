#pragma once
#include <cstdint>

struct PointerSize
{
	static const int value = sizeof(void*);
};

template<int size>
struct PointerType
{
private:
	// only valid for 32 and 64 bit builds, so skip everything else
	typedef void value_type;
};

template<>
struct PointerType<4>
{
	typedef uint32_t value_type;
};

template<>
struct PointerType<8>
{
	typedef uint64_t value_type;
};