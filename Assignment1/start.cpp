#include <iostream>
#include <vector>
#include <bits/stdc++.h>
#include <vector>
#include "producer.cpp"
#include "consumer.cpp"
using namespace std;

void getInput(int count);
void sortByArrival(vector<Process> p);

struct Process
{
    string ID;
    int AT, BT, WT, TAT, FT, PT;

    bool operator< (const Process &other) const{
        return AT < other.AT;
    }
};

// make our queues
vector<Process> Q0;
vector<Process> Q1;
vector<Process> Q2;

const vector<string> OUTPUT = { "Please enter the process name: ", "Please enter AT: ", "Please enter BT: " };


int main()
{
    producer();

    // call consumer to get input from shared memory
    vector<string> input = consumer();

    // parse input and put into Q0
    parse(input);

    // sorts processes by arrival time
    sortByArrival(Q0);

    // scheduling code...
    // process q0 +8 to pt
    // process q1 +16 to pt
    // process q2 +remaining to pt
    // keep track of time to check arrival time

    // output function


    return 0;
}

// takes in consumer vector, insert the input into structs
// insert structs into the queues
// ex. [1, p1, 0, 10]
void parse(vector<string> input)
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