#ifndef WAVEALGORITHMHPP
#define WAVEALGORITHMHPP

class WaveAlgorithm {
    struct Node2 {
        Pos2     pos;
        uint32_t wave;

        constexpr Node2() noexcept = default;
        constexpr ~Node2() noexcept = default;
        constexpr Node2(Pos2 position, uint32_t wave) noexcept : pos(position), wave(wave) {}

        bool get_neighbors(Matrix2* matrix, Matrix2* wave_matrix, uint32_t order, std::queue<Node2>* queue, bool(*func_check_condition)(Matrix2*, Pos2)) const {
            if (func_check_condition(matrix, pos)) return true;
            
            Imatrix::Element forward  = matrix->get(pos + Pos2( 0,  1));
            Imatrix::Element backward = matrix->get(pos + Pos2( 0, -1));
            Imatrix::Element right    = matrix->get(pos + Pos2( 1,  0));
            Imatrix::Element left     = matrix->get(pos + Pos2(-1,  0));

            auto func = [&](uint32_t order_byte) -> void {
                switch (order_byte) {
                case Directions::DIRECTION_FORWARD:
                    if (forward.symbol != '%' && ((forward.condition & Conditions::OUT_OF_BOUNDS_BIT) != Conditions::OUT_OF_BOUNDS_BIT) && ((forward.condition & Conditions::CHECKED_BIT) != Conditions::CHECKED_BIT)) { 
                        queue->push(Node2(pos + Pos2(0, 1), wave + 1));
                        wave_matrix->set(Imatrix::Element { .symbol = forward.symbol, 
                                                            .condition = static_cast<uint8_t>(wave + 1) },
                                         pos + Pos2(0, 1));
                        matrix->set(Imatrix::Element { .symbol = forward.symbol, 
                                                       .condition = forward.condition | Conditions::CHECKED_BIT }, 
                                    pos + Pos2(0, 1));
                    }
                break;
                case Directions::DIRECTION_BACKWARD:
                    if (backward.symbol != '%' && ((backward.condition & Conditions::OUT_OF_BOUNDS_BIT) != Conditions::OUT_OF_BOUNDS_BIT) && ((backward.condition & Conditions::CHECKED_BIT) != Conditions::CHECKED_BIT)) { 
                        queue->push(Node2(pos + Pos2(0, -1), wave + 1));
                        wave_matrix->set(Imatrix::Element { .symbol = backward.symbol, 
                                                            .condition = static_cast<uint8_t>(wave + 1) },
                                         pos + Pos2(0, -1));
                        matrix->set(Imatrix::Element { .symbol = backward.symbol, 
                                                       .condition = backward.condition | Conditions::CHECKED_BIT }, 
                                    pos + Pos2(0, -1)); 
                    }
                break;
                case Directions::DIRECTION_RIGHT:
                    if (right.symbol != '%' && ((right.condition & Conditions::OUT_OF_BOUNDS_BIT) != Conditions::OUT_OF_BOUNDS_BIT) && ((right.condition & Conditions::CHECKED_BIT) != Conditions::CHECKED_BIT)) { 
                        queue->push(Node2(pos + Pos2(1, 0), wave + 1));
                        wave_matrix->set(Imatrix::Element { .symbol = right.symbol, 
                                                            .condition = static_cast<uint8_t>(wave + 1) },
                                         pos + Pos2(1, 0));
                        matrix->set(Imatrix::Element { .symbol = right.symbol, 
                                                       .condition = right.condition | Conditions::CHECKED_BIT }, 
                                    pos + Pos2(1, 0)); 
                    }
                break;
                case Directions::DIRECTION_LEFT:
                    if (left.symbol != '%' && ((left.condition & Conditions::OUT_OF_BOUNDS_BIT) != Conditions::OUT_OF_BOUNDS_BIT) && ((left.condition & Conditions::CHECKED_BIT) != Conditions::CHECKED_BIT)) { 
                        queue->push(Node2(pos + Pos2(-1, 0), wave + 1));
                        wave_matrix->set(Imatrix::Element { .symbol = left.symbol, 
                                                            .condition = static_cast<uint8_t>(wave + 1) },
                                         pos + Pos2(-1, 0));
                        matrix->set(Imatrix::Element { .symbol = left.symbol, 
                                                       .condition = left.condition | Conditions::CHECKED_BIT }, 
                                    pos + Pos2(-1, 0)); 
                    }
                break;
                default:
                    std::runtime_error("WaveAlgorithm::Node2::get_neighbors : unknown order.");
                }
            };
             
            func(order & 0b111);
            func((order >> 3) & 0b111);
            func((order >> 6) & 0b111);
            func((order >> 9) & 0b111);

            return false;
        }
    };

