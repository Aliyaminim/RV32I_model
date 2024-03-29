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

## How to run the program
```bash
./main < [file]
```

## How to test the program
```bash
./tests/unit_test 
```

