#include <omp.h>
#include <iostream>
#include <chrono>

#define SIZE 250
#define NUM_THREADS 4

void multiply(double a[SIZE][SIZE], double b[SIZE][SIZE], double c[SIZE][SIZE])
{
    int i, j, k;
#pragma omp parallel for private(i, j, k) shared(a, b, c)
    for (i = 0; i < SIZE; i++)
    {
        for (j = 0; j < SIZE; j++)
        {
            c[i][j] = 0.0;
            for (k = 0; k < SIZE; k++)
            {
                c[i][j] = c[i][j] + a[i][k] * b[k][j];
            }
        }
    }
}

int main()
{
    double a[SIZE][SIZE], b[SIZE][SIZE], c[SIZE][SIZE];
    omp_set_num_threads(NUM_THREADS);

    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            a[i][j] = rand() % 10;
            b[i][j] = rand() % 10;
            c[i][j] = 0.0;
        }
    }

    auto start = std::chrono::high_resolution_clock::now();
    multiply(a, b, c);
    auto stop = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "Time taken by function: " << duration.count() << " microseconds" << std::endl;

    return 0;
}
