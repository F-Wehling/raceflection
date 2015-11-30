#pragma once

#include <queue>

BEGINNAMESPACE

template<typename _Value, typename _Allocator = std::allocator<_Value> >
using Deque = std::deque < _Value, _Allocator >;

template<typename _Value, typename _Container = Deque<_Value>>
using Queue = std::queue < _Value, _Container >;

template<typename _Value, typename _Comp = std::less<_Value> >
using PriorityQueue = std::priority_queue<_Value, DynArray<_Value>, _Comp>;

ENDNAMESPACE