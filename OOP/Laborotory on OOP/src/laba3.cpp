#include <windows.h>
#include <cstdint>
#include <iostream>
#include <locale>
#include <fstream>

class fio {
private:
    char* last_name;
    char* first_name;
    char* middle_name;
	static int count; 
public:
	char* getName() const { return last_name; }
	char* getFirstName() const { return first_name; }
	char* getMiddleName() const { return middle_name; }
	double getCost() const { return middle_name ? atof(middle_name) : 0.0; }
	int getQuant() const { return first_name ? atoi(first_name) : 0; }
	static int& getCount() { return count; }

    fio() : last_name(nullptr), first_name(nullptr), middle_name(nullptr) {}
    fio(char* _last_name, char* _first_name, char* _middle_name) {
        uint32_t size_ln = strlen(_last_name);
        last_name = new char[size_ln + 1];
        for (uint32_t i = 0; i < size_ln; i++) {
            last_name[i] = _last_name[i];
        }
        last_name[size_ln] = '\0';
        
        uint32_t size_fn = strlen(_first_name);
        first_name = new char[size_fn + 1];
        for (uint32_t i = 0; i < size_fn; i++) {
            first_name[i] = _first_name[i];
        }
        first_name[size_fn] = '\0';
        
        uint32_t size_mn = strlen(_middle_name);
        middle_name = new char[size_mn + 1];
        for (uint32_t i = 0; i < size_mn; i++) {
            middle_name[i] = _middle_name[i];
        }
        middle_name[size_mn] = '\0';
        count++;
    }
    fio(const fio& other) {
        if (other.last_name != nullptr) {
            uint32_t size_ln = strlen(other.last_name);
            last_name = new char[size_ln + 1];
            for (uint32_t i = 0; i < size_ln; i++) {
                last_name[i] = other.last_name[i];
            }
            last_name[size_ln] = '\0';
        } else {
            last_name = nullptr;
        }
        
        if (other.first_name != nullptr) {
            uint32_t size_fn = strlen(other.first_name);
            first_name = new char[size_fn + 1];
            for (uint32_t i = 0; i < size_fn; i++) {
                first_name[i] = other.first_name[i];
            }
            first_name[size_fn] = '\0';
        } else {
            first_name = nullptr;
        }
        
        if (other.middle_name != nullptr) {
            uint32_t size_mn = strlen(other.middle_name);
            middle_name = new char[size_mn + 1];
            for (uint32_t i = 0; i < size_mn; i++) {
                middle_name[i] = other.middle_name[i];
            }
            middle_name[size_mn] = '\0';
        } else {
            middle_name = nullptr;
        }
        count++;
    }

    fio& operator=(const fio& other) {
        if (this == &other) {
            return *this;
        }
        
        // Освобождаем старые данные
        delete[] last_name;
        delete[] first_name;
        delete[] middle_name;
        
        // Копируем новые данные
        if (other.last_name != nullptr) {
            uint32_t size_ln = strlen(other.last_name);
            last_name = new char[size_ln + 1];
            for (uint32_t i = 0; i < size_ln; i++) {
                last_name[i] = other.last_name[i];
            }
            last_name[size_ln] = '\0';
        } else {
            last_name = nullptr;
        }
        
        if (other.first_name != nullptr) {
            uint32_t size_fn = strlen(other.first_name);
            first_name = new char[size_fn + 1];
            for (uint32_t i = 0; i < size_fn; i++) {
                first_name[i] = other.first_name[i];
            }
            first_name[size_fn] = '\0';
        } else {
            first_name = nullptr;
        }
        
        if (other.middle_name != nullptr) {
            uint32_t size_mn = strlen(other.middle_name);
            middle_name = new char[size_mn + 1];
            for (uint32_t i = 0; i < size_mn; i++) {
                middle_name[i] = other.middle_name[i];
            }
            middle_name[size_mn] = '\0';
        } else {
            middle_name = nullptr;
        }
        
        return *this;
    }

