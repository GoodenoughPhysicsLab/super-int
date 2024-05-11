#include <cstddef>
#include <cstdint>
#include <iostream>
#include <ostream>
#include <type_traits>

namespace si {

template<::std::size_t N = 4>
class BitInt {
    static_assert(N > 0 && N < 8);
    using byte = uint8_t;
    static constexpr int bit_size = N;
    static constexpr int max_num = (1 << bit_size) - 1;
    byte value;
public:
    template<typename T, typename ::std::enable_if_t<
        ::std::is_unsigned_v<T> && ::std::is_integral_v<::std::remove_cv_t<::std::remove_reference_t<T>>>>* = nullptr
    >
    constexpr BitInt(const T value) noexcept {
        this->value = value & max_num;
    }

    constexpr BitInt(const intmax_t value) noexcept {
        this->value = abs(value) & max_num;
    }

    constexpr BitInt(const BitInt& other) noexcept {
        this->value = other.value;
    }

    constexpr BitInt(BitInt&& other) noexcept {
        this->value = other.value;
    }

    constexpr BitInt& operator=(const uintmax_t value) noexcept {
        this->value = value & max_num;
        return *this;
    }

    constexpr BitInt& operator=(const BitInt& other) noexcept {
        this->value = other.value;
        return *this;
    }

    constexpr BitInt operator+(const byte value) const noexcept {
        return BitInt(this->value + value);
    }

    constexpr BitInt operator+(const BitInt& other) const noexcept {
        return BitInt(this->value + other.value);
    }

    constexpr BitInt operator-(const byte value) const noexcept {
        return BitInt(this->value - value);
    }

    constexpr BitInt operator-(const BitInt& other) const noexcept {
        return BitInt(this->value - other.value);
    }

    constexpr BitInt operator*(const byte value) const noexcept {
        return BitInt(this->value * value);
    }

    constexpr BitInt operator*(const BitInt& other) const noexcept {
        return BitInt(this->value * other.value);
    }

    constexpr BitInt operator/(const byte value) const noexcept {
        return BitInt(this->value / value);
    }

    constexpr BitInt operator/(const BitInt& other) const noexcept {
        return BitInt(this->value / other.value);
    }

    constexpr BitInt operator%(const byte value) const noexcept {
        return BitInt(this->value % value);
    }

    constexpr BitInt operator%(const BitInt& other) const noexcept {
        return BitInt(this->value % other.value);
    }

    constexpr BitInt& operator+=(const byte value) noexcept {
        this->value = (this->value + value) & max_num;
        return *this;
    }

    constexpr BitInt& operator+=(const BitInt& other) noexcept {
        this->value = (this->value + other.value) & max_num;
        return *this;
    }

    constexpr BitInt& operator-=(const byte value) noexcept {
        this->value = (this->value - value) & max_num;
        return *this;
    }

    constexpr BitInt& operator-=(const BitInt& other) noexcept {
        this->value = (this->value - other.value) & max_num;
        return *this;
    }

    constexpr BitInt& operator*=(const byte value) noexcept {
        this->value = (this->value * value) & max_num;
        return *this;
    }

    constexpr BitInt& operator*=(const BitInt& other) noexcept {
        this->value = (this->value * other.value) & max_num;
        return *this;
    }

    constexpr BitInt& operator/=(const byte value) noexcept {
        this->value = (this->value / value) & max_num;
        return *this;
    }

    constexpr BitInt& operator/=(const BitInt& other) noexcept {
        this->value = (this->value / other.value) & max_num;
        return *this;
    }

    constexpr BitInt& operator%=(const byte value) noexcept {
        this->value = (this->value % value) & max_num;
        return *this;
    }

    constexpr BitInt& operator%=(const BitInt& other) noexcept {
        this->value = (this->value % other.value) & max_num;
        return *this;
    }

    constexpr BitInt& operator++() noexcept {
        this->value = (this->value + 1) & max_num;
        return *this;
    }

    constexpr BitInt operator++(int) noexcept {
        BitInt tmp = *this;
        this->value = (this->value + 1) & max_num;
        return tmp; // can I move it instead of copy it
    }

    constexpr BitInt& operator--() noexcept {
        this->value = (this->value - 1) & max_num;
        return *this;
    }

    constexpr BitInt operator--(int) noexcept {
        BitInt tmp = *this;
        this->value = (this->value - 1) & max_num;
        return tmp;
    }

    constexpr bool operator==(const byte value) const noexcept {
        return this->value == (value & max_num);
    }

    constexpr bool operator==(const BitInt& other) const noexcept {
        return this->value == (other.value & max_num);
    }

    constexpr bool operator!=(const byte value) const noexcept {
        return this->value != (value & max_num);
    }

    constexpr bool operator!=(const BitInt& other) const noexcept {
        return this->value != (other.value & max_num);
    }

    constexpr bool operator<(const byte value) const noexcept {
        return this->value < value;
    }

    constexpr bool operator<(const BitInt& other) const noexcept {
        return this->value < (other.value & max_num);
    }

    constexpr auto num() const noexcept {
        return this->value;
    }

    friend std::ostream& operator<<(std::ostream& out, const BitInt& a) noexcept {
        return out << (int)a.value;
    }
};

} // namespace si