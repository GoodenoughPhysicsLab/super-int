#if !defined(__cplusplus) || !defined(__cpp_concepts)
#   error "C++20 is required"
#endif

#include <cstring>
#include <cstdint>
#include <cstdlib>
#include <cassert>
#include <ostream>
#include <type_traits>

namespace si {

template<typename data_type = uintmax_t>
    requires ::std::is_unsigned_v<data_type>
class BigInt {
    using signed_data_type = typename ::std::make_signed_t<data_type>;

    /*
     * if BigInt is negative
     *   ==> size < 0
     * 
     * if BigInt is NaN
     *   ==> data == nullptr
     */

    signed_data_type size;
    data_type* data;
public:
    BigInt() noexcept : size(0), data(nullptr) {}

    ~BigInt() noexcept {
        free(this->data);
    }

    BigInt(signed_data_type num) noexcept {
        this->size = 1;
        this->data = (data_type*)malloc(sizeof(data_type) * this->size);
        *this->data = num >= 0? num : -num;
    }

    BigInt(const BigInt& other) noexcept {
        this->size = other.size;
        this->data = (data_type*)malloc(sizeof(data_type) * this->size);
        memcpy(this->data, other.data, sizeof(data_type) * this->size);
    }

    BigInt(BigInt&& other) {
        this->size = other.size;
        this->data = other.data;
        other.size = 0;
        other.data = nullptr;
    }

    bool operator==(const signed_data_type num) noexcept {
        for (int i = 1; i < this->size; ++i) {
            if (this->data[i] != 0) {
                return false;
            }
        }
        return this->data[0] == num;
    }

    bool operator==(const BigInt& other) noexcept {
        if (this->size <= other.size) {
            for (int i = 0; i < this->size; ++i) {
                if (this->data[i] != other.data[i]) {
                    return false;
                }
            }

            for (int i = this->size; i < other.size; ++i) {
                if (other.data[i] != 0) {
                    return false;
                }
            }
        } else {
            for (int i = 0; i < other.size; ++i) {
                if (this->data[i] != other.data[i]) {
                    return false;
                }
            }

            for (int i = other.size; i < this->size; ++i) {
                if (this->data[i] != 0) {
                    return false;
                }
            }
        }

        return true;
    }

    BigInt& operator>>(data_type num) noexcept {
        assert(this->data);

        for (; num > 0; --num) {
            data_type is_odd = 0;
            for (intmax_t i = this->size - 1; i >= 0; --i) {
                data_type is_odd_temp = this->data[i] & 1;
                this->data[i] >>= 1;
                this->data[i] |= is_odd << (sizeof(data_type) * 8 - 1);
                is_odd = is_odd_temp;
            }
        }

        return *this;
    }

    BigInt& operator<<(data_type num) noexcept {
        assert(this->data);

        data_type sign = 0;
        for (intmax_t i = 0; i < this->size; ++i) {
            data_type sign_temp = (this->data[i] & (1 << (sizeof(data_type) * 8 - 1))) > 0? 1 : 0;
            this->data[i] = (this->data[i] << 1) | sign;
            sign = sign_temp;
        }

        return *this;
    }

    friend BigInt& operator<<(::std::ostream& out, const BigInt& num) {
        if (num.is_NaN()) {
            out << "NaN";
            return out;
        }

        constexpr data_type data_t_max{data_type(-1)};

        if constexpr (::std::is_same_v<data_type, uint64_t>) {
            //
        } else if constexpr (::std::is_same_v<data_type, uint32_t>) {
            //
        } else if constexpr (::std::is_same_v<data_type, uint16_t>) {
            //
        } else if constexpr (::std::is_same_v<data_type, uint8_t>) {
            constexpr data_type delta{255 - 100};
        }
    }

    bool is_NaN() const noexcept {
        return this->data == nullptr;
    }

};

}