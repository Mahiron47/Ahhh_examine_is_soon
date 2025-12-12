#ifndef BANK_HPP
#define BANK_HPP
#include "type/fio.hpp"

#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <cstdint>

class bank {
    static std::vector<bank> instances;
    fio full_name;
    uint32_t id;
    float balance;

    bank() : full_name(), id(0), balance(0.0f) {}
public:
    fio getFullName() const { return full_name; }
    uint32_t getId() const { return id; }
    float getBalance() const { return balance; }

    bank(const fio& _full_name, float _balance);

    bank(const fio& _full_name, uint32_t _id, float _balance);

    void printInfo() const {
        std::cout << *this;
    }

    static void initialize();

    static void execute();

    static void print();

    static void exit();

    static bank* find(const fio& _full_name);

    bank& operator+(float amount) {
        this->balance += amount;
        return *this;
    }

    friend std::ostream& operator<<(std::ostream& os, const bank& b) {
        os << "ID: " << b.id << " ; Name: " << b.full_name.getLastName() << " " << b.full_name.getFirstName() << " " << b.full_name.getMiddleName() << " ; Balance: " << b.balance << std::endl;
        return os;
    }
};

#endif // BANK_HPP