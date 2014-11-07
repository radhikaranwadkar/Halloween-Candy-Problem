#include "solve_omp.h"
#include "infra.h"

#ifdef SOLUTION_OMP
int solve_omp(int homes, int max_allowed_candies, int *candies_per_house,
	            int *ret_start_house, int *ret_end_house, int *candies_collected)
{
    int start_house=1, end_house=1, diff_from_max = max_allowed_candies;
    int actual_collected_candies;
    int min, max;


    array_min_max(candies_per_house+1, homes, &min, &max);
    if (max_allowed_candies < min) {
        printf("\nNo possible solution");
        *candies_collected = -1;
        return 0;
    }

    if (homes == 1) {
        start_house = end_house = 1;
        actual_collected_candies = candies_per_house[1];
        printf("\n\tStart at %d and go to home %d getting %d pieces of  candies",
                        start_house, end_house, actual_collected_candies);
        *ret_start_house = start_house;
        *ret_end_house = end_house;
        *candies_collected = actual_collected_candies;
        return (0);

    }

    int start_house_omp[NUM_THREADS];
    int end_house_omp[NUM_THREADS];
    int diff_from_max_omp[NUM_THREADS];
    int actual_collected_candies_omp[NUM_THREADS];

    int chunk = (homes/NUM_THREADS)+1;
    printf("\nTotal number of homes = %d", homes);
    
    double start_time, stop_time;
    start_time = omp_get_wtime();

#pragma omp parallel 
{
    int tid = omp_get_thread_num();
    if (tid == 0)
	debug("\nI am tid = %d & Num of processors = %d", tid, omp_get_num_procs());

    debug("\n\tFor tid=%d: START=%d; END<%d", tid, (tid*chunk)+1, (tid+1)*chunk);


    for (int i = (tid*chunk)+1, sum=0; i < (tid+1)*chunk; i++) {
	if(i<=homes) {

        for (int j=i, sum=0; j<=homes; j++) {
           /*
            * If starting element > max_allowed_candies then
            * the sequence is invalid. Hence break and consider
            * next starting point
            */
            if (candies_per_house[j] > max_allowed_candies) {
                //sum = 0;
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
            if (j==homes) {
                if (sum > max_allowed_candies)
                    break;
                else {
                    if ((max_allowed_candies - sum) < diff_from_max_omp[tid]) {
                        start_house_omp[tid] = i;
                        end_house_omp[tid] = j;
                        actual_collected_candies_omp[tid] = sum;
                        diff_from_max_omp[tid] = max_allowed_candies - actual_collected_candies_omp[tid];
                        //printf("\n\t[start][end][sum]|[%d][%d][%d]",i,j,sum);
                        break;
                    }
                }
            }
            if ( ( (sum + candies_per_house[j+1]) > max_allowed_candies )) {
                if((max_allowed_candies - sum) < diff_from_max_omp[tid]) {
                    start_house_omp[tid] = i;
                    end_house_omp[tid] = j;
                    actual_collected_candies_omp[tid] = sum;
                    diff_from_max_omp[tid] = max_allowed_candies - actual_collected_candies_omp[tid];
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
        if (diff_from_max_omp[tid] == 0)
            break;


    }//end of if(i<=homes)

    }//end of for on i

}//end of parallel section

    stop_time = omp_get_wtime();
    PRINT_TIME("\n\nTIME OF EXECUTION USING OPENMP: %.16g milliseconds\n", (stop_time-start_time)*1000);
    for(int i=0; i<NUM_THREADS; i++) {
	debug("\n\tstart_house_omp[i] %d",start_house_omp[i]);
	debug("\n\tend_house_omp[i] %d",end_house_omp[i]);
	debug("\n\tactual_collected_candies_omp[i] %d",actual_collected_candies_omp[i]);
	debug("\n\tdiff_from_max_omp[i] %d",diff_from_max_omp[i]);
	debug("\n");
    }

    *ret_start_house = start_house;
    *ret_end_house = end_house;
    *candies_collected = actual_collected_candies;
    debug("\n\tStart at %d and go to home %d getting %d pieces of  candies",
            start_house, end_house, actual_collected_candies);

    return (0);


}
#endif

