#if !defined(VECTOR_HPP)
#define VECTOR_HPP

#include <cstdint>
#include <math.h>
#include <stdexcept>
#include <initializer_list>

template <typename T>
class vector {
private:
	size_t limit;
	size_t pointer;
	T* data;
public:
    vector();
    vector(size_t size);
	vector(std::initializer_list<T> init);
    ~vector();

	T& operator[](size_t i);
	const T& operator[](size_t i) const;

    void push_back(T val);
	void pop_back();
	T back() const;
	size_t size() const;
	size_t capacity() const;
	void insert(T pos, T val);
	void erase(T pos);
	void erase(T from, T to);
};

template <typename T>
inline vector<T>::vector() : limit{1}, pointer{0}, data{new T[limit]} {}

template <typename T>
inline vector<T>::vector(size_t size) : limit{static_cast<size_t>(0b1) << static_cast<size_t>(ceil(log2(size)))}, pointer{size}, data{new T[limit]} {}

template <typename T>
inline vector<T>::vector(std::initializer_list<T> init) : limit{static_cast<size_t>(0b1) << static_cast<size_t>(ceil(log2(init.size())))}, pointer{init.size()}, data{new T[limit]} {
	for (size_t i = 0; i < pointer; i++) {
		data[i] = init.begin()[i];
	}
}

template <typename T>
inline vector<T>::~vector() {
	delete[] data;
}

template <typename T>
inline T& vector<T>::operator[](size_t i) {
    if (i < pointer) {
        return data[i];
    } else {
        throw std::out_of_range("Index out of range");
    }
}

template <typename T>
const T& vector<T>::operator[](size_t i) const {
    if (i < pointer) {
		return data[i];
	} else {
		throw std::out_of_range("Index out of range");
	}
}

template <typename T>
inline void vector<T>::push_back(T val) {
	if (limit == pointer) {
		limit = limit << 1;
		T* new_data = new T[limit];
		for (size_t i = 0; i < pointer; i++) {
			new_data[i] = data[i];
		}
		delete[] data;
		data = new_data;
	}
	data[pointer] = val;
	pointer++;
} 

template <typename T>
inline void vector<T>::pop_back() {
	if (pointer > 0) {
		pointer--;
	}
}

template <typename T>
inline T vector<T>::back() const {
	if (pointer > 0) {
		return data[pointer - 1];
	} else {
		throw std::out_of_range("Vector is empty");
	}
}

template <typename T>
inline size_t vector<T>::size() const {
	return pointer;
}

template <typename T>
inline size_t vector<T>::capacity() const {
	return limit;
}

template <typename T>
inline void vector<T>::insert(T pos, T val) {
	if (pos > pointer) {
		throw std::out_of_range("Index out of range");
	}
	if (limit <= pointer) {
		limit = limit << 1;
		T* new_data = new T[limit];
		for (size_t i = 0; i < pos; i++) {
			new_data[i] = data[i];
		}
		new_data[pos] = val;
		for (size_t i = pos; i < pointer; i++) {
			new_data[i + 1] = data[i];
		}
		delete[] data;
		data = new_data;
	} else {
		for (size_t i = pointer; i > pos; i--) {
			data[i] = data[i - 1];
		}
		data[pos] = val;
	}
	pointer++;
}

template <typename T>
inline void vector<T>::erase(T pos) {
	if (pos >= pointer) {
		throw std::out_of_range("Index out of range");
	}
	for (size_t i = pos; i + 1 < pointer; i++) {
		data[i] = data[i + 1];
	}
	pointer--;
}

template <typename T>
inline void vector<T>::erase(T from, T to) {
	if (from >= pointer || to > pointer || from >= to) {
		throw std::out_of_range("Index out of range");
	}
	size_t shift = to - from;
	for (size_t i = from; i + shift < pointer; i++) {
		data[i] = data[i + shift];
	}
	pointer -= shift;
}

#endif // !defined(VECTOR_HPP)