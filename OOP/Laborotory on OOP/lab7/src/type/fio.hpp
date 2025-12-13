#ifndef FIO_HPP
#define FIO_HPP
#include <string>

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

    fio(string _last_name, string _first_name, string _middle_name) {
        last_name = _last_name;
        first_name = _first_name;
        middle_name = _middle_name;
    }

    fio(const fio& other) {
        last_name = other.last_name;
        first_name = other.first_name;
        middle_name = other.middle_name;
    }

    bool operator==(const fio& other) const {
        return last_name == other.last_name && 
               first_name == other.first_name && 
               middle_name == other.middle_name;
    }

    bool operator!=(const fio& other) const {
        return !(*this == other);
    }
};

#endif // FIO_HPP