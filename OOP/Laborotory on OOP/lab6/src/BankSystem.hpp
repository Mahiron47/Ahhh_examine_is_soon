#ifndef BANKSYSTEM_HPP
#define BANKSYSTEM_HPP
#include "manager/DepositManager.hpp"
#include "manager/CreditManager.hpp"

class BankSystem {
public:
    static void show_menu();

    static fio input_fio();

    static void execute();
};

#endif // BANKSYSTEM_HPP