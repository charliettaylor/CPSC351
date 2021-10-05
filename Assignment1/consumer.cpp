#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/mman.h>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

vector<string> consumer()
{
    // size in bytes
    const int SIZE = 4096;
    // name of the shared memory object
    const char *name = "OS";

    // shared memory file descriptor
    int shm_fd;
    // pointer to shared memory object
    void *ptr;

    // open the shared memory object
    shm_fd = shm_open(name, O_RDONLY, 0666);

    // memory map the shared memory object
    ptr = mmap(0, SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);

    // read from the shared memory object
    //printf("%s", (char *)ptr);

    // read from memory and put into vector
    char *charptr = (char *)ptr; // Create char pointer and make it equal to typecast ptr
    std::vector<std::string> strvec; // Create vector for the function to output
    std::string str; // Create holding variable for the string values
    do {
        if (*charptr != ','){ // Check if comma delimiter is hit
            str += *charptr; // Add character to string variable
            charptr++; // Move forward in memory
        }
        else { // Execute if delimiter is hit
            strvec.push_back(str); // Add string as an element to the output vector
            str.clear(); // clear the string so that new info can be held
            charptr++; // Move forward in memory
        }
    } while (*charptr != '\0'); // Continue loop unless the end was reached


    // remove the shared memory object
    shm_unlink(name);

    // return string vector, (# of processes, input...)
    return strvec;
    //return 0;
}