    ~fio() {
        delete[] last_name;
        delete[] first_name;
        delete[] middle_name;
    }
};

int fio::count = 0;

class FoodExcel {
private:
    fio* list;
    size_t size;

public:
	fio* getList() const { return list; }
	size_t getSize() const { return size; }

    FoodExcel() : list(nullptr), size(0) {
        std::ifstream file("../resources/data.txt");
        if (file.is_open()) {
            file >> size;
            if (size > 0) {
                list = new fio[size];
                for (size_t i = 0; i < size; i++) {
                    char first_name[20];
                    char cost_str[32];
                    char quant_str[32];
                    double cost;
                    int quant;
                    file >> first_name >> cost >> quant;
                    
                    int int_part = (int)cost;
                    int frac_part = (int)((cost - int_part) * 100 + 0.5);
                    int len = 0;
                    if (int_part == 0) {
                        cost_str[len++] = '0';
                    } else {
                        int divisor = 1;
                        while (int_part / divisor >= 10) divisor *= 10;
                        while (divisor > 0) {
                            cost_str[len++] = (int_part / divisor) % 10 + '0';
                            divisor /= 10;
                        }
                    }
                    cost_str[len++] = '.';
                    cost_str[len++] = (frac_part / 10) + '0';
                    cost_str[len++] = (frac_part % 10) + '0';
                    cost_str[len] = '\0';
                    
                    len = 0;
                    if (quant == 0) {
                        quant_str[len++] = '0';
                    } else {
                        int divisor = 1;
                        while (quant / divisor >= 10) divisor *= 10;
                        while (divisor > 0) {
                            quant_str[len++] = (quant / divisor) % 10 + '0';
                            divisor /= 10;
                        }
                    }
                    quant_str[len] = '\0';
                    
                    for (uint32_t j = 0; j < 20; j++) {
                        if (first_name[j] == '_') {
                            first_name[j] = ' ';
                        }
                        if (first_name[j] == '\0') {
                            break;
                        }
                    }
                    list[i] = fio(first_name, quant_str, cost_str);
                }
            }
            file.close();
        } else {
            std::ofstream newFile("../resources/data.txt");
            newFile << "0\n";
            newFile.close();
        }
    }
    FoodExcel(fio* _list, size_t _size) : list(_list), size(_size) {
        std::ifstream file("../resources/data.txt");
        if (file.is_open()) {
            file.close();
        } else {
            std::ofstream newFile("../resources/data.txt");
            newFile << "0\n";
            newFile.close();
        }
    }
    ~FoodExcel() {
        std::ofstream file("../resources/data.txt");
        file << size << "\n";
        for (size_t i = 0; i < size; i++) {
            for (uint32_t j = 0; j < 20; j++) {
                if (list[i].getName()[j] == ' ') {
                    list[i].getName()[j] = '_';
                }
                if (list[i].getName()[j] == '\0') {
                    break;
                }
            }
            file << list[i].getName() << " " << list[i].getCost() << " " << list[i].getQuant() << "\n";
        }
        file.close();

        delete[] list;
    }

