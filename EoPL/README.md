# Essentials

Try to solve the exercises in [__Essentials of Programming Languages__](http://www.eopl3.com/) _(3rd Edition)_.

## Progress

No.|           Chapter Name             |Exercises
:-:|:-----------------------------------|:--------:
 1 | Inductive Sets of Data             | [*] 36/36
 2 | Data Abstraction                   | [*] 31/31
 3 | Expressions                        | [*] 44/44
 4 | State                              | [*] 42/42
 5 | Continuation-Passing Interpreters  | [ ] 22/58
 6 | Continuation-Passing Style         | [ ] 0/40
 7 | Types                              | [ ] 0/30
 8 | Module                             | [ ] 0/27
 9 | Objects and Classes                | [ ] 0/42
 
## Run

The codes are interpreted with [DrRacket](http://racket-lang.org/), the language is "_Teaching Languages_" -> "_Advanced Student_".

## Interpreters

* Expressions
  * [LET](./C3_Expressions/3.18.scm)
  * [PROC](./C3_Expressions/3.27.scm)
  * [LETREC](./C3_Expressions/3.36.scm)
  * [LETREC (Lexical Addression)](./C3_Expressions/3.41.scm)
* State
  * [EXPLICIT-REFS](./C4_State/4.13.scm)
  * [IMPLICIT-REFS (Expression)](./C4_State/4.21.scm)
  * [IMPLICIT-REFS (Statement)](./C4_State/4.27.scm)
  * [MUTABLE-PAIRS](./C4_State/4.30.scm)
  * [CALL-BY-REFERENCE](./C4_State/4.35.scm)
* Continuation-Passing Interpreters
  * [Continuation-Passing](./C5_Continuation-Passing_Interpreters/5.16.scm)
  * [Trampolined](./C5_Continuation-Passing_Interpreters/5.20.scm)
  * [Imperative]()
  * [Exceptions]()
  * [Threads]()

## Contents

### Chapter 1 Inductive Sets of Data

   |   |   |   |   |   |   |   |   |   
:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:
[1.01](./C1_Inductive_Sets_of_Data/1.01.md) | [1.02](./C1_Inductive_Sets_of_Data/1.02.md) | [1.03](./C1_Inductive_Sets_of_Data/1.03.md) | [1.04](./C1_Inductive_Sets_of_Data/1.04.md) | [1.05](./C1_Inductive_Sets_of_Data/1.05.md) | [1.06](./C1_Inductive_Sets_of_Data/1.06.md) | [1.07](./C1_Inductive_Sets_of_Data/1.07.scm) | [1.08](./C1_Inductive_Sets_of_Data/1.08.scm) | [1.09](./C1_Inductive_Sets_of_Data/1.09.scm) | [1.10](./C1_Inductive_Sets_of_Data/1.10.md)
[1.11](./C1_Inductive_Sets_of_Data/1.11.md) | [1.12](./C1_Inductive_Sets_of_Data/1.12.scm) | [1.13](./C1_Inductive_Sets_of_Data/1.13.scm) | [1.14](./C1_Inductive_Sets_of_Data/1.14.md) | [1.15](./C1_Inductive_Sets_of_Data/1.15.scm) | [1.16](./C1_Inductive_Sets_of_Data/1.16.scm) | [1.17](./C1_Inductive_Sets_of_Data/1.17.scm) | [1.18](./C1_Inductive_Sets_of_Data/1.18.scm) | [1.19](./C1_Inductive_Sets_of_Data/1.19.scm) | [1.20](./C1_Inductive_Sets_of_Data/1.20.scm)
[1.21](./C1_Inductive_Sets_of_Data/1.21.scm) | [1.22](./C1_Inductive_Sets_of_Data/1.22.scm) | [1.23](./C1_Inductive_Sets_of_Data/1.23.scm) | [1.24](./C1_Inductive_Sets_of_Data/1.24.scm) | [1.25](./C1_Inductive_Sets_of_Data/1.25.scm) | [1.26](./C1_Inductive_Sets_of_Data/1.26.scm) | [1.27](./C1_Inductive_Sets_of_Data/1.27.scm) | [1.28](./C1_Inductive_Sets_of_Data/1.28.scm) | [1.29](./C1_Inductive_Sets_of_Data/1.29.scm) | [1.30](./C1_Inductive_Sets_of_Data/1.30.scm)
[1.31](./C1_Inductive_Sets_of_Data/1.31.scm) | [1.32](./C1_Inductive_Sets_of_Data/1.32.scm) | [1.33](./C1_Inductive_Sets_of_Data/1.33.scm) | [1.34](./C1_Inductive_Sets_of_Data/1.34.scm) | [1.35](./C1_Inductive_Sets_of_Data/1.35.scm) | [1.36](./C1_Inductive_Sets_of_Data/1.36.scm)| | | | 

### Chapter 2 Data Abstraction

   |   |   |   |   |   |   |   |   |   
:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:
[2.01](./C2_Data_Abstraction/2.01.scm) | [2.02](./C2_Data_Abstraction/2.02.md) | [2.03](./C2_Data_Abstraction/2.03.scm) | [2.04](./C2_Data_Abstraction/2.04.scm) | [2.05](./C2_Data_Abstraction/2.05.scm) | [2.06](./C2_Data_Abstraction/2.06.scm) | [2.07](./C2_Data_Abstraction/2.07.scm) | [2.08](./C2_Data_Abstraction/2.08.scm) | [2.09](./C2_Data_Abstraction/2.09.scm) | [2.10](./C2_Data_Abstraction/2.10.scm)
[2.11](./C2_Data_Abstraction/2.11.scm) | [2.12](./C2_Data_Abstraction/2.12.scm) | [2.13](./C2_Data_Abstraction/2.13.scm) | [2.14](./C2_Data_Abstraction/2.14.scm) | [2.15](./C2_Data_Abstraction/2.15.scm) | [2.16](./C2_Data_Abstraction/2.16.scm) | [2.17](./C2_Data_Abstraction/2.17.scm) | [2.18](./C2_Data_Abstraction/2.18.scm) | [2.19](./C2_Data_Abstraction/2.19.scm) | [2.20](./C2_Data_Abstraction/2.20.scm)
[2.21](./C2_Data_Abstraction/2.21.scm) | [2.22](./C2_Data_Abstraction/2.22.scm) | [2.23](./C2_Data_Abstraction/2.23.scm) | [2.24](./C2_Data_Abstraction/2.24.scm) | [2.25](./C2_Data_Abstraction/2.25.scm) | [2.26](./C2_Data_Abstraction/2.26.scm) | [2.27](./C2_Data_Abstraction/2.27.md) | [2.28](./C2_Data_Abstraction/2.28.scm) | [2.29](./C2_Data_Abstraction/2.29.scm) | [2.30](./C2_Data_Abstraction/2.30.scm)
[2.31](./C2_Data_Abstraction/2.31.scm)| | | | | | | | | 

### Chapter 3 Expressions

   |   |   |   |   |   |   |   |   |   
:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:
[3.01](./C3_Expressions/3.01.md) | [3.02](./C3_Expressions/3.02.md) | [3.03](./C3_Expressions/3.03.md) | [3.04](./C3_Expressions/3.04.md) | [3.05](./C3_Expressions/3.05.md) | [3.06](./C3_Expressions/3.06.scm) | [3.07](./C3_Expressions/3.07.scm) | [3.08](./C3_Expressions/3.08.scm) | [3.09](./C3_Expressions/3.09.scm) | [3.10](./C3_Expressions/3.10.scm)
[3.11](./C3_Expressions/3.11.scm) | [3.12](./C3_Expressions/3.12.scm) | [3.13](./C3_Expressions/3.13.scm) | [3.14](./C3_Expressions/3.14.scm) | [3.15](./C3_Expressions/3.15.scm) | [3.16](./C3_Expressions/3.16.scm) | [3.17](./C3_Expressions/3.17.scm) | [3.18](./C3_Expressions/3.18.scm) | [3.19](./C3_Expressions/3.19.scm) | [3.20](./C3_Expressions/3.20.scm)
[3.21](./C3_Expressions/3.21.scm) | [3.22](./C3_Expressions/3.22.scm) | [3.23](./C3_Expressions/3.23.scm) | [3.24](./C3_Expressions/3.24.scm) | [3.25](./C3_Expressions/3.25.scm) | [3.26](./C3_Expressions/3.26.scm) | [3.27](./C3_Expressions/3.27.scm) | [3.28](./C3_Expressions/3.28.scm) | [3.29](./C3_Expressions/3.29.scm) | [3.30](./C3_Expressions/3.30.md)
[3.31](./C3_Expressions/3.31.scm) | [3.32](./C3_Expressions/3.32.scm) | [3.33](./C3_Expressions/3.33.scm) | [3.34](./C3_Expressions/3.34.md) | [3.35](./C3_Expressions/3.35.scm) | [3.36](./C3_Expressions/3.36.scm) | [3.37](./C3_Expressions/3.37.scm) | [3.38](./C3_Expressions/3.38.scm) | [3.39](./C3_Expressions/3.39.scm) | [3.40](./C3_Expressions/3.40.scm)
[3.41](./C3_Expressions/3.41.scm) | [3.42](./C3_Expressions/3.42.scm) | [3.43](./C3_Expressions/3.43.scm) | [3.44](./C3_Expressions/3.44.scm)| | | | | | 

### Chapter 4 State

   |   |   |   |   |   |   |   |   |   
:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:
[4.01](./C4_State/4.01.md) | [4.02](./C4_State/4.02.md) | [4.03](./C4_State/4.03.md) | [4.04](./C4_State/4.04.md) | [4.05](./C4_State/4.05.md) | [4.06](./C4_State/4.06.md) | [4.07](./C4_State/4.07.md) | [4.08](./C4_State/4.08.scm) | [4.09](./C4_State/4.09.scm) | [4.10](./C4_State/4.10.scm)
[4.11](./C4_State/4.11.scm) | [4.12](./C4_State/4.12.scm) | [4.13](./C4_State/4.13.scm) | [4.14](./C4_State/4.14.md) | [4.15](./C4_State/4.15.md) | [4.16](./C4_State/4.16.md) | [4.17](./C4_State/4.17.scm) | [4.18](./C4_State/4.18.scm) | [4.19](./C4_State/4.19.scm) | [4.20](./C4_State/4.20.scm)
[4.21](./C4_State/4.21.scm) | [4.22](./C4_State/4.22.scm) | [4.23](./C4_State/4.23.scm) | [4.24](./C4_State/4.24.scm) | [4.25](./C4_State/4.25.scm) | [4.26](./C4_State/4.26.scm) | [4.27](./C4_State/4.27.scm) | [4.28](./C4_State/4.28.md) | [4.29](./C4_State/4.29.scm) | [4.30](./C4_State/4.30.scm)
[4.31](./C4_State/4.31.md) | [4.32](./C4_State/4.32.scm) | [4.33](./C4_State/4.33.scm) | [4.34](./C4_State/4.34.scm) | [4.35](./C4_State/4.35.scm) | [4.36](./C4_State/4.36.scm) | [4.37](./C4_State/4.37.scm) | [4.38](./C4_State/4.38.md) | [4.39](./C4_State/4.39.md) | [4.40](./C4_State/4.40.scm)
[4.41](./C4_State/4.41.md) | [4.42](./C4_State/4.42.scm)| | | | | | | | 

### Chapter 5 Continuation-Passing Interpreters

   |   |   |   |   |   |   |   |   |   
:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:
[5.01](./C5_Continuation-Passing_Interpreters/5.01.scm) | [5.02](./C5_Continuation-Passing_Interpreters/5.02.scm) | [5.03](./C5_Continuation-Passing_Interpreters/5.03.scm) | [5.04](./C5_Continuation-Passing_Interpreters/5.04.scm) | [5.05](./C5_Continuation-Passing_Interpreters/5.05.scm) | [5.06](./C5_Continuation-Passing_Interpreters/5.06.scm) | [5.07](./C5_Continuation-Passing_Interpreters/5.07.scm) | [5.08](./C5_Continuation-Passing_Interpreters/5.08.scm) | [5.09](./C5_Continuation-Passing_Interpreters/5.09.scm) | [5.10](./C5_Continuation-Passing_Interpreters/5.10.scm)
[5.11](./C5_Continuation-Passing_Interpreters/5.11.scm) | [5.12](./C5_Continuation-Passing_Interpreters/5.12.scm) | [5.13](./C5_Continuation-Passing_Interpreters/5.13.scm) | [5.14](./C5_Continuation-Passing_Interpreters/5.14.scm) | [5.15](./C5_Continuation-Passing_Interpreters/5.15.scm) | [5.16](./C5_Continuation-Passing_Interpreters/5.16.scm) | [5.17](./C5_Continuation-Passing_Interpreters/5.17.scm) | [5.18](./C5_Continuation-Passing_Interpreters/5.18.scm) | [5.19](./C5_Continuation-Passing_Interpreters/5.19.scm) | [5.20](./C5_Continuation-Passing_Interpreters/5.20.scm)
[5.21](./C5_Continuation-Passing_Interpreters/5.21.md) | [5.22](./C5_Continuation-Passing_Interpreters/5.22.md)| | | | | | | | 
