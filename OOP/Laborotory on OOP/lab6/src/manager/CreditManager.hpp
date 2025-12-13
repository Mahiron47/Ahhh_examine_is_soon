#ifndef CREDITMANAGER_HPP
#define CREDITMANAGER_HPP
#include "../type/credit.hpp"
#include "Manager.hpp"

class CreditManager : public Manager<credit> {
public:
	static void print();

	static void initialize();

	static void save();
};

#endif // CREDITMANAGER_HPP
