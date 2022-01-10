optimization excercise** In this exercise, you are given a Kodkin section (logically correct, without bugs) 
that has not been optimized,The goal is to Reach the running time as quickly as possible of the program,
 while maintaining a logical correctness. Will be given on the basis of competition in performance, 
 so that shorter running time will get a higher score.

My optimization explanation principels were :
1. we need to avoid function calls and therefore, instead of calling 6 other function,
we will merge all the function inside 'myfunction'.
2. we can define some consts that we use intansitly like max,min, logic maximum/minimum methods.
3. to avoid code motion like the calculation of size, we can calculate ones.