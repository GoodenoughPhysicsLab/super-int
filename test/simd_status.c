#if defined(__AVX2__)
    #pragma message("=> simd_status: AVX2")
#elif defined(__ARM_NEON__)
    #pragma message("=> simd_status: arm-neon")
#else
    #pragma message("=> simd_status: None")
#endif

int main(void) {
    return 0;
}
