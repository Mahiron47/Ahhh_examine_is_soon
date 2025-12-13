#include "DepositManager.hpp"

#include <fstream>
#include <windows.h>
#include <algorithm>

void DepositManager::print() {
        std::cout << "=========================== ДЕПОЗИТЫ ===========================" << std::endl;
        if (count == 0) {
            std::cout << "Нет записей о депозитах." << std::endl;
            return;
        }
        for (uint32_t i = 0; i < count; i++) {
            std::cout << *items[i] << std::endl;
        }
		std::cout << "----------------------------------------------------------------" << std::endl;
        std::cout << "Всего депозитов : " << count << std::endl;
        std::cout << "================================================================\n" << std::endl;
    }

void DepositManager::initialize() {
    std::ifstream file("res/deposits.txt");
    if (file.is_open()) {
        uint32_t file_count;
        file >> file_count;
        for (uint32_t i = 0; i < file_count; i++) {
            string last_name, first_name, middle_name;
            float balance;
            uint32_t id;
            file >> id >> last_name >> first_name >> middle_name >> balance;

            std::replace(last_name.begin(), last_name.end(), '_', ' ');
            std::replace(first_name.begin(), first_name.end(), '_', ' ');
            std::replace(middle_name.begin(), middle_name.end(), '_', ' ');

            deposit** new_deposits = new deposit*[count + 1];
            for (uint32_t j = 0; j < count; j++) {
                new_deposits[j] = items[j];
            }
            new_deposits[count] = new deposit(fio(last_name, first_name, middle_name), id, balance);
            if (items != nullptr)
                delete[] items;
            items = new_deposits;
            count++;
        }
        file.close();
    } else {
        CreateDirectoryA("res", NULL);
        std::ofstream outfile("res/deposits.txt");
        if (outfile.is_open()) {
            outfile << 0 << std::endl;
            outfile.close();
        }
    }
}

void DepositManager::save() {
    std::ofstream file("res/deposits.txt");
    file << count << "\n";
    for (uint32_t i = 0; i < count; i++) {
        string ln = items[i]->getClient().getLastName();
        string fn = items[i]->getClient().getFirstName();
        string mn = items[i]->getClient().getMiddleName();
        std::replace(ln.begin(), ln.end(), ' ', '_');
        std::replace(fn.begin(), fn.end(), ' ', '_');
        std::replace(mn.begin(), mn.end(), ' ', '_');
        file << items[i]->getId() << " " << ln << " " << fn << " " << mn
             << " " << items[i]->getBalance() << "\n";
    }
    file.close();
}
