#ifndef FILE_HANDLER_HPP
#define FILE_HANDLER_HPP

#include "input/chip_data/ChipData.hpp"
#include <commdlg.h>

#define FileHandler FileHandler_T*

class FileHandler_T {
    std::filesystem::path _folder_path;
    ChipData              _chip_data;

    FileHandler_T(const std::filesystem::path& folder_path) : _folder_path(folder_path), 
                                                              _chip_data(nullptr) {}

public:
    bool parse_def(const std::filesystem::path& file_path, DefDataPackage& def_data, const std::filesystem::path& log_dir = "") {
        std::ifstream file(file_path);
        if (!file.is_open()) {
            std::cerr << "FileHandler_T::parse : не удалось открыть файл " << file_path.string() << std::endl;
            return false;
        }

        def_data = {
            .def_units = 0,
            .die_area = DieArea {
                .top_left = Point<i32>{ 
                    .x = 0, 
                    .y = 0 
                },
                .bottom_right = Point<i32>{ 
                    .x = 0, 
                    .y = 0 
                }
            },
            .components = {},
            .nets = {},
            .stats = {}
        };

        std::string token;

        while (file >> token) {
            if (token == "#") {
                std::string line;
                file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                continue;
            }

            if (token == "UNITS") {
                std::string dist, mic;
                file >> dist >> mic;
                if (dist == "DISTANCE" && mic == "MICRONS") {
                    u32 u = 0;
                    file >> u;
                    def_data.def_units = u;
                }
            } else if (token == "DIEAREA") {
                DieArea die_area = DieArea {
                    .top_left = Point<i32>{ 
                        .x = 0, 
                        .y = 0 
                    },
                    .bottom_right = Point<i32>{ 
                        .x = 0, 
                        .y = 0 
                    }
                };

                std::string token2;

                if (file >> token2 && token2 != "(") {
                    std::cerr << "FileHandler_T::parse : ожидается '(' после 'DIEAREA', найдено '" << token2 << "'" << std::endl;
                    return false;
                }
                
                file >> die_area.top_left.x >> die_area.top_left.y;

                if (file >> token2 && token2 != ")") {
                    std::cerr << "FileHandler_T::parse : ожидается ')' после координат DIEAREA, найдено '" << token2 << "'" << std::endl;
                    return false;
                }

                if (file >> token2 && token2 != "(") {
                    std::cerr << "FileHandler_T::parse : ожидается '(' после координат DIEAREA, найдено '" << token2 << "'" << std::endl;
                    return false;
                }

                file >> die_area.bottom_right.x >> die_area.bottom_right.y;

                if (file >> token2 && token2 != ")") {
                    std::cerr << "FileHandler_T::parse : ожидается ')' после координат DIEAREA, найдено '" << token2 << "'" << std::endl;
                    return false;
                }

                if (file >> token2 && token2 != ";") {
                    std::cerr << "FileHandler_T::parse : ожидается ';' после координат DIEAREA, найдено '" << token2 << "'" << std::endl;
                    return false;
                }

                def_data.die_area = die_area;
            } else if (token == "COMPONENTS") {
                u32 count = 0;
                file >> count;
                std::string semicolon;
                
                if (file >> semicolon && semicolon != ";") {
                    std::cerr << "FileHandler_T::parse : ожидается ';' после количества COMPONENTS, найдено '" << semicolon << "'" << std::endl;
                    return false;
                }

                std::string token2;

                while (file >> token2 && token2 != "END") {
                    if (token2 == "-") {
                        DefComponent comp = {
                            .instance_name = "INSTANCE_NAME",
                            .macro_name = "MACRO_NAME",
                            .position = Point<i32>{ 
                                .x = 0, 
                                .y = 0 
                            },
                            .status = "PLACED",
                            .orientation = "N"
                        };

                        file >> comp.instance_name >> comp.macro_name;

                        std::string token3;
                        while (file >> token3 && token3 != ";") {
                            if (token3 == "+") {
                                file >> comp.status;
                                if (comp.status == "PLACED" || comp.status == "FIXED") {
                                    std::string token4;

                                    if (file >> token4 && token4 != "(") {
                                        std::cerr << "FileHandler_T::parse : ожидается '(' после 'PLACED' или 'FIXED', найдено '" << token4 << "'" << std::endl;
                                        return false;
                                    }

                                    file >> comp.position.x >> comp.position.y;
                                    
                                    if (file >> token4 && token4 != ")") {
                                        std::cerr << "FileHandler_T::parse : ожидается ')' после координат, найдено '" << token4 << "'" << std::endl;
                                        return false;
                                    }
                                    
                                    file >> comp.orientation;

                                    if (file >> token4 && token4 != ";") {
                                        std::cerr << "FileHandler_T::parse : ожидается ';' после ориентации, найдено '" << token4 << "'" << std::endl;
                                        return false;
                                    }   
                        }   }   }
                        def_data.components.push_back(comp);
                }   }

                if (file >> token2 && token2 != "COMPONENTS") {
                    std::cerr << "FileHandler_T::parse : ожидается 'END COMPONENTS', найдено '" << token2 << "'" << std::endl;
                    return false;
                }

            } else if (token == "NETS" || token == "SPECIALNETS") {
                bool is_special = (token == "SPECIALNETS");
                u32 count = 0;
                file >> count;
                
                std::string token2;

                if (file >> token2 && token2 != ";") {
                    std::cerr << "FileHandler_T::parse : ожидается ';' после количества NETS/SPECIALNETS, найдено '" << token2 << "'" << std::endl;
                    return false;
                }

                while (file >> token2 && token2 != "END") {
                    if (token2 == "-") {
                        DefNet net = DefNet {
                            .name = "NET_NAME",
                            .type = "SIGNAL",
                            .connections = {},
                            .segments = {}
                        };
                        
                        file >> net.name;

                        std::string token3;

                        while (file >> token3 && token3 != ";") {
                            if (token3 == "(") {
                                DefNet::Connection conn = DefNet::Connection {
                                    .instance_name = "INSTANCE_NAME",
                                    .pin_name = "PIN_NAME"
                                };

                                file >> conn.instance_name >> conn.pin_name;
                                
                                net.connections.push_back(conn);

                                std::string token4;
                                if (file >> token4 && token4 != ")") {
                                    std::cerr << "FileHandler_T::parse : ожидается ')' после имени instance и pin, найдено '" << token4 << "'" << std::endl;
                                    return false;
                                }
                            } else if (token3 == "+") {
                                std::string token4;

                                file >> token4;

                                if (token4 == "USE") {
                                    file >> net.type;
                                } else if (token4 == "ROUTED" || token4 == "NEW") {
                                    DefNet::Routing segment = DefNet::Routing {
                                        .layer = "metal1",
                                        .source = Point<i32>{ 
                                            .x = 0, 
                                            .y = 0 
                                        },
                                        .target = Point<i32>{ 
                                            .x = 0, 
                                            .y = 0 
                                        }
                                    };

                                    std::string token5;

                                    file >> segment.layer;

                                    if (file >> token5 && token5 != "(") {
                                        std::cerr << "FileHandler_T::parse : ожидается '(' после 'ROUTED' и имени слоя, найдено '" << token5 << "'" << std::endl;
                                        return false;
                                    }

                                    file >> segment.source.x >> segment.source.y;

                                    while (file >> token5 && token5 != ")") {
                                        std::cout << "FileHandler_T::parse : найдено extension value '" << token5 << "' после координат начала сегмента, игнорируется" << std::endl;
                                    }

                                    if (file >> token5 && token5 != "(") {
                                        net.segments.push_back(DefNet::Contact {
                                            .layer = segment.layer,
                                            .source = segment.source,
                                            .procedure_name = token5
                                        });
                                    } else {
                                        file >> segment.target.x >> segment.target.y;

                                        while (file >> token5 && token5 != ")") {
                                            std::cout << "FileHandler_T::parse : найдено extension value '" << token5 << "' после координат конца сегмента, игнорируется" << std::endl;
                                        }

                                        net.segments.push_back(segment);
                        }   }   }   }
                        def_data.nets.push_back(net);
                }   }
                
                if (file >> token2 && token2 != (is_special ? "SPECIALNETS" : "NETS")) {
                    std::cerr << "FileHandler_T::parse : ожидается 'END NETS' или 'END SPECIALNETS', найдено '" << token2 << "'" << std::endl;
                    return false;
                }
            }
        }

        file.close();

        std::set<std::string> unique_macro_set;
        std::set<i32> occupied_rows_set;

        for (const auto& comp : def_data.components) {
            unique_macro_set.insert(comp.macro_name);
            if (comp.status == "PLACED" || comp.status == "FIXED") {
                occupied_rows_set.insert(comp.position.y);
            }
        }

        ChipStatistics stats = ChipStatistics {
            .total_macros = unique_macro_set.size(),
            .total_components = def_data.components.size(),
            .unique_components = unique_macro_set.size(),
            .occupied_rows = occupied_rows_set.size(),
            .total_nets = def_data.nets.size(),
            .clock_nets = 0,
            .signal_net_names = {}
        };

        for (const auto& net : def_data.nets) {
            if (net.type == "CLOCK") {
                stats.clock_nets++;
            }
            if (net.type == "SIGNAL") {
                stats.signal_net_names.push_back(net.name);
            }
        }

        SetConsoleOutputCP(CP_UTF8);
        SetConsoleCP(CP_UTF8);

        std::cout << "\n=================== STAT_DEF ===================\n";
        std::cout << "Всего компонентов: " << stats.total_components << "\n";
        std::cout << "Количество сигнальных цепей: " << stats.signal_net_names.size() << "\n";
        std::cout << "Имена сигнальных цепей:\n";
        for (const auto& name : stats.signal_net_names) {
            std::cout << "  - " << name << "\n";
        }
        std::cout << "Дополнительная статистика:\n";
        std::cout << "  * Всего цепей: " << stats.total_nets << "\n";
        std::cout << "  * Количество тактовых цепей: " << stats.clock_nets << "\n";
        std::cout << "  * Количество уникальных компонентов: " << stats.unique_components << "\n";
        std::cout << "  * Количество занятых строк: " << stats.occupied_rows << "\n";
        std::cout << "================================================\n\n";

        if (log_dir.empty()) {
            std::cout << "FileHandler_T: логирование статистики отключено (log_dir пустой)" << std::endl;
            return true;
        }

        std::ofstream log_file(log_dir / "stat_def.log");
        if (log_file.is_open()) {
            log_file << "Всего компонентов: " << stats.total_components << "\n";
            log_file << "Количество сигнальных цепей: " << stats.signal_net_names.size() << "\n";
            log_file << "Имена сигнальных цепей:\n";
            for (const auto& name : stats.signal_net_names) {
                log_file << name << "\n";
            }
            log_file << "\nДополнительная статистика:\n";
            log_file << "Всего цепей: " << stats.total_nets << "\n";
            log_file << "Количество тактовых цепей: " << stats.clock_nets << "\n";
            log_file << "Количество уникальных компонентов: " << stats.unique_components << "\n";
            log_file << "Количество занятых строк: " << stats.occupied_rows << "\n";
            log_file.close();
            std::cout << "FileHandler_T: статистика сохранена в " << (log_dir / "stat_def.log").string() << std::endl;
        }

        return true;
    }

