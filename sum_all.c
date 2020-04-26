/* 
*	Neha Bhoi
*	026627560
*/

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
// include SSE intrinsics
#if defined(_MSC_VER)
#include <intrin.h>
#elif defined(__GNUC__) && (defined(__x86_64__) || defined(__i386__))
#include <x86intrin.h>
#endif
// define normal function
static int sum_naive(int total_number, int *a)
{
    int sum = 0;
    for (int i = 0; i < total_number; i++)
    {
        sum += a[i];
    }
    return sum;
}


// define vectorized sum function using 128-bit intrinsics
static int sum_vectorized_128Intrinsics(int total_number, int *a)
{

    // initialize vector sum = 0
    __m128i sum = _mm_setzero_si128();
    for (int i = 0; i < (total_number/4*4); i += 4) {
    	__m128i temp = _mm_loadu_si128((__m128i *) (a + i));
    	sum = _mm_add_epi32(temp, sum);
    }

    // initialize pointer ptr
    int ptr[4] = {0, 0, 0, 0};

    // store vector sum at pointer
    _mm_storeu_si128((__m128i *) ptr, sum);

    // sum all
    int final = 0;
    final += ptr[0] + ptr[1] + ptr[2] + ptr[3];

    // tail case
    for (int i = (total_number/4*4); i < total_number; i++)
    	final += a[i];
    return final;
}


// define vectorized sum function using 256-bit intrinsics
static int sum_vectorized_256Intrinsics(int total_number, int *a)
{
    // initialize vector sum = 0
    __m256i sum = _mm256_setzero_si256 ();
    for (int i = 0; i < (total_number/8*8); i += 8) {
        __m256i temp = _mm256_loadu_si256((__m256i *) (a + i));
        sum = _mm256_add_epi32(temp, sum);
    }
    // initialize pointer ptr
    int ptr[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    // store vector sum at pointer
    _mm256_storeu_si256((__m256i *) ptr, sum);
    // sum all
    int final = 0;
    final += ptr[0] + ptr[1] + ptr[2] + ptr[3] + ptr[4] + ptr[5] + ptr[6] + ptr[7];
    // tail case
    for (int i = (total_number/8*8); i < total_number; i++)
        final += a[i];
    return final;
}

int main(int argc, char **argv)
{
    const int total_number =1000;
    // initialize the array with random values
        srand48(time(NULL));
        int array[total_number] __attribute__((aligned(16)));
        for (int i = 0; i < total_number; i++)
        {
            array[i] = lrand48();
        }
        
    // execute normal sum function
    clock_t begin_clock = clock();
    int sum_native_result = sum_naive(total_number,array);
    clock_t end_clock = clock();
    double time_of_execution = (double)((end_clock - begin_clock) *1000)/ CLOCKS_PER_SEC;
    printf("Sum by normal sum function: %d\n",sum_native_result);
    printf("Time took by normal sum function: %f \n",time_of_execution);
   
    // execute vectorized sum function
    begin_clock = clock();
    int sum_vectorized_128_result = sum_vectorized_128Intrinsics(total_number,array);
    end_clock = clock();
    double time_of_execution_128 = (double)((end_clock - begin_clock) *1000)/ CLOCKS_PER_SEC;
    printf("Sum by vectorized sum function using 128-bit intrinsics: %d\n",sum_vectorized_128_result);
    printf("Time took by vectorized sum function using 128-bit intrinsics: %f \n",time_of_execution_128);
    printf("Speed up using 128-bit intrinsics: %f \n",time_of_execution/time_of_execution_128);

    // execute vectorized sum function
    begin_clock = clock();
    int sum_vectorized_256_result = sum_vectorized_256Intrinsics(total_number,array);
    end_clock = clock();
    double time_of_execution_256 = (double)((end_clock - begin_clock) *1000)/ CLOCKS_PER_SEC;
    printf("Sum by vectorized sum function using 256-bit intrinsics: %d\n",sum_vectorized_256_result);
    printf("Time took by vectorized sum function using 256-bit intrinsics: %f \n",time_of_execution_256);
    printf("Speed up using 256-bit intrinsics: %f \n",time_of_execution/time_of_execution_256);
    return 0;
}