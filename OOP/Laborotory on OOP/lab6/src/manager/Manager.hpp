#ifndef MANAGER_HPP
#define MANAGER_HPP
#include "../type/fio.hpp"
#include <cstdint>

template<typename T>
class Manager {
protected:
    static T** items;
    static uint32_t count;

public:
	static void add(T* item) {
		T** new_items = new T*[count + 1];
		for (uint32_t i = 0; i < count; i++) {
			new_items[i] = items[i];
		}
		new_items[count] = item;
		if (items != nullptr)
			delete[] items;
		items = new_items;
		count++;
	}

    static T* find(const fio& client) {
		for (uint32_t i = 0; i < count; i++) {
			if (items[i]->getClient() == client) {
				return items[i];
			}
		}
		return nullptr;
	}

	static T** findAll(const fio& client, uint32_t& result_count) {
		result_count = 0;
		for (uint32_t i = 0; i < count; i++) {
			if (items[i]->getClient() == client) {
				result_count++;
			}
		}
		
		if (result_count == 0) return nullptr;
		
		T** results = new T*[result_count];
		uint32_t idx = 0;
		for (uint32_t i = 0; i < count; i++) {
			if (items[i]->getClient() == client) {
				results[idx] = new T();
				*results[idx] = *items[i];  
				idx++;
			}
		}
		return results;
	}

    static void cleanup() {
		if (items != nullptr) {
			for (uint32_t i = 0; i < count; i++) {
				delete items[i];
			}
			delete[] items;
			items = nullptr;
			count = 0;
		}
	}

    static uint32_t getCount() { return count; }
};

template<typename T>
T** Manager<T>::items = nullptr;
template<typename T>
uint32_t Manager<T>::count = 0;

#endif // MANAGER_HPP
