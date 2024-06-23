#include <type_traits>

#include "../sint/bitint.hh"
using si::BitInt;

#ifdef NDEBUG
#undef NDEBUG
#endif

void test_bitint_init() noexcept {
    constexpr auto a = BitInt<7>{0x7f};
    // constexpr auto b = BitInt<7>{0xff}; // err
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

// void test_bitint_add() noexcept {
//     constexpr auto a = BitInt<8>{0xFF};
//     constexpr auto b = BitInt<8>(0x0F);
//     constexpr auto c = BitInt<8>(0xF0);
//     static_assert(a == b + c);

//     // constexpr auto d = BitInt<8>(1);
//     // static_assert(d == a + d);
// }

int main() noexcept {
    return 0;
}