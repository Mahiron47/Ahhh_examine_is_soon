#ifndef MATRIX2HPP
#define MATRIX2HPP

struct Pos2;

class Matrix2 : public Imatrix {
    uint32_t  _size;
    Element** _mat;

public:
    template<typename... Args> requires (std::convertible_to<Args, char> && ...) Matrix2(const Args&... args) : _size(std::sqrtl(sizeof...(args))) {
        static_assert(_size * _size == sizeof...(args));
        
        this->_mat = new Element*[_size];

        char mat[sizeof...(args)]; 
        /* unpacking */ {
            uint32_t i = 0; 
        
            ((mat[i++] = args), ...);
        } /* unpacking */

        for (uint32_t i = 0; i < _size; i++) { 
            _mat[i] = new Element[_size]
            for (uint32_t j = 0; j < _size; j++) {
                _mat[i][j].symbol = mat[i * _size + j];
                _mat[i][j].condition = UNCHECKED;
        }   }
    }

   ~Matrix2() noexcept {
        for (uint32_t i = 0; i < _size; i++) { 
            delete[] _mat[i];
        }

        delete[] _mat;
    }

    void print() const noexcept override {
        for (uint32_t i = 0; i < _size; i++) {
            for (uint32_t j = 0; j < _size; j++) {
                if (j != _size) {
                    std::cout << _mat[i][j].symbol << ' ';
                } else {
                    std::cout << _mat[i][j].symbol << '\n';
    }   }   }   }

    uint8_t get_dimensions() const noexcept override {
        return 2;
    }

    using Imatrix::get;

    Element get(Pos2 pos) const noexcept {
        if (0 < pos.x && pos.x >= _size && 0 < pos.y && pos.y >= _size) 
            return Element { .symbol = '\0', .condition = 0b1111'1111 };

        return this->get(pos.x, pos.y);
    };

    using Imatrix::set;

    void set(Element elem, Pos2 pos) {
        this->set(elem, pos.x, pos.y);
    }

    Element get_element(std::span<const uint32_t> coords) const override {
        return _mat[coords[0]][coords[1]];
    }

    void set_element(Element elem, std::span<const uint32_t> coords) override {
        _mat[coords[0]][coords[1]] = elem;
    }

};

#endif // MATRIX2HPP