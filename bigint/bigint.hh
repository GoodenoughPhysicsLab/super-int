#if !defined(__cplusplus) || !defined(__cpp_concepts)
#   error "C++20 is required"
#endif

#include <cstdint>
#include <cstdlib>
#include <cassert>
#include <type_traits>

namespace si {

template<typename data_type = uintmax_t>
    requires ::std::is_unsigned_v<data_type>
class BigInt {
    using signed_data_type = typename ::std::make_signed_t<data_type>;

    signed_data_type size;
    data_type* data;
public:
    BigInt() noexcept : size(0), data(nullptr) {}

    BigInt(signed_data_type num) noexcept {
        // TODO
    }

    ~BigInt() noexcept {
        free(this->data);
    }

    BigInt(const BigInt& other) noexcept {
        this->size = other.size;
        this->data = new data_type[this->size];
    }

    BigInt& operator>>(data_type num) noexcept {
        assert(num && this->data);

        data_type is_odd = 0;
        for (intmax_t i = this->size - 1; i >= 0; --i) {
            data_type is_odd_temp = this->data[i] & 1;
            this->data[i] >>= 1;
            this->data[i] |= is_odd << (sizeof(data_type) * 8 - 1);
            is_odd = is_odd_temp;
        }

        return *this;
    }

    constexpr bool is_NaN() const noexcept {
        return this->data == nullptr;
    }

};

}