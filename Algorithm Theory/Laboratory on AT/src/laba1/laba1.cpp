#include <intrin.h>
#include <iostream>
#include <cstdint>
#include <math.h>

long long get_time() {
    return __rdtsc();
}

class simples {
private:
	uint32_t* data;
	bool* new_fls;
	size_t size;
	size_t temp = 0;
public:
	simples(size_t n) : data{ new uint32_t[n] }, new_fls{ new bool[n] }, size{n} {
		for (size_t i = 0; i < n; i++) {
			data[i] = UINT32_MAX;
			new_fls[i] = true;
		}
	}
	~simples() {
		delete[] data;
		delete[] new_fls;
	}
	const uint32_t& operator[](size_t i) const {
		if (i < size) {
			return data[i];
		} else {
			std::cout << "Index out of range\n";
			throw new std::out_of_range("Index out of range");
		}
	}
	size_t get_size() {
		return temp;
	}
	bool is_new(size_t index) {
		if (new_fls[index]) {
			new_fls[index] = false;
			return true;
		} else {
			return false;
		}
	}
	void push_back(uint32_t value) {
		if (temp < size) {
			data[temp] = value;
			new_fls[temp] = true;
			temp++;
		} else {
			size_t old_size = size;
			size = size << 1;
			uint32_t* old_data = data;
			bool* old_new_fls = new_fls;
			data = new uint32_t[size];
			new_fls = new bool[size];
			for (size_t i = 0; i < old_size; i++) {
				data[i] = old_data[i];
				new_fls[i] = old_new_fls[i];
			} for (size_t i = old_size; i < size; i++) {
				data[i] = UINT32_MAX;
				new_fls[i] = true;
			}
			data[old_size] = value;
			new_fls[old_size] = true;
			temp = old_size;
			temp++;
		}
	}
	size_t search_index(uint32_t value) {
		size_t shift_i = 0; 
		size_t size_i = size >> 1;
		do {
			uint32_t op = data[shift_i + size_i];
			if (data[shift_i + size_i] < value) {
				shift_i += size_i;
			} else if (data[shift_i + size_i] == value) {
				return shift_i + size_i;
			}
			size_i >>= 1;
		} while (size_i != 0);
		if (data[shift_i + size_i] == value) return shift_i + size_i;
		return UINT64_MAX;
	}
};

//6n\pm 1
int main() {
	long long a_point, b_point, delta = 0;
	
	simples* s = new simples(16);

	auto func = [&s](uint32_t value) -> uint32_t { 
		if (s->search_index(value) != UINT64_MAX) {
			return value;
		}

		for (uint32_t i = 2; i < sqrt(value) + 1; i++) {
			if (value % i == 0) {
				return 0;
			}
		}

		s->push_back(value);
		
		return value;
	};

	int32_t N;

	for (int32_t N = 1; N != 0;) {
		std::cout << "Enter the upper limit for the calculation: ";
		std::cin >> N;
		
		if (N < 0) {
			std::cout << "Input out of range\n";
			continue;
		}

		long long shift = 0;

		switch (N) {
			case 0:
			case 1:
				break;
			case 2:
				a_point = get_time();
				if (s->search_index(2) == UINT64_MAX) s->push_back(2);
				b_point = get_time();
				goto SKIP;
			case 3:
				a_point = get_time();
				if (s->search_index(2) == UINT64_MAX) s->push_back(2);
				if (s->search_index(3) == UINT64_MAX) s->push_back(3);
				b_point = get_time();
				goto SKIP;
			default:
				a_point = get_time();
				if (s->search_index(2) == UINT64_MAX) s->push_back(2);
				if (s->search_index(3) == UINT64_MAX) s->push_back(3);
				b_point = get_time();
				delta, shift += b_point - a_point;
				break;
		}
		
		a_point = get_time();
		for (uint32_t i = 1; 6 * i - 1 <= N; i++) {
			uint32_t f1 = func(6 * i - 1);
			//if (f1 != 0) std::cout << f1 << std::endl;
			if (6 * i + 1 > N) break;
			uint32_t f2 = func(6 * i + 1);
			//if (f2 != 0) std::cout << f2 << std::endl;
		}
		b_point = get_time();

		SKIP:

		delta += b_point - a_point;

		for (size_t i = 0; i < s->get_size(); i++) {
			if ((*s)[i] > N) break;
			if ((*s).is_new(i)) std::cout << "New number calculated" << std::endl;
			std::cout << (*s)[i] << std::endl;
		}
		std::cout << "That should be enough" << std::endl;

		std::cout << "All primes up to " << N << " have been calculated for " << b_point - a_point + shift << " ticks" << std::endl;
	}
	std::cout << "Total time: " << delta << " ticks" << std::endl;
	delete s;
	return 0;
}