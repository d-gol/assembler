# One Pass Assembler

## What is it about? ##
The project is an implementation of an one pass assembler. The goal is read the input assembly file, check for errors and provide linker with results. The results are the table of symbols, relocation records, and data and text sections.

The source code is read only once, during parsing. Processing the symbols that were referenced before they have been defined was solved by using backpatching. That is, for every symbol, the information was saved about where has it been referenced. Then, after the reading of the entire input file, the assembler goes through the list of such symbols and updates information about them.

The result of the program is the linker file, if successful.
The error is displayed if the input assembly file cannot be parsed (undefined symbol, multiple definition, wrong format of an instruction...).

## How is it implemented? ## 

This project is implemented in C++. It is divided in modules, each with a specific functionality.
