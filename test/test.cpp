#include <cassert>
#include <cstdint>

#include "../bigint/bigint.hh"

void test_shlr() {
    auto a = si::BigInt<uint8_t>(0xFF);
    a >> 1;
    assert(a == 0x7F);
}

int main() noexcept {
    test_shlr();

    return 0;
}