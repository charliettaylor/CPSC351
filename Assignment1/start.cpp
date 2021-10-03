#include <iostream>
#include <vector>
#include <bits/stdc++.h>
#include <vector>
using namespace std;

void getInput(int count);
void sortByArrival(vector<Process> p);

struct Process
{
    string ID;
    int AT, BT, WT, TAT;

    bool operator< (const Process &other) const{
        return AT < other.AT;
    }
};

// make our queues
vector<Process> Q0;
vector<Process> Q1;
vector<Process> Q2;

const vector<string> OUTPUT = { "Please enter the process name", "Please enter AT", "Please enter BT" };


int main(int argc, char *argv[])
{
    vector<Process> p;
    vector<string> params;
    string input;
    int pCount = 0;
    

    cout << "Enter the number of processes";
    cin >> pCount;

    getInput(pCount);
    // sorts processes by arrival time
    sortByArrival(Q0);

    return 0;
}

void getInput(int count)
{
    for (int i = 0; i < count; i++)
    {
        Process temp;
        Q0.push_back(temp);
        cout << OUTPUT[0];
        cin >> Q0[i].ID;

        cout << OUTPUT[1];
        cin >> Q0[i].AT;

        cout << OUTPUT[2];
        cin >> Q0[i].BT;
    }
}

void sortByArrival(vector<Process> p)
{
    sort(p.begin(), p.end());
}