    struct Node3 {
        Pos3     pos;
        uint32_t wave;

        constexpr Node3() noexcept = default;
        constexpr ~Node3() noexcept = default;
        constexpr Node3(Pos3 position, uint32_t wave) noexcept : pos(position), wave(wave) {}

        bool get_neighbors(Matrix3* matrix, Matrix3* wave_matrix, uint32_t order, std::queue<Node3>* queue, bool(*func_check_condition)(Matrix3*, Pos3)) const {
            if (func_check_condition(matrix, pos)) return true;

            bool trace_flag = true;
            
            Imatrix::Element forward  = matrix->get(pos + Pos3( 0,  1,  0));
            Imatrix::Element backward = matrix->get(pos + Pos3( 0, -1,  0));
            Imatrix::Element up       = matrix->get(pos + Pos3( 0,  0,  1));
            Imatrix::Element down     = matrix->get(pos + Pos3( 0,  0, -1));
            Imatrix::Element right    = matrix->get(pos + Pos3( 1,  0,  0));
            Imatrix::Element left     = matrix->get(pos + Pos3(-1,  0,  0));

            auto func = [&](uint32_t order_byte) -> void {
                switch (order_byte) {
                case Directions::DIRECTION_FORWARD:
                    if (forward.symbol != '%' && ((forward.condition & Conditions::OUT_OF_BOUNDS_BIT) != Conditions::OUT_OF_BOUNDS_BIT) && ((forward.condition & Conditions::CHECKED_BIT) != Conditions::CHECKED_BIT)) { 
                        queue->push(Node3(pos + Pos3(0, 1, 0), wave + 1));
                        wave_matrix->set(Imatrix::Element { .symbol = forward.symbol, 
                                                            .condition = static_cast<uint8_t>(wave + 1) },
                                         pos + Pos3(0, 1, 0));
                        matrix->set(Imatrix::Element { .symbol = forward.symbol, 
                                                       .condition = forward.condition | Conditions::CHECKED_BIT }, 
                                    pos + Pos3(0, 1, 0));
                        trace_flag = false;
                    }
                break;
                case Directions::DIRECTION_BACKWARD:
                    if (backward.symbol != '%' && ((backward.condition & Conditions::OUT_OF_BOUNDS_BIT) != Conditions::OUT_OF_BOUNDS_BIT) && ((backward.condition & Conditions::CHECKED_BIT) != Conditions::CHECKED_BIT)) { 
                        queue->push(Node3(pos + Pos3(0, -1, 0), wave + 1));
                        wave_matrix->set(Imatrix::Element { .symbol = backward.symbol, 
                                                            .condition = static_cast<uint8_t>(wave + 1) },
                                         pos + Pos3(0, -1, 0));
                        matrix->set(Imatrix::Element { .symbol = backward.symbol, 
                                                       .condition = backward.condition | Conditions::CHECKED_BIT }, 
                                    pos + Pos3(0, -1, 0)); 
                        trace_flag = false;
                    }
                break;
                case Directions::DIRECTION_UP:
                    if (up.symbol != '%' && ((up.condition & Conditions::OUT_OF_BOUNDS_BIT) != Conditions::OUT_OF_BOUNDS_BIT) && ((up.condition & Conditions::CHECKED_BIT) != Conditions::CHECKED_BIT)) { 
                        queue->push(Node3(pos + Pos3(0, 0, 1), wave + 1));
                        wave_matrix->set(Imatrix::Element { .symbol = up.symbol, 
                                                            .condition = static_cast<uint8_t>(wave + 1) },
                                         pos + Pos3(0, 0, 1));
                        matrix->set(Imatrix::Element { .symbol = up.symbol, 
                                                       .condition = up.condition | Conditions::CHECKED_BIT }, 
                                    pos + Pos3(0, 0, 1)); 
                        trace_flag = false;
                    }
                break;
                case Directions::DIRECTION_DOWN:
                    if (down.symbol != '%' && ((down.condition & Conditions::OUT_OF_BOUNDS_BIT) != Conditions::OUT_OF_BOUNDS_BIT) && ((down.condition & Conditions::CHECKED_BIT) != Conditions::CHECKED_BIT)) { 
                        queue->push(Node3(pos + Pos3(0, 0, -1), wave + 1));
                        wave_matrix->set(Imatrix::Element { .symbol = down.symbol, 
                                                            .condition = static_cast<uint8_t>(wave + 1) },
                                         pos + Pos3(0, 0, -1));
                        matrix->set(Imatrix::Element { .symbol = down.symbol, 
                                                       .condition = down.condition | Conditions::CHECKED_BIT }, 
                                    pos + Pos3(0, 0, -1)); 
                        trace_flag = false;
                    }
                break;
                case Directions::DIRECTION_RIGHT:
                    if (right.symbol != '%' && ((right.condition & Conditions::OUT_OF_BOUNDS_BIT) != Conditions::OUT_OF_BOUNDS_BIT) && ((right.condition & Conditions::CHECKED_BIT) != Conditions::CHECKED_BIT)) { 
                        queue->push(Node3(pos + Pos3(1, 0, 0), wave + 1));
                        wave_matrix->set(Imatrix::Element { .symbol = right.symbol, 
                                                            .condition = static_cast<uint8_t>(wave + 1) },
                                         pos + Pos3(1, 0, 0));
                        matrix->set(Imatrix::Element { .symbol = right.symbol, 
                                                       .condition = right.condition | Conditions::CHECKED_BIT }, 
                                    pos + Pos3(1, 0, 0)); 
                        trace_flag = false;
                    }
                break;
                case Directions::DIRECTION_LEFT:
                    if (left.symbol != '%' && ((left.condition & Conditions::OUT_OF_BOUNDS_BIT) != Conditions::OUT_OF_BOUNDS_BIT) && ((left.condition & Conditions::CHECKED_BIT) != Conditions::CHECKED_BIT)) { 
                        queue->push(Node3(pos + Pos3(-1, 0, 0), wave + 1));
                        wave_matrix->set(Imatrix::Element { .symbol = left.symbol, 
                                                            .condition = static_cast<uint8_t>(wave + 1) },
                                         pos + Pos3(-1, 0, 0));
                        matrix->set(Imatrix::Element { .symbol = left.symbol, 
                                                       .condition = left.condition | Conditions::CHECKED_BIT }, 
                                    pos + Pos3(-1, 0, 0)); 
                        trace_flag = false;
                    }
                break;
                default:
                    std::runtime_error("WaveAlgorithm::Node2::get_neighbors : unknown order.");
                }
            };
             
            func(order & 0b111);
            func((order >> 3) & 0b111);
            func((order >> 6) & 0b111);
            func((order >> 9) & 0b111);
            func((order >> 12) & 0b111);
            func((order >> 15) & 0b111);

            return trace_flag;
        }


    };

