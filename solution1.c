/*
 * Halloween Candy problem 'Trick or treat'
 * Input: How many candies each house gives,
 *	  Number of houses in the beighbourhood
 *        How many max total candies a child can take
 * Problem: Find the best sequence a child should traverse
 *	    so that he collects as many cansies as possible
 *	    within his set limit. In case of multiple best
 *	    sequences, the one with least numbered starting
 *	    house should be selected
 * Files needed: Makefile, infra.c,h solve_omp.c,h input.txt
 *
 * -
 * Radhika Ranwadkar
 * radhikaranwadkar@gmail.com
 */

#include <stdlib.h>
#include <stdio.h>
#include "infra.h"
#include "solve_omp.h"

#ifdef SOLUTION1
int solve1(int, int, int *, int *, int *, int *);
#endif

#ifdef SOLUTION2
int solve2(int, int, int *, int *, int *, int *);
#endif

int main()
{
   /*
    * Declarations:
    * max_allowed_candies: Number of candies the child is allowed
    *		    homes: Total number of homes in the neighbourhood
    *	candies_per_house: pointer to input array of candies per home
    * start_house, end_house, candies_collected: To collect output data
    */
    FILE *fp;
    int ret_val=0;
    int max_allowed_candies, homes;
    int *candies_per_house = NULL;
    int start_house=0, end_house=0, candies_collected=0;
    int min, max, sum;

    //Declaring variables to calculate exec time
    double tstart1, tstop1, ttime1;
    double tstart2, tstop2, ttime2;
    double tstart_omp, tstop_omp, ttime_omp;


   /*
    * Set infra. Create test input file.
    * Function for calulating time of execution. etc. 
    * Setting debug flag etc
    */

    dbg_flag = DBG_FLAG;
    print_time_flag = EXEC_TIME;
    print_output_flag = OUTPUT;

#ifdef CONST_INPUT
    ret_val = generate_constant_input_file();
    if (ret_val != 0) {
	printf("\nERROR in generating input file");
	return (-1);
    }
#else
    //Uses rand() function to generate the input data set
    ret_val = generate_input_file();
    if (ret_val != 0) {
        printf("\nERROR in generating input file");
        return (-1);
    }
#endif
   /*
    * Read input from file
    * 'my_input.txt' is my test input file. Comment it out later
    * And uncomment 'input.txt'
    */
    fp = fopen("my_input.txt", "r");
    //fp = fopen("input.txt", "r");
    if (fp==NULL) {
	printf("\nERROR in opening input file");
	return (-1);
    }
    fscanf(fp, "%d %d", &homes, &max_allowed_candies);

    candies_per_house = (int *)malloc((homes+1)*sizeof(int)); //index starts at 1
    if (candies_per_house == NULL) {
	DEBUG("\nERROR: in malloc");
	return (-1);
    }
    for (int i=1; i<=homes; i++)
	fscanf(fp, "%d", &candies_per_house[i]);
    fclose(fp);

    array_min_max(candies_per_house+1, homes, &min, &max);
    DEBUG("\n\tmin %d\tmax %d", min, max);

#ifdef SOLUTION1
    //=======================[1]=================================
   /*
    * Start timer. Call the function solve1() to find the solution to
    * the Halloween Candy Problem.
    * Stop the timer. Print output to file 'output1.txt'
    */
    PRINT_OUTPUT("\nsolve1():");
    tstart1 = dtime();
    ret_val = solve1(homes, max_allowed_candies, candies_per_house,
	    &start_house, &end_house, &candies_collected);
    tstop1 = dtime();
    ttime1 = tstop1 - tstart1;      //ttime is time in seconds
    PRINT_TIME("\n\tTime for solve1(): %f milli seconds", ttime1*1000.0);

    fp = fopen("output1.txt", "w");
    if (candies_collected == -1)
	fprintf(fp, "Don't go here");
    else
	fprintf(fp,"Start at home %d and go to home %d collecting %d pieces of candy",
		start_house, end_house, candies_collected);
    fclose(fp);

    //Verify:
    sum=0;
    for (int i=start_house; i<=end_house; i++)  sum+=candies_per_house[i];
    DEBUG("\n\tVerifying solve1(): sum = %d", sum);


#endif


#ifdef SOLUTION2
    //=======================[2]=================================
   /*
    * Another method. Linear order O(N)
    * Writes output to 'output2.txt' file
    */
    PRINT_OUTPUT("\nsolve2();");
    tstart2 = dtime();
    ret_val = solve2(homes, max_allowed_candies, candies_per_house,
	    &start_house, &end_house, &candies_collected);  
    tstop2 = dtime();
    ttime2 = tstop2 - tstart2;      //ttime is time in seconds
    PRINT_TIME("\n\tTime for solve2(): %f milli seconds", ttime2*1000.0);

    fp = fopen("output2.txt", "w");
    if (candies_collected == -1)
        fprintf(fp, "Don't go here");
    else
        fprintf(fp,"Start at home %d and go to home %d collecting %d pieces of candy",
                start_house, end_house, candies_collected);
    fclose(fp);

    //Verify:
    sum=0;
    for (int i=start_house; i<=end_house; i++)  sum+=candies_per_house[i];
    DEBUG("\n\tVerifying solve2(): sum = %d", sum);



#endif

#ifdef SOLUTION_OMP
    //=======================[2]=================================
   /*
    * Solution using OpenMP (incomplete)
    */
    tstart_omp = dtime();	    //ttime is time in seconds
    ret_val = solve_omp(homes, max_allowed_candies, candies_per_house,
	    &start_house, &end_house, &candies_collected);
    tstop_omp = dtime();
    ttime_omp = tstop_omp - tstart_omp;
    PRINT_TIME("\n\n\tTime for solve_omp(): %f milli seconds", ttime_omp*1000.0);
    printf("\n");  

    //Verify:
    sum=0;
    for (int i=start_house; i<=end_house; i++)	sum+=candies_per_house[i];
    DEBUG("\n\tVerifying splve_omp: sum = %d", sum);
#endif

    printf("\n");
    return 0;
}