    void execute() {
        while (true) {
            std::cout << "Пищевое производство" << std::endl;
            std::cout << "--------------------------------------------------------------" << std::endl;
            std::cout << "Добавить новый элемент                                    …… 1" << std::endl
                      << "Распечатать базу изделий                                  …… 2" << std::endl
                      << "Поиск изделия по названию                                 …… 3" << std::endl
                      << "Найти все изделия меньше заданного количества             …… 4" << std::endl
                      << "Получить количество созданных объектов                    …… 5" << std::endl
                      << "Выход из программы                                        …… 6" << std::endl
                      << "……………………………………………………………………………………………………………………………………………………………………" << std::endl
                      << "Введите номер функции ";
            int choice;
            std::cin >> choice;
			if (choice < 1 || choice > 6) {
                std::cout << "Неверный выбор. Выберите номер функции от 1 до 5." << std::endl;
                continue;
            }
            std::cout << std::endl
                      << "--------------------------------------------------------------" << std::endl;
            size_t _size = 0;
            fio* result = nullptr;
            switch (choice) {
            case 1:
                std::cout << "Название продукта (пробел через \"_\"): ";
                char first_name[20];
                try {
                    std::cin >> first_name;
                } catch (...) {
                    std::cout << "Name is too big, max size is 20 characters" << std::endl;
                    break;
                }
                for (uint32_t i = 0; i < 20; i++) {
                    if (first_name[i] == '_') {
                        first_name[i] = ' ';
                    }
                    if (first_name[i] == '\0') {
                        break;
                    }
                }
                std::cout << "Стоимость продукта: ";
                double cost;
                std::cin >> cost;
                std::cout << "Количество продукта: ";
                int quant;
                std::cin >> quant;
                
                // Преобразуем cost в строку
                char cost_str[32];
                int int_part = (int)cost;
                int frac_part = (int)((cost - int_part) * 100 + 0.5);
                int len = 0;
                if (int_part == 0) {
                    cost_str[len++] = '0';
                } else {
                    int divisor = 1;
                    while (int_part / divisor >= 10) divisor *= 10;
                    while (divisor > 0) {
                        cost_str[len++] = (int_part / divisor) % 10 + '0';
                        divisor /= 10;
                    }
                }
                cost_str[len++] = '.';
                cost_str[len++] = (frac_part / 10) + '0';
                cost_str[len++] = (frac_part % 10) + '0';
                cost_str[len] = '\0';
                
                // Преобразуем quant в строку
                char quant_str[32];
                len = 0;
                if (quant == 0) {
                    quant_str[len++] = '0';
                } else {
                    int divisor = 1;
                    while (quant / divisor >= 10) divisor *= 10;
                    while (divisor > 0) {
                        quant_str[len++] = (quant / divisor) % 10 + '0';
                        divisor /= 10;
                    }
                }
                quant_str[len] = '\0';
                
                add(fio(first_name, quant_str, cost_str));
                std::cout << "--------------------------------------------------------------" << std::endl;
                break;
            case 2:
                print();
                break;
            case 3:
                std::cout << "Название продукта (пробел через \"_\"): ";
                char _last_name[20];
                try {
                    std::cin >> _last_name;
                } catch (...) {
                    std::cout << "Name is too big, max size is 20 characters" << std::endl;
                    break;
                }
                for (uint32_t i = 0; i < 20; i++) {
                    if (_last_name[i] == '_') {
                        _last_name[i] = ' ';
                    }
                    if (_last_name[i] == '\0') {
                        break;
                    }
                }
                std::cout << std::endl;
                _size = 0;
                result = find(_last_name, _size);
                print(result, _size);
                break;
            case 4:
                std::cout << "Цена продукта (сепаратор в формате \".\"): ";
                double _first_name;
                std::cin >> _first_name;
                _size = 0;
                result = find(_first_name, _size);
                print(result, _size);
                break;
            case 5:
                std::cout << "Количество созданных объектов: " << fio::getCount() << std::endl
                          << "--------------------------------------------------------------" << std::endl;
                break;
            case 6:
                return;
            }
        }
    }

private:
    void add(const fio& _food) {
        fio* new_list = new fio[size + 1];
        for (size_t i = 0; i < size; i++) {
            new_list[i] = list[i];
        }
        new_list[size] = _food;
        delete[] list;
        list = new_list;
        size++;
    }

    fio* find(const char* first_name, size_t& _size) {
        fio* _list = new fio[size];
        _size = 0;
        for (size_t i = 0; i < size; i++) {
            if (strcmp(list[i].getName(), first_name) == 0) {
                _list[_size] = list[i];
                _size++;
            }
        }
        return _list;
    }

