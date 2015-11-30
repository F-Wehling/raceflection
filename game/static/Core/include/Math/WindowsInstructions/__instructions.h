#pragma once

#if !__Included_by_instructions_h
#error "This header should not be included directly, include Math.h instead"
#endif

#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXCollision.h>
#include <DirectXColors.h>

#define TYPEDEF(Alias, Type) \
	typedef Type Alias

#define NAMEALIAS(Alias, Function) \
	namespace detail{ typedef decltype(Function) _t ## Alias; } \
	extern PP_JOIN(detail::_t, Alias)& Alias

#define DX_TYPEDEF(alias, type) TYPEDEF(alias, DirectX::XM ## type)
#define DX_TYPEDEF_PACKED(alias, type) TYPEDEF(alias, DirectX::PackedVector::XM ## type)

#define DX_NAMEALIAS(name) NAMEALIAS(name, DirectX::XM ## name)
#define DX_NAMEALIAS_PACKED(name) NAMEALIAS(name, DirectX::PackedVector::XM ## name)

#define IMPL_NAMEALIAS(Alias, Function) \
	PP_JOIN(detail::_t, Alias)& Alias = Function

#define DX_IMPL_NAMEALIAS(name) IMPL_NAMEALIAS(name, DirectX::XM ## name)
#define DX_IMPL_NAMEALIAS_PACKED(name) IMPL_NAMEALIAS(name, DirectX::PackedVector::XM ## name)