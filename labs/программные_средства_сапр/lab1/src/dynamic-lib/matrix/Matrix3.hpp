#ifndef MATRIX3HPP
#define MATRIX3HPP

class Matrix3 : public Imatrix {
    uint32_t   _size_z;
    uint32_t   _size_xy;
    Element*** _mat;

public:
    template<typename... Args> requires (std::convertible_to<Args, char> && ...) Matrix3(uint32_t size_z, const Args&... args) : _size_xy(std::sqrtl(sizeof...(args) / size_z)), _size_z(size_z) {
        static_assert(size_z * std::sqrtl(sizeof...(args) * std::sqrtl(sizeof...(args) == sizeof...(args))));
        
        this->_mat = new Element**[_size_z];

        char mat[sizeof...(args)]; 
        /* unpacking */ {
            uint32_t i = 0; 
        
            ((mat[i++] = args), ...);
        } /* unpacking */

        for (uint32_t i = 0; i < _size_z; i++) { 
            _mat[i] = new Element*[_size_z];
            for (uint32_t j = 0; j < _size_xy; j++) {
                _mat[i][j] = new Element[_size_xy];
                for (uint32_t k = 0; k < _size_xy; k++) {
                    _mat[i][j][k].symbol = mat[i * _size_xy * _size_xy + j * _size_xy + k];
                    _mat[i][j][k].condition = 0;
    }   }   }   }

   ~Matrix3() {
        for (uint32_t i = 0; i < _size_z; i++) { 
            delete[] _mat[i];                
            for (uint32_t j = 0; j < _size_xy; j++) {
                delete[] _mat[i][j];                
        }   }
    
        delete[] _mat;
    }

    void print() const noexcept override {
        for (uint32_t i = 0; i < _size_z; i++) {
            for (uint32_t j = 0; j < _size_xy; j++) {
                for (uint32_t k = 0; k < _size_xy; k++) {
                    if (k != _size_xy) {
                        std::cout << _mat[i][j][k].symbol << ' ';
                    } else if (j != _size_xy) {
                        std::cout << _mat[i][j][k].symbol << '\n';
                    } else {
                        std::cout << _mat[i][j][k].symbol << "\n\n";
    }   }   }   }   } 

    uint8_t get_dimensions() const noexcept override {
        return 3;
    }

    using Imatrix::get;

    Element get(Pos3 pos) const {
        if (0 < pos.x || pos.x >= _size_xy || 0 < pos.y || pos.y >= _size_xy || 0 < pos.z || pos.z >= _size_z) 
            return Element { .symbol = '\0', 
                             .condition = Conditions::OUT_OF_BOUNDS_BIT };

        return this->get(pos.x, pos.y, pos.z);
    }

    using Imatrix::set;

    void set(Element elem, Pos3 pos) {
        this->set(elem, pos.x, pos.y, pos.z);
    }

    Element get_element(std::span<const uint32_t> coords) const override {
        return _mat[coords[0]][coords[1]][coords[2]];
    }

    void set_element(Element elem, std::span<const uint32_t> coords) override {
        _mat[coords[0]][coords[1]][coords[2]] = elem;
    }
    
};

#endif // MATRIX3HPP