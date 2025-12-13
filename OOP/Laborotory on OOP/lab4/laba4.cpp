#include <windows.h>
#include <cstdint>
#include <iostream>
#include <locale>
#include <fstream>
#include <cstring>
#include <cmath>
#include <string>
#include <algorithm>
#include <iomanip>

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

    fio(string _last_name, string _first_name, string _middle_name) 
        : last_name(_last_name), first_name(_first_name), middle_name(_middle_name) {}

    fio(const fio& other) 
        : last_name(other.last_name), first_name(other.first_name), middle_name(other.middle_name) {}

    bool operator==(const fio& other) const {
        return last_name == other.last_name && 
               first_name == other.first_name && 
               middle_name == other.middle_name;
    }
};

class common {
protected:
    fio client;
    uint32_t id;
    static uint32_t next_id;

public:
    common() : client(), id(next_id++) {}
    
    common(const fio& _client, uint32_t _id) : client(_client), id(_id) {
        if (_id >= next_id) next_id = _id + 1;
    }

    virtual ~common() {}

    fio getClient() const { return client; }
    uint32_t getId() const { return id; }

    virtual void print(std::ostream& os) const = 0;
    virtual common& operator+(float amount) = 0;
    virtual float getAmount() const = 0;
    
    friend std::ostream& operator<<(std::ostream& os, const common& obj) {
        obj.print(os);
        return os;
    }
};

uint32_t common::next_id = 0;

class deposit : public common {
    float balance;

public:
    deposit() : common(), balance(0.0f) {}
    
    deposit(const fio& _client, float _balance) 
        : common(_client, next_id++), balance(_balance) {}
    
    deposit(const fio& _client, uint32_t _id, float _balance) 
        : common(_client, _id), balance(_balance) {}

    float getBalance() const { return balance; }
    
    virtual float getAmount() const override { return balance; }

    virtual void print(std::ostream& os) const override {
          os << "ID : " << id << " | Клиент : " << client.getLastName() << " " 
           << client.getFirstName() << " " << client.getMiddleName() 
              << " | Депозит : " << std::fixed << std::setprecision(2) << balance << " руб.";
    }

    virtual deposit& operator+(float amount) override {
        balance += amount;
        return *this;
    }
};

class credit : public common {
    uint64_t card_number;
    float credit_amount;
    float interest_rate;

public:
    credit() : common(), card_number(0), credit_amount(0.0f), interest_rate(0.0f) {}
    
    credit(const fio& _client, uint64_t _card_number, float _credit_amount, float _interest_rate)
        : common(_client, next_id - 1), card_number(_card_number), 
          credit_amount(_credit_amount), interest_rate(_interest_rate) {}
    
    credit(const fio& _client, uint32_t _id, uint64_t _card_number, 
           float _credit_amount, float _interest_rate)
        : common(_client, _id), card_number(_card_number), 
          credit_amount(_credit_amount), interest_rate(_interest_rate) {}

    uint64_t getCardNumber() const { return card_number; }
    float getCreditAmount() const { return credit_amount; }
    float getInterestRate() const { return interest_rate; }
    
    virtual float getAmount() const override { return credit_amount; }

    virtual void print(std::ostream& os) const override {
          os << "ID : " << id << " | Клиент : " << client.getLastName() << " " 
           << client.getFirstName() << " " << client.getMiddleName() 
              << " | Карта : " << card_number 
              << " | Кредит : " << std::fixed << std::setprecision(2) << credit_amount << " руб."
              << " | Процент : " << interest_rate << "%";
    }

    virtual credit& operator+(float amount) override {
        credit_amount += amount;
        return *this;
    }
};

class DepositManager {
    static deposit** deposits;
    static uint32_t count;

public:
    static void add(const fio& client, float balance) {
        deposit** new_deposits = new deposit*[count + 1];
        for (uint32_t i = 0; i < count; i++) {
            new_deposits[i] = deposits[i];
        }
        new_deposits[count] = new deposit(client, balance);
        if (deposits != nullptr) delete[] deposits;
        deposits = new_deposits;
        count++;
    }

    static void print() {
        std::cout << "=========================== ДЕПОЗИТЫ ===========================" << std::endl;
        if (count == 0) {
            std::cout << "Нет записей о депозитах." << std::endl;
            return;
        }
        for (uint32_t i = 0; i < count; i++) {
            std::cout << *deposits[i] << std::endl;
        }
		std::cout << "----------------------------------------------------------------" << std::endl;
        std::cout << "Всего депозитов : " << count << std::endl;
        std::cout << "================================================================\n" << std::endl;
    }

    static deposit* find(const fio& client) {
        for (uint32_t i = 0; i < count; i++) {
            if (deposits[i]->getClient() == client) {
                return deposits[i];
            }
        }
        return nullptr;
    }

