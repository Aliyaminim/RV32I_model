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
./main [file]
```
As an example, you can use the prepared hello-world program:
```bash
./main ../hello_test/hello.bin 
```

## How to test the program
Sorry, unit testing is down today due to new changes, but some tests are still passing. I will fix this asap.
```bash
./tests/unit_test 
```

