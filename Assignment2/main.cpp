#include <iostream>
#include <vector>
#include <map>
#include <limits>
#include <mutex>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
using namespace std;

struct Block
{
    // size of block
    int size;
    // allocations to block
    vector<int> blocks;

    // return whether cell is full
    bool isFull() const{
        int sum = 0;
        for (int block : blocks)
            sum += block;
        return size == sum;
    }

    // return space left to allocate
    int spaceLeft() const{
        int sum = 0;
        for (int block : blocks)
            sum += block;
        return size - sum;
    }

    Block() : size(0) {}
};

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

// blocks to be filled
vector<Block> ram;
// size of blocks to be placed
vector<int> allocate;
// lock for threads
mutex mtx;

int main()
{
    vector<int> stuff = { 100, 200, 300 };
    Block *test;
    for(int num : stuff)
    {
        test = new Block();
        test->size = num;
        ram.push_back(*test);
    }
    
    allocate.push_back(10);
    allocate.push_back(100);
    allocate.push_back(20);
    bestFitAllocation();
    worstFitAllocation();
    firstFitAllocation(); 
    //bestFitAllocation();
    return 0;
}

// parse function to get user input

// function to load data into ram

struct args {
    int idx;
};

void firstFitAllocation()
{
    int size = ram.size();
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
    int size = ram.size();
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
    int size = ram.size();
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
    if(firstSpace(allocate[size]) != -1 && ram[firstSpace(size)].spaceLeft() >= size)
    {
        ram[firstSpace(size)].blocks.push_back(size);
        done = true;
    }
    mtx.unlock();
    
    if(!done)
    {
        printf("couldn't place block of size: %d", size);
    }
    return 0;
}

void *worstFit(void *params)
{
    bool done = false;
    mtx.lock();
    int idx = ((struct args*)params)->idx;
    int size = allocate[idx];
    if(mostSpace(allocate[size]) != -1 && ram[mostSpace(size)].spaceLeft() >= size)
    {
        ram[mostSpace(size)].blocks.push_back(size);
        done = true;
    }
    mtx.unlock();
    
    if(!done)
    {
        printf("couldn't place block of size: %d", size);
    }
    return 0;
}

void *bestFit(void *params)
{
    bool done = false;
    mtx.lock();
    int idx = ((struct args*)params) -> idx;
    int size = allocate[idx];
    if(leastSpace(allocate[size]) != -1 && ram[leastSpace(size)].spaceLeft() >= size)
    {
        ram[leastSpace(size)].blocks.push_back(size);
        done = true;
    }
    mtx.unlock();

    if(!done)
    {
        cout << "Could not place block of size: " << size << endl;
    }
    return 0;
}

// first fit
int firstSpace(int limit)
{
    int first = 0, idx;
    for(int i = 0; i < ram.size(); i++)
    {
        if(ram[i].spaceLeft() > limit)
        {
            first = ram[i].spaceLeft();
            idx = i;
            break;
        }
    }

    if(first < limit) 
    {
        return -1;
    }

    return idx;
}

// worst fit
int mostSpace(int limit)
{
    int max = 0, idx;
    for(int i = 0; i < ram.size(); i++)
    {
        if(ram[i].spaceLeft() > max && ram[i].spaceLeft() > limit)
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
int leastSpace(int limit)
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
        cout << "Block size: " << slot.size << "\n";
        cout << "Allocations are: ";
        for (int num : slot.blocks) { cout << num << " "; }
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
        slot.blocks.clear();
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