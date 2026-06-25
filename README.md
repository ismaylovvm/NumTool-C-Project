# NumTool - Number Theory Calculation Tool

NumTool is a command-line interface (CLI) based, fully modular number theory calculation tool developed in the C programming language.
This project reads and parses complex commands from an external text file, stores them in dynamic memory structures, calculates the requested mathematical operations using optimized custom algorithms, and writes the results to an output file. Without using the standard `<math.h>` library, all algorithms are built from scratch in strict compliance with time complexity constraints.

## 🚀 Key Features

* **Advanced Parsing:** Cleans up unnecessary spaces in the input file, ignores comments starting with `#`, and successfully processes multiple commands separated by semicolons (`;`) on the same line.
* **Dynamic Memory Management:** Fixed-size arrays are not used. All data is managed dynamically in memory regardless of the command count, and is safely cleared with `free()` at the end of the program to prevent memory leaks.
* **Modular Architecture:** The project is divided into different modules (`.c` and `.h` files) for file I/O operations, mathematical calculations, and the main control mechanism.
* **Automated Compilation:** Thanks to the `Makefile` integration, the project can be compiled and made ready to run with a single command.

## 🧠 Algorithms and Optimizations

All calculations within the project are optimized with the following constraints to prevent timeouts when working with extremely large numbers:

1. **GCD (Greatest Common Divisor):** Runs in $O(\log(\min(a,b)))$ time using the Euclidean Algorithm.
2. **POW (Binary Exponentiation):** Calculated in $O(\log b)$ time using the "Binary Exponentiation" method, taking the modulo at each step to prevent memory overflow.
3. **PRIME (Primality Test):** Applies the $6k \pm 1$ optimization to check for primality in $O(\sqrt{N})$ worst-case complexity instead of dividing numbers one by one.
4. **INV (Modular Inverse):** Runs in $O(\log(\min(a,m)))$ time using the Extended Euclidean Algorithm. Negative coefficients are mathematically converted to their correct positive modular values.
5. **PHI (Euler Totient):** Calculated in $O(\sqrt{N})$ time using strictly integer division to prevent precision loss caused by floating-point numbers.
6. **CHECK (Verification):** Algorithmically tests the accuracy of the calculated modular inverse.

## 🛠️ Compilation and Execution

After opening a terminal (command line) window in the project folder, you can use the Makefile to automate the compilation process.

**To Compile the Program:**

```bash
make
```

**To Run the Program:**

```bash
./numtool input.txt output.txt
```

**Windows:**

```dos
.\numtool.exe input.txt output.txt
```

**To Clean Up** (deletes generated `.o` and `.exe` files):

```bash
make clean
```

## 📋 Example Command Formats

Commands can be written in the input file (`input.txt`) using the following formats:

| Command | Description |
|---|---|
| `GCD 48 18` | Calculates the GCD of 48 and 18 |
| `POW 2 30 1000000007` | Calculates 2 to the power of 30, modulo 1000000007 |
| `PRIME 2147483647` | Checks if the number is prime |
| `INV 3 11` | Finds the modular inverse of 3 modulo 11 |
| `PHI 36` | Calculates the Euler Totient value of 36 |

Multiple commands can be placed on the same line, separated by semicolons (`;`). Lines beginning with `#` are treated as comments and ignored.
