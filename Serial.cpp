#include <iostream>
#include <chrono>

using namespace std;

#define SIZE 250

void multiply(double a[SIZE][SIZE], double b[SIZE][SIZE], double c[SIZE][SIZE])
{
    int i, j, k;
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

    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            a[i][j] = rand() % 10;
            b[i][j] = rand() % 10;
            c[i][j] = 0.0;
        }
    }

    auto start = chrono::high_resolution_clock::now();
    multiply(a, b, c);
    auto stop = chrono::high_resolution_clock::now();

    auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
    cout << "Time taken by function: " << duration.count() << " microseconds" << endl;

    return 0;
}
