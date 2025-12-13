#include "BankSystem.hpp"
#include <limits>

void BankSystem::show_menu() {
    std::cout << "================================================================" << std::endl;
    std::cout << "                    БАНКОВСКАЯ СИСТЕМА"                           << std::endl;
    std::cout << "================================================================" << std::endl;
    std::cout << "1.  Добавить депозит (ввод >>)"                                  << std::endl;
    std::cout << "2.  Добавить кредит (ввод >>)"                                   << std::endl;
    std::cout << "3.  Показать все депозиты"                                        << std::endl;
    std::cout << "4.  Показать все кредиты"                                         << std::endl;
    std::cout << "5.  Показать всё"                                                 << std::endl;
    std::cout << "6.  Найти по ФИО (депозиты)"                                      << std::endl;
    std::cout << "7.  Найти по ФИО (кредиты)"                                       << std::endl;
    std::cout << "8.  Найти по ФИО (везде)"                                         << std::endl;
    std::cout << "9.  Увеличить депозит"                                            << std::endl;
    std::cout << "10. Увеличить кредит"                                             << std::endl;
    std::cout << "11. Платёж по кредиту"                                            << std::endl;
    std::cout << "12. Списание с депозита"                                          << std::endl;
    std::cout << "0.  Выход и сохранение"                                           << std::endl;
    std::cout << "================================================================" << std::endl;
    std::cout << "Выберите действие : ";
}

fio BankSystem::input_fio() {
    string last_name, first_name, middle_name;
    std::cout << "Фамилия : ";
    std::cin  >> last_name;
    std::cout << "Имя : ";
    std::cin  >> first_name;
    std::cout << "Отчество : ";
    std::cin  >> middle_name;
    return fio(last_name, first_name, middle_name);
}

