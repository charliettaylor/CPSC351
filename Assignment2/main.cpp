#include <iostream>
#include <vector>
#include <vector>
#include <map>
#include <limits>
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
};

bool worstFit(vector<int>);
int mostSpace(int);
int leastSpace(int);
void clearRam();
int remainingSpace();

vector<Block> ram;

int main()
{
    return 0;
}

// parse function to get user input

// function to load data into ram

bool worstFit(vector<int> blocks)
{
    vector<int> unallocated;
    for(int block : blocks)
    {
        int idx, large;
        for(Block slot : ram)
        {
            if(mostSpace(block) != -1 && ram[mostSpace(block)].spaceLeft() >= block)
            {
                ram[mostSpace(block)].blocks.push_back(block);
            }

            unallocated.push_back(block);
            printf("couldn't place block of size: %d", block);
        }
    }

    printf("The space remaining: ", remainingSpace());
    printf("These block were not allocated:\n");
    for(int num : unallocated)
    {
        cout << num << endl;
    }
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

// clear ram
void clearRam()
{
    for(Block slot : ram)
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