#ifdef SOLUTION1
int solve1(int homes, int max_allowed_candies, int *candies_per_house,
	int *ret_start_house, int *ret_end_house, int *ret_candies_collected)
{
    int start_house=1, end_house=1, diff_from_max = max_allowed_candies;
    int actual_collected_candies;
    int min, max;

    // If max allowed cadies for the child is less than min of the array,
    // no solution exists.
    array_min_max(candies_per_house+1, homes, &min, &max);
    if (max_allowed_candies < min) {
	DEBUG("\nNo possible solution");
	*ret_candies_collected = -1;
	return 0;
    }

    //Now take care of the corner case where total number of houses is 1
    if (homes == 1) {
	start_house = end_house = 1;
	actual_collected_candies = candies_per_house[1];
	DEBUG("\n\tStart at %d and go to home %d getting %d pieces of  candies",
	                start_house, end_house, actual_collected_candies);
	*ret_start_house = start_house;
	*ret_end_house = end_house;
	*ret_candies_collected = actual_collected_candies;
	return (0);

    }
   
    //i loops over possible starting points; j loops to find end point for its i 
    for (int i=1, sum=0; i<=homes; i++) {
	for (int j=i, sum=0; j<=homes; j++) {
	   /*
	    * If starting element > max_allowed_candies then
	    * the sequence is invalid. Hence break and consider
	    * next starting point
	    */
	    if (candies_per_house[j] > max_allowed_candies) {
		break;
	    }

	    sum += candies_per_house[j];

	   /*
	    * If total number of candies collected after going to next ((j+1)th) house 
	    * will exceed max_allowed_candies, then we have reached the end of sequence at j
	    * when i is the start.
	    * But now check how many less candies we are getting in this case!!
	    * The diff_from_max we have now, should be less than
	    * our previous found out sequence. If our diff_from_max has now reduced,
	    * then store this as a new sequence which is better.
	    * Hence store start, end, sum and diff_from_max.
	    */

	    //This if-else stops from accessing invalid (next to last) input array element
	    if (j==homes) {
		if (sum > max_allowed_candies)
		    break;
		else {
		    if ((max_allowed_candies - sum) < diff_from_max) {
			start_house = i;
			end_house = j;
			actual_collected_candies = sum;
			diff_from_max = max_allowed_candies - actual_collected_candies;
			//printf("\n\t[start][end][sum]|[%d][%d][%d]",i,j,sum);
			break;
		    }
		}
	    }
	    if ( ( (sum + candies_per_house[j+1]) > max_allowed_candies )) {
		if((max_allowed_candies - sum) < diff_from_max) {
		    start_house = i;
		    end_house = j;
		    actual_collected_candies = sum;
		    diff_from_max = max_allowed_candies - actual_collected_candies;
		}
		//printf("\nDebug!! Print each seq:");
		//printf("\n\t[start][end][sum]|[%d][%d][%d]",i,j,sum);
	        break; //break since seq is identified for i^th starting house
	    }
	}

       /*
	* If with this sequence, diff_from_max is 0, then this is the best possible sequence for
	* least value of i.
	* This satisfies the condition that start_house_number is smaller if another sequence
	* exists.
	*/
	if (diff_from_max == 0 || end_house == homes)
	    //if end_house is homes, no other seq with remaining strt points exist. hence break.
	    break;

    }
    *ret_start_house = start_house;
    *ret_end_house = end_house;
    *ret_candies_collected = actual_collected_candies;
    PRINT_OUTPUT("\n\tStart at %d and go to home %d getting %d pieces of  candies",
	    start_house, end_house, actual_collected_candies);

    return (0);
}
#endif


