#ifndef FILE_HANDLER_HPP
#define FILE_HANDLER_HPP

#include "input/plot_data/PlotData.hpp"

#define FileHandler FileHandler_T*

class FileHandler_T {
    const std::filesystem::path* _folder_path;
    std::deque<PlotData> _plot_datas;

    FileHandler_T(const std::filesystem::path* folder_path,
                  std::deque<PlotData> plot_datas = std::deque<PlotData>()) : _folder_path(folder_path),
                                                                              _plot_datas(plot_datas) {}

public:
    static FileHandler init(const std::filesystem::path&);
    static void destroy(FileHandler);

    std::deque<PlotData>& execute_file_peeker(HWND);

    std::deque<PlotData>& get_plot_data() noexcept {
        return _plot_datas;
    }

   ~FileHandler_T() {
        if (_folder_path) delete _folder_path;
        for (auto& elem : _plot_datas) {
            PlotData_T::destroy(elem);
        }
    }
    FileHandler_T(FileHandler_T&& other) noexcept : _folder_path(other._folder_path),
                                                    _plot_datas(other._plot_datas) {
        other._folder_path = nullptr;
        other._plot_datas = std::deque<PlotData>();
    }
    FileHandler_T& operator=(FileHandler_T&& other) noexcept {
        if (this == &other) return *this;

        _folder_path = other._folder_path;
        _plot_datas = other._plot_datas;

        other._folder_path = nullptr;
        other._plot_datas = std::deque<PlotData>();

        return *this;
    }
    FileHandler_T(const FileHandler_T&) = delete;
    FileHandler_T& operator=(const FileHandler_T&) = delete;
};

inline FileHandler FileHandler_T::init(const std::filesystem::path& path) {
    return new FileHandler_T(new std::filesystem::path(path));
}

inline void FileHandler_T::destroy(FileHandler handler) {
    delete handler;
}

inline std::deque<PlotData>& FileHandler_T::execute_file_peeker(HWND h_wnd) {
    wchar_t file_path[MAX_PATH] = {};
    OPENFILENAMEW ofn;

    ZeroMemory(&ofn, sizeof(ofn));

    ofn = OPENFILENAMEW {
        .lStructSize       = sizeof(OPENFILENAME),                                                  // Размер структуры в байтах (версионирование Win32 API)
        .hwndOwner         = h_wnd,                                                                 // Дескриптор родительского окна-владельца диалога
        .hInstance         = NULL,                                                                  // Дескриптор экземпляра модуля (нужен при использовании кастомного шаблона диалога)
        .lpstrFilter       = L"Текстовые файлы (*.txt)\0*.txt\0Все файлы (*.*)\0*.*\0",             // Строка фильтров типов файлов (формат: описание\0шаблон\0...\0\0)
        .lpstrCustomFilter = NULL,                                                                  // Буфер для сохранения пользовательского шаблона фильтра
        .nMaxCustFilter    = 0,                                                                     // Размер буфера пользовательского фильтра
        .nFilterIndex      = 1,                                                                     // 1-based индекс изначально выбранного фильтра из lpstrFilter
        .lpstrFile         = file_path,                                                             // Буфер, в который будет записан полный путь к выбранному файлу
        .nMaxFile          = MAX_PATH,                                                              // Максимальный размер буфера пути в символах (включая завершающий нуль-терминатор)
        .lpstrFileTitle    = NULL,                                                                  // Буфер для сохранения имени файла без пути (не требуется)
        .nMaxFileTitle     = 0,                                                                     // Размер буфера имени файла без пути
        .lpstrInitialDir   = _folder_path && !_folder_path->empty() ? _folder_path->c_str() : NULL, // Начальная директория при открытии проводника
        .lpstrTitle        = L"Выберите файл с данными графиков",                                   // Заголовок диалогового окна выбора файла
        .Flags             = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR,               // Флаги: проверка существования пути и файла, запрет изменения текущей директории приложения
        .nFileOffset       = 0,                                                                     // Смещение до имени файла в строке пути (выходной параметр API)
        .nFileExtension    = 0,                                                                     // Смещение до расширения файла в строке пути (выходной параметр API)
        .lpstrDefExt       = L"txt",                                                                // Расширение по умолчанию при вводе имени без расширения
        .lCustData         = 0,                                                                     // Пользовательские данные, передаваемые в hook-процедуру
        .lpfnHook          = NULL,                                                                  // Указатель на hook-процедуру (обработчик сообщений диалога)
        .lpTemplateName    = NULL,                                                                  // Имя шаблона диалогового окна из ресурсов
        .pvReserved        = 0,                                                                     // Зарезервировано
        .dwReserved        = 0,                                                                     // Зарезервировано
        .FlagsEx           = 0                                                                      // Расширение
    };

    if (GetOpenFileNameW(&ofn)) {
        MessageBoxW(h_wnd, ofn.lpstrFile, L"Имя файла", MB_OK);
    } else {
        return _plot_datas;
    }

    std::ifstream file(file_path);
    
    if (!file.is_open()) throw std::runtime_error("FileHandler_T::init : failed to open plot data file.");
    
    _plot_datas.clear();

    u32 num_plots;
    file >> num_plots;

    std::string* names = new std::string[num_plots];

    for (u32 i = 0; i < num_plots; ++i) {
        std::getline(file >> std::ws, names[i]);
    }

    std::deque<POINT*> points;

    LONG x_value;
    while (file >> x_value) {
        points.push_back(new POINT[num_plots]);

        for (u32 i = 0; i < num_plots; ++i) {
            LONG y_value;

            if (!(file >> y_value)) {
                throw std::runtime_error("FileHandler_T::init : count of Y values don't match.");
            }

            points.back()[i] = POINT {
                .x = x_value,
                .y = y_value
            };
        }
    }

    for (u32 i = 0; i < num_plots; ++i) {
        POINT* temp = new POINT[points.size()];
        
        for (u32 j = 0; j < points.size(); ++j) {
            temp[j] = points[j][i];
        }

        _plot_datas.push_back(PlotData_T::init(std::move(names[i]), temp, points.size()));
    }

    delete[] names;

    for (auto& elem : points) {
        delete[] elem;
    }

    return _plot_datas;
}

#endif // FILE_HANDLER_HPP