#include <iostream>
#include <vector>
#include <map>
#include <limits>
#include <mutex>
#include <pthread.h>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "block.cpp"

using namespace std;


void worstFitAllocation();
void bestFitAllocation();
void firstFitAllocation(); 
void *worstFit(void *params);
void *bestFit(void *params);
void *firstFit(void *params); 
int mostSpace(int);
int leastSpace(int);
int firstSpace(int);
void printRam();
void clearRam();
int remainingSpace();
void printSpace();
void parseFunction(string path);

// blocks to be filled
vector<Block> ram;
// size of blocks to be placed
vector<int> allocate;
// lock for threads
mutex mtx;

int main(int argc, char *argv[])
{
    if (argc == 1) // Check to see if no commandline arguments are made
    {
        parseFunction("input.txt");
    }
    if (argc > 1) // If commandline argument is given for input file, use that for parsing data
    {
        parseFunction(argv[1]);
    }
    
    bestFitAllocation(); 
    worstFitAllocation();
    firstFitAllocation(); 

    return 0;
}


struct args {
    int idx;
};

void firstFitAllocation()
{
    int size = allocate.size();
    pthread_t threads[size];

    for (int i = 0; i < size; i++)
    {
        struct args *thread_args = (struct args *)malloc(sizeof(struct args));
        thread_args->idx = i;
        if (pthread_create(&threads[i], NULL, firstFit, (void *)thread_args) != 0)
        {
            perror("pthread_create");
            exit(-1);
        }
    }

    for (int i = 0; i < size; i++)
    {
        if (pthread_join(threads[i], NULL) != 0)
        {
            perror("pthread_join");
            exit(-1);
        }
    }

    printSpace();
    printf("First allocation space remaining: %d\n", remainingSpace());
    printRam();
    clearRam();
}

void worstFitAllocation()
{
    int size = allocate.size();
    pthread_t threads[size];

    for (int i = 0; i < size; i++)
    {
        struct args *thread_args = (struct args *)malloc(sizeof(struct args));
        thread_args->idx = i;
        if (pthread_create(&threads[i], NULL, worstFit, (void *)thread_args) != 0)
        {
            perror("pthread_create");
            exit(-1);
        }
    }

    for (int i = 0; i < size; i++)
    {
        if (pthread_join(threads[i], NULL) != 0)
        {
            perror("pthread_join");
            exit(-1);
        }
    }

    printSpace();
    printf("Worst allocation space remaining: %d\n", remainingSpace());
    printRam();
    clearRam();
}

void bestFitAllocation() 
{
    int size = allocate.size();
    pthread_t threads[size];

    for (int i = 0; i < size; i++)
    {
        struct args *thread_args = (struct args *)malloc(sizeof(struct args));
        thread_args -> idx = i;
        if (pthread_create(&threads[i], NULL, bestFit, (void *)thread_args) != 0)
        {
            perror("pthread_create");
            exit(-1);
        }
    }

    for (int i = 0; i < size; i++)
    {
        if (pthread_join(threads[i], NULL) != 0)
        {
            perror("pthread_join");
            exit(-1);
        }
    }

    printSpace();
    cout << "Best allocation space remaining: " << remainingSpace() << endl;
    printRam();
    clearRam();

}

void *firstFit(void *params)
{
    bool done = false;
    mtx.lock();
    int idx = ((struct args*)params)->idx;
    int size = allocate[idx];
    if(firstSpace(size) != -1 && ram[firstSpace(size)].spaceLeft() >= size)
    {
        ram[firstSpace(size)].push(size);
        done = true;
    }
    mtx.unlock();
    
    if(!done)
    {
        printf("First fit couldn't place block of size: %d \n", size);
    }
    return 0;
}

void *worstFit(void *params)
{
    bool done = false;
    mtx.lock();
    int idx = ((struct args*)params)->idx;
    int size = allocate[idx];
    if(mostSpace(size) != -1 && ram[mostSpace(size)].spaceLeft() >= size)
    {
        ram[mostSpace(size)].push(size);
        done = true;
    }
    mtx.unlock();
    
    if(!done)
    {
        printf("Worst fit couldn't place block of size: %d \n", size);
    }
    return 0;
}

void *bestFit(void *params)
{
    mtx.lock();
    bool done = false;    
    int idx = ((struct args*)params) -> idx;
    int size = allocate[idx];
    if(leastSpace(size) != -1 && ram[leastSpace(size)].spaceLeft() >= size)
    {
        ram[leastSpace(size)].push(size);
        done = true;
    }
    mtx.unlock();

    if(!done)
    {
        cout << "Best fit Could not place block of size: " << size << endl;
    }
    return 0;
}

// first fit
int firstSpace(int limit) // limit is size of block to be placed
{
    for(int i = 0; i < ram.size(); i++)
    {
        if(ram[i].spaceLeft() >= limit)
        {
            return i;   
        }
    }

    return -1;

}

// worst fit
int mostSpace(int limit) // limit is size of block to be placed
{
    int max = 0, idx;
    for(int i = 0; i < ram.size(); i++)
    {
        if(ram[i].spaceLeft() > max && ram[i].spaceLeft() >= limit) 
        {
            max = ram[i].spaceLeft();
            idx = i;
        }
    }

    if(max < limit)
    {
        return -1;
    }

    return idx;
}

// best fit
// looks for smallest space that also fits the limit condition
int leastSpace(int limit) // limit is the size of block to be placed
{
    int min = numeric_limits<int>::max(), idx;
    for(int i = 0; i < ram.size(); i++)
    {
        if(ram[i].spaceLeft() < min && ram[i].spaceLeft() >= limit)
        {
            min = ram[i].spaceLeft();
            idx = i;
        }
    }

    if(min < limit)
    {
        return -1;
    }

    return idx;
}

// prints out contents of ram
void printRam()
{
    for(Block slot : ram)
    {
        printSpace();
        cout << "Block size: " << slot.getSize() << "\n";
        cout << "Allocations are: ";
        for (int num : slot.getBlocks()) { cout << num << " "; }
        cout << "\n";
    }
    printSpace();
    cout << endl;
}

// clear ram
void clearRam()
{
    for(Block& slot : ram)
    {
        slot.clearblocks();
    }
}

// space left in ram
int remainingSpace()
{
    int sum = 0;
    for(Block slot : ram)
    {
        sum += slot.spaceLeft();
    }

    return sum;
}

void printSpace()
{
    cout << "========================================" << "\n";
}

// Parse file to load data
void parseFunction(string path)
{
    string myline;
    string input;
    Block *test;
    ifstream inFile;
    inFile.open(path);
    if (inFile.is_open())
    {
        getline(inFile, myline); // Get first line of file for ram blocks
        for (char ch : myline)
        {
            if (ch != ',')
            {
                input += ch;
            }
            else // Put input into block in ram
            {
                test = new Block(stoi(input));
                ram.push_back(*test);
                input.clear();
            }
        }
        getline(inFile, myline);
        for (char ch : myline)
        {
            if (ch != ',')
            {
                input += ch;
            }
            else // Push input into allocate
            {
                allocate.push_back(stoi(input));
                input.clear();
            }
        }
    }
    inFile.close();


    //Print out data read from file
    cout << "Parsed data is: " << endl;
    cout << "Ram pages: " << endl;
    for (Block b : ram)
    {
        cout << b.getSize() << " ";
    }
    cout << endl << endl;
    cout << "Blocks to be allocated: " << endl;
    for (int i : allocate)
    {
        cout << i << " ";
    }
    cout << endl << endl;
}