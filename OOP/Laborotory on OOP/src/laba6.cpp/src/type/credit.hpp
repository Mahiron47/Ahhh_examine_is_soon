#ifndef CREDIT_HPP
#define CREDIT_HPP
#include <iostream>
#include <iomanip>
#include <limits>
#include <stdexcept>
#include <cmath>
#include "common.hpp"

class credit : public common {
    uint64_t card_number;
    float credit_amount;
    float interest_rate;
    uint32_t term_months;    
    float monthly_payment; 

    void calculateMonthlyPayment() {
        if (term_months == 0 || credit_amount <= 0) {
            monthly_payment = 0.0f;
            return;
        }
        float monthly_rate = interest_rate / 100.0f / 12.0f;
        if (monthly_rate == 0) {
            monthly_payment = credit_amount / term_months;
        } else {
            monthly_payment = credit_amount * monthly_rate * 
                std::pow(1 + monthly_rate, term_months) / 
                (std::pow(1 + monthly_rate, term_months) - 1);
        }
    }

public:
    credit() : common(), card_number(0), credit_amount(0.0f), interest_rate(0.0f), 
               term_months(0), monthly_payment(0.0f) {}
    
    credit(const fio& _client, uint64_t _card_number, float _credit_amount, 
           float _interest_rate, uint32_t _term_months)
        : common(_client, next_id - 1), card_number(_card_number), 
          credit_amount(_credit_amount), interest_rate(_interest_rate), 
          term_months(_term_months) {
        if (_credit_amount < 0) throw std::invalid_argument("Сумма кредита не может быть отрицательной");
        if (_interest_rate < 0) throw std::invalid_argument("Процент не может быть отрицательным");
        if (_term_months == 0) throw std::invalid_argument("Срок кредита должен быть больше нуля");
        calculateMonthlyPayment();
    }
    
    credit(const fio& _client, uint32_t _id, uint64_t _card_number, 
           float _credit_amount, float _interest_rate, uint32_t _term_months)
        : common(_client, _id), card_number(_card_number), 
          credit_amount(_credit_amount), interest_rate(_interest_rate),
          term_months(_term_months) {
        if (_credit_amount < 0) throw std::invalid_argument("Сумма кредита не может быть отрицательной");
        if (_interest_rate < 0) throw std::invalid_argument("Процент не может быть отрицательным");
        if (_term_months == 0) throw std::invalid_argument("Срок кредита должен быть больше нуля");
        calculateMonthlyPayment();
    }

    uint64_t getCardNumber() const { return card_number; }
    float getCreditAmount() const { return credit_amount; }
    float getInterestRate() const { return interest_rate; }
    uint32_t getTermMonths() const { return term_months; }
    float getMonthlyPayment() const { return monthly_payment; }
    
    virtual float getAmount() const override { return credit_amount; }

    virtual void print(std::ostream& os) const override {
          os << "ID : " << id << " | Клиент : " << client.getLastName() << " " 
           << client.getFirstName() << " " << client.getMiddleName() 
              << " | Карта : " << card_number 
              << " | Кредит : " << std::fixed << std::setprecision(2) << credit_amount << " руб."
              << " | Процент : " << interest_rate << "%"
              << " | Срок : " << term_months << " мес."
              << " | Платёж : " << monthly_payment << " руб./мес.";
    }

    virtual credit& operator+(float amount) override {
        if (amount < 0) throw std::invalid_argument("Сумма увеличения должна быть положительной");
        credit_amount += amount;
        calculateMonthlyPayment();
        return *this;
    }

    credit& operator=(const credit& other) {
        if (this != &other) {
            client = other.client;
            id = other.id;
            card_number = other.card_number;
            credit_amount = other.credit_amount;
            interest_rate = other.interest_rate;
            term_months = other.term_months;
            monthly_payment = other.monthly_payment;
        }
        return *this;
    }

    void makePayment(float amount) {
        if (amount < 0) throw std::invalid_argument("Сумма платежа должна быть положительной");
        if (amount < monthly_payment * 0.9f) 
            throw std::runtime_error("Сумма платежа меньше минимального ежемесячного платежа");
        if (amount > credit_amount) 
            throw std::runtime_error("Сумма платежа превышает остаток по кредиту");
        credit_amount -= amount;
        if (credit_amount > 0) calculateMonthlyPayment();
        else monthly_payment = 0.0f;
    }

    friend std::istream& operator>>(std::istream& is, credit& c) {
        string last_name, first_name, middle_name;
        uint64_t card_num;
        float credit_amt, rate;
        uint32_t term;
        
        std::cout << "Фамилия: ";
        is >> last_name;
        if (is.fail()) throw std::runtime_error("Ошибка ввода фамилии");
        
        std::cout << "Имя: ";
        is >> first_name;
        if (is.fail()) throw std::runtime_error("Ошибка ввода имени");
        
        std::cout << "Отчество: ";
        is >> middle_name;
        if (is.fail()) throw std::runtime_error("Ошибка ввода отчества");
        
        std::cout << "Номер карты: ";
        is >> card_num;
        if (is.fail()) {
            is.clear();
            is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            throw std::runtime_error("Ошибка ввода номера карты: необходимо целое число");
        }
        
        std::cout << "Сумма кредита: ";
        is >> credit_amt;
        if (is.fail()) {
            is.clear();
            is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            throw std::runtime_error("Ошибка ввода суммы кредита: необходимо число");
        }
        if (credit_amt < 0) throw std::invalid_argument("Сумма кредита не может быть отрицательной");
        
        std::cout << "Процент: ";
        is >> rate;
        if (is.fail()) {
            is.clear();
            is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            throw std::runtime_error("Ошибка ввода процента: необходимо число");
        }
        if (rate < 0) throw std::invalid_argument("Процент не может быть отрицательным");
        
        std::cout << "Срок (месяцев): ";
        is >> term;
        if (is.fail()) {
            is.clear();
            is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            throw std::runtime_error("Ошибка ввода срока: необходимо целое число");
        }
        if (term == 0) throw std::invalid_argument("Срок кредита должен быть больше нуля");
        
        c.client = fio(last_name, first_name, middle_name);
        c.card_number = card_num;
        c.credit_amount = credit_amt;
        c.interest_rate = rate;
        c.term_months = term;
        c.calculateMonthlyPayment();
        return is;
    }
};

#endif // CREDIT_HPP