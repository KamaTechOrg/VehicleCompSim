# how to use

1. read the docomentation of [google benchmark](https://github.com/google/benchmark)


2. Complie the needed target **Note:** Compile in Release mode:
    ```bash
    cd build
    cmake -DCMAKE_BUILD_TYPE=Release ..
    ```
    ```bash
    cmake --build .
    ```
    Or, to build a specific target:
    ```bash
    cmake --build . --target aes_bench
    ```


3. Run the benchmark:
    ```bash
    ./benchmark/aes_bench
    ```

4. To store the results, you can run:
    ```bash
    ./benchmark/aes_bench --benchmark_format=csv > ../benchmark/results/aes_bench-$(date +%Y-%m-%d).csv
    ```

5. Learn more:
    ```bash
    ./benchmark/aes_bench --help
    ```
   
