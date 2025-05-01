# FIRST and FOLLOW Set Calculator in C

This project is a simple C program to calculate the **FIRST** and **FOLLOW** sets of a context-free grammar. It is designed for students learning compiler construction and syntax analysis.

## 🧩 Features

- Supports multiple grammar rules.
- Handles epsilon (empty string) productions using `#`.
- Accepts rules with multiple right-hand side options using `|`.
- Automatically calculates:
  - `FIRST(X)` for all non-terminals
  - `FOLLOW(X)` for all non-terminals

## 🛠️ How to Compile and Run

You can compile and run this program using any standard C compiler like `gcc`:

```bash
gcc -o first_follow first_follow.c
./first_follow
📥 Input Format
The program will first ask for the number of rules.

Then enter the rules in the format:


A->BCD
A->a|b|#
Use # to represent epsilon (empty string).

All non-terminals must be uppercase letters (A-Z), and terminals lowercase or symbols.
📤 Sample Input

Nombre de règles : 3
Règle 1 : E->TR
Règle 2 : R->+TR|#
Règle 3 : T->FT'
📊 Sample Output



--- PREMIER ---
PREMIER(E) = { ( }
PREMIER(R) = { +, # }
PREMIER(T) = { ( }

--- SUIVANT ---
SUIVANT(E) = { $, ) }
SUIVANT(R) = { $, ) }
SUIVANT(T) = { +, $, ) }
