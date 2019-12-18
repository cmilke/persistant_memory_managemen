#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>

using namespace std;

int main(int argc, char** argv) {
    if (argc != 3) {
        cout << "Usage: manage_mem <tmpfs filepath> <arbitrary number>" << endl;
        exit(1);
    }
    int length_of_array = 100;
    size_t size_of_array_in_bytes = length_of_array * sizeof(int);

    // If you set the example_modifier to zero, I've programmed this to only read data (no write)
    char* filename = argv[1];
    int example_modifier = atoi(argv[2]);

    /*############################################################*/
    /*## The three lines of code that follow are the critical   ##*/
    /*## lines that you will need to copy into your own program ##*/
    /*## in order to access the persistant memory object.       ##*/
    /*## You will also need the two lines at the end of this    ##*/
    /*## file in order to safely close the memory file.         ##*/
    /*############################################################*/

    // Open the RAM file
    int ramspace = open(filename, O_RDWR);

    // Map the memory space of the file so we can write to it
    void* memory_map = mmap(0, size_of_array_in_bytes, PROT_READ | PROT_WRITE, MAP_SHARED, ramspace, 0);

    // Interpret the raw memory space as an arry of integers
    int* giant_array = (int*) memory_map;

    /*############################################################*/

    // Example where I iterate through the array, read out all the elements,
    // and then modify them based on the second argument given to this program
    for (int index = 0; index < length_of_array; index++) {
        cout << giant_array[index] << endl;
        if (example_modifier != 0) {
            giant_array[index] = index*example_modifier;
        }
    }

    // Close the file, leaving the array still accessible in RAM
    munmap( memory_map, size_of_array_in_bytes );
    close(ramspace);
}
