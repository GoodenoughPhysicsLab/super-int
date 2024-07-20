#pragma once

#include <limits>
#include <cstdint>
#include <cassert>
#include <iostream>
#include <ostream>
#include <type_traits>

namespace si {

using bitsize_type = uint16_t;

/*
 * @brief BitInt
 * @tparam N: bit size, don't include sign
 *
 * example: BitInt<7> <==> int8_t
 */
template<bitsize_type N>
class BitInt {

private:
    template<typename T>
    static constexpr T bitint_max_num() {
        T res{};

        for (bitsize_type i = 0; i < N; ++i) {
            res |= (static_cast<T>(1) << i);
        }
        return res;
    }

    template<typename T>
    static constexpr T bitint_min_num() {
        T min_num{};
        if constexpr (N != sizeof(int*) * 8 - 1) {
            min_num = -(static_cast<T>(1) << N);
        } else {
            min_num = std::numeric_limits<T>::min();
        }
        return min_num;
    }

public:
    using ubyte =
        ::std::conditional_t<0 < N && N < 8, uint_least8_t,
            ::std::conditional_t<N < 16, uint_least16_t,
#ifdef INT64_MAX // 64 bit
                ::std::conditional_t<N < 32, uint_least32_t, uint_least64_t>
#else // 32 bit
                uint_least32_t
#endif
            >
        >;
    using byte = ::std::make_signed_t<ubyte>;

    static_assert(N < sizeof(ubyte) * 8, "N is too large");

    static constexpr bitsize_type bit_size = N;
    static constexpr ubyte max_num = bitint_max_num<ubyte>();
    static constexpr byte min_num = bitint_min_num<byte>();

private:
    bool is_neg{false};
    /*
     * Whether negative or not, _value is the original code
     *
     * except "-0", which means the min of BitInt
     */
    ubyte _value{};

public:
    /* return true if self is the min of BitInt */
    constexpr bool is_min() const noexcept {
        return this->is_neg && this->_value == 0;
    }

public:
    constexpr BitInt() noexcept = default;

    constexpr BitInt(const intmax_t value) noexcept {
        if (value == min_num) {
            this->is_neg = true;
            this ->_value = 0;
        } else {
            this->is_neg = value < 0;
            this->_value = static_cast<ubyte>((value < 0? -value : value) & max_num);
        }
    }

    template<bitsize_type M>
    constexpr BitInt(const BitInt<M>& other) noexcept {
        this->is_neg = other.is_negtive();
        this->_value = other.num() & max_num;
    }

    // no need to write `BitInt(BitInt&&)`

    BitInt& operator=(const intmax_t value) noexcept {
        if (value == min_num) {
            this->is_neg = true;
            this ->_value = 0;
        } else {
            this->is_neg = value < 0;
            this->_value = static_cast<ubyte>((value < 0? -value : value) & max_num);
        }
        return *this;
    }

    BitInt& operator=(const BitInt& other) noexcept {
        this->is_neg = other.is_neg;
        this->_value = other._value;
        return *this;
    }

