#include <iostream>
#include <cstdlib>
#include <ctime>
#include <climits>
#include <omp.h>

// Function to find maximum using recursive decomposition
int findMaxRecursive(const int arr[], int left, int right)
{
    if (left == right)
    {
        return arr[left];
    }

    int mid = (left + right) / 2;
    int max_left, max_right;

#pragma omp task shared(max_left) // Create a task for left subarray
    {
        max_left = findMaxRecursive(arr, left, mid);
    }
#pragma omp task shared(max_right) // Create a task for right subarray
    {
        max_right = findMaxRecursive(arr, mid + 1, right);
    }

#pragma omp taskwait // Wait for both tasks to finish

    return std::max(max_left, max_right);
}

int main()
{
    const int SIZE = 10000000; // Size of the array
    int *arr = new int[SIZE];

    // Fill the array with random integers
    std::srand(std::time(nullptr));
    for (int i = 0; i < SIZE; ++i)
    {
        arr[i] = std::rand();
    }

    // Set number of threads
    omp_set_num_threads(4);

    // Sequential version
    int seq_max;
    double seq_start = omp_get_wtime();
    seq_max = findMaxRecursive(arr, 0, SIZE - 1);
    double seq_end = omp_get_wtime();
    std::cout << "Sequential Max: " << seq_max << std::endl;
    std::cout << "Sequential Time: " << seq_end - seq_start << " seconds" << std::endl;

    // Parallel version
    int par_max;
    double par_start = omp_get_wtime();
#pragma omp parallel
    {
#pragma omp single
        {
            par_max = findMaxRecursive(arr, 0, SIZE - 1);
        }
    }
    double par_end = omp_get_wtime();
    std::cout << "Parallel Max: " << par_max << std::endl;
    std::cout << "Parallel Time: " << par_end - par_start << " seconds" << std::endl;

    // Calculate speedup
    double speedup = (seq_end - seq_start) / (par_end - par_start);
    std::cout << "Speedup: " << speedup << std::endl;

    delete[] arr; // Free the dynamically allocated array
    return 0;
}
