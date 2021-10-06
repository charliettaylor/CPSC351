#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/mman.h>
#include <vector>
#include <string>
#include <iostream>

using namespace std;

const vector<string> OUTPUT = { 
    "Please enter the process name: ",
    "Please enter AT: ",
    "Please enter BT: "
    };

vector<string> getInput();

void producer()
{
    // size in bytes
    const int SIZE = 4096;
    // name of the shared memory object
    const char *name = "OS";
    // strings written to shared memory
    const char *message_0 = "Hello";
    const char *message_1 = "World!";

    // shared memory file descriptor
    int shm_fd;
    // pointer to shared memory object
    void *ptr;
    char *charptr;

    // create the shared memory object
    shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
    
    // configure the size of the shared memory object
    ftruncate(shm_fd, SIZE);

    // memory map of the shared memory object
    ptr = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);
    charptr = (char *) ptr;

    // get input
    vector<string> input = getInput();

    // write the input to the shared memory
    for (string in : input)
    {
        sprintf(charptr, "%s" , in.data());
        charptr += in.size();
        sprintf(charptr, ",");
        charptr++;
    }
    cout << "\n";
}

vector<string> getInput()
{
    string input;
    vector<string> in;
    int count = 0;
    while (input != "done")
    {
        cout << OUTPUT[count % 3];
        cin >> input;
        if (input != "done"){
            in.push_back(input);
        }
        count++;
    }

    return in;
}