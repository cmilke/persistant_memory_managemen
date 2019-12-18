#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>

using namespace std;


int main(int argc, char** argv) {
    if (argc != 3) {
        cout << "Usage: manage_mem <tmpfs filepath> <open/close>" << endl;
        exit(1);
    }
    /*############################################################*/
    /*## You will need to modify length of the array as per     ##*/
    /*## your particular requirments.                           ##*/
    /*############################################################*/
    int length_of_array = 100;
    size_t size_of_array_in_bytes = length_of_array * sizeof(int);

    char* filename = argv[1];
    if ( strcmp(argv[2],"open") == 0 ) {
        // Open and create tmpfs file
        int ramspace = open(filename, O_CREAT | O_RDWR | O_TRUNC, 0644);

        // We need to expand the size of the file,
        // so here we search ahead to where we want the file to end
        int seek_status = lseek(ramspace, size_of_array_in_bytes-1, SEEK_SET);
        if (seek_status == -1) {
            close(ramspace);
            perror("Error calling lseek() to 'stretch' the file");
            exit(EXIT_FAILURE);
        }

        // Write an empty character to the end of where we want the file to be,
        // expanding the filesize to this point
        int write_status = write(ramspace, "", 1);
        if (write_status != 1) {
            close(ramspace);
            perror("Error writing last byte of the file");
            exit(EXIT_FAILURE);
        }

        // Map the memory space of the file so we can write to it
        void* memory_map = mmap(0, size_of_array_in_bytes, PROT_READ | PROT_WRITE, MAP_SHARED, ramspace, 0);

        // Fill the array with arbitrary data for testing
        /*############################################################*/
        /*## You will need to modify this initialization step to    ##*/
        /*## match your particular situation.                       ##*/
        /*############################################################*/

        int* giant_array = (int*) memory_map;
        cout << "Assigned!" << endl;
        for (int index = 0; index < length_of_array; index++) {
            cout << "|----index = " << index << endl;
            giant_array[index] = index*2;
        }

        // Close the file, leaving the array in accessible RAM
        munmap( memory_map, size_of_array_in_bytes );
        close(ramspace);

    } else if ( strcmp(argv[2],"close") == 0 ) {
        //open the RAM file
        int ramspace = open(filename, O_RDWR);
        //delete the file
        unlink(filename);
        //close the c++ memory image of the file
        close(ramspace);

    } else {
        cerr << "I don't know what you want me to do!" << endl;
    }
}
