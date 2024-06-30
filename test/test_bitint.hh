#pragma once

#include <cassert>
#include <cstdint>
#include <type_traits>

#include "../sint/bitint.hh"

#ifdef NDEBUG
#undef NDEBUG
#endif

namespace test_BitInt {

using si::BitInt;

void test_compile_init() noexcept {
    constexpr auto a = BitInt<7>{0x7f};
    constexpr auto b = BitInt<7>{0xff};
    // constexpr auto c = BitInt<64>{}; // err

    constexpr BitInt<5> d{a};

    static_assert(a == b);
    static_assert(d == 31);
}

void test_compile_byte_type() noexcept {
    static_assert(::std::is_same_v<BitInt<7>::ubyte, uint8_t>);
    static_assert(BitInt<7>::max_num == 0x7F);
    static_assert(::std::is_same_v<BitInt<8>::ubyte, uint16_t>);
    static_assert(::std::is_same_v<BitInt<15>::ubyte, uint16_t>);
    static_assert(BitInt<15>::max_num == 0x7F'FF);
    static_assert(::std::is_same_v<BitInt<31>::ubyte, uint32_t>);
    static_assert(BitInt<31>::max_num == 0x7F'FF'FF'FF);
    static_assert(::std::is_same_v<BitInt<63>::ubyte, uint64_t>);
    static_assert(BitInt<63>::max_num == 0x7F'FF'FF'FF'FF'FF'FF'FF);
}

void test_compile_add() noexcept {
    constexpr auto a = BitInt<8>{0xFF};
    constexpr auto b = BitInt<8>(0x0F);
    constexpr auto c = BitInt<8>(0xF0);
    constexpr auto nc = BitInt<8>(-0xF0);
    static_assert(a == 0xF0 + b);
    static_assert(a == b + c);
    static_assert(a + 2 + (1 << 8) == 1);

    constexpr auto d = BitInt<8>(-2);
    static_assert(d + 3 == 1);
    static_assert(d + 1 == -1);
    static_assert(a + nc == b);
}

void test_compile_sub() noexcept {
    constexpr auto a = BitInt<8>{0xFF};
    static_assert(-a == -0xFF);

    constexpr auto b = BitInt<8>(0x0F);
    constexpr auto c = BitInt<8>(0xF0);
    static_assert(b == a - 0xF0);
    static_assert(b == 0xFF - c);
    static_assert(b == a - c);

    constexpr auto d = BitInt<8>(2);
    static_assert(d - 1 - (1 << 8) == 1);
}

void test_compile_eq() noexcept {
    constexpr auto a = BitInt<8>{-0xFF};
    static_assert(a == -0xFF);

    constexpr auto b = BitInt<8>(-0xFF);
    static_assert(a == b);

    static_assert(BitInt<4>{-16} == -16);
    static_assert(BitInt<7>{-128} == -128);
    static_assert(BitInt<8>{-256} == -256);
}

void test_compile_invert() noexcept {
    constexpr auto a = BitInt<8>{};
    static_assert(~a == -1);
}

void test_compile_min() {
    static_assert(BitInt<4>::min_num == -16);
    static_assert(BitInt<7>::min_num == -128);
    static_assert(BitInt<8>::min_num == -256);
    static_assert(BitInt<63>::min_num == static_cast<intmax_t>(1) << 63);
}

void runtime_plusplus() noexcept {
    auto a = BitInt<8>{-1};
    assert(++a == 0);
    assert(a++ == 0);
    assert(a == 1);

    auto b = BitInt<7>{1};
    int8_t c = 1;
    do {
        assert(b++ == c);
    } while (++c != 1);
}

} // namespace test_BitInt