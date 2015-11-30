#pragma once
/*
#include <crc32.h>
#include <sha256.h>
#include <sha1.h>

BEGINNAMESPACE
	constexpr uint32 const_hash(const char* str, const std::size_t size = 1009, const std::size_t idx = 0, const std::size_t h = 0);
	namespace detail {
		constexpr uint32 oat_part_one(const std::size_t& h, const char c);
		constexpr uint32 oat_part_two(const std::size_t& h);
		constexpr uint32 oat_part_three(const std::size_t& h);
		constexpr uint32 oat_part_four(const std::size_t& h);
		constexpr uint32 oat_part_five(const std::size_t& h);
		constexpr uint32 oat_part_six(const std::size_t& h);

		constexpr uint32 oat_part_one(const std::size_t& h, const char c) {
			return (h + static_cast<uint32>(c));
		}

		constexpr unsigned int oat_part_two(const std::size_t& h) {
			return (h << 10);
		} constexpr unsigned int oat_part_three(const std::size_t& h) {
			return (h >> 6);
		}

		constexpr unsigned int oat_part_four(const std::size_t& h) {
			return (h << 3);
		} constexpr unsigned int oat_part_five(const std::size_t& h) {
			return (h >> 11);
		}

		constexpr unsigned int oat_part_six(const std::size_t& h) {
			return (h << 15);
		}

		constexpr std::size_t string_length(const char* str, std::size_t index = 0) {
			return (str == nullptr || str[index] == '\0') ? 0 : 1 + string_length(str, index + 1);
		}
	}

	constexpr uint32 const_hash(const char* str, const std::size_t size, const std::size_t idx, const std::size_t h) {
		return (
			(idx == detail::string_length(str)) ? (
				(
					(
						(h + detail::oat_part_four(h)) ^
						detail::oat_part_five(h + detail::oat_part_four(h))
						) +
					detail::oat_part_six(
						(h + detail::oat_part_four(h)) ^
						detail::oat_part_five(h + detail::oat_part_four(h))
						)
					) % size
				) : (
					const_hash(str, size, idx + 1,
						(
							detail::oat_part_one(h, str[idx]) +
							detail::oat_part_two(h + static_cast< uint32 >(str[idx]))
							) ^
						detail::oat_part_three(detail::oat_part_one(h, str[idx]) +
							detail::oat_part_two(detail::oat_part_one(h, str[idx]))
							)
						)
					)
			);
	}


	//
	/// wrapper for the hash-library

	inline uint32 crc32(Byte* data, size_type length) {
		static CRC32 hasher;
		hasher.reset();
		hasher.add(data, length);
		return hasher.getHash();
	}

	typedef struct {Byte value[SHA256::HashBytes];} Sha256;
	inline Sha256 sha256(Byte* data, size_type length) {
		static SHA256 hasher;
		hasher.reset();
		hasher.add(data, length);
		Sha256 hash;
		hasher.getHash(hash.value);
		return hash;
	}

	typedef struct { Byte value[SHA1::HashBytes]; } Sha1;
	inline Sha1 sha1(Byte* data, size_type length) {
		static SHA1 hasher;
		hasher.reset();
		hasher.add(data, length);
		Sha1 hash;
		hasher.getHash(hash.value);
		return hash;
	}
ENDNAMESPACE
*/