//
// Created by alvar on 08/06/2023.
//

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <mpi.h>

using namespace std;

void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}
int partition(int* array, int low, int high) {
    int pivot = array[high];
    int i = low - 1;

    for (int j = low; j <= high - 1; j++) {
        if (array[j] < pivot) {
            i++;
            swap(&array[i], &array[j]);
        }
    }

    swap(&array[i + 1], &array[high]);
    return i + 1;
}
void quicksort(int* array, int low, int high) {
    if (low < high) {
        int pivotIndex = partition(array, low, high);
        quicksort(array, low, pivotIndex - 1);
        quicksort(array, pivotIndex + 1, high);
    }
}



int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int* array = nullptr;
    int n = 0;

    if (rank == 0) {
        // Input size
        cout << "Enter the size of the array: ";
        cin >> n;

        // Generate random array
        srand(time(nullptr));
        array = new int[n];
        for (int i = 0; i < n; i++) {
            array[i] = rand() % 100;
        }

        // Print original array
        cout << "Original Array: ";
        for (int i = 0; i < n; i++) {
            cout << array[i] << " ";
        }
        cout << endl;
    }

    // Broadcast array size
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Scatter array to all processes
    int* subarray = new int[n];
    MPI_Scatter(array, n, MPI_INT, subarray, n, MPI_INT, 0, MPI_COMM_WORLD);

    // Perform quicksort
    quicksort(subarray, 0, n - 1);

    // Gather sorted subarrays to root process
    MPI_Gather(subarray, n, MPI_INT, array, n, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        // Print sorted array
        cout << "Sorted Array: ";
        for (int i = 0; i < n; i++) {
            cout << array[i] << " ";
        }
        cout << endl;

        delete[] array;
    }

    delete[] subarray;

    MPI_Finalize();
    return 0;
}