    bool parse_lef(const std::filesystem::path& file_path, LefDataPackage& lef_data, const std::filesystem::path& log_dir = "") {
        std::ifstream file(file_path);
        if (!file.is_open()) {
            std::cerr << "LefParser::parse : не удалось открыть файл " << file_path.string() << std::endl;
            return false;
        }

        lef_data = {
            .database_microns = 0,
            .layers = {},
            .macros = {}
        };

        std::string token;

        while (file >> token) {
            if (token == "#") {
                std::string line;
                file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                continue;
            }

            if (token == "UNITS") {
                std::string dist, mic;
                file >> dist >> mic;
                if (dist == "DATABASE" && mic == "MICRONS") {
                    f64 u = 0.0;
                    file >> u;
                    lef_data.database_microns = u;
                }
            } else if (token == "LAYER") {
                LefLayer layer = LefLayer {
                    .name = "LAYER_NAME",
                    .type = "",
                    .width = 0.0,
                    .pitch = 0.0,
                    .thickness = 0.0
                };

                if (file >> layer.name && (layer.name == ";" || layer.name == "STRING")) {
                    file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    continue;
                }

                std::string token2;

                while (file >> token2 && token2 != "END") {
                    std::string token3;

                    file >> token3;

                    if (token3 == "TYPE") {
                        file >> layer.type;
                    } else if (token3 == "WIDTH") {
                        file >> layer.width;
                    } else if (token3 == "PITCH") {
                        file >> layer.pitch;
                    } else if (token3 == "THICKNESS") {
                        file >> layer.thickness;
                    } 
                }

                if (file >> token2 && token2 != layer.name) {
                    std::cerr << "LefParser::parse : ожидается 'END " << layer.name << "', найдено '" << token2 << "'" << std::endl;
                    return false;
                }

                lef_data.layers[layer.name] = layer;
            } else if (token == "MACRO") {
                LefMacro macro = LefMacro {
                    .name = "MACRO_NAME",
                    .width = 0.0,
                    .height = 0.0,
                    .pins = {}
                };

                file >> macro.name;

                std::string token2;

                while (file >> token2) {
                    if (token2 == "END" && file >> token2 && token2 == macro.name) {
                        break;
                    }

                    if (token2 == "SIZE") {
                        file >> macro.width;
                        
                        std::string token3;

                        if (file >> token3 && token3 != "BY") {
                            std::cerr << "LefParser::parse : ожидается 'BY' после ширины MACRO, найдено '" << token3 << "'" << std::endl;
                            return false;
                        }

                        file >> macro.height;
                    } else if (token2 == "PIN") {
                        LefMacro::Pin pin = LefMacro::Pin {
                            .name = "PIN_NAME",
                            .direction = ""
                        };

                        std::string token3;

                        file >> pin.name;

                        while (file >> token3) {
                            if (token3 == "END" && file >> token3 && token3 == pin.name) {
                                break;
                            }
                            
                            if (token3 == "DIRECTION") {
                                file >> pin.direction;
                            }
                        }
                        macro.pins.push_back(pin);
                }   }
                lef_data.macros[macro.name] = macro;
        }   }

        file.close();

        SetConsoleOutputCP(CP_UTF8);
        SetConsoleCP(CP_UTF8);

        if (log_dir.empty()) {
            std::cout << "LefParser: логирование статистики отключено (log_dir пустой)" << std::endl;
            return true;
        }

        std::ofstream log_file(log_dir / "stat_lef.log");
        if (log_file.is_open()) {
            for (const auto& [name, macro] : lef_data.macros) {
                f64 area = macro.width * macro.height;
                log_file << "Cell: " << name 
                         << ", Pins: " << macro.pins.size() 
                         << ", Size: " << macro.width << " x " << macro.height 
                         << ", Area: " << area << "\n";
            }
            log_file.close();
            std::cout << "LefParser: статистика сохранена в " << (log_dir / "stat_lef.log").string() << std::endl;
        }

        return true;
    }

public:
    static FileHandler init(const std::filesystem::path& path) {
        return new FileHandler_T(path);
    }

