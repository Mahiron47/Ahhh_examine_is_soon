#ifndef MATRIX3HPP
#define MATRIX3HPP

class Matrix3 : public Imatrix {
    uint32_t   _size_x;
    uint32_t   _size_y;
    uint32_t   _size_z;
    Element*** _mat;

public:
    Matrix3(uint32_t size_x, 
            uint32_t size_y, 
            uint32_t size_z) : _size_x(size_x), 
                               _size_y(size_y), 
                               _size_z(size_z) {
        this->_mat = new Element**[_size_z];

        for (uint32_t i = 0; i < _size_z; i++) {
            _mat[i] = new Element*[_size_x];
            for (uint32_t j = 0; j < _size_x; j++) {
                _mat[i][j] = new Element[_size_y];
                for (uint32_t k = 0; k < _size_y; k++) {
                    _mat[i][j][k].symbol = '\0';
                    _mat[i][j][k].condition = 0;
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
            uint32_t i = 0; 
        
            ((mat[i++] = args), ...);
        } /* unpacking */

        for (uint32_t i = 0; i < _size_z; i++) { 
            _mat[i] = new Element*[_size_x];
            for (uint32_t j = 0; j < _size_x; j++) {
                _mat[i][j] = new Element[_size_y];
                for (uint32_t k = 0; k < _size_y; k++) {
                    _mat[i][j][k].symbol = mat[i * _size_x * _size_y + j * _size_y + k];
                    _mat[i][j][k].condition = 0;
    }   }   }   }

   ~Matrix3() {
        for (uint32_t i = 0; i < _size_z; i++) { 
            delete[] _mat[i];                
            for (uint32_t j = 0; j < _size_x; j++) {
                delete[] _mat[i][j];                
        }   }
    
        delete[] _mat;
    }

    void print(char(*print_func)(Element)) const noexcept override {
        for (uint32_t i = 0; i < _size_z; i++) {
            for (uint32_t j = 0; j < _size_x; j++) {
                for (uint32_t k = 0; k < _size_y; k++) {
                    if (k != _size_y - 1) {
                        std::cout << print_func(_mat[i][j][k]) << ' ';
                    } else if (j != _size_x - 1) {
                        std::cout << print_func(_mat[i][j][k]) << '\n';
                    } else {
                        std::cout << print_func(_mat[i][j][k]) << "\n\n";
    }   }   }   }   } 

    uint8_t get_dimensions() const noexcept override {
        return 3;
    }

    using Imatrix::get;

    Element get(Pos3 pos) const {
        if (pos.x >= _size_x || pos.y >= _size_y || pos.z >= _size_z) return Element { .symbol = '\0', .condition = Conditions::OUT_OF_BOUNDS_BIT };

        return this->get(pos.x, pos.y, pos.z);
    }

    using Imatrix::set;

    void set(Element elem, Pos3 pos) {
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
    
};

#endif // MATRIX3HPP