    static void initialize() {
        std::ifstream file("../resources/deposits.txt");
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
                    new_deposits[j] = deposits[j];
                }
                new_deposits[count] = new deposit(fio(last_name, first_name, middle_name), id, balance);
                if (deposits != nullptr) delete[] deposits;
                deposits = new_deposits;
                count++;
            }
            file.close();
        } else {
            CreateDirectoryA("../resources", NULL);
            std::ofstream outfile("../resources/deposits.txt");
            if (outfile.is_open()) {
                outfile << 0 << std::endl;
                outfile.close();
            }
        }
    }

    static void save() {
        std::ofstream file("../resources/deposits.txt");
        file << count << "\n";
        for (uint32_t i = 0; i < count; i++) {
            string ln = deposits[i]->getClient().getLastName();
            string fn = deposits[i]->getClient().getFirstName();
            string mn = deposits[i]->getClient().getMiddleName();
            std::replace(ln.begin(), ln.end(), ' ', '_');
            std::replace(fn.begin(), fn.end(), ' ', '_');
            std::replace(mn.begin(), mn.end(), ' ', '_');
            file << deposits[i]->getId() << " " << ln << " " << fn << " " << mn 
                 << " " << deposits[i]->getBalance() << "\n";
        }
        file.close();
    }

    static void cleanup() {
        if (deposits != nullptr) {
            for (uint32_t i = 0; i < count; i++) {
                delete deposits[i];
            }
            delete[] deposits;
            deposits = nullptr;
            count = 0;
        }
    }

    static uint32_t getCount() { return count; }
};

deposit** DepositManager::deposits = nullptr;
uint32_t DepositManager::count = 0;

class CreditManager {
    static credit** credits;
    static uint32_t count;

public:
    static void add(const fio& client, uint64_t card_number, float credit_amount, float interest_rate) {
        credit** new_credits = new credit*[count + 1];
        for (uint32_t i = 0; i < count; i++) {
            new_credits[i] = credits[i];
        }
        new_credits[count] = new credit(client, card_number, credit_amount, interest_rate);
        if (credits != nullptr) delete[] credits;
        credits = new_credits;
        count++;
    }

    static void print() {
        std::cout << "=========================== КРЕДИТЫ ============================" << std::endl;
        if (count == 0) {
            std::cout << "Нет записей о кредитах." << std::endl;
            return;
        }
        for (uint32_t i = 0; i < count; i++) {
            std::cout << *credits[i] << std::endl;
        }
        std::cout << "----------------------------------------------------------------" << std::endl;
        std::cout << "Всего кредитов : " << count << std::endl;
        std::cout << "================================================================\n" << std::endl;
    }

    static credit* find(const fio& client) {
        for (uint32_t i = 0; i < count; i++) {
            if (credits[i]->getClient() == client) {
                return credits[i];
            }
        }
        return nullptr;
    }

    static void initialize() {
        std::ifstream file("../resources/credits.txt");
        if (file.is_open()) {
            uint32_t file_count;
            file >> file_count;
            for (uint32_t i = 0; i < file_count; i++) {
                string last_name, first_name, middle_name;
                uint64_t card_number;
                float credit_amount, interest_rate;
                uint32_t id;
                file >> id >> last_name >> first_name >> middle_name 
                     >> card_number >> credit_amount >> interest_rate;
                
                std::replace(last_name.begin(), last_name.end(), '_', ' ');
                std::replace(first_name.begin(), first_name.end(), '_', ' ');
                std::replace(middle_name.begin(), middle_name.end(), '_', ' ');

                credit** new_credits = new credit*[count + 1];
                for (uint32_t j = 0; j < count; j++) {
                    new_credits[j] = credits[j];
                }
                new_credits[count] = new credit(fio(last_name, first_name, middle_name), 
                                                 id, card_number, credit_amount, interest_rate);
                if (credits != nullptr) delete[] credits;
                credits = new_credits;
                count++;
            }
            file.close();
        } else {
            CreateDirectoryA("../resources", NULL);
            std::ofstream outfile("../resources/credits.txt");
            if (outfile.is_open()) {
                outfile << 0 << std::endl;
                outfile.close();
            }
        }
    }

    static void save() {
        std::ofstream file("../resources/credits.txt");
        file << count << "\n";
        for (uint32_t i = 0; i < count; i++) {
            string ln = credits[i]->getClient().getLastName();
            string fn = credits[i]->getClient().getFirstName();
            string mn = credits[i]->getClient().getMiddleName();
            std::replace(ln.begin(), ln.end(), ' ', '_');
            std::replace(fn.begin(), fn.end(), ' ', '_');
            std::replace(mn.begin(), mn.end(), ' ', '_');
            file << credits[i]->getId() << " " << ln << " " << fn << " " << mn << " "
                 << credits[i]->getCardNumber() << " " << credits[i]->getCreditAmount() 
                 << " " << credits[i]->getInterestRate() << "\n";
        }
        file.close();
    }

    static void cleanup() {
        if (credits != nullptr) {
            for (uint32_t i = 0; i < count; i++) {
                delete credits[i];
            }
            delete[] credits;
            credits = nullptr;
            count = 0;
        }
    }

