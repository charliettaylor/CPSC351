#include <vector>

using namespace std;

class Block
{
    private:
    int size; // Size of ram page
    vector<int> blocks; // blocks allocated to page

    public:

    // Returns size of Ram page
    int getSize()
    {
        return size;
    }

    // Returns blocks allocated to page
    vector<int> getBlocks()
    {
        return blocks;
    }

    // Returns true if page is full
    bool isFull()
    {
        int sum = 0;
        for (int block : blocks)
        {
            sum += block;
        }
        return size == sum;
    }

    // Returns amount of space left on page
    int spaceLeft()
    {
        int sum = 0;
        for (int block : blocks)
            sum += block;
        return size - sum;
    }

    // Clears allocated blocks from page
    void clearblocks()
    {
        blocks.clear();
    }

    // Allocates block to page
    void push(int value)
    {
        blocks.push_back(value);
    }

    // Blank constructor, Default size = 0
    Block()
    {
        size = 0;
    }

    // Constructor uses int value for size, no blocks allocated
    Block(int value)
    {
        size = value;
    }

    // Copy constructor for use with for loops in range
    Block(const Block &b1)
    {
        size = b1.size;
        for (int i = 0; i < b1.blocks.size(); i++)
        {
            blocks.push_back(b1.blocks[i]);
        }
    }

    // Destructor, unneccesary
    ~Block() 
    {

    }
};