    Imatrix* _matrix;
    Ipos*    _point_a;
    Ipos*    _point_b;
    uint32_t _order;

    uint32_t matrix2_solution() const;
    uint32_t matrix3_solution() const;

public:
    WaveAlgorithm(Imatrix* matrix, 
                  Ipos* point_a, 
                  Ipos* point_b, 
                  uint32_t order) : _matrix(matrix),
                                    _point_a(point_a), 
                                    _point_b(point_b),
                                    _order(order) {}
    WaveAlgorithm(Imatrix* matrix, 
                  uint32_t order) : _matrix(matrix),
                                    _order(order) {
        switch (_matrix->get_dimensions()) {
        case 2: {
            Matrix2 matrix2 = *static_cast<Matrix2*>(_matrix);
            for (uint32_t i = 0; i < matrix2.get_size().x; i++) {
                for (uint32_t j = 0; j < matrix2.get_size().y; j++) {
                    if (_point_a != nullptr) throw std::runtime_error("WaveAlgorithm::WaveAlgorithm : point a not found.");
                    if (_point_b != nullptr) throw std::runtime_error("WaveAlgorithm::WaveAlgorithm : point b not found.");
                    if (matrix2.get(i, j).symbol == 'A') _point_a = new Pos2(i, j);
                    if (matrix2.get(i, j).symbol == 'B') _point_b = new Pos2(i, j);
        }   }   } break; case 3: {
            Matrix3 matrix3 = *static_cast<Matrix3*>(_matrix);
            for (uint32_t i = 0; i < matrix3.get_size().x; i++) {
                for (uint32_t j = 0; j < matrix3.get_size().y; j++) {
                    for (uint32_t k = 0; k < matrix3.get_size().z; k++) {
                        if (_point_a != nullptr) throw std::runtime_error("WaveAlgorithm::WaveAlgorithm : point a not found.");
                        if (_point_b != nullptr) throw std::runtime_error("WaveAlgorithm::WaveAlgorithm : point b not found.");
                        if (matrix3.get(i, j, k).symbol == 'A') _point_a = new Pos3(i, j, k);
                        if (matrix3.get(i, j, k).symbol == 'B') _point_b = new Pos3(i, j, k);
        }   }   }   } break; default:
            throw std::runtime_error("WaveAlgorithm::WaveAlgorithm : unpredicted Matrix.");
        }
    }
   ~WaveAlgorithm() {
        delete _point_a;
        delete _point_b;
        delete _matrix;
    }

