#include <windows.h>
#include <cstdint>
#include <iostream>
#include <locale>
#include <fstream>
#include <cstring>
#include <cmath>
#include <string>
#include <algorithm>
#include <cassert>

using std::string;

class fio {
    string last_name;
    string first_name;
    string middle_name;
public:
    inline string getLastName() const { return last_name; }
    inline string getFirstName() const { return first_name; }
    inline string getMiddleName() const { return middle_name; }

    fio() : last_name(""), first_name(""), middle_name("") {}

    fio(string _last_name, string _first_name, string _middle_name) {
        last_name = _last_name;
        first_name = _first_name;
        middle_name = _middle_name;
    }

    fio(const fio& other) {
        last_name = other.last_name;
        first_name = other.first_name;
        middle_name = other.middle_name;
    }
};

class bank {
    static bank** instances;
    static uint32_t count;
    fio full_name;
    uint32_t id;
    float balance;

    bank() : full_name(), id(0), balance(0.0f) {}
public:
    fio getFullName() const { return full_name; }
    uint32_t getId() const { return id; }
    float getBalance() const { return balance; }

    bank(const fio& _full_name, float _balance) : full_name(_full_name), balance(_balance) {
        id = count++;
        bank** _instances = new bank * [id + 1];
        if (instances != nullptr) {
            for (uint32_t i = 0; i < id; i++) {
                _instances[i] = instances[i];
            }
            delete[] instances;
            _instances[id] = this;
        }
        else {
            _instances[0] = this;
        }
        instances = _instances;
    }

    bank(const fio& _full_name, uint32_t _id, float _balance) : full_name(_full_name), id(_id), balance(_balance) {
        bank** _instances = new bank * [id + 1];
        if (instances != nullptr) {
            for (uint32_t i = 0; i < id; i++) {
                _instances[i] = instances[i];
            }
            delete[] instances;
            _instances[id] = this;
        }
        else {
            _instances[0] = this;
        }
        instances = _instances;
    }

    void printInfo() const {
        std::cout << *this;
    }

    static void initialize() {
        if (instances == nullptr) {
            std::ifstream file("../resources/data.txt");
            if (file.is_open()) {
                file >> count;
                if (count > 0) {
                    for (uint32_t i = 0; i < count; i++) {
                        string first_name;
                        string last_name;
                        string middle_name;
                        float balance;

                        file >> last_name >> first_name >> middle_name >> balance;

                        std::replace(first_name.begin(), first_name.end(), '_', ' ');
                        std::replace(last_name.begin(), last_name.end(), '_', ' ');
                        std::replace(middle_name.begin(), middle_name.end(), '_', ' ');

                        new bank(fio(last_name, first_name, middle_name), i, balance);
                    }
                    file.close();
                }
            } else {
                CreateDirectoryA("../resources", NULL);
                std::ofstream outfile("../resources/data.txt");
                if (outfile.is_open()) {
                    outfile << 0 << std::endl;
                    outfile.close();
                }
            }
        }
    }

    static void execute() {
        while (true) {
            std::cout << std::endl
                << "----------------------------------------------------------------" << std::endl
                << "Меню банка :" << std::endl
                << "1. Добавить запись" << std::endl
                << "2. Показать все записи" << std::endl
                << "3. Найти запись по ФИО" << std::endl
                << "4. Увеличить баланс по ФИО" << std::endl
                << "5. Выход и сохранение" << std::endl
                << "----------------------------------------------------------------" << std::endl;
            uint32_t choice;
            std::cin >> choice;
            switch (choice) {
            case 1: {
                string last_name;
                string first_name;
                string middle_name;
                float balance;
                std::cout << "Фамилия : ";
                std::cin >> last_name;
                std::cout << "Имя : ";
                std::cin >> first_name;
                std::cout << "Отчество : ";
                std::cin >> middle_name;
                std::cout << "Баланс : ";
                std::cin >> balance;
                bank::add(fio(last_name, first_name, middle_name), balance);
            }
            break;
            case 2: 
                bank::print();
            break;
            case 3: {
                string last_name;
                string first_name;
                string middle_name;
                std::cout << "Фамилия : ";
                std::cin >> last_name;
                std::cout << "Имя : ";
                std::cin >> first_name;
                std::cout << "Отчество : ";
                std::cin >> middle_name;
                fio _full_name(last_name, first_name, middle_name);
                bank* found = bank::find(_full_name);
                if (found != nullptr) {
                    std::cout << "Запись найдена : ";
                    found->printInfo();
                } else {
                    std::cout << "Запись не найдена." << std::endl;
                }
            }
            break;
            case 4: {
                string last_name;
                string first_name;
                string middle_name;
                float increase;
                std::cout << "Фамилия : ";
                std::cin >> last_name;
                std::cout << "Имя : ";
                std::cin >> first_name;
                std::cout << "Отчество : ";
                std::cin >> middle_name;
                std::cout << "Увеличение баланса на : ";
                std::cin >> increase;
                fio _full_name(last_name, first_name, middle_name);
                bank* found = bank::find(_full_name);
                if (found != nullptr) {
                    *found + increase;
                    std::cout << "Баланс успешно обновлён. Новая информация : ";
                    found->printInfo();
                } else {
                    std::cout << "Запись не найдена." << std::endl;
                }
            }
            break;
            case 5:
                bank::exit();
                return;
            default: 
                std::cout << "Неверный выбор. Пожалуйста, попробуйте снова." << std::endl;
            }
        }
    }

