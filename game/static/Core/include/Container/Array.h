#pragma once

#include <array>
#include <vector>

BEGINNAMESPACE


template<typename Value, size_type size>
using Array = std::array<Value, size>;

template<typename _Value, typename _Allocator = std::allocator<_Value> >
using DynArray = std::vector<_Value, _Allocator>;

typedef DynArray<String> StringList;
typedef DynArray<Byte> ByteArray;

ENDNAMESPACE