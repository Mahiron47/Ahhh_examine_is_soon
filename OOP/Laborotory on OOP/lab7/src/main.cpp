#include "Bank.hpp"

#include <windows.h>

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    bank::initialize();
    bank::execute();

    return 0;
}