#pragma once

#include <CompileTime/Templates.h>

BEGINNAMESPACE

template<typename T>
class TConfigSettings {
public:
	inline TConfigSettings(const ansichar* name, const ansichar* synopsis, T initialValue){
		m_Name = name;
		m_Value = initialValue;
		m_Min = std::numeric_limits<T>::min();
		m_Max = std::numeric_limits<T>::max();
		addToList();
	}

	inline TConfigSettings(const ansichar* name, const ansichar* synopsis, T initialValue, T minValue, T maxValue) : m_Name(name), m_Synopsis(synopsis), m_Min(minValue), m_Max(maxValue) {
		m_Name = name;
		m_Value = min(m_Min, max(initialValue, m_Max));
		addToList();
	}

	inline TConfigSettings& operator = (T value) {
        m_Value = min(m_Max, max(value, m_Min));
		return *this;
	}

	inline operator T() const { return m_Value; }

	inline static TConfigSettings* FindSetting(const ansichar* name) {
		TConfigSettings<T>* curr = head;
		while (curr) {
			if (strcmp(name, curr->m_Name) == 0) return curr;
			curr = curr->m_Next;
		}
		return nullptr;
	}
private:
	inline void addToList() {
		if (head) {
			tail->m_Next = this;
			tail = this;
		}
		else {
			head = tail = this;
		}
	}

	TConfigSettings<T>* m_Next;
	const ansichar* m_Name;
	const ansichar* m_Synopsis;
	T m_Value;
	T m_Min;
	T m_Max;
private:
	static TConfigSettings<T>* head;
	static TConfigSettings<T>* tail;
};

template<uint32 N>
class TConfigSettings<ansichar[N]>
{
public:
	inline TConfigSettings(const ansichar* name, const ansichar* synopsis, ansichar* initialValue) {
		m_Name = name;
		std::memset(m_Value, 0, N * sizeof(ansichar));
		uint32 n = strlen(initialValue);
		std::copy(&initialValue[0], &initialValue[min(n,N)], m_Value);
		addToList();
	}

	inline TConfigSettings& operator = (ansichar* value) {
		uint32 n = strlen(value);
		std::copy(value, value + min(N, n), m_Value);
		return *this;
	}

	inline operator const ansichar*() const { return &m_Value[0]; }

	inline static TConfigSettings* FindSetting(const ansichar* name) {
		TConfigSettings<ansichar[N]>* curr = head;
		while (curr) {
			if (strcmp(name, curr->m_Name) == 0) return curr;
			curr = curr->m_Next;
		}
		return nullptr;
	}
private:
	inline void addToList() {
		if (head) {
			tail->m_Next = this;
			tail = this;
		}
		else {
			head = tail = this;
		}
	}

	TConfigSettings<ansichar[N]>* m_Next;
	const ansichar* m_Name;
	const ansichar* m_Synopsis;
	ansichar m_Value[N];
private:
	static TConfigSettings<ansichar[N]>* head;
	static TConfigSettings<ansichar[N]>* tail;
};

template<typename T>
TConfigSettings<T>* TConfigSettings<T>::head = nullptr;

template<typename T>
TConfigSettings<T>* TConfigSettings<T>::tail = nullptr;

template<uint32 N>
TConfigSettings<ansichar[N]>* TConfigSettings<ansichar[N]>::head = nullptr;

template<uint32 N>
TConfigSettings<ansichar[N]>* TConfigSettings<ansichar[N]>::tail = nullptr;

typedef TConfigSettings<int32> ConfigSettingInt32;
typedef TConfigSettings<uint32> ConfigSettingUint32;
typedef TConfigSettings<float32> ConfigSettingFloat32;
typedef TConfigSettings<bool> ConfigSettingBool;
typedef TConfigSettings<ansichar[512]> ConfigSettingAnsichar;


ENDNAMESPACE
