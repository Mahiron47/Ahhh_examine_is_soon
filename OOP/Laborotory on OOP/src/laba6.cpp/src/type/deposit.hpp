#ifndef DEPOSIT_HPP
#define DEPOSIT_HPP
#include <iostream>
#include <iomanip>
#include <limits>
#include <stdexcept>
#include "common.hpp"

class deposit : public common {
    float balance;

public:
    deposit() : common(), balance(0.0f) {}
    
    deposit(const fio& _client, float _balance) 
        : common(_client, next_id++), balance(_balance) {
        if (_balance < 0) throw std::invalid_argument("Баланс не может быть отрицательным");
    }
    
    deposit(const fio& _client, uint32_t _id, float _balance) 
        : common(_client, _id), balance(_balance) {
        if (_balance < 0) throw std::invalid_argument("Баланс не может быть отрицательным");
    }

    float getBalance() const { return balance; }
    
    virtual float getAmount() const override { return balance; }

    virtual void print(std::ostream& os) const override {
          os << "ID : " << id << " | Клиент : " << client.getLastName() << " " 
           << client.getFirstName() << " " << client.getMiddleName() 
              << " | Баланс : " << std::fixed << std::setprecision(2) << balance << " руб.";
    }

    virtual deposit& operator+(float amount) override {
        if (amount < 0) throw std::invalid_argument("Сумма пополнения должна быть положительной");
        balance += amount;
        return *this;
    }

    deposit& operator=(const deposit& other) {
        if (this != &other) {
            client = other.client;
            id = other.id;
            balance = other.balance;
        }
        return *this;
    }

    void withdraw(float amount) {
        if (amount < 0) throw std::invalid_argument("Сумма списания должна быть положительной");
        if (amount > balance) throw std::runtime_error("Недостаточно средств на депозите");
        balance -= amount;
    }

    friend std::istream& operator>>(std::istream& is, deposit& d) {
        string last_name, first_name, middle_name;
        float balance_input;
        
        std::cout << "Фамилия: ";
        is >> last_name;
        if (is.fail()) throw std::runtime_error("Ошибка ввода фамилии");
        
        std::cout << "Имя: ";
        is >> first_name;
        if (is.fail()) throw std::runtime_error("Ошибка ввода имени");
        
        std::cout << "Отчество: ";
        is >> middle_name;
        if (is.fail()) throw std::runtime_error("Ошибка ввода отчества");
        
        std::cout << "Баланс: ";
        is >> balance_input;
        if (is.fail()) {
            is.clear();
            is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            throw std::runtime_error("Ошибка ввода баланса: необходимо число");
        }
        if (balance_input < 0) throw std::invalid_argument("Баланс не может быть отрицательным");
        
        d.client = fio(last_name, first_name, middle_name);
        d.balance = balance_input;
        return is;
    }
};

#endif // DEPOSIT_HPP