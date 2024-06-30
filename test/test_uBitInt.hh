#pragma once

#include "../sint/bitint.hh"

namespace test_uBitInt {

using si::uBitInt;

inline void compile_init() noexcept {
    constexpr auto a = uBitInt<8>{};
    static_assert(a == 0);
}

} // namespace test_uBitInt