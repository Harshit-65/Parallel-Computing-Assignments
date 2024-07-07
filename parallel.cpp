#include <omp.h>
#include <algorithm>
#include <iostream>
#include <chrono>
#include <cstdlib>

const int SIZE = 355555555;

int find_max(int *arr, int left, int right)
{
    if (right - left <= 100000)
    {
        return *std::max_element(arr + left, arr + right);
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
        return std::max(max1, max2);
    }
}

int main()
{

    int *arr = new int[SIZE];

    srand(time(0));
    for (int i = 0; i < SIZE; i++)
    {
        arr[i] = rand() % 100;
    }

    int max_val;
    auto start = std::chrono::high_resolution_clock::now();
#pragma omp parallel num_threads(4)
#pragma omp single
    max_val = find_max(arr, 0, SIZE);
    auto end = std::chrono::high_resolution_clock::now();

    std::cout << "Maximum value is " << max_val << std::endl;

    std::chrono::duration<double> diff = end - start;
    std::cout << "Time taken to find the maximum value is " << diff.count() << " seconds" << std::endl;

    delete[] arr;

    return 0;
}
