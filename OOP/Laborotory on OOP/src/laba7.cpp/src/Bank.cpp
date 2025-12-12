#include "Bank.hpp"

#include <fstream>
#include <windows.h>
#include <algorithm>
#include <numeric>
#include <functional>
#include <cstdint>

std::vector<bank> bank::instances;

bank::bank(const fio& _full_name, float _balance) : full_name(_full_name), balance(_balance) {
    id = static_cast<uint32_t>(instances.size());
}

bank::bank(const fio& _full_name, uint32_t _id, float _balance) : full_name(_full_name), id(_id), balance(_balance) {
}

void bank::initialize() {
    if (instances.empty()) {
        std::ifstream file("res/data.txt");
        if (file.is_open()) {
            uint32_t count;
            file >> count;
            if (count > 0) {
                instances.reserve(count);
                for (uint32_t i = 0; i < count; i++) {
                    string first_name;
                    string last_name;
                    string middle_name;
                    float balance;

                    file >> last_name >> first_name >> middle_name >> balance;

                    std::transform(first_name.begin(), first_name.end(), first_name.begin(),
                                   [](char c) { return c == '_' ? ' ' : c; });
                    std::transform(last_name.begin(), last_name.end(), last_name.begin(),
                                   [](char c) { return c == '_' ? ' ' : c; });
                    std::transform(middle_name.begin(), middle_name.end(), middle_name.begin(),
                                   [](char c) { return c == '_' ? ' ' : c; });

                    instances.push_back(bank(fio(last_name, first_name, middle_name), i, balance));
                }
                file.close();
            }
        } else {
            CreateDirectoryA("res", NULL);
            std::ofstream outfile("res/data.txt");
            if (outfile.is_open()) {
                outfile << 0 << std::endl;
                outfile.close();
            }
        }
    }
}

void bank::execute() {
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
            instances.push_back(bank(fio(last_name, first_name, middle_name), balance));
        } break;
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
        } break;
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
        } break;
        case 5:
            bank::exit();
            return;
        default:
            std::cout << "Неверный выбор. Пожалуйста, попробуйте снова." << std::endl;
        }
    }
}

void bank::print() {
    std::cout << "----------------------------------------------------------------" << std::endl;
    if (instances.empty()) {
        std::cout << "Записи не найдены" << std::endl
                  << "----------------------------------------------------------------" << std::endl;
        return;
    }
    std::cout << "Полное                         Номер счёта  Депозит             " << std::endl
              << "Имя                            (число)      (руб)               " << std::endl
              << "----------------------------------------------------------------" << std::endl;
    
    std::for_each(instances.begin(), instances.end(), [](const bank& b) -> void {
        auto name_length = b.getFullName().getLastName().size() + 
                          b.getFullName().getFirstName().size() + 
                          b.getFullName().getMiddleName().size();
        string spaces1(30 - name_length - 1, ' ');
        string spaces2(13 - std::to_string(b.getId()).size(), ' ');
        
        std::cout << b.getFullName().getLastName() << ' '
                  << b.getFullName().getFirstName() << ' '
                  << b.getFullName().getMiddleName() << spaces1
                  << b.getId() << spaces2 << b.getBalance()
                  << std::endl;
    });
    
    float total_balance = std::accumulate(instances.begin(), instances.end(), 0.0f,
                                         [](float sum, const bank& b) -> float {
                                             return sum + b.getBalance();
                                         });
    
    std::cout << "----------------------------------------------------------------" << std::endl
              << "Общий баланс " << total_balance << std::endl
              << "Количество записей в базе " << instances.size() << std::endl
              << "----------------------------------------------------------------" << std::endl;
}

void bank::exit() {
    std::ofstream file("res/data.txt");
    file << instances.size() << "\n";
    
    std::for_each(instances.begin(), instances.end(), [&file](const bank& b) -> void {
        file << b.getFullName().getLastName() << " " 
             << b.getFullName().getFirstName() << " " 
             << b.getFullName().getMiddleName() << " " 
             << b.getBalance() << "\n";
    });
    
    file.close();
    instances.clear();
}

bank* bank::find(const fio& _full_name) {
    auto it = std::find_if(instances.begin(), instances.end(), 
                          [&_full_name](const bank& b) -> bool {
                              return b.getFullName() == _full_name;
                          });
    
    if (it != instances.end()) {
        return &(*it);
    }
    return nullptr;
}