    [[nodiscard]] uint32_t initiate() {
        switch (_matrix->get_dimensions()) {
        case 2:
            return matrix2_solution();
        case 3:
            return matrix3_solution();
        default:
            throw std::runtime_error("WaveAlgorithm::initiate : unpredicted Matrix.");
    }   }
    
};

inline uint32_t WaveAlgorithm::matrix2_solution() const {
    Matrix2 matrix = *static_cast<Matrix2*>(_matrix);
    Pos2 point_a = *static_cast<Pos2*>(_point_a);
    Pos2 point_b = *static_cast<Pos2*>(_point_b);

    Imatrix::Element element_at_point_a = matrix.get(point_a);
    Imatrix::Element element_at_point_b = matrix.get(point_b);

    if (element_at_point_a.condition & Conditions::OUT_OF_BOUNDS_BIT) {
        std::cout << "Wrong input: point a is out of bounds." << std::endl;
        return Results::INVALID_INPUT;
    }
    if (element_at_point_b.condition & Conditions::OUT_OF_BOUNDS_BIT) {
        std::cout << "Wrong input: point b is out of bounds." << std::endl;
        return Results::INVALID_INPUT;
    }

    if (element_at_point_a.symbol == '#') {
        std::cout << "Wrong input: point a is colide with wall." << std::endl;
        return Results::INVALID_INPUT;
    }
    if (element_at_point_b.symbol == '#') {
        std::cout << "Wrong input: point b is colide with wall." << std::endl;
        return Results::INVALID_INPUT;
    }


    matrix.set(Imatrix::Element { .symbol = 'A',
                                   .condition = element_at_point_a.condition | Conditions::POINT_A_BIT | Conditions::CHECKED_BIT },
               point_a);
    matrix.set(Imatrix::Element { .symbol = 'B', 
                                   .condition = element_at_point_b.condition | Conditions::POINT_B_BIT }, 
               point_b);

    element_at_point_a = matrix.get(point_a);
    element_at_point_b = matrix.get(point_b);

    Matrix2 wave_matrix = Matrix2(matrix);

    wave_matrix.set(Imatrix::Element { .symbol = element_at_point_a.symbol, 
                                       .condition = 0 },
                    point_a);
    wave_matrix.set(Imatrix::Element { .symbol = element_at_point_b.symbol, 
                                       .condition = 0 },
                    point_b);
    
    std::queue<Node2> queue;

    queue.push(Node2(point_a, 0));

    while (!queue.empty()) {
        Node2 node = queue.front();

        if (node.get_neighbors(&matrix, &wave_matrix, _order, &queue, [](Matrix2* matrix, Pos2 pos) -> bool {
                return matrix->get(pos).condition & Conditions::POINT_B_BIT;
        })) break;

        queue.pop();
    }

    if (queue.empty()) {
        std::cout << "Path not found." << std::endl;
        return Results::PATH_NOT_FOUND;
    }

    Node2 final_node = queue.front();

    if (final_node.pos != point_b) {
        std::cout << "Path not found." << std::endl;
        return Results::PATH_NOT_FOUND;
    } 

    Node2 temp_node = final_node;

    auto func = [&](uint32_t order, uint32_t wave) -> bool {
        switch (order) {
        case Directions::DIRECTION_FORWARD:
            temp_node.pos = final_node.pos + Pos2(0, 1);
            return wave_matrix.get(final_node.pos + Pos2(0, -1)).condition == wave - 1;
        break;
        case Directions::DIRECTION_BACKWARD:
            temp_node.pos = final_node.pos + Pos2(0, -1);
            return wave_matrix.get(final_node.pos + Pos2(0, 1)).condition == wave - 1;
        break;
        case Directions::DIRECTION_RIGHT:
            temp_node.pos = final_node.pos + Pos2(1, 0);
            return wave_matrix.get(final_node.pos + Pos2(1, 0)).condition == wave - 1;
        break;
        case Directions::DIRECTION_LEFT:
            temp_node.pos = final_node.pos + Pos2(-1, 0);
            return wave_matrix.get(final_node.pos + Pos2(-1, 0)).condition == wave - 1;
        default:
            throw std::runtime_error("WaveAlgorithm::initiate : unknown order.");
        }   
    };

    for (uint32_t i = final_node.wave; i != 0; i--) {
        if (func(_order & 0b111, i)) {
            matrix.set(Imatrix::Element { .symbol = wave_matrix.get(final_node.pos).symbol, 
                                          .condition = wave_matrix.get(final_node.pos).condition | Conditions::MARK_AS_PATH_BIT },
                       final_node.pos);
            final_node.pos = temp_node.pos;
        } else if (func((_order >> 3) & 0b111, i)) {
            matrix.set(Imatrix::Element { .symbol = wave_matrix.get(final_node.pos).symbol, 
                                          .condition = wave_matrix.get(final_node.pos).condition | Conditions::MARK_AS_PATH_BIT },
                       final_node.pos);
            final_node.pos = temp_node.pos;
        } else if (func((_order >> 6) & 0b111, i)) {
            matrix.set(Imatrix::Element { .symbol = wave_matrix.get(final_node.pos).symbol, 
                                          .condition = wave_matrix.get(final_node.pos).condition | Conditions::MARK_AS_PATH_BIT },
                       final_node.pos);
            final_node.pos = temp_node.pos;
        } else if (func((_order >> 9) & 0b111, i)) {
            matrix.set(Imatrix::Element { .symbol = wave_matrix.get(final_node.pos).symbol, 
                                          .condition = wave_matrix.get(final_node.pos).condition | Conditions::MARK_AS_PATH_BIT },
                       final_node.pos);
            final_node.pos = temp_node.pos;
        } else {
            throw std::runtime_error("WaveAlgorithm::initiate : path not found.");
    }   }
    return Results::SUCCESS;
}

