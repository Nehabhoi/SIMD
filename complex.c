/* 
*	Neha Bhoi
*	026627560
*/

#include <immintrin.h>
#include <stdio.h>
#include <time.h>

// define normal function
typedef struct COMPLEX{
    int a;
    int b;
}Complex;
Complex multiply(Complex, Complex);
void multiply_vector();

Complex multiply(Complex x, Complex y){
    Complex z;
    z.a = x.a * y.a - x.b * y.b;
    z.b = x.a * y.b + x.b * y.a;
    return z;
}

void multiply_vector(){
  Complex v11,v12,v21,v22;
        v11.a=4;
        v11.b=5;
        v12.a=13;
        v12.b=6;

        v21.a=9;
        v21.b=3;
        v22.a=6;
        v22.b=7;

    Complex vect1[2],vect2[2],vec_result[2];
    vect1[0]=v11;
    vect1[1]=v12;

    vect2[0]=v21;
    vect2[1]=v22;

    printf("Result of complex vector multiplication\n");    
    for(int i=0;i<2;i++){
        vec_result[i] = multiply(vect1[i],vect2[i]);
        printf("After multiplication: %d+%di\n", vec_result[i].a, vec_result[i].b);
    }
}

// define function using 256-bit intrinsics
static void complex_multiplication(){
 
  __m256d vec1 = _mm256_setr_pd(4.0, 5.0, 13.0, 6.0);
  __m256d vec2 = _mm256_setr_pd(9.0, 3.0, 6.0, 7.0);
  __m256d neg = _mm256_setr_pd(1.0, -1.0, 1.0, -1.0);
  
  __m256d vec3 = _mm256_mul_pd(vec1, vec2);

  vec2 = _mm256_permute_pd(vec2, 0x5);
  
  vec2 = _mm256_mul_pd(vec2, neg);  
  
  __m256d vec4 = _mm256_mul_pd(vec1, vec2);

  vec1 = _mm256_hsub_pd(vec3, vec4);
  
  double* res = (double*)&vec1;
  
  printf("%lf %lf %lf %lf\n", res[0], res[1], res[2], res[3]);
}

int main() {
  
  //execute normal function
  clock_t begin_clock = clock();  
  multiply_vector();
  clock_t end_clock = clock();
  double time_of_execution = (double)((end_clock - begin_clock) *1000)/ CLOCKS_PER_SEC;
  printf("Time took by normal complex vector multiplication function: %f \n\n",time_of_execution);
  

  //execute function using 256-bit intrinsics
  printf("Result of complex vector multiplication\n");
  begin_clock = clock();  
  complex_multiplication();
  end_clock = clock();
  double time_of_execution_256 = (double)((end_clock - begin_clock) *1000)/ CLOCKS_PER_SEC;
  printf("Time took by AVX complex vector multiplication function: %f \n",time_of_execution_256);

  //Calculate speed up
  printf("Speed Up: %f \n",time_of_execution/time_of_execution_256);
  
  return 0;
}