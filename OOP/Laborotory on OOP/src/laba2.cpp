#include <windows.h>
#include <cstdint>
#include <iostream>
#include <locale>
#include <fstream>

class food {
private:
    // название изделий
    char* name;
    // количество
    int quant;
    // стоимость
    double cost;
	// количество созданных объектов
	static int count; 
public:
	char* getName() const { return name; }
	double getCost() const { return cost; }
	int getQuant() const { return quant; }
	static int& getCount() { return count; }

    food() : name(nullptr), quant(0), cost(0) {}
    food(char* _name, double _cost, int _quant) : quant(_quant), cost(_cost) {
        uint32_t size = strlen(_name);
        name = new char[size + 1];
        for (uint32_t i = 0; i < size; i++) {
            name[i] = _name[i];
        }
        name[size] = '\0';
        count++;
    }
    food(const food& other) : quant(other.getQuant()), cost(other.getCost()) {
        uint32_t size = strlen(other.getName());
        name = new char[size + 1];
        for (uint32_t i = 0; i < size; i++) {
            name[i] = other.getName()[i];
        }
        name[size] = '\0';
        count++;
    }

    food& operator=(const food& other) {
        if (this == &other) {
            return *this;
        }
        if (other.getName() == nullptr) {
            if (name != nullptr) {
                delete[] name;
            }
            name = nullptr;
            cost = 0;
            quant = 0;
        } else if (this != &other) {
            delete[] name;
            uint32_t size = strlen(other.getName());
            name = new char[size + 1];
            for (uint32_t i = 0; i < size; i++) {
                name[i] = other.getName()[i];
            }
            name[size] = '\0';
            cost = other.getCost();
            quant = other.getQuant();
        }
        return *this;
    }

    ~food() {
        delete[] name;
    }
};

int food::count = 0;

class FoodExcel {
private:
    food* list;
    size_t size;

public:
	food* getList() const { return list; }
	size_t getSize() const { return size; }

    FoodExcel() : list(nullptr), size(0) {
        std::ifstream file("../resources/data.txt");
        if (file.is_open()) {
            file >> size;
            if (size > 0) {
                list = new food[size];
                for (size_t i = 0; i < size; i++) {
                    char name[20];
                    double cost;
                    int quant;
                    file >> name >> cost >> quant;
                    for (uint32_t j = 0; j < 20; j++) {
                        if (name[j] == '_') {
                            name[j] = ' ';
                        }
                        if (name[j] == '\0') {
                            break;
                        }
                    }
                    list[i] = food(name, cost, quant);
                }
            }
            file.close();
        } else {
            std::ofstream newFile("../resources/data.txt");
            newFile << "0\n";
            newFile.close();
        }
    }
    FoodExcel(food* _list, size_t _size) : list(_list), size(_size) {
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
            food* result = nullptr;
            switch (choice) {
            case 1:
                std::cout << "Название продукта (пробел через \"_\"): ";
                char name[20];
                try {
                    std::cin >> name;
                } catch (...) {
                    std::cout << "Name is too big, max size is 20 characters" << std::endl;
                    break;
                }
                for (uint32_t i = 0; i < 20; i++) {
                    if (name[i] == '_') {
                        name[i] = ' ';
                    }
                    if (name[i] == '\0') {
                        break;
                    }
                }
                std::cout << "Стоимость продукта: ";
                double cost;
                std::cin >> cost;
                std::cout << "Количество продукта: ";
                int quant;
                std::cin >> quant;
                add(food(name, cost, quant));
                std::cout << "--------------------------------------------------------------" << std::endl;
                break;
            case 2:
                print();
                break;
            case 3:
                std::cout << "Название продукта (пробел через \"_\"): ";
                char _name[20];
                try {
                    std::cin >> _name;
                } catch (...) {
                    std::cout << "Name is too big, max size is 20 characters" << std::endl;
                    break;
                }
                for (uint32_t i = 0; i < 20; i++) {
                    if (_name[i] == '_') {
                        _name[i] = ' ';
                    }
                    if (_name[i] == '\0') {
                        break;
                    }
                }
                std::cout << std::endl;
                _size = 0;
                result = find(_name, _size);
                print(result, _size);
                break;
            case 4:
                std::cout << "Цена продукта (сепаратор в формате \".\"): ";
                double _cost;
                std::cin >> _cost;
                _size = 0;
                result = find(_cost, _size);
                print(result, _size);
                break;
            case 5:
                std::cout << "Количество созданных объектов: " << food::getCount() << std::endl
                          << "--------------------------------------------------------------" << std::endl;
                break;
            case 6:
                return;
            }
        }
    }

private:
    void add(const food& _food) {
        food* new_list = new food[size + 1];
        for (size_t i = 0; i < size; i++) {
            new_list[i] = list[i];
        }
        new_list[size] = _food;
        delete[] list;
        list = new_list;
        size++;
    }

    food* find(const char* name, size_t& _size) {
        food* _list = new food[size];
        _size = 0;
        for (size_t i = 0; i < size; i++) {
            if (strcmp(list[i].getName(), name) == 0) {
                _list[_size] = list[i];
                _size++;
            }
        }
        return _list;
    }

    food* find(int quant, size_t& _size) {
        food* _list = new food[size];
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

    void print(const food* foodList, size_t foodSize) {
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

    FoodExcel foodexcel = FoodExcel(/*new food[4]{
        food(new char[20]{"Coconuts type 1"}, 100, 2),
        food(new char[20]{"Apples"}, 50, 3),
        food(new char[20]{"Bananas"}, 200, 1),
        food(new char[20]{"Potatoes"}, 50, 5)
    }, 4*/);
    foodexcel.execute();

    return 0;
}