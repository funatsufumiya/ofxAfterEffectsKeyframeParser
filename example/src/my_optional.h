
#if __cplusplus >= 201703L
#if __has_include(<optional>)

#define USE_STD_OPTIONAL
#include <optional>

#endif // __has_include(<optional>)
#endif // __cplusplus >= 201703L

namespace opt {

#ifdef USE_STD_OPTIONAL
    using std::optional;

    static const std::nullopt_t nullopt = std::nullopt;
#else

    struct nullopt_t {};
    static const nullopt_t nullopt = nullopt_t();

    template<typename T>
    class optional {
    public:
        optional() : has_value_(false) {}
        optional(const T& value) : value_(value), has_value_(true) {}
        optional(nullopt_t) : has_value_(false) {}

        void reset() { has_value_ = false; }
        void set(const T& value) { value_ = value; has_value_ = true; }
        bool has_value() const { return has_value_; }
        T& value() { return value_; }
        const T& value() const { return value_; }
        explicit operator bool() const { return has_value_; }
        T& operator*() { return value_; }
        const T& operator*() const { return value_; }

    private:
        T value_;
        bool has_value_;
    };

#endif // USE_STD_OPTIONAL

} // namespace opt