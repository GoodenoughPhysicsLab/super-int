#include <algorithm>
#include <cstdint>

namespace si {

using bits_type = uintmax_t;

template<bits_type N>
class uBitInt {
    static_assert(N != 0, "BitInt size must be greater than 0");

public:
    using data_type = uintmax_t;
    static constexpr uintmax_t data_length = (N / sizeof(data_type)) + 1;

private:
    data_type data[data_length]{};

public:
    template<bits_type M>
    constexpr auto operator&(uBitInt<M> const& other) const noexcept {
        uBitInt<::std::max(N, M)> result{*this};
        for (decltype(data_length) i{}; i < result.data_length; ++i) {
            result.data[i] = data[i] & other.data[i];
        }
        return result;
    }
};

} // namespace si
