#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#define type __int128

bool eq(type *a, type *b, size_t n) {
  for (int i = 0; i < n; i++)
    if (a[i] != b[i])
      return false;
  return true;
}

void buadd(type *a, type *b, type *dest, size_t n) {
  type zero[n];
  type carry[n];
  type mask[n];
  for (int i = 0; i < n; i++) {
    dest[i] = a[i];
    carry[i] = b[i];
    zero[i] = 0;
  }
  while (!eq(carry, zero, n)){
    for (int i = 0; i < n; i++) {
      mask[i] = dest[i] & carry[i]; // use mask as tmp_carry
      dest[i] = dest[i] ^ carry[i];
      carry[i] = mask[i] << 1;
      mask[i] = mask[i] >> 127;//63;
      if (i != 0)
        carry[i] |= mask[i - 1];
    }
  }
}

void bu_shift_right(type* num, size_t n){
   for(int i = 0; i<n; i++)
   {
       num[i] = num[i] >> 1;
       if(i<n-1)
           num[i] |= (num[i+1] << 63);
   }
}

void butob(type* num, char* str, size_t n){
    size_t len = n*64+1;
    for(int i = len-2; i>=0; i--){
        str[i] = num[0]%2 + '0'; // if even then '0' else '1'
        bu_shift_right(num,n);
    }
    str[len-1] = '\0';
}

void fib(size_t N, size_t size, size_t bufl){
    //FILE * f = fopen("output.txt", "w");
    const size_t n = size;
    type* a = alloca(sizeof(type)*n);
    type* b = alloca(sizeof(type)*n);
    type* c = alloca(sizeof(type)*n);
    /* for(int i = 0; i<n; i++){ */
    /*     a[i] = 0; */
    /*     b[i] = 0; */
    /*     c[i] = 0; */
    /* } */
    b[0] = 1;
    //char bin[64*n+1];

    //char buffer [bufl];
    size_t p = 0;
    for(long j = 3; j<=N; j++){
        buadd(a, b, c, n);
        for(int i = 0; i<n; i++)
        {
            type* tmp = a;
            a = b;
            b = c;
            c = tmp;
        }
        //butob(c, bin, n);
        //p += sprintf(buffer+p, "%lu %s\n", j, bin);
    }
    //butob(c, bin, n);
    //p += sprintf(buffer+p, "%lu %s\n", N, bin);
    //fprintf(f, "%s", buffer);
    //fprintf(f, "%s\n", bin);
    //fclose(f);
}

int main(int argc, char* argv[]) {
    size_t niter = argc>1?strtol(argv[1], NULL, 10):100;
    struct timeval stop, start;
    const size_t digits = (0.71/128)*niter +1;
    const size_t buf = (digits*128 + 100);
    gettimeofday(&start, NULL);
    fib(niter, digits, buf ); // nth fib, bin digit necessary (log_2(~phi(x))), charc nec
    gettimeofday(&stop, NULL);
    long delta = (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec;
    printf("%lu %f\n", niter, (float) delta/1000000);
}
