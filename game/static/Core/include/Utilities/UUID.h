#pragma once

BEGINNAMESPACE

struct UUID { uint64 data[2]; };

UUID generateUUID();

inline bool operator == (const UUID& uuid_lhs, const UUID& uuid_rhs) {
	return uuid_lhs.data[0] == uuid_rhs.data[0] && uuid_lhs.data[1] == uuid_rhs.data[1];
}

inline bool operator != (const UUID& uuid_lhs, const UUID& uuid_rhs) {
	return !operator == (uuid_lhs, uuid_rhs);
}

inline bool operator > (const UUID& uuid_lhs, const UUID& uuid_rhs) {
	return uuid_lhs.data[0] > uuid_rhs.data[0] || (uuid_lhs.data[0] == uuid_rhs.data[0] && uuid_lhs.data[1] > uuid_rhs.data[1]);
}

inline bool operator < (const UUID& uuid_lhs, const UUID& uuid_rhs) {
	return uuid_lhs.data[0] < uuid_rhs.data[0] || (uuid_lhs.data[0] == uuid_rhs.data[0] && uuid_lhs.data[1] < uuid_rhs.data[1]);
}

inline bool operator >= (const UUID& uuid_lhs, const UUID& uuid_rhs) {
	return uuid_lhs.data[0] > uuid_rhs.data[0] || (uuid_lhs.data[0] == uuid_rhs.data[0] && uuid_lhs.data[1] >= uuid_rhs.data[1]);
}

inline bool operator <= (const UUID& uuid_lhs, const UUID& uuid_rhs) {
	return uuid_lhs.data[0] < uuid_rhs.data[0] || (uuid_lhs.data[0] == uuid_rhs.data[0] && uuid_lhs.data[1] <= uuid_rhs.data[1]);
}

ENDNAMESPACE