void BankSystem::execute() {
    while (true) {
        try {
            show_menu();
            int choice;
            std::cin >> choice;
            
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Ошибка ввода! Введите число." << std::endl;
                continue;
            }

            switch (choice) {
            case 1: {
                try {
                    deposit* d = new deposit();
                    std::cin >> *d;
                    DepositManager::add(d);
                    std::cout << "Депозит добавлен успешно." << std::endl;
                } catch (const std::exception& e) {
                    std::cout << "Ошибка: " << e.what() << std::endl;
                }
                break;
            }
            case 2: {
                try {
                    credit* c = new credit();
                    std::cin >> *c;
                    CreditManager::add(c);
                    std::cout << "Кредит добавлен успешно." << std::endl;
                } catch (const std::exception& e) {
                    std::cout << "Ошибка: " << e.what() << std::endl;
                }
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
                uint32_t result_count = 0;
                deposit** results = DepositManager::findAll(client, result_count);
                if (results != nullptr && result_count > 0) {
                    std::cout << "\nНайдено депозитов: " << result_count << std::endl;
                    for (uint32_t i = 0; i < result_count; i++) {
                        std::cout << *results[i] << std::endl;
                        delete results[i];
                    }
                    delete[] results;
                } else {
                    std::cout << "Депозиты не найдены." << std::endl;
                }
                break;
            }
            case 7: {
                fio client = input_fio();
                uint32_t result_count = 0;
                credit** results = CreditManager::findAll(client, result_count);
                if (results != nullptr && result_count > 0) {
                    std::cout << "\nНайдено кредитов: " << result_count << std::endl;
                    for (uint32_t i = 0; i < result_count; i++) {
                        std::cout << *results[i] << std::endl;
                        delete results[i];
                    }
                    delete[] results;
                } else {
                    std::cout << "Кредиты не найдены." << std::endl;
                }
                break;
            }
            case 8: {
                fio client = input_fio();
                uint32_t dep_count = 0, cred_count = 0;
                deposit** dep_results = DepositManager::findAll(client, dep_count);
                credit** cred_results = CreditManager::findAll(client, cred_count);
                
                if (dep_count > 0) {
                    std::cout << "\nДепозиты (найдено " << dep_count << "):" << std::endl;
                    for (uint32_t i = 0; i < dep_count; i++) {
                        std::cout << *dep_results[i] << std::endl;
                        delete dep_results[i];
                    }
                    delete[] dep_results;
                }
                
                if (cred_count > 0) {
                    std::cout << "\nКредиты (найдено " << cred_count << "):" << std::endl;
                    for (uint32_t i = 0; i < cred_count; i++) {
                        std::cout << *cred_results[i] << std::endl;
                        delete cred_results[i];
                    }
                    delete[] cred_results;
                }
                
                if (dep_count == 0 && cred_count == 0) {
                    std::cout << "Записи не найдены." << std::endl;
                }
                break;
            }
            case 9: {
                try {
                    fio client = input_fio();
                    deposit* d = DepositManager::find(client);
                    if (d != nullptr) {
                        float amount;
                        std::cout << "Сумма пополнения : ";
                        std::cin >> amount;
                        if (std::cin.fail()) {
                            std::cin.clear();
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            throw std::runtime_error("Ошибка ввода суммы");
                        }
                        *d + amount;
                        std::cout << "Депозит обновлён :\n" << *d << std::endl;
                    } else {
                        std::cout << "Депозит не найден." << std::endl;
                    }
                } catch (const std::exception& e) {
                    std::cout << "Ошибка: " << e.what() << std::endl;
                }
                break;
            }
            case 10: {
                try {
                    fio client = input_fio();
                    credit* c = CreditManager::find(client);
                    if (c != nullptr) {
                        float amount;
                        std::cout << "Сумма увеличения кредита : ";
                        std::cin >> amount;
                        if (std::cin.fail()) {
                            std::cin.clear();
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            throw std::runtime_error("Ошибка ввода суммы");
                        }
                        *c + amount;
                        std::cout << "Кредит обновлён :\n" << *c << std::endl;
                    } else {
                        std::cout << "Кредит не найден." << std::endl;
                    }
                } catch (const std::exception& e) {
                    std::cout << "Ошибка: " << e.what() << std::endl;
                }
                break;
            }
            case 11: {
                try {
                    fio client = input_fio();
                    credit* c = CreditManager::find(client);
                    if (c != nullptr) {
                        float amount;
                        std::cout << "Минимальный платёж : " << c->getMonthlyPayment() << " руб." << std::endl;
                        std::cout << "Введите сумму платежа : ";
                        std::cin >> amount;
                        if (std::cin.fail()) {
                            std::cin.clear();
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            throw std::runtime_error("Ошибка ввода суммы");
                        }
                        c->makePayment(amount);
                        std::cout << "Платёж выполнен. Остаток кредита:\n" << *c << std::endl;
                    } else {
                        std::cout << "Кредит не найден." << std::endl;
                    }
                } catch (const std::exception& e) {
                    std::cout << "Ошибка: " << e.what() << std::endl;
                }
                break;
            }
            case 12: {
                try {
                    fio client = input_fio();
                    deposit* d = DepositManager::find(client);
                    if (d != nullptr) {
                        float amount;
                        std::cout << "Сумма списания : ";
                        std::cin >> amount;
                        if (std::cin.fail()) {
                            std::cin.clear();
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            throw std::runtime_error("Ошибка ввода суммы");
                        }
                        d->withdraw(amount);
                        std::cout << "Списание выполнено. Остаток депозита:\n" << *d << std::endl;
                    } else {
                        std::cout << "Депозит не найден." << std::endl;
                    }
                } catch (const std::exception& e) {
                    std::cout << "Ошибка: " << e.what() << std::endl;
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
        } catch (const std::exception& e) {
            std::cout << "Критическая ошибка: " << e.what() << std::endl;
        } catch (...) {
            std::cout << "Неизвестная критическая ошибка!" << std::endl;
        }
    }
}