    static uint32_t getCount() { return count; }
};

credit** CreditManager::credits = nullptr;
uint32_t CreditManager::count = 0;

class BankSystem {
public:
    static void show_menu() {
        std::cout << "================================================================" << std::endl;
        std::cout << "                    БАНКОВСКАЯ СИСТЕМА"                             << std::endl;
        std::cout << "================================================================"   << std::endl;
        std::cout << "1.  Добавить депозит"                                               << std::endl;
        std::cout << "2.  Добавить кредит"                                                << std::endl;
        std::cout << "3.  Показать все депозиты"                                          << std::endl;
        std::cout << "4.  Показать все кредиты"                                           << std::endl;
        std::cout << "5.  Показать всё"                                                   << std::endl;
        std::cout << "6.  Найти по ФИО (депозиты)"                                        << std::endl;
        std::cout << "7.  Найти по ФИО (кредиты)"                                         << std::endl;
        std::cout << "8.  Найти по ФИО (везде)"                                           << std::endl;
        std::cout << "9.  Увеличить депозит"                                              << std::endl;
        std::cout << "10. Увеличить кредит"                                               << std::endl;
        std::cout << "0.  Выход и сохранение"                                             << std::endl;
        std::cout << "================================================================"   << std::endl;
        std::cout << "Выберите действие : ";
    }

    static fio input_fio() {
        string last_name, first_name, middle_name;
        std::cout << "Фамилия : ";
        std::cin >> last_name;
        std::cout << "Имя : ";
        std::cin >> first_name;
        std::cout << "Отчество : ";
        std::cin >> middle_name;
        return fio(last_name, first_name, middle_name);
    }

    static void execute() {
        while (true) {
            show_menu();
            int choice;
            std::cin >> choice;

            switch (choice) {
            case 1: {
                fio client = input_fio();
                float balance;
                std::cout << "Сумма депозита : ";
                std::cin >> balance;
                DepositManager::add(client, balance);
                std::cout << "Депозит добавлен." << std::endl;
                break;
            }
            case 2: {
                fio client = input_fio();
                uint64_t card_number;
                float credit_amount, interest_rate;
                std::cout << "Номер кредитной карты : ";
                std::cin >> card_number;
                std::cout << "Сумма кредита : ";
                std::cin >> credit_amount;
                std::cout << "Процент по кредиту : ";
                std::cin >> interest_rate;
                CreditManager::add(client, card_number, credit_amount, interest_rate);
                std::cout << "Кредит добавлен." << std::endl;
                break;
            }
            case 3:
                DepositManager::print();
                break;
            case 4:
                CreditManager::print();
                break;
            case 5:
                DepositManager::print();
                CreditManager::print();
                break;
            case 6: {
                fio client = input_fio();
                deposit* d = DepositManager::find(client);
                if (d != nullptr) {
                    std::cout << "\nЗапись найдена :\n" << *d << std::endl;
                } else {
                    std::cout << "Депозит не найден." << std::endl;
                }
                break;
            }
            case 7: {
                fio client = input_fio();
                credit* c = CreditManager::find(client);
                if (c != nullptr) {
                    std::cout << "\nЗапись найдена :\n" << *c << std::endl;
                } else {
                    std::cout << "Кредит не найден." << std::endl;
                }
                break;
            }
            case 8: {
                fio client = input_fio();
                deposit* d = DepositManager::find(client);
                credit* c = CreditManager::find(client);
                bool found = false;
                if (d != nullptr) {
                    std::cout << "\nДепозит найден :\n" << *d << std::endl;
                    found = true;
                }
                if (c != nullptr) {
                    std::cout << "\nКредит найден :\n" << *c << std::endl;
                    found = true;
                }
                if (!found) {
                    std::cout << "Записи не найдены." << std::endl;
                }
                break;
            }
            case 9: {
                fio client = input_fio();
                deposit* d = DepositManager::find(client);
                if (d != nullptr) {
                    float amount;
                    std::cout << "Сумма пополнения : ";
                    std::cin >> amount;
                    *d + amount;
                    std::cout << "Депозит обновлён :\n" << *d << std::endl;
                } else {
                    std::cout << "Депозит не найден." << std::endl;
                }
                break;
            }
            case 10: {
                fio client = input_fio();
                credit* c = CreditManager::find(client);
                if (c != nullptr) {
                    float amount;
                    std::cout << "Сумма увеличения кредита : ";
                    std::cin >> amount;
                    *c + amount;
                    std::cout << "Кредит обновлён :\n" << *c << std::endl;
                } else {
                    std::cout << "Кредит не найден." << std::endl;
                }
                break;
            }
            case 0:
                DepositManager::save();
                CreditManager::save();
                DepositManager::cleanup();
                CreditManager::cleanup();
                return;
            default:
                std::cout << "Неверный выбор." << std::endl;
            }
        }
    }
};

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    DepositManager::initialize();
    CreditManager::initialize();
    BankSystem::execute();

    return 0;
}