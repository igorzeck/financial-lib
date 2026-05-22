# README

Functions for financial calculations, including:

1. Simple interest;
2. Simple interest amount;
3. Compound interest amount;
4. Compound interest;
5. Fixed installment;
6. Armotaization schedules;
7. Future value (FV) with compounding interest;
8. Present value (PV);
9. PV with compounding interest;

This library, forked from [this repository](https://github.com/AlanEdward19/Teste-Desenvolvimento-C-BDS-), was developed as part of the interniship program evaluation of the company BDSDataSolutions. More info on what was expected on `INSTRUCTIONS.md` (formely `README.md`).

## Running and testing

This library is supported on both Linux and Windows machines.

### GCC

To compile the code, `gcc` is needed:

#### For linux:

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

You may need to look into the [specific package manager for your distro]().

Tests can be made using the provided `Makefile`:

**Automated tests**: 

```bash
make main && ./app/out
```

**Interactive tests**: 
```bash
make interactive && ./app/interactive
```

#### For PowerShell terminal

Tests can be made using the provided `Makefile`:

You may need to install `make` in your system `winget install ezwinports.make`

After installing it, if using `WinGM` the command may be `mingw32-make` instead of `make`:

```shell
mingw32-make interactive; ./bin/interactive
```

or,

```shell
mingw32-make main; ./bin/out
```

### Make file internal commands

The Makefile uses gcc for it's compilation:

```bash
gcc -g examples/interactive.c src/finance.c src/interface.c -I./include -lm -o interactive
```

and,

```bash
gcc -g tests/main.c src/finance.c src/interface.c -I./include -lm -o out
```

After the tests are made, run:

```shell
make clean
```

to clean the output executables.

## Sources and References

Here are the major sources and references for the making of this project:

**Basic C consultation**: https://beej.us/guide/bgclr/ (and others)

**Basic financial formulae**: `INSTRUCTIONS.md`

**Library strcuture**: https://github.com/curl/curl/blob/master/include/curl/curl.h (and others)

**Armotization**: https://www.investopedia.com/terms/a/amortization.asp

**Future value e Present value**: https://corporatefinanceinstitute.com/resources/valuation/future-value-formula/

## Acknowledgments of AI

AI was used in the making of this library as an evaluation and consultation tool. No code was generated and injected directly onto this project. However, the authors, conced that during the making of this project consultation directly (via Chatbots) and indirectly (via AI search engine functionalities) was made.

The structure of the projecte, as well as some functions recomendations was made with AI's help, namely Anthropic's Claude and, indirectly, Google's Gemini.