# ITSE121 Project - Simple Compiler with C

This project is aimed at creating a simple compiler using the C programming language. The compiler will be able to parse a given source code and generate executable machine code.

## Table of Contents

- [Introduction](#introduction)
- [Features](#features)
- [Todo](#todo)
- [Getting Started](#getting-started)
- [Usage](#usage)
- [Contributing](#contributing)
- [License](#license)

## Introduction

In this project, we will be implementing a basic compiler
 <!-- that can perform lexical analysis, syntax analysis, semantic analysis, and code generation. The compiler will take a source code written in a specific programming language and produce an executable binary file. -->
 ### Output files
  - The compiler will generate the following output files:
    - A log file containing compilation information and errors. [Log](output/log.txt)
    - A symbol table containing information about the variables and functions in the source code.  [Log](output/log.txt)
    - A file containing the tokens extracted from the source code. [Tokens](output/tokens.txt)
    - A file containing the source code without spaces, comments, and empty lines. [Cleaned Code](output/clean.txt)

## Features
- ( 0.1 ) the compiler can read a source text file and print it to a new text file.
- ( 0.2 ) read char by char
- ( 0.3 ) Lexial analysis
  - ( 0.3.1 ) symbol table
  - ( 0.3.2 ) find tokens: identifires, operators, keywords only in a basic way
  
              | Name | Type | Size | Dimension | Line of Declaration | Line of Use | Address |

  - ( 0.3.3 ) find tokens: identifires, operators, keywords, numbers, strings, comments, and special characters
  - ( 0.4 ) Show the tokens in a new text file
  - ( 0.5 ) Show symbol table in a new text file
  - ( 0.6 ) Show the errors in a new text file ( log file )
  - ( 0.7 ) Show compilation info in the log file and in the console
  - ( 0.8 ) Show the source code without spaces, comments and empty lines in a new text file
  - ( 0.9 ) Solve spacing errors: the compiler should ignore spaces, tabs, and new lines
<!-- 
- Lexical analysis: Tokenizing the source code into meaningful tokens.
- Syntax analysis: Parsing the tokens to check for correct syntax.
- Semantic analysis: Checking for semantic errors and generating symbol tables.
- Code generation: Generating machine code from the parsed source code.
- Error handling: Providing meaningful error messages for easier debugging. -->

## Todo



## Getting Started

To get started with this project, follow these steps:

1. Clone the repository: `git clone https://github.com/your-username/itse121-compiler.git`
2. Install the required dependencies.
3. Build the project using the provided build script.
4. Run the compiler on a sample source code file.

## Usage

To use the compiler, run the following command:
    
    ```bash
    $ make compiler
    $ ./compiler
    ```

## Contributing

To contribute to this project, follow these steps:

1. Fork this repository.
2. Create a branch: `git checkout -b <branch-name>`
3. Make your changes and commit them: `git commit -m '<commit-message>'`
4. Push to the original branch: `git push origin <project-name>/<location>`
5. Create the pull request.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.


