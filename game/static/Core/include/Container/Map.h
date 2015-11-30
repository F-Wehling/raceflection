#pragma once

#include <unordered_map>
#include <map>

BEGINNAMESPACE

template<typename _Key, typename _Value, typename _Hasher = std::hash<_Key>, typename _Comp = std::equal_to<_Key>, typename _Allocator = std::allocator<std::pair<const _Key, _Value>>>
using HashMap = std::unordered_map<_Key, _Value, _Hasher, _Comp, _Allocator>;

template<typename _Key, typename _Value, typename _Hasher = std::hash<_Key>, typename _Comp = std::equal_to<_Key>, typename _Allocator = std::allocator<std::pair<const _Key, _Value>>>
using HashMultiMap = std::unordered_multimap<_Key, _Value, _Hasher, _Comp, _Allocator >;

template<typename _Key, typename _Value, typename _Comp = std::less<_Key>, typename _Allocator = std::allocator<std::pair<const _Key,_Value>>>
using Map = std::map<_Key, _Value, _Comp, _Allocator>;

template<typename _Key, typename _Value, typename _Comp = std::less<_Key>, typename _Allocator = std::allocator<std::pair<const _Key, _Value>>>
using MultiMap = std::multimap<_Key, _Value, _Comp, _Allocator>;

ENDNAMESPACE