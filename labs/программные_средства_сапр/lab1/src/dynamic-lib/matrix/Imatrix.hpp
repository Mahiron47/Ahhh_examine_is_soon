#ifndef IMATRIXHPP
#define IMATRIXHPP

class Imatrix {
public:
    struct Element {
        char    symbol;
        uint8_t condition;
    };

protected:
    virtual Element get_element(std::span<const uint32_t>) const = 0;
    virtual void set_element(Element, std::span<const uint32_t>) = 0;

public:
    virtual ~Imatrix() noexcept = default;

    virtual void print() const noexcept = 0;
    virtual uint8_t get_dimensions() const noexcept = 0;
    
    template<typename... Args> requires (std::convertible_to<Args, uint32_t> && ...)
    Element get(Args... args) const {
        const uint32_t coords[] = { static_cast<uint32_t>(args)... };
        return this->get_element(coords);
    }
    template<typename... Args> requires (std::convertible_to<Args, uint32_t> && ...)
    void set(Element elem, Args... args) {
        const uint32_t coords[] = { static_cast<uint32_t>(args)... };
        this->set_element(elem, coords);   
    }
};

#endif // IMATRIXHPP