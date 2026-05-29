# README

Functions for financial calculations.

This library, forked from [this repository](https://github.com/AlanEdward19/Teste-Desenvolvimento-C-BDS-), was developed as part of the internship program evaluation of the company **BDSDataSolutions**. More info on what was expected on `INSTRUCTIONS.md` (formerly `README.md`).

## Functionalities

The library has the following financial functions:

1. Simple interest;
2. Simple interest amount;
3. Compound interest amount;
4. Compound interest;
5. Fixed installment;
6. Amortization schedules;
7. Future value (FV);
8. Present value (PV);
9. Net Present Value (NPV);
10. Internal Rate of Return (IRR);

Those functions can be used upon inclusion from `include.h` header file. Some auxiliary functions are also included.

## Dependencies

This library is supported on both Linux and Windows machines.

### GCC

To compile the code, `gcc` is needed:

#### For Linux:

```bash
sudo apt update
sudo apt install build-essential -y
```

#### For Windows:

The easier method is through VSCode with MinGW: https://code.visualstudio.com/docs/cpp/config-mingw

#### Version
The version used to make the library:

```bash
gcc version 13.3.0 (Ubuntu 13.3.0-6ubuntu2~24.04.1)
```
### Make

#### For bash terminal

You may need to install `make` in your system:

If Ubuntu/Debian:

```bash
sudo apt install make
```

Other distros:

You may need to look into the [specific package manager for your distro](https://distrowatch.com/dwres.php?resource=package-management).

## Tests

Tests can be made using the provided `Makefile`:

**Automated tests**: 

Directly,

```bash
make
```

Or more explicitly,

```bash
make main && ./bin/main
```

For PowerShell terminals:

Tests can also be made using the provided `Makefile`. You may need to install `make` in your system: `winget install ezwinports.make`

After installing it, if using `WinGM`, the command *may be* `mingw32-make` instead of `make`:

```shell
mingw32-make main; ./bin/main
```

### Methodology

Tests are made by using the input inside `tests/in.txt`, calculating their values using some function, and then, comparing those values with the output inside `tests/out.txt` with a certain precision (`threshold`) initially at 1.0.

For amortization table tests, there is an additional folder `tests/tables` with TSV table files that are loaded on demand during the tests.

Values from `out.txt` were obtained using Libre Office Calc:

```text
Version: 26.2.2.2 (X86_64)
Locale: pt-BR (pt_BR.UTF-8); UI: pt-BR
```

**Note**: Some liberty were taken to handle invalid values.

The comparison is made line by line with each file, loading both IN and OUT line into their own `AmortizationTable` struct.

After all comparisons are made, the `threshold` is reduced by a factor of 10. This is repeated until the threshold gets below a minimum value `1e-13`.

Here is an example of a comparison: 
```text
Threshold set to: 1-e13
Comparison 183 (line 1):
   (OK ) Simple interest: 600.00000000000000000 == 600.00000000000000000
   (OK ) Simple interest amount: 1600.00000000000000000 == 1600.00000000000000000
   (NOK) Compound interest amount: 1795.85632602213013342 == 1795.85632602212990605
   (NOK) Compound interest: 795.85632602213013342 == 795.85632602213001974
   (NOK) Fixed installment: 112.82541002081534032 == 112.82541002081499926
   (NOK) Future value: 1795.85632602213013342 == 1795.85632602212990605
   (OK ) Present value: 1000.00000000000000000 == 1000.00000000000000000
   (NOK) Net present value: 1859.45471405699413481 == 1859.45471405699004208
   (NOK) Internal rate of return: 1.00527633726596810 == 1.00527643664039990
  Failed 6 times with threshold of: 0.0000000000001
```

`OK` for an exact match with the threshold and `NOK` for an inexact match.

With the defaults values and variables, the tests should hold up to a threshold of `1-e7`.

The accuracy is mainly held down by the `internal_return_rate` function with it's iterative approach. The functions parameters may be changed for better accuracy when needed.

## Examples

The interactive test allows the input of values for the `principal`, `rate` and `periods`. Those values can be used to calculate all values for the [functionalities](#functionalities) of this library.

If calculating the **NPV/IRR** is also necessary to supply a array of values, with the array size equaling to periods.

**Interactive tests**: 
```bash
make interactive && ./app/interactive
```

or,

```shell
mingw32-make interactive; ./bin/interactive
```


## Makefile

The Makefile uses gcc for it's compilation. Without flags:

```bash
gcc -g examples/interactive.c src/finance.c src/interface.c -I./include -lm -o interactive
```

and,

```bash
gcc -g tests/main.c src/finance.c src/interface.c -I./include -lm -o main
```

After the tests are made, run:

```shell
make clean
```

to clean the output executables.

## Sources and References

Here are the major sources and references for the making of this project:

**Basic C consultation**: https://beej.us/guide/bgclr/ (and others)

**Makefile tutorial**: https://makefiletutorial.com/

**Basic financial formulae**: `INSTRUCTIONS.md`

**Library structure**: https://github.com/curl/curl/blob/master/include/curl/curl.h (and others)

**Amortization**: https://www.investopedia.com/terms/a/amortization.asp

**Future value and Present value**: https://corporatefinanceinstitute.com/resources/valuation/future-value-formula/

**Net Present Value and Internal Rate of Return**: https://www.financialprofessionals.org/training-resources/resources/articles/details/net-present-value-vs.-internal-rate-of-return

**Net Present Value Libre Office Calc use**: https://help.libreoffice.org/latest/en-US/text/scalc/01/04060103.html?&DbPAR=SHARED&System=UNIX#bm_id3153948

## Acknowledgments of AI

AI was used in the making of this library as an evaluation and consultation tool. No code was generated directly and injected directly onto this project. However, the author concede that during the making of this project some code may have being generate directly (via ChatBots) and indirectly (via AI search engine functionalities), though it's usage and implementation was human-made.

The structure of the project, as well as some functions recommendations was made under AI's help.