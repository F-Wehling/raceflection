#pragma once

#include <unordered_set>
#include <set>

BEGINNAMESPACE

template<typename _Key, typename _Hasher = std::hash<_Key>, typename _Comp = std::equal_to<_Key>, typename _Allocator = std::allocator<_Key>>
using HashSet = std::unordered_set<_Key, _Hasher, _Comp, _Allocator>;

template<typename _Key, typename _Hasher = std::hash<_Key>, typename _Comp = std::equal_to<_Key>, typename _Allocator = std::allocator<_Key>>
using HashMultiSet = std::unordered_multiset<_Key, _Hasher, _Comp, _Allocator>;

template<typename _Key, typename _Comp = std::less<_Key>, typename _Allocator = std::allocator<_Key>>
using Set = std::set < _Key, _Comp, _Allocator >;

template<typename _Key, typename _Comp = std::less<_Key>, typename _Allocator = std::allocator<_Key>>
using MultiSet = std::multiset<_Key, _Comp, _Allocator>;

ENDNAMESPACE