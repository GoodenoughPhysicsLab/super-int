#include "../sint/bitint.hh"
using si::uBitInt;

void test_compile_init() {
    constexpr auto a = uBitInt<8>{};
    static_assert(a == 0);
}

int main() noexcept {
    return 0;
}