#include <omp.h>
#include <algorithm>
#include <iostream>
#include <chrono>
#include <cstdlib>

using namespace std;
using namespace std::chrono;

const int SIZE = 355555;

int find_max(int *arr, int left, int right)
{
    if (right - left <= 1000)
    {
        return *max_element(arr + left, arr + right);
    }
    else
    {
        int mid = left + (right - left) / 2;
        int max1, max2;
#pragma omp task shared(max1)
        max1 = find_max(arr, left, mid);
#pragma omp task shared(max2)
        max2 = find_max(arr, mid, right);
#pragma omp taskwait
        return max(max1, max2);
    }
}

int main()
{

    int *arr = new int[SIZE];

    srand(time(0));
    for (int i = 0; i < SIZE; i++)
    {
        arr[i] = rand() % 1000;
    }

    int max_val;
    auto start_parallel = high_resolution_clock::now();
#pragma omp parallel num_threads(4)
#pragma omp single
    max_val = find_max(arr, 0, SIZE);
    auto end_parallel = high_resolution_clock::now();

    cout << "Maximum value is " << max_val << endl;

    duration<double> diff_parallel = end_parallel - start_parallel;
    cout << "Time taken to find the maximum value in parallel is " << diff_parallel.count() << " seconds" << endl;

    auto start_sequential = high_resolution_clock::now();
    max_val = arr[0];
    for (int i = 1; i < SIZE; i++)
    {
        if (arr[i] > max_val)
        {
            max_val = arr[i];
        }
    }
    auto end_sequential = high_resolution_clock::now();

    cout << "Maximum value is " << max_val << endl;

    duration<double> diff_sequential = end_sequential - start_sequential;
    cout << "Time taken to find the maximum value sequentially is " << diff_sequential.count() << " seconds" << endl;

    double speedup = diff_sequential.count() / diff_parallel.count();
    cout << "Speedup is " << speedup << endl;

    delete[] arr;

    return 0;
}