    static void add(const fio& _full_name, float _balance) {
        new bank(_full_name, _balance);
    }

    static void print() {
        std::cout << "----------------------------------------------------------------" << std::endl;
        if (instances == nullptr || count == 0) {
            std::cout << "Записи не найдены" << std::endl
                << "----------------------------------------------------------------" << std::endl;
            return;
        }
        std::cout << "Полное                         Номер счёта  Депозит             " << std::endl
                  << "Имя                            (число)      (руб)               " << std::endl
                  << "----------------------------------------------------------------" << std::endl;
        for (uint64_t i = 0; i < count; i++) {
            std::cout << instances[i]->getFullName().getLastName() << ' '
                << instances[i]->getFullName().getFirstName() << ' '
                << instances[i]->getFullName().getMiddleName() << [](uint32_t number) -> string {
                string result = "";
                for (uint32_t i = 0; i <= 30 - number - 2; i++) {
                    result += " ";
                }
                return result;
                }(instances[i]->getFullName().getLastName().size() + instances[i]->getFullName().getFirstName().size() + instances[i]->getFullName().getMiddleName().size())
                    << instances[i]->getId() << [](uint32_t number) -> string {
                    string result = "";
                    for (uint32_t i = 0; i < 13 - number; i++) {
                        result += " ";
                    }
                    return result;
                    }(std::to_string(instances[i]->getId()).size()) << instances[i]->getBalance() << std::endl;
        }
        std::cout << "----------------------------------------------------------------" << std::endl
                  << "Общий баланс " << []() -> float {
            float _sum = 0;
            for (uint64_t i = 0; i < count; i++) {
                _sum += instances[i]->getBalance();
            }
            return _sum;
            }() << std::endl
                << "Количество записей в базе" << ' ' << count << std::endl
                << "----------------------------------------------------------------" << std::endl;
    }

    static void exit() {
        std::ofstream file("../resources/data.txt");
        file << count << "\n";
        for (uint64_t i = 0; i < count; i++) {
            file << instances[i]->getFullName().getLastName() << " " << instances[i]->getFullName().getFirstName() << " " << instances[i]->getFullName().getMiddleName() << " " << instances[i]->getBalance() << "\n";
        }
        file.close();

        if (instances != nullptr) {
            for (uint32_t i = 0; i < count; i++) {
                delete instances[i];
            }
            delete[] instances;
            instances = nullptr;
            count = 0;
        }
    }

    static bank* find(fio& _full_name) {
        if (instances == nullptr || count == 0) {
            return nullptr;
        }
        for (uint32_t i = 0; i < count; i++) {
            if (instances[i]->getFullName().getLastName() == _full_name.getLastName() &&
                instances[i]->getFullName().getFirstName() == _full_name.getFirstName() &&
                instances[i]->getFullName().getMiddleName() == _full_name.getMiddleName()) {
                return instances[i];
            }
        }
        return nullptr;
    }

    bank& operator+(float amount) {
        this->balance += amount;
        return *this;
    }

    friend std::ostream& operator<<(std::ostream& os, const bank& b) {
        os << "ID: " << b.id << " ; Name: " << b.full_name.getLastName() << " " << b.full_name.getFirstName() << " " << b.full_name.getMiddleName() << " ; Balance: " << b.balance << std::endl;
        return os;
    }
};

bank** bank::instances = nullptr;
uint32_t bank::count = 0;

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    bank::initialize();
    bank::execute();

    return 0;
}