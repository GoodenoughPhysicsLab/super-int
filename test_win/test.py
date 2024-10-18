import os
import argparse

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--compiler", choices=["gcc", "clang"], default="gcc")
    parser.add_argument("--simd", action="store_true")
    args = parser.parse_args()

    if args.compiler == "gcc":
        c_compiler = "gcc"
        cxx_compiler = "g++"
    elif args.compiler == "clang":
        c_compiler = "clang"
        cxx_compiler = "clang++"
    else:
        raise Exception("InternalError: Unknown compiler")

    PROJECT_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
    cmake_cmd = f"cmake -G \"MinGW Makefiles\" -Wno-dev " \
            f"-S \"{os.path.join(PROJECT_DIR, 'test')}\" " \
            f"-B \"{os.path.join(PROJECT_DIR, 'build')}\" " \
            f"-DCMAKE_C_COMPILER={c_compiler} -DCMAKE_CXX_COMPILER={cxx_compiler} " \
            f"-DCMAKE_MAKE_PROGRAM=gnumake "
    if args.simd:
        cmake_cmd += "-DSINT_SIMD=ON "
    print(">>> ", cmake_cmd)
    ret_code = os.system(cmake_cmd)
    if ret_code != 0:
        raise Exception("CMake failed")
    build_cmd = f"cmake --build \"{os.path.join(PROJECT_DIR, 'build')}\""
    print(">>> ", build_cmd)
    os.system(build_cmd)