    fio* find(int quant, size_t& _size) {
        fio* _list = new fio[size];
        _size = 0;
        for (size_t i = 0; i < size; i++) {
            if (list[i].getQuant() < quant) {
                _list[_size] = list[i];
                _size++;
            }
        }
        return _list;
    }
    void print() {
        if (size == 0) {
            std::cout << "Товаров нет" << std::endl
                      << "--------------------------------------------------------------" << std::endl;
            return;
        }
        if (list == nullptr) {
            std::cout << "Товары не найдены" << std::endl
                      << "--------------------------------------------------------------" << std::endl;
            return;
        }
        std::cout << "Название             Цена      Количество      Общая стоимость" << std::endl
                  << "Товара               (руб)        (шт)              (руб)     " << std::endl
                  << "--------------------------------------------------------------" << std::endl;
        for (size_t i = 0; i < size; i++) {
            std::cout << [this](int number) -> char* {
                char* result = new char[83];
                int shift = 0;
                for (size_t i = 0; i < 20; i++) {
                    if ((int)list[number].getName()[i] < 0) {
                        if (i < strlen(list[number].getName())) {
                            shift++;
                            result[i] = list[number].getName()[i];
                        } else {
                            if (shift % 2 == 0) {
                                result[i] = ' ';
                            }
                        }
                    } else {
                        if (i < strlen(list[number].getName())) {
                            result[i] = list[number].getName()[i];
                        } else {
                            result[i] = ' ';
                        }
                    }
                }
                shift /= 2;
                for (int i = 0; i < shift; i++) {
                    result[20 + i] = ' ';
                }
                result[20 + shift] = ' ';
                long cost = list[number].getCost() * 100;
                for (int i = 29; i > 27; i--) {
                    result[i + shift] = cost % 10 + '0';
                    cost /= 10;
                }
                result[27 + shift] = '.';
                for (int i = 26; i > 20; i--) {
                    if (cost == 0) {
                        result[i + shift] = ' ';
                    } else {
                        result[i + shift] = cost % 10 + '0';
                        cost /= 10;
                    }
                }
                result[30 + shift] = ' ';
                int quant = list[number].getQuant();
                for (int i = 41; i > 30; i--) {
                    if (quant == 0) {
                        result[i + shift] = ' ';
                    } else {
                        result[i + shift] = quant % 10 + '0';
                        quant /= 10;
                    }
                }
                result[42 + shift] = ' ';
                long sum = list[number].getCost() * list[number].getQuant() * 100;
                for (int i = 61; i > 59; i--) {
                    result[i + shift] = sum % 10 + '0';
                    sum /= 10;
                }
                result[59 + shift] = '.';
                for (int i = 58; i > 42; i--) {
                    if (sum == 0) {
                        result[i + shift] = ' ';
                    } else {
                        result[i + shift] = sum % 10 + '0';
                        sum /= 10;
                    }
                }
                result[62 + shift] = '\0';
                return result;
            }(i) << std::endl;
        }
        std::cout << "--------------------------------------------------------------" << std::endl
                  << "Всего изделий на сумму" << [this]() -> char* {
            char* result = new char[41];
            double _sum = 0;
            for (size_t i = 0; i < size; i++) {
                _sum += list[i].getCost() * list[i].getQuant();
            }
            long sum = _sum * 100;
            for (int i = 39; i > 37; i--) {
                result[i] = sum % 10 + '0';
                sum /= 10;
            }
            result[37] = '.';
            for (int i = 36; i >= 0; i--) {
                if (sum == 0) {
                    result[i] = ' ';
                } else {
                    result[i] = sum % 10 + '0';
                    sum /= 10;
                }
            }
            result[40] = '\0';
            return result;
        }() << std::endl
            << "Количество записей в базе" << ' ' << size << std::endl <<
            "--------------------------------------------------------------" << std::endl;
    }