    static void destroy(FileHandler handler) {
        delete handler;
    }

    ChipData get_chip_data() const noexcept {
        return _chip_data;
    }

    bool execute_file_peeker(HWND h_wnd) {
        wchar_t lef_path[MAX_PATH] = {};
        OPENFILENAMEW ofn_lef;
        ZeroMemory(&ofn_lef, sizeof(ofn_lef));

        ofn_lef = OPENFILENAMEW {
            .lStructSize       = sizeof(OPENFILENAMEW),
            .hwndOwner         = h_wnd,
            .hInstance         = NULL,
            .lpstrFilter       = L"LEF files (*.lef)\0*.lef\0All files (*.*)\0*.*\0",
            .lpstrCustomFilter = NULL,
            .nMaxCustFilter    = 0,
            .nFilterIndex      = 1,
            .lpstrFile         = lef_path,
            .nMaxFile          = MAX_PATH,
            .lpstrFileTitle    = NULL,
            .nMaxFileTitle     = 0,
            .lpstrInitialDir   = _folder_path.empty() ? NULL : _folder_path.c_str(),
            .lpstrTitle        = L"Выберите LEF файл (библиотека ячеек)",
            .Flags             = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR,
            .nFileOffset       = 0,
            .nFileExtension    = 0,
            .lpstrDefExt       = L"lef",
            .lCustData         = 0,
            .lpfnHook          = NULL,
            .lpTemplateName    = NULL,
            .pvReserved        = NULL,
            .dwReserved        = 0,
            .FlagsEx           = 0
        };

        if (!GetOpenFileNameW(&ofn_lef)) {
            return false;
        }

        wchar_t def_path[MAX_PATH] = {};
        OPENFILENAMEW ofn_def;
        ZeroMemory(&ofn_def, sizeof(ofn_def));

        ofn_def = OPENFILENAMEW {
            .lStructSize       = sizeof(OPENFILENAMEW),
            .hwndOwner         = h_wnd,
            .hInstance         = NULL,
            .lpstrFilter       = L"DEF files (*.def)\0*.def\0All files (*.*)\0*.*\0",
            .lpstrCustomFilter = NULL,
            .nMaxCustFilter    = 0,
            .nFilterIndex      = 1,
            .lpstrFile         = def_path,
            .nMaxFile          = MAX_PATH,
            .lpstrFileTitle    = NULL,
            .nMaxFileTitle     = 0,
            .lpstrInitialDir   = _folder_path.empty() ? NULL : _folder_path.c_str(),
            .lpstrTitle        = L"Выберите DEF файл (размещение и трассировка)",
            .Flags             = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR,
            .nFileOffset       = 0,
            .nFileExtension    = 0,
            .lpstrDefExt       = L"def",
            .lCustData         = 0,
            .lpfnHook          = NULL,
            .lpTemplateName    = NULL,
            .pvReserved        = NULL,
            .dwReserved        = 0,
            .FlagsEx           = 0
        };

        if (!GetOpenFileNameW(&ofn_def)) {
            return false;
        }

        std::filesystem::path selected_def = def_path;
        std::filesystem::path selected_lef = lef_path;

        DefDataPackage def_data;
        LefDataPackage lef_data;

        bool ok_lef = parse_lef(selected_lef, lef_data, selected_lef.parent_path());
        bool ok_def = parse_def(selected_def, def_data, selected_def.parent_path());

        if (_chip_data != nullptr) {
            ChipData_T::destroy(_chip_data);
        }
        this->_chip_data = ChipData_T::init(def_data, lef_data);

        return ok_lef && ok_def;
    }

    ~FileHandler_T() {
        ChipData_T::destroy(_chip_data);
    }

    FileHandler_T(const FileHandler_T&) = delete;
    FileHandler_T& operator=(const FileHandler_T&) = delete;
};

#endif // FILE_HANDLER_HPP