inline uint32_t WaveAlgorithm::matrix3_solution() const {
    Matrix3* matrix = static_cast<Matrix3*>(_matrix);
    Pos3 point_a = *static_cast<Pos3*>(_point_a);
    Pos3 point_b = *static_cast<Pos3*>(_point_b);

    Imatrix::Element element_at_point_a = matrix->get(point_a);
    Imatrix::Element element_at_point_b = matrix->get(point_b);

    if (element_at_point_a.condition & Conditions::OUT_OF_BOUNDS_BIT) {
        std::cout << "Wrong input: point a is out of bounds." << std::endl;
        return Results::INVALID_INPUT;
    }
    if (element_at_point_b.condition & Conditions::OUT_OF_BOUNDS_BIT) {
        std::cout << "Wrong input: point b is out of bounds." << std::endl;
        return Results::INVALID_INPUT;
    }

    if (element_at_point_a.symbol == '#') {
        std::cout << "Wrong input: point a is colide with wall." << std::endl;
        return Results::INVALID_INPUT;
    }
    if (element_at_point_b.symbol == '#') {
        std::cout << "Wrong input: point b is colide with wall." << std::endl;
        return Results::INVALID_INPUT;
    }

     matrix->set(Imatrix::Element { .symbol = 'A',
                                    .condition = element_at_point_a.condition | Conditions::POINT_A_BIT },
                 point_a);
     matrix->set(Imatrix::Element { .symbol = 'B', 
                                    .condition = element_at_point_b.condition | Conditions::POINT_B_BIT }, 
                 point_b);

    Matrix3 wave_matrix = Matrix3(*matrix);

    wave_matrix.set(Imatrix::Element { .symbol = element_at_point_a.symbol, 
                                       .condition = 0 },
                    point_a);
    wave_matrix.set(Imatrix::Element { .symbol = element_at_point_b.symbol, 
                                       .condition = 0 },
                    point_b);
            
    std::queue<Node3> queue;

    queue.push(Node3(point_a, 0));

    while (!queue.empty()) {
        Node3 node = queue.back();
        
        if (node.get_neighbors(matrix, &wave_matrix, _order, &queue, [](Matrix3* matrix, Pos3 pos) -> bool {
                return matrix->get(pos).condition & Conditions::POINT_B_BIT;
        })) break;

        queue.pop();
    }

    Node3 final_node = queue.back();

    if (final_node.pos != point_b) {
        std::cout << "Path not found." << std::endl;
        return Results::PATH_NOT_FOUND;
    }

    auto func = [&](uint32_t order, uint32_t wave) -> bool {
        switch (order) {
        case Directions::DIRECTION_FORWARD:
            return wave_matrix.get(final_node.pos + Pos3(0, -1, 0)).condition == wave - 1;
        break;
        case Directions::DIRECTION_BACKWARD:
            return wave_matrix.get(final_node.pos + Pos3(0, 1, 0)).condition == wave - 1;
        break;
        case Directions::DIRECTION_UP:
            return wave_matrix.get(final_node.pos + Pos3(0, 0, -1)).condition == wave - 1;
        break;
        case Directions::DIRECTION_DOWN:
            return wave_matrix.get(final_node.pos + Pos3(0, 0, 1)).condition == wave - 1;
        break;
        case Directions::DIRECTION_RIGHT:
            return wave_matrix.get(final_node.pos + Pos3(1, 0, 0)).condition == wave - 1;
        break;
        case Directions::DIRECTION_LEFT:
            return wave_matrix.get(final_node.pos + Pos3(-1, 0, 0)).condition == wave - 1;
        default:
            throw std::runtime_error("WaveAlgorithm::initiate : unknown order.");
        }   
    };

    for (uint32_t i = final_node.wave; i != 0; i--) {
        if (func(_order & 0b111, i)) {
            final_node.pos = final_node.pos + Pos3(0, -1, 0);
        } else if (func((_order >> 3) & 0b111, i)) {
            final_node.pos = final_node.pos + Pos3(0, 1, 0);
        } else if (func((_order >> 6) & 0b111, i)) {
            final_node.pos = final_node.pos + Pos3(0, 0, -1);
        } else if (func((_order >> 9) & 0b111, i)) {
            final_node.pos = final_node.pos + Pos3(0, 0, 1);
        } else if (func((_order >> 12) & 0b111, i)) {
            final_node.pos = final_node.pos + Pos3(1, 0, 0);
        } else if (func((_order >> 15) & 0b111, i)) {
            final_node.pos = final_node.pos + Pos3(-1, 0, 0);
        } else {
            throw std::runtime_error("WaveAlgorithm::initiate : path not found.");
    }   }
    return Results::SUCCESS;   
}

#endif // WAVEALGORITHMHPP