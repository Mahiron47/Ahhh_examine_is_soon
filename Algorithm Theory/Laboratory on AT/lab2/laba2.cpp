#include "vector.hpp"
#include <vector>
#include <iostream>
#include <cstdlib>
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <algorithm>
//#include <thread>

template<typename T>
vector<T>& insert_sort(vector<T>& list, bool(*func)(T, T)) {
	for (size_t i = 0; i + 1 < list.size(); i++) {
		for (size_t j = 0; j < i + 1; j++) {
			if (func(list[j], list[i + 1])) {
				T cache = list[i];
				list[j] = list[i + 1];
				list[i + 1] = cache;
			}
		}
	}
	return list;
}

int main() {
	char* char_heap = new char[10'000];
	int* int_heap = new int[10'000];
	double* double_heap = new double[10'000];
	char** string_heap = new char*[10'000];

	for (uint64_t i = 0; i < 10'000; i++) {
		char_heap[i] = static_cast<char>(static_cast<uint8_t>(i));
		int_heap[i] = i;
		double_heap[i] = static_cast<double>(i) + (static_cast<double>(i) + 1.0) / 10.0 + (static_cast<double>(i) + 2.0) / 100.0;
		string_heap[i] = new char[10'000] {"PKIMS RULEZ\0"};
	}
//	std::this_thread::sleep_for(std::chrono::seconds(100000));

	double seconds_per_tick = static_cast<double>(std::chrono::high_resolution_clock::duration::period::num) / 
	                          static_cast<double>(std::chrono::high_resolution_clock::duration::period::den);
	double nanoseconds_per_tick = seconds_per_tick * 1'000'000'000;


	// ========== char via printf ==========
	auto t_char_printf_0 = std::chrono::high_resolution_clock::now();
	for (uint32_t i = 0; i < 10'000; i++) {
		printf("%c", char_heap[i]);
	}
	auto t_char_printf_1 = std::chrono::high_resolution_clock::now();
	auto delta_ns_char_printf = std::chrono::duration_cast<std::chrono::nanoseconds>(t_char_printf_1 - t_char_printf_0).count();
	
	// ========== char via std::cout ==========
	auto t_char_cout_0 = std::chrono::high_resolution_clock::now();
	for (uint32_t i = 0; i < 10'000; i++) {
		std::cout << char_heap[i];
	}
	auto t_char_cout_1 = std::chrono::high_resolution_clock::now();
	auto delta_ns_char_cout = std::chrono::duration_cast<std::chrono::nanoseconds>(t_char_cout_1 - t_char_cout_0).count();

	// ========== int via printf ==========
	auto t_int_printf_0 = std::chrono::high_resolution_clock::now();
	for (uint32_t i = 0; i < 10'000; i++) {
		printf("%d ", int_heap[i]);
	}
	auto t_int_printf_1 = std::chrono::high_resolution_clock::now();
	auto delta_ns_int_printf = std::chrono::duration_cast<std::chrono::nanoseconds>(t_int_printf_1 - t_int_printf_0).count();
	
	// ========== int via std::cout ==========
	auto t_int_cout_0 = std::chrono::high_resolution_clock::now();
	for (uint32_t i = 0; i < 10'000; i++) {
		std::cout << int_heap[i];
	}
	auto t_int_cout_1 = std::chrono::high_resolution_clock::now();
	auto delta_ns_int_cout = std::chrono::duration_cast<std::chrono::nanoseconds>(t_int_cout_1 - t_int_cout_0).count();
	
	// ========== double via printf ==========
	auto t_double_printf_0 = std::chrono::high_resolution_clock::now();
	for (uint32_t i = 0; i < 10'000; i++) {
		printf("%f ", double_heap[i]);
	}
	auto t_double_printf_1 = std::chrono::high_resolution_clock::now();
	auto delta_ns_double_printf = std::chrono::duration_cast<std::chrono::nanoseconds>(t_double_printf_1 - t_double_printf_0).count();
	
	// ========== double via std::cout ==========
	auto t_double_cout_0 = std::chrono::high_resolution_clock::now();
	for (uint32_t i = 0; i < 10'000; i++) {
		std::cout << double_heap[i];
	}
	auto t_double_cout_1 = std::chrono::high_resolution_clock::now();
	auto delta_ns_double_cout = std::chrono::duration_cast<std::chrono::nanoseconds>(t_double_cout_1 - t_double_cout_0).count();
	
	// ========== char[] (string) via printf ==========
	auto t_string_printf_0 = std::chrono::high_resolution_clock::now();
	for (uint32_t i = 0; i < 10'000; i++) {
		printf("%s ", string_heap[i]);
	}
	auto t_string_printf_1 = std::chrono::high_resolution_clock::now();
	auto delta_ns_string_printf = std::chrono::duration_cast<std::chrono::nanoseconds>(t_string_printf_1 - t_string_printf_0).count();
	
	// ========== char[] (string) via std::cout ==========
	auto t_string_cout_0 = std::chrono::high_resolution_clock::now();
	for (uint32_t i = 0; i < 10'000; i++) {
		std::cout << string_heap[i];
	}
	auto t_string_cout_1 = std::chrono::high_resolution_clock::now();
	auto delta_ns_string_cout = std::chrono::duration_cast<std::chrono::nanoseconds>(t_string_cout_1 - t_string_cout_0).count();
	
	for (uint64_t i = 0; i < 10'000; i++) {
		delete[] string_heap[i];
	}
	delete[] string_heap;
	delete[] char_heap;
	delete[] int_heap;
	delete[] double_heap;

	vector<int32_t> vec3 = {3, 1, 2};
	vector<int32_t> vec5 = {4, 2, 5, 1, 3};
	vector<int32_t> vec10 = {7, 3, 9, 1, 5, 10, 2, 8, 4, 6};

	auto t_vec3_insert_sort_0 = std::chrono::high_resolution_clock::now();
	insert_sort<int32_t>(vec3, [](int32_t a, int32_t b) -> bool { return a > b; });
	auto t_vec3_insert_sort_1 = std::chrono::high_resolution_clock::now();
	auto delta_ns_vec3_insert_sort = std::chrono::duration_cast<std::chrono::nanoseconds>(t_vec3_insert_sort_1 - t_vec3_insert_sort_0).count();

	auto t_vec5_insert_sort_0 = std::chrono::high_resolution_clock::now();
	insert_sort<int32_t>(vec5, [](int32_t a, int32_t b) -> bool { return a > b; });
	auto t_vec5_insert_sort_1 = std::chrono::high_resolution_clock::now();
	auto delta_ns_vec5_insert_sort = std::chrono::duration_cast<std::chrono::nanoseconds>(t_vec5_insert_sort_1 - t_vec5_insert_sort_0).count();

	auto t_vec10_insert_sort_0 = std::chrono::high_resolution_clock::now();
	insert_sort<int32_t>(vec10, [](int32_t a, int32_t b) -> bool { return a > b; });
	auto t_vec10_insert_sort_1 = std::chrono::high_resolution_clock::now();
	auto delta_ns_vec10_insert_sort = std::chrono::duration_cast<std::chrono::nanoseconds>(t_vec10_insert_sort_1 - t_vec10_insert_sort_0).count();

	std::vector<int32_t> std_vec3 = {3, 1, 2};
	std::vector<int32_t> std_vec5 = {4, 2, 5, 1, 3};
	std::vector<int32_t> std_vec10 = {7, 3, 9, 1, 5, 10, 2, 8, 4, 6};

	auto t_vec3_std_sort_0 = std::chrono::high_resolution_clock::now();
	std::sort(std_vec3.begin(), std_vec3.end(), [](int32_t a, int32_t b) -> bool { return a > b; });
//	std::this_thread::sleep_for(std::chrono::seconds(1));
	auto t_vec3_std_sort_1 = std::chrono::high_resolution_clock::now();
	auto delta_ns_vec3_std_sort = std::chrono::duration_cast<std::chrono::nanoseconds>(t_vec3_std_sort_1 - t_vec3_std_sort_0).count();

	auto t_vec5_std_sort_0 = std::chrono::high_resolution_clock::now();
	std::sort(std_vec5.begin(), std_vec5.end(), [](int32_t a, int32_t b) -> bool { return a > b; });
	auto t_vec5_std_sort_1 = std::chrono::high_resolution_clock::now();
	auto delta_ns_vec5_std_sort = std::chrono::duration_cast<std::chrono::nanoseconds>(t_vec5_std_sort_1 - t_vec5_std_sort_0).count();
	
	auto t_vec10_std_sort_0 = std::chrono::high_resolution_clock::now();
	std::sort(std_vec10.begin(), std_vec10.end(), [](int32_t a, int32_t b) -> bool { return a > b; });
	auto t_vec10_std_sort_1 = std::chrono::high_resolution_clock::now();
	auto delta_ns_vec10_std_sort = std::chrono::duration_cast<std::chrono::nanoseconds>(t_vec10_std_sort_1 - t_vec10_std_sort_0).count();

	auto allign_topic = [](double value, const char word[100], char* pointer, uint32_t i = 0) -> char* {
		pointer = new char[100];
		for (uint32_t j = 0; j < 100; j++) {
			pointer[j] = ' ';
		}

		uint32_t i_value = static_cast<uint32_t>(value);
		
		while (i_value /= 10) {
			i++;
		}

		uint32_t shift = i;
		i = 0;
		while (word[i++] != '\0') {
			pointer[shift / 2 + i] = word[i - 1];
		}

		pointer[shift + i] = '\0';
		
		return pointer;
	};

	auto allign_mantisa = [](double previous, double value, char* pointer, uint32_t i = 0) -> char* {
		pointer = new char[100];
		for (uint32_t j = 0; j < 100; j++) {
			pointer[j] = '0';
		}

		uint32_t i_value = static_cast<uint32_t>(value);
		do {
			i++;
		} while (i_value /= 10);
		
		uint32_t length = i;
		uint32_t cache = i;
		i_value = static_cast<uint32_t>(value);
		do {
			pointer[--i] = i_value % 10 + 48;
		} while (i_value /= 10);

		pointer[length] = '.';
		
		value = value - static_cast<uint32_t>(value);
		i_value = static_cast<uint32_t>(value * 1'000);

		length += 3;
		i = length;
		do {
			pointer[i--] = i_value % 10 + 48;
		}
		while (i_value /= 10);

		i = 0;
		i_value = static_cast<uint32_t>(previous);
		do {
			i++;
		} while (i_value /= 10);

		if (cache < i)
			pointer[++length + i - cache] = '\0';
		else 
			pointer[++length] = '\0';

		return pointer;
	};

	double delta_ms_char_printf = static_cast<double>(delta_ns_char_printf) / 1'000'000.0;
	double delta_ms_char_cout = static_cast<double>(delta_ns_char_cout) / 1'000'000.0;
	double delta_ms_int_printf = static_cast<double>(delta_ns_int_printf) / 1'000'000.0;
	double delta_ms_int_cout = static_cast<double>(delta_ns_int_cout) / 1'000'000.0;
	double delta_ms_double_printf = static_cast<double>(delta_ns_double_printf) / 1'000'000.0;
	double delta_ms_double_cout = static_cast<double>(delta_ns_double_cout) / 1'000'000.0;
	double delta_ms_string_printf = static_cast<double>(delta_ns_string_printf) / 1'000'000.0;
	double delta_ms_string_cout = static_cast<double>(delta_ns_string_cout) / 1'000'000.0;

	char* pointer = new char[100];
	
	std::cout << std::endl << std::endl << "Tick period: " << nanoseconds_per_tick << " nanoseconds per tick\n";

	std::cout << std::endl << "          |"  
	<< allign_topic(delta_ms_char_printf > delta_ms_char_cout ? delta_ms_char_printf : delta_ms_char_cout, "  char   \0", pointer) << "|" 
	<< allign_topic(delta_ms_int_printf > delta_ms_int_cout ? delta_ms_int_printf : delta_ms_int_cout, "  int    \0", pointer) <<  "|" 
	<< allign_topic(delta_ms_double_printf > delta_ms_double_cout ? delta_ms_double_printf : delta_ms_double_cout, " double  \0", pointer) <<  "|" 
	<< allign_topic(delta_ms_string_printf > delta_ms_string_cout ? delta_ms_string_printf : delta_ms_string_cout, "\"PKIMS RULEZ\"\0", pointer) << "|\n";

	std::cout << "printf    | " 
	<< allign_mantisa(delta_ms_char_cout, delta_ms_char_printf, pointer) << " ms | " 
	<< allign_mantisa(delta_ms_int_cout, delta_ms_int_printf, pointer) << " ms | " 
	<< allign_mantisa(delta_ms_double_cout, delta_ms_double_printf, pointer) << " ms |   " 
	<< allign_mantisa(delta_ms_string_cout, delta_ms_string_printf, pointer) << " ms   |\n";
	
	std::cout << "std::cout | " 
	<< allign_mantisa(delta_ms_char_printf, delta_ms_char_cout, pointer) << " ms | " 
	<< allign_mantisa(delta_ms_int_printf, delta_ms_int_cout, pointer) << " ms | "
	<< allign_mantisa(delta_ms_double_printf, delta_ms_double_cout, pointer) << " ms |   "
	<< allign_mantisa(delta_ms_string_printf, delta_ms_string_cout, pointer) << " ms   |\n";

	std::cout << std::endl << "                   |"
	<< allign_topic(delta_ns_vec3_insert_sort > delta_ns_vec3_std_sort ? delta_ns_vec3_insert_sort : delta_ns_vec3_std_sort, "  3 elements  \0", pointer) << "|"
	<< allign_topic(delta_ns_vec5_insert_sort > delta_ns_vec5_std_sort ? delta_ns_vec5_insert_sort : delta_ns_vec5_std_sort, "  5 elements  \0", pointer) << "|"
	<< allign_topic(delta_ns_vec10_insert_sort > delta_ns_vec10_std_sort ? delta_ns_vec10_insert_sort : delta_ns_vec10_std_sort, " 10 elements  \0", pointer) << "|\n";

	std::cout << "insertion_sort_2   |    "
	<< allign_mantisa(delta_ns_vec3_std_sort, delta_ns_vec3_insert_sort, pointer) << " ns   |    "
	<< allign_mantisa(delta_ns_vec5_std_sort, delta_ns_vec5_insert_sort, pointer) << " ns   |    "
	<< allign_mantisa(delta_ns_vec10_std_sort, delta_ns_vec10_insert_sort, pointer) << " ns   |   \n";

	std::cout << "std::sort          |    "
	<< allign_mantisa(delta_ns_vec3_insert_sort, delta_ns_vec3_std_sort, pointer) << " ns   |    "
	<< allign_mantisa(delta_ns_vec5_insert_sort, delta_ns_vec5_std_sort, pointer) << " ns   |    "
	<< allign_mantisa(delta_ns_vec10_insert_sort, delta_ns_vec10_std_sort, pointer) << " ns   |\n";

	delete[] pointer;

	return 0;
}