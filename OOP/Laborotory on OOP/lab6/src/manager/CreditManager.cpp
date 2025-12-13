#include "CreditManager.hpp"

#include <fstream>
#include <windows.h>
#include <algorithm>

void CreditManager::print() {
    std::cout << "=========================== КРЕДИТЫ ============================" << std::endl;
    if (count == 0) {
        std::cout << "Нет записей о кредитах." << std::endl;
        return;
    }
    for (uint32_t i = 0; i < count; i++) {
        std::cout << *items[i] << std::endl;
    }
    std::cout << "----------------------------------------------------------------" << std::endl;
    std::cout << "Всего кредитов : " << count << std::endl;
    std::cout << "================================================================\n" << std::endl;
}

void CreditManager::initialize() {
    std::ifstream file("res/credits.txt");
    if (file.is_open()) {
        uint32_t file_count;
        file >> file_count;
        for (uint32_t i = 0; i < file_count; i++) {
            string last_name, first_name, middle_name;
            uint64_t card_number;
            float credit_amount, interest_rate;
            uint32_t id, term_months;
            file >> id >> last_name >> first_name >> middle_name >> card_number >> credit_amount >> interest_rate >> term_months;

            std::replace(last_name.begin(), last_name.end(), '_', ' ');
            std::replace(first_name.begin(), first_name.end(), '_', ' ');
            std::replace(middle_name.begin(), middle_name.end(), '_', ' ');

            credit** new_credits = new credit*[count + 1];
            for (uint32_t j = 0; j < count; j++) {
                new_credits[j] = items[j];
            }
            new_credits[count] = new credit(fio(last_name, first_name, middle_name),
                                            id, card_number, credit_amount, interest_rate, term_months);
            if (items != nullptr)
                delete[] items;
            items = new_credits;
            count++;
        }
        file.close();
    } else {
        CreateDirectoryA("res", NULL);
        std::ofstream outfile("res/credits.txt");
        if (outfile.is_open()) {
            outfile << 0 << std::endl;
            outfile.close();
        }
    }
}

void CreditManager::save() {
    std::ofstream file("res/credits.txt");
    file << count << "\n";
    for (uint32_t i = 0; i < count; i++) {
        string ln = items[i]->getClient().getLastName();
        string fn = items[i]->getClient().getFirstName();
        string mn = items[i]->getClient().getMiddleName();
        std::replace(ln.begin(), ln.end(), ' ', '_');
        std::replace(fn.begin(), fn.end(), ' ', '_');
        std::replace(mn.begin(), mn.end(), ' ', '_');
        file << items[i]->getId() << " " << ln << " " << fn << " " << mn << " "
             << items[i]->getCardNumber() << " " << items[i]->getCreditAmount()
             << " " << items[i]->getInterestRate() << " " << items[i]->getTermMonths() << "\n";
    }
    file.close();
}
