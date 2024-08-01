# how to use

1. read the docomentation of [googleTest](https://github.com/google/googletest)

2. Complie:
    ```bash
    cd build
    cmake ..
    ```
    ```bash
    cmake --build .
    ```
    Or, to build a specific target:
    ```bash
    cmake --build . --target aes_test
    ```


3. Run all tests:
    ```bash
    make test
    ```
    Or
    ```bash
    ctest
    ```
    Or, to run a specific test target directly:
    ```bash
    ./test/aes_test
    ```

4. Learn more about test options:
    ```bash
    ./test/aes_test --help
    ```
   
