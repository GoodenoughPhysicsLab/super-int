#include <cassert>
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
    static_assert(::std::is_same_v<BitInt<7>::ubyte, uint8_t>);
    static_assert(BitInt<7>::max_num == 0x7F);
    static_assert(::std::is_same_v<BitInt<15>::ubyte, uint16_t>);
    static_assert(BitInt<15>::max_num == 0x7F'FF);
    static_assert(::std::is_same_v<BitInt<31>::ubyte, uint32_t>);
    static_assert(BitInt<31>::max_num == 0x7F'FF'FF'FF);
    static_assert(::std::is_same_v<BitInt<63>::ubyte, uint64_t>);
    static_assert(BitInt<63>::max_num == 0x7F'FF'FF'FF'FF'FF'FF'FF);
}

void test_bitint_add() noexcept {
    auto a = BitInt<8>{0xFF};
    auto b = BitInt<8>(0x0F);
    auto c = BitInt<8>(0xF0);
    assert(a == b + 0xF0);
    assert(a == b + c);

    assert(a + 2 + (1 << 8) == 1);
}

int main() noexcept {
    test_bitint_add();
    return 0;
}