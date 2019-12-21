#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <time.h>



const long int amount = 50000000; // Kolichestvo tochek na kajdiy pryamougolnik
double S = 0;

double func(double x) // Only for positive function
{
    return x*x;
}

struct rectangle
{
    double a;
    double b;
    double count;
};

void *calculate(void * annoy)
{
    struct rectangle * limit = (struct rectangle *)annoy;
    int i;
    long int hit = 0;
    double x, y, dx, M=0;
    dx = (double)(limit->b - limit->a)/limit->count;
    for(i=0; i <= limit->count; i++)
    {
        if(func(limit->a+dx*i)>M)
            M = func(limit->a+dx*i);
    }

    for(i=0; i < limit->count; i++)
    {
        int r1, r2;
        rand_r(&r1);
        rand_r(&r2);
        x = limit->a + (double)(limit->b - limit->a)*r1/RAND_MAX;
        y = (double)M*r2/RAND_MAX;

        if(y<func(x))
            hit++;
    }

    S += (limit->b - limit->a)*M*((double)hit/limit->count);

    return NULL;
}

int main()
{
    int  n, i, result;
    double lk, rk, dl;

    printf("Amount of threads?\n");
    scanf("%d", &n);
    printf("Left end of section?\n");
    scanf("%lf", &lk);
    printf("Right end of section?\n");
    scanf("%lf", &rk);

    pthread_t * thid = (pthread_t *)malloc(n*sizeof(pthread_t ));
    struct rectangle * rects = (struct rectangle *)malloc(n*sizeof(struct rectangle));

    dl = (double)(rk-lk)/n;

    for(i = 0; i<n; i++)
    {
        rects[i].a = lk + dl*i;
        rects[i].b = rects[i].a + dl;
        rects[i].count = amount / n;
    }

    clock_t start = clock();

    for( i = 0; i<n; i++)
    {
        result = pthread_create( thid + i, (pthread_attr_t *)NULL, calculate, rects + i);
        if(result != 0){
            printf ("Error on thread create, return value = %d\n", result);
            exit(-1);
        }
    }

    for( i = 0; i<n; i++)
    {
        pthread_join(thid[i], (void **)NULL);
    }

    printf("Time: %.3f ms\n", 1000*((double)clock()-start)/((double)CLOCKS_PER_SEC));
    printf("Square is %lf\n", S);

    free(thid);
    free(rects);

    return 0;
}
