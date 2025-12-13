#include "BankSystem.hpp"

#include <windows.h>
#include <iostream>

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    try {
        DepositManager::initialize();
        CreditManager::initialize();
        BankSystem::execute();
    } catch (const std::exception& e) {
        std::cout << "Критическая ошибка: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cout << "Неизвестная критическая ошибка!" << std::endl;
        return 1;
    }

    return 0;
}