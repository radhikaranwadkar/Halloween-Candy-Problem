/*
 * infra.c defines all the functions which build the infra for
 * solution1.c. It has functions to generate test input file,
 * function for time, function to find min/max of array.
 * Corresponding header file has debug, print flags and other
 * required preprocessors
 *
 */


#include "infra.h"


#ifdef CONST_INPUT
/*
 * Generates input file 'my_input.txt' with hard coded values
 * Uncomment CONST_INPUT in infra.h file to use this.
 * (Then use my_input.txt in main())
 */
int generate_constant_input_file()
{
    FILE *fp = NULL;
    fp = fopen("my_input.txt", "w");
    if (fp == NULL) {
	printf("\nERROR: INPUT FILE ERROR");
	return -1;
    }

    DEBUG("\n\tmax allowed candies for the child %d", 10);
    DEBUG("\n\thomes in the neighbourhood %d", 5);
    DEBUG("\n\tmax candies that a house can give %d", 2);

    fprintf(fp, "%d\n", 3);
    fprintf(fp, "%d\n", 5);

    fprintf(fp, "%d\n", 4);
    fprintf(fp, "%d\n", 3);
    fprintf(fp, "%d\n", 1);
//    fprintf(fp, "%d\n", 3);
//    fprintf(fp, "%d\n", 1);
//    fprintf(fp, "%d\n", 3);
    fclose(fp);
    return 0;
}
#else
/*
 * Generates test input file 'my_input.txt' using rand() function
 * Comment CONST_INPUT to use this
 */
int generate_input_file()
{
    FILE *fp = NULL;
    int candies_per_house;
    int max_allowed_candies, homes, max_candies_per_house;
    
    fp = fopen("my_input.txt", "w");
    if (fp == NULL) {
	printf("\nERROR: INPUT FILE ERROR");
	return -1;
    }
    
    srand ( time(NULL) );
    max_allowed_candies = rand() % MAX_ALLOWED_CANDIES + 1;
    max_candies_per_house = rand() % MAX_CANDIES_PER_HOUSE + 1;
    homes = rand() % MAX_HOMES + 1;

    DEBUG("\n\tmax allowed candies for the child %d", max_allowed_candies);
    DEBUG("\n\thomes in the neighbourhood %d", homes);
    DEBUG("\n\tmax candies that a house can give %d", max_candies_per_house);

    fprintf(fp, "%d\n", homes);
    fprintf(fp, "%d\n", max_allowed_candies);

    for (int i=0; i<homes; i++) {
	candies_per_house = rand() % max_candies_per_house + 1;
	if (candies_per_house == max_allowed_candies) {
	    candies_per_house = 0;   
	}
	fprintf(fp, "%d\n", candies_per_house);
    }
    fclose(fp);

    return (0);
}
#endif

void array_min_max(int *arr, int size, int *min, int *max)
{
    *min = *max = arr[0];

    for(int i=0; i<size; i++) {
	if (arr[i] < *min)    *min = arr[i];
	if (arr[i] > *max)    *max = arr[i];
    }
    
}

double dtime()
{
        double tseconds = 0.0;
        struct timeval mytime;
        gettimeofday(&mytime,(struct timezone*)0);
        tseconds = (double)(mytime.tv_sec +
        mytime.tv_usec*1.0e-6);
        return( tseconds );
}

