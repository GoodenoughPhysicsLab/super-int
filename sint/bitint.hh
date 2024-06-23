#ifndef __cpp_constinit
#   error "do not use c++20"
#endif

#include <cstdint>
#include <cassert>
#include <iostream>
#include <ostream>
#include <type_traits>

namespace si::detail {

template<typename T>
constexpr T bitint_max_num(uint8_t n) {
    T res{};

    for (uint8_t i = 0; i < n; ++i) {
        res |= (static_cast<intmax_t>(1) << i);
    }
    return res;
}

} // namespace si::detail

namespace si {

template<uint8_t N = 4>
class BitInt {
public:
    using byte =
        ::std::conditional_t<0 < N && N < 8, int_least8_t,
            ::std::conditional_t<N < 16, int_least16_t,
                ::std::conditional_t<N < 32, int_least32_t, int_least64_t>
            >
        >;
    static constexpr uint8_t bit_size = N;
    static constexpr byte max_num = detail::bitint_max_num<byte>(N);

private:
    byte _value{};

public:
    constexpr BitInt() noexcept = default;

    /* Note: BitInt(0xff) will be interpreted as -1 instead of
     *
     *       We strongly recommend to use BitInt{0xff} to let compiler throw error
     */
    constexpr BitInt(const byte value) noexcept {
        assert(value <= max_num);
        this->_value = value;
    }

    constexpr BitInt(const BitInt& other) noexcept {
        this->_value = other._value;
    }

    constexpr BitInt(BitInt&& other) noexcept {
        this->_value = other._value;
    }

    constexpr BitInt& operator=(const byte value) noexcept {
        assert(value <= max_num);
        this->_value = value;
        return *this;
    }

    constexpr BitInt& operator=(const BitInt& other) noexcept {
        this->_value = other._value;
        return *this;
    }

    constexpr BitInt operator+(const byte value) noexcept {
        assert(value <= max_num);
        // bool _value_sign = this->_value < 0;
        // bool value_sign = value < 0;
        auto res_v = this->_value + value;
        return BitInt<N>{res_v};
    }

    template<uint8_t U>
    constexpr BitInt& operator+(BitInt<U>& other) noexcept {
        this->_value = this->operator+(other._value);
        return *this;
    }

    BitInt& operator+(const BitInt& other) {}

    constexpr BitInt operator-(const byte value) noexcept {
        return BitInt(this->_value - value);
    }

    constexpr BitInt operator-(const BitInt& other) noexcept {
        return BitInt(this->_value - other._value);
    }

    constexpr BitInt operator*(const byte value) noexcept {
        return BitInt(this->_value * value);
    }

    constexpr BitInt operator*(const BitInt& other) noexcept {
        return BitInt(this->_value * other._value);
    }

    constexpr BitInt operator/(const byte value) noexcept {
        return BitInt(this->_value / value);
    }

    constexpr BitInt operator/(const BitInt& other) noexcept {
        return BitInt(this->_value / other._value);
    }

    constexpr BitInt operator%(const byte value) noexcept {
        return BitInt(this->_value % value);
    }

    constexpr BitInt operator%(const BitInt& other) noexcept {
        return BitInt(this->_value % other._value);
    }

    constexpr BitInt& operator+=(const byte value) noexcept {
        this->_value = (this->_value + value) & max_num;
        return *this;
    }

    constexpr BitInt& operator+=(const BitInt& other) noexcept {
        this->_value = (this->_value + other._value) & max_num;
        return *this;
    }

    constexpr BitInt& operator-=(const byte value) noexcept {
        this->_value = (this->_value - value) & max_num;
        return *this;
    }

    constexpr BitInt& operator-=(const BitInt& other) noexcept {
        this->_value = (this->_value - other._value) & max_num;
        return *this;
    }

    constexpr BitInt& operator*=(const byte value) noexcept {
        this->_value = (this->_value * value) & max_num;
        return *this;
    }

    constexpr BitInt& operator*=(const BitInt& other) noexcept {
        this->_value = (this->_value * other._value) & max_num;
        return *this;
    }

    constexpr BitInt& operator/=(const byte value) noexcept {
        this->_value = (this->_value / value) & max_num;
        return *this;
    }

    constexpr BitInt& operator/=(const BitInt& other) noexcept {
        this->_value = (this->_value / other._value) & max_num;
        return *this;
    }

    constexpr BitInt& operator%=(const byte value) noexcept {
        this->_value = (this->_value % value) & max_num;
        return *this;
    }

    constexpr BitInt& operator%=(const BitInt& other) noexcept {
        this->_value = (this->_value % other._value) & max_num;
        return *this;
    }

    constexpr BitInt& operator++() noexcept {
        this->_value = (this->_value + 1) & max_num;
        return *this;
    }

    constexpr BitInt operator++(int) noexcept {
        BitInt tmp = *this;
        this->_value = (this->_value + 1) & max_num;
        return tmp; // can I move it instead of copy it
    }

    constexpr BitInt& operator--() noexcept {
        this->_value = (this->_value - 1) & max_num;
        return *this;
    }

    constexpr BitInt operator--(int) noexcept {
        BitInt tmp = *this;
        this->_value = (this->_value - 1) & max_num;
        return tmp;
    }

    constexpr bool operator==(const byte value) const noexcept {
        return this->_value == value;
    }

    constexpr bool operator==(const BitInt& other) const noexcept {
        return this->_value == other._value;
    }

    constexpr bool operator!=(const byte value) const noexcept {
        return this->_value != (value & max_num);
    }

    constexpr bool operator!=(const BitInt& other) const noexcept {
        return this->_value != (other._value & max_num);
    }

    constexpr bool operator<(const byte value) const noexcept {
        return this->_value < value;
    }

    constexpr bool operator<(const BitInt& other) const noexcept {
        return this->_value < (other._value & max_num);
    }

    constexpr auto num() const noexcept {
        return this->_value;
    }

    constexpr BitInt& abs() noexcept {
        if (this->_value < 0) {
            this->_value = -this->_value;
        }
        return *this;
    }

    //TODO support methods in bitset

    friend std::ostream& operator<<(std::ostream& out, const BitInt& a) noexcept {
        return out << (int)a._value;
    }
};

} // namespace si