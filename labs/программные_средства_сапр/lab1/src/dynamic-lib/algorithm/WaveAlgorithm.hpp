#ifndef WAVEALGORITHMHPP
#define WAVEALGORITHMHPP

struct Pos2;
struct Pos3;

class WaveAlgorithm {
    struct Node2 {
        Pos2     pos;
        uint32_t wave;

        Node2() noexcept : pos(Pos2()), wave(0) {}
        Node2(Pos2 position, uint32_t wave) noexcept : pos(position), wave(wave) {}

        void get_neighbors(Matrix2* matrix, std::queue<Node2>* queue) const noexcept {
            Imatrix::Element up = matrix->get(pos + Pos2(0, 1));
            Imatrix::Element left = matrix->get(pos + Pos2(-1, 0));
            Imatrix::Element right = matrix->get(pos + Pos2(1, 0));
            Imatrix::Element down = matrix->get(pos + Pos2(0, -1));
            
            if (up.symbol != '%' && up.condition != CHECKED) { 
                queue->push(Node2(pos + Pos2(0, 1), wave + 1));
                matrix->set(Imatrix::Element { .symbol = up.symbol, .condition = CHECKED }, pos + Pos2(0, 1)); 
            }
            if (left.symbol != '%' && left.condition != CHECKED) { 
                queue->push(Node2(pos + Pos2(-1, 0), wave + 1));
                matrix->set(Imatrix::Element { .symbol = left.symbol, .condition = CHECKED }, pos + Pos2(-1, 0)); 
            }
            if (right.symbol != '%' && right.condition != CHECKED) { 
                queue->push(Node2(pos + Pos2(1, 0), wave + 1));
                matrix->set(Imatrix::Element { .symbol = right.symbol, .condition = CHECKED }, pos + Pos2(1, 0)); 
            }
            if (down.symbol != '%' && down.condition != CHECKED) { 
                queue->push(Node2(pos + Pos2(0, -1), wave + 1));
                matrix->set(Imatrix::Element { .symbol = down.symbol, .condition = CHECKED }, pos + Pos2(0, -1)); 
            }
        }
    };

    struct Node3 {
        Pos3     pos;

        Node3(Pos3 pos) noexcept : pos(pos) {}
    };

    Imatrix* _matrix;
    Ipos*    _point_a;
    Ipos*    _point_b;

public:
    WaveAlgorithm(Ipos* point_a, Ipos* point_b, Imatrix* matrix) : _point_a(point_a), 
                                                                   _point_b(point_b), 
                                                                   _matrix(matrix) {
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

            std::queue<Node2> queue;

            queue.push(Node2(point_a, 0));

            while (!queue.empty()) {
                Node2 node = queue.back();
                node.get_neighbors(matrix, &queue);

                queue.pop();
            }
        } case 3: {
            Node3** layers = new Node3*[1];
            Pos3 point_a = *static_cast<Pos3*>(_point_a);
            Pos3 point_b = *static_cast<Pos3*>(_point_b);

            std::queue<Node3> queue;

            while (true) {
                uint32_t count = 0;
            }
        } default: {
            std::runtime_error("WaveAlgorithm::initiate : unpredicted Matrix.");
        }   }

        return *this;
    }
};

#endif // WAVEALGORITHMHPP