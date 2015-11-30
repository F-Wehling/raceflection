#pragma once

#include <CompileTime/Preprocessor.h>

BEGINNAMESPACE

template<typename T>
class Flags {
	static const size_type Count = T::Count;
	typedef typename T::Enum Enum;
	typedef typename T::Bits Bits;
public:
	typedef ansichar Description[512];

	inline Flags() : m_flags(0){}

	inline Flags(Enum flag) : m_flags((uint32)flag){}

	inline Flags& operator = (Enum flag) {
		m_flags = (uint32)flag;
		return *this;
	}

	inline operator uint32() const { return m_flags; }

	inline Flags& operator = (const Flags& other) {
		m_flags = other.m_flags;
		return *this;
	}

	inline void set(Enum flag) {
		m_flags |= (uint32)flag;
	}

	inline void unset(Enum flag) {
		m_flags &= ~(uint32)flag;
	}

	inline void clear() {
		m_flags = 0;
	}

	inline bool isSet(Enum flag) const {
		return ((m_flags & (uint32)flag) != 0);
	}

	inline Flags operator | (Enum flag) const {
		return Flags(m_flags | (uint32)flag);
	}

	inline Flags operator | (Flags other) const {
		return Flags(m_flags | other.m_flags);
	}

	inline bool operator & (Enum flag) const {
		return (m_flags & (uint32)flag) != 0;
	}

	inline bool operator & (Flags other) const {
		return (m_flags & other.m_flags) != other.m_flags;
	}

	inline Flags& operator |= (Enum flag) {
		m_flags |= (uint32)flag;
		return *this;
	}

	inline Flags& operator |= (Flags other) {
		m_flags |= other.m_flags;
		return *this;
	}

	inline const tchar* toString(const Description& desc) const {
		int32 offset = 0;
		for (size_t i = 0; i < T::Count; ++i)
		{
			if ((m_flags & (1u << i)) != 0)
			{
                offset += std::snprintf(desc + offset, sizeof(desc) - offset, "%s, ", T::ToString(1u << i));
			}
		}
        if (offset > 1)	desc[offset - 2] = 0;
        return desc;
	}
private:
	inline explicit Flags(uint32 flag) : m_flags(flag) {}

	union {
		uint32 m_flags;
		Bits m_bits;
	};
};

template<typename T>
class ExclusiveFlags {
	static const size_type Count = T::Count;
	typedef typename T::Enum Enum;
	typedef typename T::Bits Bits;
public:
	typedef ansichar Description[512];

	inline ExclusiveFlags() : m_flags(0) {}

	inline ExclusiveFlags(Enum flag) : m_flags((uint32)flag) {}

	inline ExclusiveFlags& operator = (Enum flag) {
		m_flags = (uint32)flag;
		return *this;
	}

	inline operator uint32() const { return m_flags; }

	inline ExclusiveFlags& operator = (const ExclusiveFlags& other) {
		m_flags = other.m_flags;
		return *this;
	}

	inline void set(Enum flag) {
		m_flags = (uint32)flag;
	}

	inline void unset(Enum flag) {
		m_flags &= ~(uint32)flag;
	}

	inline void clear() {
		m_flags = 0;
	}

	inline bool isSet(Enum flag) const {
		return m_flags == flag;
	}

	inline ExclusiveFlags operator | (Enum flag) const {
		return ExclusiveFlags(m_flags | (uint32)flag);
	}

	inline ExclusiveFlags operator | (ExclusiveFlags other) const {
		return m_flags | other.m_flags;
	}

	inline bool operator & (Enum flag) const {
		return (m_flags & (uint32)flag) != 0;
	}

	inline bool operator & (ExclusiveFlags other) const {
		return (m_flags == other.m_flags);
	}

	inline ExclusiveFlags& operator |= (Enum flag) {
		m_flags = (uint32)flag;
		return *this;
	}

	inline ExclusiveFlags& operator |= (ExclusiveFlags other) {
		m_flags = other.m_flags;
		return *this;
	}

    inline const tchar* toString(const Description& description) const {
		int32 offset = 0;
		for (size_t i = 0; i < T::Count; ++i)
		{
			if ((m_flags & (1u << i)) != 0)
			{
				offset += std::snprintf(description + offset, sizeof(description) - offset, "%s, ", T::ToString(1u << i));
			}
		}
		if (offset > 1)	description[offset - 2] = 0;
		return description;
	}
private:
	inline explicit ExclusiveFlags(uint32 flag) : m_flags(flag) {}

	union {
		uint32 m_flags;
		Bits m_bits;
	};
};

#define __DECL_FLAGS(...) \
        static const size_type Count = PP_NUM_ARGS(__VA_ARGS__); \
		static_assert(Count <= 32, "Too many flags used. 32 is max"); \
		enum Enum { \
            PP_IND_1(PP_EXPAND_ARGS(PP_DECLARE_FLAGS_ENUM, __VA_ARGS__)) \
		}; \
		struct Bits { \
            PP_IND_1(PP_EXPAND_ARGS(PP_DECLARE_FLAGS_BITS, __VA_ARGS__)) \
		};\
        static const ansichar* toString(size_type value) { \
			switch(value) { \
                PP_IND_1(PP_EXPAND_ARGS(PP_DECLARE_FLAGS_TO_STRING, __VA_ARGS__)) \
				default: return "Unknown"; \
			}\
		}\

#define DECLARE_FLAGS_STRUCT(name, ...) \
	struct name { \
		__DECL_FLAGS(__VA_ARGS__) \
	};	

#	define DECLARE_OR_OPERATOR(name) \
    inline Flags<name> operator | (name::Enum lhs, name::Enum rhs) { \
		return (Flags<name>(lhs) | Flags<name>(rhs)); \
	} 

#	define DECLARE_FLAGS(name, ...) \
		DECLARE_FLAGS_STRUCT(name, __VA_ARGS__) \
		DECLARE_OR_OPERATOR(name) \
        typedef Flags<name> name ## Flags

#	define DECLARE_EXCLUSIVE_FLAGS(name, ...) \
		DECLARE_FLAGS_STRUCT(name, __VA_ARGS__) \
        typedef ExclusiveFlags<name> name ## Flags

#	define DECLARE_MEM_FLAGS(name, ...) \
		__DECL_FLAGS(name, __VA_ARGS__) \
        typedef Flags<name> name ## Flags

#	define DECLARE_MEM_EXCLUSIVE_FLAGS(name, ...) \
		__DECL_FLAGS(__VA_ARGS__) \
        typedef ExclusiveFlags<name> name ## Flags

ENDNAMESPACE
