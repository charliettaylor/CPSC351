#pragma once
#include <vector>

using namespace std;

class Block
{
    private:
    int size;
    vector<int> blocks;

    public:

    int getSize()
    {
        return size;
    }

    vector<int> getBlocks()
    {
        return blocks;
    }

    bool isFull()
    {
        int sum = 0;
        for (int block : blocks)
        {
            sum += block;
        }
        return size == sum;
    }

    int spaceLeft()
    {
        int sum = 0;
        for (int block : blocks)
            sum += block;
        return size - sum;
    }

    void clearblocks()
    {
        blocks.clear();
    }

    void push(int value)
    {
        blocks.push_back(value);
    }

    Block()
    {
        size = 0;
    }

    Block(int value)
    {
        size = value;
    }

    Block(const Block &b1)
    {
        size = b1.size;
        for (int i = 0; i < b1.blocks.size(); i++)
        {
            blocks.push_back(b1.blocks[i]);
        }
    }

    ~Block() 
    {
        
    }
};

