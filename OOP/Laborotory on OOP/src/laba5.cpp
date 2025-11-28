#include <windows.h>
#include <cctype>
#include <cstdlib>
#include <exception>
#include <iomanip>
#include <iostream>
#include <limits>
#include <random>
#include <string>
#include <type_traits>

template <typename T>
void fill_random_array_ptr(T* arr, std::size_t count, T min_value, T max_value) {
	static bool seeded = false;
	if (!seeded) {
		std::srand(static_cast<unsigned int>(std::random_device{}()));
		seeded = true;
	}
	if constexpr (std::is_integral_v<T>) {
		long long low = static_cast<long long>(min_value);
		long long high = static_cast<long long>(max_value);
		long long range = high - low + 1;
		for (std::size_t i = 0; i < count; ++i) {
			if (range <= 0) {
				arr[i] = static_cast<T>(min_value);
			} else {
				arr[i] = static_cast<T>(low + (std::rand() % static_cast<int>(range)));
			}
		}
	} else if constexpr (std::is_floating_point_v<T>) {
		double low = static_cast<double>(min_value);
		double high = static_cast<double>(max_value);
		double span = high - low;
		for (std::size_t i = 0; i < count; ++i) {
			double r = static_cast<double>(std::rand()) / (static_cast<double>(RAND_MAX) + 1.0);
			arr[i] = static_cast<T>(low + r * span);
		}
	} else {
		for (std::size_t i = 0; i < count; ++i) {
			arr[i] = T{};
		}
	}
}

template <typename T, std::size_t N>
void fill_random_array(T (&arr)[N], T min_value, T max_value) {
	fill_random_array_ptr(arr, N, min_value, max_value);
}

template <typename T, std::size_t N>
void input_array(T (&arr)[N], const std::string& label) {
	std::cout << "Введите " << N << " значений для " << label << ":\n";
	for (std::size_t i = 0; i < N; ++i) {
		while (true) {
			std::cout << label << "[" << i << "] : ";
			if (std::cin >> arr[i]) {
				break;
			}
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << "Некорректный ввод. Повторите попытку." << std::endl;
		}
	}
}

template <std::size_t N>
void input_array(char (&arr)[N], const std::string& label) {
	std::cout << "Введите " << N << " символов для " << label << " (цифры запрещены):\n";
	for (std::size_t i = 0; i < N; ++i) {
		while (true) {
			std::string token;
			std::cout << label << "[" << i << "] : ";
			std::cin >> token;
			if (token.size() == 1 && !std::isdigit(static_cast<unsigned char>(token[0]))) {
				arr[i] = token[0];
				break;
			}
			std::cout << "Ошибка : введите один символ (не цифру)." << std::endl;
		}
	}
}

template <typename T, std::size_t N>
void print_array(const T (&arr)[N], std::size_t per_line, const std::string& title) {
	std::cout << "\n" << title << " (по " << per_line << " элементов в строке):\n";
	for (std::size_t i = 0; i < N; ++i) {
		std::cout << std::setw(10) << arr[i];
		if ((i + 1) % per_line == 0 || i + 1 == N) {
			std::cout << '\n';
		}
	}
}

template <typename T>
class DynamicArray {
private:
	int n;
	T* Bp;
	T* Tp;

public:
	DynamicArray(int size, T min_value, T max_value)
		: n(size), Bp(nullptr), Tp(nullptr) {
		if (n <= 0) {
			throw std::invalid_argument("Размер массива должен быть положительным");
		}
		Bp = new T[n];
		Tp = Bp;

		fill_random_array_ptr(Bp, static_cast<std::size_t>(n), min_value, max_value);
	}

	~DynamicArray() {
		delete[] Bp;
		Bp = nullptr;
		Tp = nullptr;
		n = 0;
	}

	void print_table(int per_line = 5) const {
		std::cout << "\nДинамический массив на " << n << " элементов :\n";
		for (int i = 0; i < n; ++i) {
			std::cout << std::setw(10) << Bp[i];
			if ((i + 1) % per_line == 0 || i + 1 == n) {
				std::cout << '\n';
			}
		}
	}

	T* find_min() {
		if (n == 0) {
			return nullptr;
		}
		T* min_ptr = Bp;
		Tp = Bp;
		for (int i = 1; i < n; ++i) {
			++Tp;
			if (*Tp < *min_ptr) {
				min_ptr = Tp;
			}
		}
		Tp = min_ptr;
		return min_ptr;
	}
};

int main() {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	constexpr std::size_t INT_COUNT = 5;
	constexpr std::size_t DOUBLE_COUNT = 4;
	constexpr std::size_t CHAR_COUNT = 6;

	int int_static[INT_COUNT];
	double double_static[DOUBLE_COUNT];
	char char_static[CHAR_COUNT];

	fill_random_array(int_static, -50, 50);
	fill_random_array(double_static, -10.0, 10.0);
	fill_random_array(char_static, 'A', 'Z');

	print_array(int_static, 5, "Случайный статический массив int");
	print_array(double_static, 4, "Случайный статический массив double");
	print_array(char_static, 6, "Случайный статический массив char");

	std::cout << "\n================== Ввод массивов с клавиатуры ==================\n" << std::endl;
	input_array(int_static, "целочисленного массива");
	input_array(double_static, "вещественного массива");
	input_array(char_static, "символьного массива");

	print_array(int_static, 5, "Введённый пользователем массив int");
	print_array(double_static, 4, "Введённый пользователем массив double");
	print_array(char_static, 6, "Введённый пользователем массив char");

	std::cout << "\n===================== Динамические массивы =====================" << std::endl;
	try {
		DynamicArray<int> dyn_int(8, -100, 100);
		DynamicArray<double> dyn_double(6, -5.0, 5.0);

		dyn_int.print_table(4);
		dyn_double.print_table(3);

		std::cout << std::endl;

		if (int* min_int = dyn_int.find_min()) {
			std::cout << "Минимальный элемент в динамическом int массиве : " << *min_int << std::endl;
		}
		if (double* min_double = dyn_double.find_min()) {
			std::cout << "Минимальный элемент в динамическом double массиве : " << *min_double << std::endl;
		}
	} catch (const std::exception& ex) {
		std::cerr << "Ошибка : " << ex.what() << std::endl;
	}

	return 0;
}
