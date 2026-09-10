#ifndef MATRIX2HPP
#define MATRIX2HPP

class Matrix2 : public Imatrix {
    uint32_t  _size_x;
    uint32_t  _size_y;
    Element** _mat;

public:
    Matrix2(uint32_t size_x, uint32_t size_y) : _size_x(size_x), _size_y(size_y) {
        this->_mat = new Element*[_size_x];

        for (uint32_t i = 0; i < _size_x; i++) { 
            _mat[i] = new Element[_size_y];
            for (uint32_t j = 0; j < _size_y; j++) {
                _mat[i][j].symbol = '\0';
                _mat[i][j].condition = 0;
    }   }   }

    template<typename... Args> requires (std::convertible_to<Args, char> && ...) && (sizeof...(Args) > 2)
    Matrix2(uint32_t size_x, uint32_t size_y, const Args&... args) : _size_x(size_x), _size_y(size_y) {
        assert(_size_x * _size_y == sizeof...(args));

        this->_mat = new Element*[_size_x];

        char mat[sizeof...(args)]; 
        /* unpacking */ {
            uint32_t i = 0; 
        
            ((mat[i++] = args), ...);
        } /* unpacking */

        for (uint32_t i = 0; i < _size_x; i++) { 
            _mat[i] = new Element[_size_y];
            for (uint32_t j = 0; j < _size_y; j++) {
                _mat[i][j].symbol = mat[i * _size_x + j];
                _mat[i][j].condition = 0;
        }   }
    }

   ~Matrix2() {
        for (uint32_t i = 0; i < _size_x; i++) { 
            delete[] _mat[i];
        }

        delete[] _mat;
    }

    void print(char(*print_func)(Element)) const noexcept override {
        for (uint32_t i = 0; i < _size_x; i++) {
            for (uint32_t j = 0; j < _size_y; j++) {
                if (j != _size_y - 1) {
                    std::cout << print_func(_mat[i][j]) << ' ';
                } else {
                    std::cout << print_func(_mat[i][j]) << '\n';
    }   }   }   }

    uint8_t get_dimensions() const noexcept override {
        return 2;
    }

    using Imatrix::get;

    Element get(Pos2 pos) const noexcept {
        if (pos.x >= _size_x || pos.y >= _size_y) return Element { .symbol = '\0', .condition = Conditions::OUT_OF_BOUNDS_BIT };

        return this->get(pos.x, pos.y);
    };

    using Imatrix::set;

    void set(Element elem, Pos2 pos) {
        this->set(elem, pos.x, pos.y);
    }

    Element get_element(std::span<const uint32_t> coords) const override {
        return _mat[coords[1]][coords[0]];
    }

    void set_element(Element elem, std::span<const uint32_t> coords) override {
        _mat[coords[1]][coords[0]] = elem;
    }

    Pos2 get_size() const noexcept {
        return Pos2 { _size_x, _size_y };
    }

    Matrix2(const Matrix2& other) : _size_x(other._size_x), 
                                    _size_y(other._size_y) {
        this->_mat = new Element*[_size_x];

        for (uint32_t i = 0; i < _size_x; i++) { 
            _mat[i] = new Element[_size_y];
            for (uint32_t j = 0; j < _size_y; j++) {
                _mat[i][j] = other._mat[i][j];
    }   }   }

    Matrix2& operator=(const Matrix2& other) {
        if (this == &other) return *this;

        for (uint32_t i = 0; i < _size_x; i++) { 
            delete[] _mat[i];
        }
        delete[] _mat;

        this->_size_x = other._size_x;
        this->_size_y = other._size_y;
        this->_mat = new Element*[_size_x];

        for (uint32_t i = 0; i < _size_x; i++) { 
            _mat[i] = new Element[_size_y];
            for (uint32_t j = 0; j < _size_y; j++) {
                _mat[i][j] = other._mat[i][j];
        }   }   
        return *this;   
    }
};

#endif // MATRIX2HPP