/*
 * In solve2(), taking sum of same elements again.
 * For a given starting point (START), end point is found(END).
 * Then take the next starting point by subtracting value at START and
 * go on adding values after the END till max allowed value is reach
 */


#ifdef SOLUTION2
int solve2(int homes, int max_allowed_candies, int *candies_per_house,
	int *ret_start_house, int *ret_end_house, int *ret_candies_collected)
{
    int start_house=1, end_house=1;
    int START, END, SUM, DIFF, sum;
    START=1;
    END=1;
    DIFF = max_allowed_candies;
    SUM=sum=candies_per_house[1];
    int i;


    for (i=homes; i>0 && candies_per_house[i]>max_allowed_candies; i--);
    if (i<1) {
	DEBUG("\nEach house gave candies more than alllowed number!\n" \
		"Sad! Return with no candy in hand! :(");
	*ret_candies_collected = -1;
	return 0;
    } else {
	homes = i;
    }

    for (start_house=1; start_house<=homes; start_house++) {

	//If start point of the sequence has value > max allowed, then go to next element as start point
	//At the beginning, start_house=end_house and sum = candies_per_house[start_house]
	if (candies_per_house[start_house] > max_allowed_candies) {
	    end_house = start_house+1;
	    sum=candies_per_house[start_house+1];//initialize the sum
	    continue; 
	}

	for (int r=1; r<=homes-end_house; r++) {
	    //If by taking next value, sum will exceed max allowed value, then end is reached
	    //But if this seq is better than prev, i.e. diff from max is reduced, then update
	    //the sequence
	    if (sum+candies_per_house[end_house+r] > max_allowed_candies) {
		if ((max_allowed_candies-sum) < DIFF) {
		    START = start_house;
		    END = end_house+r-1;
		    SUM = sum;
		    DIFF = max_allowed_candies-SUM;
		}
		end_house = end_house+r-1;//Have found out seq here though not better. so temp end point needs to be updated
		//printf("\n\t[start][end][sum]|[%d][%d][%d]",START,END,SUM);
		break;
	    }
	    sum += candies_per_house[end_house+r];
	    //Corner case, when we have reached the end of houses
	    if (end_house+r == homes) {
		if ((max_allowed_candies-sum) < DIFF){
			START = start_house;
			END = end_house+r;
			SUM = sum;
			DIFF = max_allowed_candies-SUM;
		}
		end_house = end_house+r;
		break;
	    }
	}//End of for on r

	//If diff from max is zero, then we have got the best sequence. so break.
	//Or if we have reached at the end of the input array, then also break.
	if (DIFF==0 || end_house==homes)
	    break;

       /* If element after new end point > max allowed, 
	* no starting point exists between start point and end point
	* Hence re-initialize the start point and end point and sum 
	*/
	if (candies_per_house[end_house+1] > max_allowed_candies) {
		start_house = end_house;//after conitnue, outer for loop increments this by 1. hence dont increment now.
		end_house = end_house+1;
		sum = candies_per_house[end_house];
		continue;
	}

	
       /* subtract value at start house from current sum.
	* then increment the for loop on start house to select next start house &
	* start accumulating sum from current end house in for loop of r
	*/
	sum = sum - candies_per_house[start_house];
	
    }
    *ret_start_house = START;
    *ret_end_house = END;
    *ret_candies_collected = SUM;
    PRINT_OUTPUT("\n\tStart at %d and go to home %d collecting %d pieces of candy",
    	START, END, SUM);

}
#endif


