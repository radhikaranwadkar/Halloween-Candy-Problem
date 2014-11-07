#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX_ALLOWED_CANDIES	1000    //Max number of candies that the child may collect
#define MAX_CANDIES_PER_HOUSE	1000	//Max number of candies given by each hosue
#define MAX_HOMES		10000	//MAX number of homes on the block

#define DBG_FLAG 0
#define OUTPUT 1
#define EXEC_TIME 1

int dbg_flag;
int print_output_flag;
int print_time_flag;

#define DEBUG if(dbg_flag) printf
#define PRINT_TIME if(print_time_flag) printf
#define PRINT_OUTPUT if(print_output_flag) printf

#define SOLUTION1	//To compile and use solve1()
#define SOLUTION2	//To compile and use solve2()
//#define SOLUTION_OMP	//To compile and use solve_omp()

//#define CONST_INPUT

#ifdef CONST_INPUT
int generate_constant_input_file();
#else
int generate_input_file();
#endif

void array_min_max(int *array, int size, int *min, int *max);
double dtime();
