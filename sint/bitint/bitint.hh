#pragma once

#include <limits>
#include <cstdint>
#include <cassert>
#include <iostream>
#include <ostream>
#include <type_traits>

namespace si {

using bitsize_type = uint16_t;

/*
 * @brief BitInt
 * @tparam N: bit size, don't include sign
 *
 * example: BitInt<7> <==> int8_t
 */
template<bitsize_type N>
class BitInt {
};

} // namespace si