    void print(const fio* foodList, size_t foodSize) {
        std::cout << std::endl 
        << "--------------------------------------------------------------" << std::endl;
        if (foodList == nullptr || foodSize == 0) {
            std::cout << "Товары не найдены" << std::endl
                      << "--------------------------------------------------------------" << std::endl;
            return;
        }
        std::cout << "Название             Цена      Количество      Общая стоимость" << std::endl
                  << "Товара               (руб)        (шт)              (руб)     " << std::endl
                  << "--------------------------------------------------------------" << std::endl;
        for (size_t i = 0; i < foodSize; i++) {
            std::cout << [&foodList](int number) -> char* {
                char* result = new char[83];
                int shift = 0;
                for (size_t i = 0; i < 20; i++) {
                    if ((int)foodList[number].getName()[i] < 0) {
                        if (i < strlen(foodList[number].getName())) {
                            shift++;
                            result[i] = foodList[number].getName()[i];
                        } else {
                            if (shift % 2 == 0) {
                                result[i] = ' ';
                            }
                        }
                    } else {
                        if (i < strlen(foodList[number].getName())) {
                            result[i] = foodList[number].getName()[i];
                        } else {
                            result[i] = ' ';
                        }
                    }
                }
                shift /= 2;
                for (int i = 0; i < shift; i++) {
                    result[20 + i] = ' ';
                }
                result[20 + shift] = ' ';
                long cost = foodList[number].getCost() * 100;
                for (int i = 29; i > 27; i--) {
                    result[i + shift] = cost % 10 + '0';
                    cost /= 10;
                }
                result[27 + shift] = '.';
                for (int i = 26; i > 20; i--) {
                    if (cost == 0) {
                        result[i + shift] = ' ';
                    } else {
                        result[i + shift] = cost % 10 + '0';
                        cost /= 10;
                    }
                }
                result[30 + shift] = ' ';
                int quant = foodList[number].getQuant();
                for (int i = 41; i > 30; i--) {
                    if (quant == 0) {
                        result[i + shift] = ' ';
                    } else {
                        result[i + shift] = quant % 10 + '0';
                        quant /= 10;
                    }
                }
                result[42 + shift] = ' ';
                long sum = foodList[number].getCost() * foodList[number].getQuant() * 100;
                for (int i = 61; i > 59; i--) {
                    result[i + shift] = sum % 10 + '0';
                    sum /= 10;
                }
                result[59 + shift] = '.';
                for (int i = 58; i > 42; i--) {
                    if (sum == 0) {
                        result[i + shift] = ' ';
                    } else {
                        result[i + shift] = sum % 10 + '0';
                        sum /= 10;
                    }
                }
                result[62 + shift] = '\0';
                return result;
            }(i) << std::endl;
        }
        std::cout << "--------------------------------------------------------------" << std::endl
                  << "Всего изделий на сумму" << [&foodList, &foodSize]() -> char* {
            char* result = new char[41];
            double _sum = 0;
            for (size_t i = 0; i < foodSize; i++) {
                _sum += foodList[i].getCost() * foodList[i].getQuant();
            }
            long sum = _sum * 100;
            for (int i = 39; i > 37; i--) {
                result[i] = sum % 10 + '0';
                sum /= 10;
            }
            result[37] = '.';
            for (int i = 36; i >= 0; i--) {
                if (sum == 0) {
                    result[i] = ' ';
                } else {
                    result[i] = sum % 10 + '0';
                    sum /= 10;
                }
            }
            result[40] = '\0';
            return result;
        }() << std::endl
            << "Количество записей в базе" << ' ' << foodSize << std::endl
            << "--------------------------------------------------------------" << std::endl;
    }
};

int main() {
    setlocale(LC_ALL, "ru_RU.UTF-8");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    FoodExcel foodexcel = FoodExcel();
    foodexcel.execute();

    return 0;
}