#ifndef PLOT_DATA_HPP
#define PLOT_DATA_HPP

struct PlotDataPack {
    std::string name;
    POINT*  points;
    u32 num_points;
};

#define PlotData PlotData_T*

class PlotData_T {
    std::string _name;
    POINT*  _points;
    u32 _num_points;

    PlotData_T(std::string name, 
             POINT* points, 
             u32 num_points) : _name(name),
                               _points(points),
                               _num_points(num_points) {}

public:
    static PlotData init(PlotData instance);
    static PlotData init(std::string name, POINT* points, u32 num_points);
    static void destroy(PlotData instance);

    constexpr PlotDataPack get_data() const noexcept {
        return PlotDataPack { 
            _name, _points, _num_points 
        };
    }

    ~PlotData_T() {
        if (_points) delete[] _points;
    }
    /*PlotData_T(PlotData_T&& other) noexcept : _name(std::move(other._name)),
                                              _points(other._points),
                                              _num_points(other._num_points) {
        other._points = nullptr;
        other._num_points = 0;
    }
    PlotData_T& operator=(PlotData_T&& other) noexcept {
        if (this == &other) return *this;

        if (_points) delete[] _points;

        this->_name = std::move(other._name);
        this->_points = other._points;
        this->_num_points = other._num_points;

        other._points = nullptr;
        other._num_points = 0;

        return *this;
    }*/
    PlotData_T(const PlotData_T& other) : _name(std::string(other._name)),
                                          _points(new POINT[other._num_points]),
                                          _num_points(other._num_points) {
        for (u32 i = 0; i < _num_points; ++i) {
            _points[i] = other._points[i];
        }
    }
    /*PlotData_T& operator=(const PlotData_T& other) {
        if (this == &other) return *this;
        
        this->_name = std::string(other._name);
        this->_num_points = other._num_points;
        
        if (_points) delete[] _points;
        
        this->_points = new POINT[other._num_points];
        
        for (u32 i = 0; i < this->_num_points; ++i) {
            this->_points[i] = other._points[i];
        }

        return *this;
    };*/
    PlotData_T(PlotData_T&&) = delete;
    PlotData_T& operator=(PlotData_T&&) = delete;
    PlotData_T& operator=(const PlotData_T&) = delete;
};
    
inline PlotData PlotData_T::init(PlotData instance) {
    return new PlotData_T(*instance);
}

inline PlotData PlotData_T::init(std::string name, POINT* points, u32 num_points) {
    return new PlotData_T(name, points, num_points);
}

inline void PlotData_T::destroy(PlotData instance) {
    delete instance;
}

#endif // PLOT_DATA_HPP