#include <iostream>
#include <vector>
using namespace std;

vector<string> getInput(vector<string> output, int& count);

struct Process
{
    string id;
    int at;
    int bt;
};

int main(int argc, char *argv[])
{
    vector<Process> p;
    vector<string> params;
    string input;
    int inputCount = 0;
    vector<string> output = { "Please enter the process name", "Please enter AT", "Please enter BT" };

    params = getInput(output, inputCount);

    // write function to parse user inputs

    return 0;
}

vector<string> getInput(vector<string> output, int& count)
{
    string input;
    vector<string> params;

    while (input != "done")
    {
        // each process has 3 pieces of data
        cout << output[count % 3];
        cin >> input;
        if (input != "done")
        {
            params.push_back(input);
            count++;
        }
    }

    return params;
}