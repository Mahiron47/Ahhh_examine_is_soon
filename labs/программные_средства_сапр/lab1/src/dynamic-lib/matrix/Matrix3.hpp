#ifndef MATRIX3HPP
#define MATRIX3HPP

class Matrix3 : public Imatrix {
    uint32_t   _size_x;
    uint32_t   _size_y;
    uint32_t   _size_z;
    Element*** _mat;

public:
    Matrix3() : _size_x(0), _size_y(0), _size_z(0), _mat(nullptr) {}
    Matrix3(uint32_t size_x, 
            uint32_t size_y, 
            uint32_t size_z) : _size_x(size_x), 
                               _size_y(size_y), 
                               _size_z(size_z) {
        this->_mat = new Element**[_size_z];

        for (uint32_t z = 0; z < _size_z; z++) {
            _mat[z] = new Element*[_size_y];
            for (uint32_t y = 0; y < _size_y; y++) {
                _mat[z][y] = new Element[_size_x];
                for (uint32_t x = 0; x < _size_x; x++) {
                    _mat[z][y][x].symbol = '\0';
                    _mat[z][y][x].condition = 0;
                    _mat[z][y][x].info = 0;
    }   }   }   }

    template<typename... Args> requires (std::convertible_to<Args, char> && ...) && (sizeof...(Args) > 3) 
    Matrix3(uint32_t size_x, 
            uint32_t size_y, 
            uint32_t size_z, 
            const Args&... args) : _size_x(size_x), 
                                   _size_y(size_y), 
                                   _size_z(size_z) {
        assert(size_z * size_x *  size_y == sizeof...(args));
        
        this->_mat = new Element**[_size_z];

        char mat[sizeof...(args)]; 
        /* unpacking */ {
            uint32_t idx = 0; 
            ((mat[idx++] = args), ...);
        } /* unpacking */

        for (uint32_t z = 0; z < _size_z; z++) { 
            _mat[z] = new Element*[_size_y];
            for (uint32_t y = 0; y < _size_y; y++) {
                _mat[z][y] = new Element[_size_x];
                for (uint32_t x = 0; x < _size_x; x++) {
                    _mat[z][y][x].symbol = mat[z * _size_y * _size_x + y * _size_x + x];
                    _mat[z][y][x].condition = 0;
                    _mat[z][y][x].info = 0;
    }   }   }   }

   ~Matrix3() {
        if (_mat == nullptr) return;

        for (uint32_t z = 0; z < _size_z; z++) { 
            for (uint32_t y = 0; y < _size_y; y++) {
                delete[] _mat[z][y];
            }
            delete[] _mat[z];                
        }

        delete[] _mat;
    }

    void print(std::function<std::string(Element)> print_func) const noexcept override {
        for (uint32_t z = 0; z < _size_z; z++) {
            for (uint32_t y = 0; y < _size_y; y++) {
                for (uint32_t x = 0; x < _size_x; x++) {
                    if (x != _size_x - 1) {
                        std::cout << print_func(_mat[z][y][x]) << ' ';
                    } else if (y != _size_y - 1) {
                        std::cout << print_func(_mat[z][y][x]) << '\n';
                    } else {
                        std::cout << print_func(_mat[z][y][x]) << "\n\n";
    }   }   }   }   }

    uint8_t get_dimensions() const noexcept override {
        return 3;
    }

    using Imatrix::get;

    Element get(Pos3 pos) const {
        if (pos.x >= _size_x || pos.y >= _size_y || pos.z >= _size_z) return Element { .symbol = '\0', 
                                                                                       .condition = Conditions::OUT_OF_BOUNDS_BIT, 
                                                                                       .info = 0 };

        return this->get(pos.x, pos.y, pos.z);
    }

    using Imatrix::set;

    void set(Element elem, Pos3 pos) {
        if (pos.x >= _size_x || pos.y >= _size_y || pos.z >= _size_z) return;

        this->set(elem, pos.x, pos.y, pos.z);
    }

    Element get_element(std::span<const uint32_t> coords) const override {
        return _mat[coords[2]][coords[1]][coords[0]];
    }

    void set_element(Element elem, std::span<const uint32_t> coords) override {
        _mat[coords[2]][coords[1]][coords[0]] = elem;
    }

    Pos3 get_size() const noexcept {
        return Pos3 { _size_x, _size_y, _size_z };
    }
    
    Matrix3(const Matrix3& other) : _size_x(other._size_x), 
                                    _size_y(other._size_y), 
                                    _size_z(other._size_z) {
        if (other._mat == nullptr) {
            this->_mat = nullptr;
            return;
        }

        this->_mat = new Element**[_size_z];

        for (uint32_t z = 0; z < _size_z; z++) { 
            _mat[z] = new Element*[_size_y];
            for (uint32_t y = 0; y < _size_y; y++) {
                _mat[z][y] = new Element[_size_x];
                for (uint32_t x = 0; x < _size_x; x++) {
                    _mat[z][y][x] = other._mat[z][y][x];
    }   }   }   }

    Matrix3(Matrix3&& other) noexcept
        : _size_x(other._size_x), 
          _size_y(other._size_y), 
          _size_z(other._size_z), 
          _mat(other._mat) {
        other._size_x = 0;
        other._size_y = 0;
        other._size_z = 0;
        other._mat = nullptr;
    }

    Matrix3& operator=(const Matrix3& other) {
        if (this == &other) return *this;

        if (_mat != nullptr) {
            for (uint32_t z = 0; z < _size_z; z++) { 
                for (uint32_t y = 0; y < _size_y; y++) {
                    delete[] _mat[z][y];                
                }   
                delete[] _mat[z];                
            }
            delete[] _mat;
        }

        this->_size_x = other._size_x;
        this->_size_y = other._size_y;
        this->_size_z = other._size_z;

        if (other._mat == nullptr) {
            this->_mat = nullptr;
            return *this;
        }

        this->_mat = new Element**[_size_z];

        for (uint32_t z = 0; z < _size_z; z++) { 
            _mat[z] = new Element*[_size_y];
            for (uint32_t y = 0; y < _size_y; y++) {
                _mat[z][y] = new Element[_size_x];
                for (uint32_t x = 0; x < _size_x; x++) {
                    _mat[z][y][x] = other._mat[z][y][x];
        }   }   }   
        return *this;
    }

    Matrix3& operator=(Matrix3&& other) noexcept {
        if (this == &other) return *this;

        if (_mat != nullptr) {
            for (uint32_t z = 0; z < _size_z; z++) { 
                for (uint32_t y = 0; y < _size_y; y++) {
                    delete[] _mat[z][y];                
                }   
                delete[] _mat[z];                
            }
            delete[] _mat;
        }

        this->_size_x = other._size_x;
        this->_size_y = other._size_y;
        this->_size_z = other._size_z;
        this->_mat    = other._mat;

        other._size_x = 0;
        other._size_y = 0;
        other._size_z = 0;
        other._mat = nullptr;

        return *this;
    }

};

#endif // MATRIX3HPP