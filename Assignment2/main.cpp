#include <iostream>
#include <vector>
#include <vector>
#include <map>
using namespace std;

struct FreeCell
{
    // size of cell
    int size;
    // allocations to cell
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

int main()
{
    return 0;
}