    // TODO support -0(min of BitInt)
    constexpr BitInt operator+(const uintmax_t value) const noexcept {
        ubyte _value = static_cast<ubyte>(value & max_num);

        BitInt<N> res;
        if (this->is_neg) {
            if (_value - this->_value < 0) {
                res._value = this->_value - _value;
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
        if (other.is_neg) {
            return *this - other;
        }
        return *this + static_cast<ubyte>(other._value & max_num);
    }

    BitInt& operator++() noexcept {
        if (!this->is_neg && this->_value == max_num) [[unlikely]] {
            // -0 means the min of BitInt: -pow(2, N)
            this->is_neg = true;
            this->_value = 0;
        } else if (this->is_min()) [[unlikely]] {
            this->_value = max_num;
        } else if (this->is_neg && this->_value == 1) [[unlikely]] {
            this->is_neg = false;
            this->_value = 0;
        } else if (!this->is_neg) [[likely]] {
            this->_value = (this->_value + 1) & max_num;
        } else { // this->is_neg
            this->_value = (this->_value - 1) & max_num;
        }

        return *this;
    }

    BitInt operator++(int) noexcept {
        BitInt tmp{*this};
        ++(*this);
        return tmp;
    }

    constexpr BitInt operator-() const noexcept {
        return BitInt<N>{-this->_value};
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

    constexpr BitInt operator-(const BitInt& other) const noexcept {
        return *this - static_cast<ubyte>(other._value & max_num);
    }

    BitInt& operator--() noexcept {
        this->_value = (this->_value - 1) & max_num;
        return *this;
    }

    BitInt operator--(int) noexcept {
        BitInt tmp = *this;
        this->_value = (this->_value - 1) & max_num;
        return tmp;
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

    /*
     * because BitInt NEVER use complement code
     * therefore, res equals to origin code of inverting code
     */
    constexpr BitInt operator~() const noexcept {
        BitInt<N> res;
        res.is_neg = !this->is_neg;
        res._value = (this->_value + 1) & max_num;

        return res;
    }

    constexpr bool operator==(const intmax_t value) const noexcept {
        if (this->is_min()) {
            return value == min_num;
        }
        if (!this->is_neg && value >= 0 || this->is_neg && value < 0) {
            ubyte _value = static_cast<ubyte>((value < 0? -value : value) & max_num);;
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
        if (other.is_min()) {
            return out << static_cast<intmax_t>(min_num);
        }
        if (other.is_neg) {
            return out << "-" << static_cast<intmax_t>(other._value);
        }
        return out << static_cast<intmax_t>(other._value);
    }
};

template<bitsize_type N>
constexpr BitInt<N> operator+(const uintmax_t value, const BitInt<N>& other) noexcept {
    return other + value;
}

template<bitsize_type N>
constexpr BitInt<N> operator-(const uintmax_t value, const BitInt<N>& other) noexcept {
    return -other + value;
}


/* unsigned BitInt */
template<bitsize_type N>
class uBitInt {
public:
    using ubyte = typename BitInt<N>::ubyte;
    static constexpr ubyte max_num = BitInt<N>::max_num;
    static constexpr ubyte min_num = 0;

private:
    ubyte _value{};

public:
    constexpr uBitInt() noexcept = default;

    constexpr uBitInt(const uintmax_t value) noexcept {
        this->_value = value & max_num;
    }

    template<bitsize_type M>
    constexpr uBitInt(const BitInt<M>& other) noexcept {
        this->_value = other.num() & max_num;
    }

    uBitInt& operator=(const uintmax_t value) noexcept {
        this->_value = value & max_num;
        return *this;
    }

    template<bitsize_type M>
    uBitInt& operator=(const BitInt<M>& other) noexcept {
        this->_value = other.num() & max_num;
        return *this;
    }

    constexpr uBitInt operator+(const intmax_t value) const noexcept {
        return uBitInt(static_cast<intmax_t>(value + this->_value));
    }

    template<bitsize_type M>
    constexpr uBitInt operator+(const BitInt<M>& other) const noexcept {
        return uBitInt(other.num() + this->_value);
    }

    constexpr uBitInt operator-(const intmax_t value) const noexcept {
        return uBitInt(static_cast<intmax_t>(this->_value - value));
    }

    template<bitsize_type M>
    constexpr uBitInt operator-(const BitInt<M>& other) const noexcept {
        return uBitInt(this->_value - other.num());
    }

    constexpr bool operator==(const ubyte value) const noexcept {
        return this->_value == value;
    }

    template<bitsize_type M>
    constexpr bool operator==(const BitInt<M>& other) const noexcept {
        return this->_value == other.num();
    }

    constexpr auto num() const noexcept {
        return this->_value;
    }
};

} // namespace si