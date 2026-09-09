#ifndef WAVEALGORITHMHPP
#define WAVEALGORITHMHPP

class WaveAlgorithm {
    struct Node2 {
        Pos2     pos;
        uint32_t wave;

        constexpr Node2() noexcept = default;
        constexpr ~Node2() noexcept = default;
        constexpr Node2(Pos2 position, uint32_t wave) noexcept : pos(position), wave(wave) {}

        bool get_neighbors(Matrix2* matrix, uint32_t order, std::queue<Node2>* queue) const {
            if (matrix->get(pos).condition & Conditions::POINT_B_BIT) return true;
            
            Imatrix::Element forward  = matrix->get(pos + Pos2( 0,  1));
            Imatrix::Element backward = matrix->get(pos + Pos2( 0, -1));
            Imatrix::Element right    = matrix->get(pos + Pos2( 1,  0));
            Imatrix::Element left     = matrix->get(pos + Pos2(-1,  0));

            auto func = [&](uint32_t order_byte) -> void {
                switch (order_byte) {
                case Directions::DIRECTION_FORWARD:
                    if (forward.symbol != '%' && forward.condition & (Conditions::OUT_OF_BOUNDS_BIT | Conditions::CHECKED_BIT)) { 
                        queue->push(Node2(pos + Pos2(0, 1), wave + 1));
                        matrix->set(Imatrix::Element { .symbol = forward.symbol, 
                                                       .condition = forward.condition | Conditions::CHECKED_BIT }, 
                                    pos + Pos2(0, 1));
                    }
                break;
                case Directions::DIRECTION_BACKWARD:
                    if (backward.symbol != '%' && backward.condition & (Conditions::OUT_OF_BOUNDS_BIT | Conditions::CHECKED_BIT)) { 
                        queue->push(Node2(pos + Pos2(0, -1), wave + 1));
                        matrix->set(Imatrix::Element { .symbol = backward.symbol, 
                                                       .condition = backward.condition | Conditions::CHECKED_BIT }, 
                                    pos + Pos2(0, -1)); 
                    }
                break;
                case Directions::DIRECTION_RIGHT:
                    if (right.symbol != '%' && right.condition & (Conditions::OUT_OF_BOUNDS_BIT | Conditions::CHECKED_BIT)) { 
                        queue->push(Node2(pos + Pos2(1, 0), wave + 1));
                        matrix->set(Imatrix::Element { .symbol = right.symbol, 
                                                       .condition = right.condition | Conditions::CHECKED_BIT }, 
                                    pos + Pos2(1, 0)); 
                    }
                break;
                case Directions::DIRECTION_LEFT:
                    if (left.symbol != '%' && left.condition & (Conditions::OUT_OF_BOUNDS_BIT | Conditions::CHECKED_BIT)) { 
                        queue->push(Node2(pos + Pos2(-1, 0), wave + 1));
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

        bool get_neighbors(Matrix3* matrix, uint32_t order, std::queue<Node3>* queue) const {
            if (matrix->get(pos).condition & Conditions::POINT_B_BIT) return true;
            
            Imatrix::Element forward  = matrix->get(pos + Pos3( 0,  1,  0));
            Imatrix::Element backward = matrix->get(pos + Pos3( 0, -1,  0));
            Imatrix::Element up       = matrix->get(pos + Pos3( 0,  0,  1));
            Imatrix::Element down     = matrix->get(pos + Pos3( 0,  0, -1));
            Imatrix::Element right    = matrix->get(pos + Pos3( 1,  0,  0));
            Imatrix::Element left     = matrix->get(pos + Pos3(-1,  0,  0));

            auto func = [&](uint32_t order_byte) -> void {
                switch (order_byte) {
                case Directions::DIRECTION_FORWARD:
                    if (forward.symbol != '%' && forward.condition & (Conditions::OUT_OF_BOUNDS_BIT | Conditions::CHECKED_BIT)) { 
                        queue->push(Node3(pos + Pos3(0, 1, 0), wave + 1));
                        matrix->set(Imatrix::Element { .symbol = forward.symbol, 
                                                       .condition = forward.condition | Conditions::CHECKED_BIT }, 
                                    pos + Pos3(0, 1, 0));
                    }
                break;
                case Directions::DIRECTION_BACKWARD:
                    if (backward.symbol != '%' && backward.condition & (Conditions::OUT_OF_BOUNDS_BIT | Conditions::CHECKED_BIT)) { 
                        queue->push(Node3(pos + Pos3(0, -1, 0), wave + 1));
                        matrix->set(Imatrix::Element { .symbol = backward.symbol, 
                                                       .condition = backward.condition | Conditions::CHECKED_BIT }, 
                                    pos + Pos3(0, -1, 0)); 
                    }
                break;
                case Directions::DIRECTION_UP:
                    if (up.symbol != '%' && up.condition & (Conditions::OUT_OF_BOUNDS_BIT | Conditions::CHECKED_BIT)) { 
                        queue->push(Node3(pos + Pos3(0, 0, 1), wave + 1));
                        matrix->set(Imatrix::Element { .symbol = up.symbol, 
                                                       .condition = up.condition | Conditions::CHECKED_BIT }, 
                                    pos + Pos3(0, 0, 1)); 
                    }
                break;
                case Directions::DIRECTION_DOWN:
                    if (down.symbol != '%' && down.condition & (Conditions::OUT_OF_BOUNDS_BIT | Conditions::CHECKED_BIT)) { 
                        queue->push(Node3(pos + Pos3(0, 0, -1), wave + 1));
                        matrix->set(Imatrix::Element { .symbol = down.symbol, 
                                                       .condition = down.condition | Conditions::CHECKED_BIT }, 
                                    pos + Pos3(0, 0, -1)); 
                    }
                break;
                case Directions::DIRECTION_RIGHT:
                    if (right.symbol != '%' && right.condition & (Conditions::OUT_OF_BOUNDS_BIT | Conditions::CHECKED_BIT)) { 
                        queue->push(Node3(pos + Pos3(1, 0, 0), wave + 1));
                        matrix->set(Imatrix::Element { .symbol = right.symbol, 
                                                       .condition = right.condition | Conditions::CHECKED_BIT }, 
                                    pos + Pos3(1, 0, 0)); 
                    }
                break;
                case Directions::DIRECTION_LEFT:
                    if (left.symbol != '%' && left.condition & (Conditions::OUT_OF_BOUNDS_BIT | Conditions::CHECKED_BIT)) { 
                        queue->push(Node3(pos + Pos3(-1, 0, 0), wave + 1));
                        matrix->set(Imatrix::Element { .symbol = left.symbol, 
                                                       .condition = left.condition | Conditions::CHECKED_BIT }, 
                                    pos + Pos3(-1, 0, 0)); 
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

            return false;
        }
    };

    Imatrix* _matrix;
    Ipos*    _point_a;
    Ipos*    _point_b;
    uint32_t _order;

public:
    WaveAlgorithm(Imatrix* matrix, 
                  Ipos* point_a, 
                  Ipos* point_b, 
                  uint32_t order) : _matrix(matrix),
                                    _point_a(point_a), 
                                    _point_b(point_b),
                                    _order(order) {
        assert(matrix != nullptr);
        assert((dynamic_cast<Matrix2*>(matrix) != nullptr) || (dynamic_cast<Matrix3*>(matrix) != nullptr));
    }
   ~WaveAlgorithm() {
        delete _point_a;
        delete _point_b;
        delete _matrix;
    }

    [[nodiscard]] WaveAlgorithm initiate() {
        switch (_matrix->get_dimensions()) {
        case 2: {
            Matrix2* matrix = static_cast<Matrix2*>(_matrix);
            Pos2 point_a = *static_cast<Pos2*>(_point_a);
            Pos2 point_b = *static_cast<Pos2*>(_point_b);

            Imatrix::Element element_at_point_a = matrix->get(point_a);
            Imatrix::Element element_at_point_b = matrix->get(point_b);

            if (element_at_point_a.condition & Conditions::OUT_OF_BOUNDS_BIT) throw std::runtime_error("WaveAlgorithm::initiate : point a is out of bounds."); // TODO: repeate input
            if (element_at_point_b.condition & Conditions::OUT_OF_BOUNDS_BIT) throw std::runtime_error("WaveAlgorithm::initiate : point b is out of bounds."); // TODO: repeate input

            if (element_at_point_a.symbol == '#') throw std::runtime_error("WaveAlgorithm::initiate : point a is colide with wall."); // TODO: repeate input
            if (element_at_point_b.symbol == '#') throw std::runtime_error("WaveAlgorithm::initiate : point b is colide with wall."); // TODO: repeate input

            matrix->set(Imatrix::Element { .symbol = 'A',
                                           .condition = element_at_point_a.condition | Conditions::POINT_A_BIT },
                        point_a);
            matrix->set(Imatrix::Element { .symbol = 'B', 
                                           .condition = element_at_point_b.condition | Conditions::POINT_B_BIT }, 
                        point_b);

            std::queue<Node2> queue;

            queue.push(Node2(point_a, 0));

            while (!queue.empty()) {
                Node2 node = queue.front();
                if (node.get_neighbors(matrix, _order, &queue)) break;

                queue.pop();
            }
        } case 3: {
            Matrix3* matrix = static_cast<Matrix3*>(_matrix);
            Pos3 point_a = *static_cast<Pos3*>(_point_a);
            Pos3 point_b = *static_cast<Pos3*>(_point_b);

            Imatrix::Element element_at_point_a = matrix->get(point_a);
            Imatrix::Element element_at_point_b = matrix->get(point_b);

            if (element_at_point_a.condition & Conditions::OUT_OF_BOUNDS_BIT) throw std::runtime_error("WaveAlgorithm::initiate : point a is out of bounds."); // TODO: repeate input
            if (element_at_point_b.condition & Conditions::OUT_OF_BOUNDS_BIT) throw std::runtime_error("WaveAlgorithm::initiate : point b is out of bounds."); // TODO: repeate input

            if (element_at_point_a.symbol == '#') throw std::runtime_error("WaveAlgorithm::initiate : point a is colide with wall."); // TODO: repeate input
            if (element_at_point_b.symbol == '#') throw std::runtime_error("WaveAlgorithm::initiate : point b is colide with wall."); // TODO: repeate input

            matrix->set(Imatrix::Element { .symbol = 'A',
                                           .condition = element_at_point_a.condition | Conditions::POINT_A_BIT },
                        point_a);
            matrix->set(Imatrix::Element { .symbol = 'B', 
                                           .condition = element_at_point_b.condition | Conditions::POINT_B_BIT }, 
                        point_b);

            std::queue<Node3> queue;

            queue.push(Node3(point_a, 0));

            while (!queue.empty()) {
                Node3 node = queue.back();
                if (node.get_neighbors(matrix, _order, &queue)) break;

                queue.pop();
            }
        } default: {
            std::runtime_error("WaveAlgorithm::initiate : unpredicted Matrix.");
        }   }

        return *this;
    }
};

#endif // WAVEALGORITHMHPP