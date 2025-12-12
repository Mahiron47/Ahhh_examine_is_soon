#ifndef COMMON_HPP
#define COMMON_HPP
#include <iostream>
#include "fio.hpp"

class common {
protected:
    fio client;
    uint32_t id;
    static uint32_t next_id;

public:
    common() : client(), id(next_id++) {}
    
    common(const fio& _client, uint32_t _id) : client(_client), id(_id) {
        if (_id >= next_id) next_id = _id + 1;
    }

    virtual ~common() {}

    fio getClient() const { return client; }
    uint32_t getId() const { return id; }

    virtual void print(std::ostream& os) const = 0;
    virtual common& operator+(float amount) = 0;
    virtual float getAmount() const = 0;
    
    friend std::ostream& operator<<(std::ostream& os, const common& obj) {
        obj.print(os);
        return os;
    }
};

#endif // COMMON_HPP
