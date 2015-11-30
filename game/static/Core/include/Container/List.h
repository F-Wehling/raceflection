#pragma once

#include <list>
#include <forward_list>

BEGINNAMESPACE

template<typename _Value, typename _Allocator = std::allocator<_Value> >
using List = std::list<_Value, _Allocator>;

template<typename _Value, typename _Allocator = std::allocator<_Value> >
using ForwardList = std::forward_list<_Value, _Allocator>;

ENDNAMESPACE