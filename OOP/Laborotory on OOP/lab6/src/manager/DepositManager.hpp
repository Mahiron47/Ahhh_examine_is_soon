#ifndef DEPOSITMANAGER_HPP
#define DEPOSITMANAGER_HPP
#include "../type/deposit.hpp"
#include "Manager.hpp"

class DepositManager : public Manager<deposit> {
public:
    static void print();

    static void initialize();

    static void save();
};

#endif // DEPOSITMANAGER_HPP
