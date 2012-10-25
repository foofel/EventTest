#pragma once

namespace es {

struct PointerSize
{
	static const int value = sizeof(void*);
};

template<int size>
struct PointerType
{
private:
	// only valid for 32 and 64 bit builds, so skip everything else
	typedef char value_type;
};

template<>
struct PointerType<4>
{
	typedef unsigned int value_type;
};

template<>
struct PointerType<8>
{
	typedef unsigned long long value_type;
};

}; //namespace