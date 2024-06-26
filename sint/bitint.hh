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

/*
 * @brief BitInt
 * @tparam N: bit size
 * @tparam check: checking ubyte value is overflow or not
 */
template<uint8_t N = 4, bool check = true>
class BitInt {
public:
    using ubyte =
        ::std::conditional_t<0 < N && N < 8, uint_least8_t,
            ::std::conditional_t<N < 16, uint_least16_t,
                ::std::conditional_t<N < 32, uint_least32_t, uint_least64_t>
            >
        >;
    using byte = ::std::make_signed_t<ubyte>;

    static_assert(N <= sizeof(ubyte) * 8, "N is too large");

    static constexpr uint8_t bit_size = N;
    static constexpr ubyte max_num = detail::bitint_max_num<ubyte>(N);

private:
    bool is_neg{false};
    ubyte _value{}; // Whether negative or not, _value is the original code

public:
    constexpr BitInt() noexcept = default;

    /* Note: BitInt(0xff) will be interpreted as -1 instead of error
     *
     *       We strongly recommend to use BitInt{0xff} to let compiler throw error
     */
    constexpr BitInt(const intmax_t value) noexcept {
        if constexpr (check) {
            assert(value <= max_num);

            this->is_neg = value < 0;
            this->_value = static_cast<ubyte>((value < 0? -value : value) & max_num);
        } else {
            //
        }
    }

    constexpr BitInt(const BitInt& other) noexcept {
        this->_value = other._value;
    }

    BitInt& operator=(const intmax_t value) noexcept {
        assert(value <= max_num);

        this->is_neg = value < 0;
        this->_value = static_cast<ubyte>((this->is_neg? -value : value) & max_num);
        return *this;
    }

    constexpr BitInt& operator=(const BitInt& other) noexcept {
        this->is_neg = other.is_neg;
        this->_value = other._value;
        return *this;
    }

    constexpr BitInt operator+(const uintmax_t value) const noexcept {
        ubyte _value = static_cast<ubyte>(value & max_num);

        BitInt<N> res;
        if (this->is_neg) {
            if (_value - this->_value < 0) {
                res._value = (this->_value - _value) & max_num;
                res.is_neg = true;
                return res;
            }
            res._value = (_value - this->_value) & max_num;
        } else {
            res._value = (_value + this->_value) & max_num;
        }

        return res;
    }

    constexpr BitInt operator+(const BitInt& other) const noexcept {
        return *this + static_cast<ubyte>(other._value & max_num);
    }

    constexpr BitInt operator-(const uintmax_t value) const noexcept {
        ubyte _value = static_cast<ubyte>(value & max_num);
        BitInt<N> res;

        if (this->is_neg) {
            res.is_neg = true;
            res._value = (this->_value + _value) & max_num;
        } else {
            res.is_neg = this->_value - _value < 0;
            if (res.is_neg) {
                res._value = (_value - this->_value) & max_num;
            } else {
                res._value = (this->_value - _value) & max_num;
            }
       }

        return res;
    }

    constexpr BitInt operator-(const BitInt& other) noexcept {
        return BitInt(this->_value - other._value);
    }

    constexpr BitInt operator*(const ubyte value) noexcept {
        return BitInt(this->_value * value);
    }

    constexpr BitInt operator*(const BitInt& other) noexcept {
        return BitInt(this->_value * other._value);
    }

    constexpr BitInt operator/(const ubyte value) noexcept {
        return BitInt(this->_value / value);
    }

    constexpr BitInt operator/(const BitInt& other) noexcept {
        return BitInt(this->_value / other._value);
    }

    constexpr BitInt operator%(const ubyte value) noexcept {
        return BitInt(this->_value % value);
    }

    constexpr BitInt operator%(const BitInt& other) noexcept {
        return BitInt(this->_value % other._value);
    }

    constexpr BitInt& operator+=(const ubyte value) noexcept {
        this->_value = (this->_value + value) & max_num;
        return *this;
    }

    constexpr BitInt& operator+=(const BitInt& other) noexcept {
        this->_value = (this->_value + other._value) & max_num;
        return *this;
    }

    constexpr BitInt& operator-=(const ubyte value) noexcept {
        this->_value = (this->_value - value) & max_num;
        return *this;
    }

    constexpr BitInt& operator-=(const BitInt& other) noexcept {
        this->_value = (this->_value - other._value) & max_num;
        return *this;
    }

    constexpr BitInt& operator*=(const ubyte value) noexcept {
        this->_value = (this->_value * value) & max_num;
        return *this;
    }

    constexpr BitInt& operator*=(const BitInt& other) noexcept {
        this->_value = (this->_value * other._value) & max_num;
        return *this;
    }

    constexpr BitInt& operator/=(const ubyte value) noexcept {
        this->_value = (this->_value / value) & max_num;
        return *this;
    }

    constexpr BitInt& operator/=(const BitInt& other) noexcept {
        this->_value = (this->_value / other._value) & max_num;
        return *this;
    }

    constexpr BitInt& operator%=(const ubyte value) noexcept {
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

    constexpr BitInt operator~() const noexcept {
        BitInt<N> res;
        res.is_neg = !this->is_neg;
        res._value = ~this->_value;

        return res;
    }

    constexpr bool operator==(const intmax_t value) const noexcept {
        if (!this->is_neg && value >= 0 || this->is_neg && value < 0) {
            ubyte _value = static_cast<ubyte>((value < 0? -value : value) & max_num);
            return this->_value == _value;
        }
        return false;
    }

    constexpr bool operator==(const BitInt& other) const noexcept {
        return this->_value == other._value && this->is_neg == other.is_neg;
    }

    constexpr bool operator!=(const ubyte value) const noexcept {
        return this->_value != (value & max_num);
    }

    constexpr bool operator!=(const BitInt& other) const noexcept {
        return this->_value != (other._value & max_num);
    }

    constexpr bool operator<(const ubyte value) const noexcept {
        return this->_value < value;
    }

    constexpr bool operator<(const BitInt& other) const noexcept {
        return this->_value < (other._value & max_num);
    }

    constexpr bool is_negtive() const noexcept {
        return this->is_neg;
    }

    constexpr auto num() const noexcept {
        return this->_value;
    }

    constexpr BitInt abs() const noexcept {
        if (this->_value < 0) {
            return BitInt<N>{-this->_value};
        }
        return *this;
    }

    constexpr BitInt complement() const noexcept {
        return BitInt<N>{(~this->_value + 1) & max_num};
    }

    //TODO support methods in bitset

    friend std::ostream& operator<<(std::ostream& out, const BitInt& other) noexcept {
        if (other.is_neg) {
            return out << "-" << (intmax_t)other._value;
        }
        return out << (intmax_t)other._value;
    }
};

template<uint8_t N>
constexpr BitInt<N> operator+(const uintmax_t value, const BitInt<N>& other) noexcept {
    return other + value;
}

} // namespace si