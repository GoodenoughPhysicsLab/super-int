#ifdef NDEBUG
#undef NDEBUG
#endif

#include <type_traits>

#include "../sint/bitint.hh"
using si::BitInt;

void test_bitint_init() noexcept {
    constexpr auto a = BitInt<7>(0xFF);
    // static_assert(a == 0x6F);
}

void test_bitint_byte_type() noexcept {
    static_assert(::std::is_same_v<BitInt<7>::byte, int8_t>);
    static_assert(BitInt<7>::max_num == 0x7F);
    static_assert(::std::is_same_v<BitInt<15>::byte, int16_t>);
    static_assert(BitInt<15>::max_num == 0x7F'FF);
    static_assert(::std::is_same_v<BitInt<31>::byte, int32_t>);
    static_assert(BitInt<31>::max_num == 0x7F'FF'FF'FF);
    static_assert(::std::is_same_v<BitInt<63>::byte, int64_t>);
    static_assert(BitInt<63>::max_num == 0x7F'FF'FF'FF'FF'FF'FF'FF);
}

void test_bitint_add() noexcept {
    constexpr auto a = BitInt<8>(0xFF);
    constexpr auto b = BitInt<8>(0x0F);
    constexpr auto c = BitInt<8>(0xF0);

    static_assert(a == b + c);
}

int main() noexcept {
    test_bitint_init();
    return 0;
}