#ifndef CHIP_DATA_HPP
#define CHIP_DATA_HPP


struct LefMacro {
    struct Pin {
        std::string name;
        std::string direction;
    };

    std::string name;
    f64 width = 0.0;
    f64 height = 0.0;
    std::deque<LefMacro::Pin> pins;
};

struct LefLayer {
    std::string name;
    std::string type;
    f64 width = 0.0;
    f64 pitch = 0.0;
    f64 thickness = 0.0;
};

struct DefComponent {
    std::string instance_name;
    std::string macro_name;
    Point<u32> position;
    std::string status;
    std::string orientation;
};

struct DefNet {
    struct Connection {
        std::string instance_name;
        std::string pin_name;
    };

    struct Routing {
        std::string layer;
        Point<u32> source;
        Point<u32> target;
    };

    struct Contact {
        std::string layer;
        Point<u32> source;
        std::string procedure_name;
    };

    using Segment = std::variant<Routing, Contact>;

    std::string name;
    std::string type; // "SIGNAL", "CLOCK", ...
    std::deque<Connection> connections; 
    std::deque<Segment> segments;
};

struct DieArea {
    Point<u32> top_left;
    Point<u32> bottom_right;

    constexpr u32 width() const noexcept { return bottom_right.x - top_left.x; }
    constexpr u32 height() const noexcept { return bottom_right.y - top_left.y; }
};

struct ChipStatistic {
    u32 total_components = 0;
    std::deque<std::string> signal_net_names;
};

struct LefDataPackage {
    u32 database_microns;
    std::map<std::string, LefLayer> layers;
    std::map<std::string, LefMacro> macros;
};

struct DefDataPackage {
    u32 def_units;
    DieArea die_area;
    std::deque<DefComponent> components;
    std::deque<DefNet> nets;
};

class ChipData_T {
    u32                             _database_microns;
    std::map<std::string, LefLayer> _layers;
    std::map<std::string, LefMacro> _macros;
    DieArea                         _die_area;
    u32                             _def_units;
    std::deque<DefComponent>        _components;
    std::deque<DefNet>              _nets;
    ChipStatistic                   _stats;

    ChipData_T(DefDataPackage def_data, LefDataPackage lef_data, ChipStatistic stats)
        : _database_microns(lef_data.database_microns),
          _layers(lef_data.layers),
          _macros(lef_data.macros),
          _die_area(def_data.die_area),
          _def_units(def_data.def_units),
          _components(def_data.components),
          _nets(def_data.nets),
          _stats(stats) {}

public:
    static ChipData_T* init(DefDataPackage def_data, LefDataPackage lef_data, ChipStatistic stats) {
        return new ChipData_T(def_data, lef_data, stats);
    }
    static void destroy(ChipData_T* data) {
        delete data;
    }

    constexpr DefDataPackage get_def_data() const {
        return DefDataPackage {
            .def_units = _def_units,
            .die_area = _die_area,
            .components = _components,
            .nets = _nets,
        };
    }

    constexpr LefDataPackage get_lef_data() const {
        return LefDataPackage {
            .database_microns = _database_microns,
            .layers = _layers,
            .macros = _macros
        };
    }

    constexpr const ChipStatistic& get_stats() const {
        return _stats;
    }

    ChipData_T(const ChipData_T&) = delete;
    ChipData_T& operator=(const ChipData_T&) = delete;
    ChipData_T(ChipData_T&&) = delete;
    ChipData_T& operator=(ChipData_T&&) = delete;
};

#define ChipData ChipData_T*

#endif // CHIP_DATA_HPP