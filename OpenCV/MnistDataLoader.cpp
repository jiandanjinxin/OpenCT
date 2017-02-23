#include <fstream>
#include <cassert>
#include "MnistDataLoader.h"

template<typename T>
T reverse_endian(T p)
{
	std::reverse(reinterpret_cast<char*>(&p), reinterpret_cast<char*>(&p) + sizeof(T));
	return p;
}

inline bool is_little_endian() 
{
	int x = 1;
	return *(char*)&x != 0;
}