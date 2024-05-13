# RV32I model

Implementation of a functional RISC-V model supporting the RV32I ISA

## How to build
```bash
git clone https://github.com/Aliyaminim/RV32I_model
cd RV32I_model
cmake -S ./ -B build/
cd build/
cmake --build .
```

## How to run the simulator
```bash
./rv32i_sim [file]
```
As an example, you can use the prepared hello-world program:
```bash
./rv32i_sim ../hello_test/hello.bin 
```

## How to test the program
```bash
./tests/unit_test 
```
or use [llvm-snippy](https://github.com/syntacore/snippy)

The dynamic library for snippy is located in ./build/snippy_model/libsnippy_model.so


