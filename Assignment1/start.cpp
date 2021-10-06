#include <iostream>
#include <vector>
#include <bits/stdc++.h>
#include <vector>
#include <map>
#include "consumer.cpp"
using namespace std;

struct Process
{
    string ID;
    // arrival, burst, process, interrupt
    int AT, BT, PT, IT;

    bool operator< (const Process &other) const{
        return AT < other.AT;
    }

    bool operator== (const Process other) const{
        return this->AT == other.AT && this->ID == other.ID && this->BT == other.BT;
    }

    Process() : AT(0), BT(0), PT(0), IT(0) {}
};

void parse(vector<string>& cou);
void sortByArrival(vector<Process>& p);
void scheduler();
int qCheck(vector<Process>& q);
void roundRobin(int idx, int q, vector<Process>& queue);
string pop_front(vector<string>& input);
void completeProcess(vector<Process>& queue, int i);
void commentator(Process& p, vector<Process>& q, int length, bool done);


// make our queues
vector<Process> Q0;
vector<Process> Q1;
vector<Process> Q2;
vector<Process> done;

// stime for scheduler
int stime = 0;

int main()
{
    // call consumer to get input from shared memory
    vector<string> input = consumer();

    // parse input and put into Q0
    parse(input);

    // sorts processes by arrival stime
    sortByArrival(Q0);

    // run scheduler, should have commentator function
    
    scheduler();

    return 0;
}

// takes in consumer vector, insert the input into structs
// insert structs into the queues
// ex. [1, p1, 0, 10]
void parse(vector<string>& input)
{ 
    //check if vector size is a mult of 3 and every 2nd and 3rd are nums
    if(input.size() % 3  !=  0)
    {
        cout << "exiting bad input" << endl;
        exit(1);
    }

    for (int i = 0; i < input.size(); i++)
    {
        Process temp;
        Q0.push_back(temp);
        Q0[i].ID = pop_front(input);

        try
        {
            Q0[i].AT= stoi(pop_front(input));
        }
        catch(exception &err)
        {
            cerr << "Conversion failure"<< endl;
            exit(1);
        }

        try
        {
            Q0[i].BT  = stoi(pop_front(input));
        }
        catch(exception &err)
        {
            cerr << "Conversion failure"<< endl;
            exit(1);
        }
    }

    vector<Process> test = Q0;
}


void sortByArrival(vector<Process>& q)
{
    sort(q.begin(), q.end());
}

// n is number of processes
void scheduler()
{
    // while there is at least 1 process in some queue
    while (!Q0.empty() || !Q1.empty() || !Q2.empty())
    {
        if (qCheck(Q0) != -1)
        {
            roundRobin(qCheck(Q0), 8, Q0);
        }
        else if (qCheck(Q1) != -1)
        {
            roundRobin(qCheck(Q1), 16, Q1);
        }
        else if (qCheck(Q2) != -1)
        {
            // only ran when other q's are empty or p's not arrived
            completeProcess(Q2, 0);
        }
        else
        {
            stime += 1;
        }
    }
}

/* check if there is a process to be executed
returns -1 if no such process is found */
int qCheck(vector<Process>& q)
{
    for (int i = 0; i < q.size(); i++)
    {
        if (q[i].AT <= stime)
        {
            return i;
        }
    }

    return -1;
}

void roundRobin(int idx, int q, vector<Process>& queue)
{
    int BT = queue[idx].BT;
    if (BT - queue[idx].PT <= q)
    {
        completeProcess(queue, idx);
    }
    else
    {
        // increment stime
        stime += q;
        // add the quantum to the process stime
        queue[idx].PT += q;
        // add to the interrupt count
        queue[idx].IT += 1;
        // output to user what's happening
        commentator(queue[idx], queue, q, false);
        // move the process to the next queue and delete from current
        if(queue == Q0)
        {
            Q1.push_back(queue[idx]);
            queue.erase(queue.begin() + idx);
        }
        else if(queue == Q1)
        {
            Q2.push_back(queue[idx]);
            queue.erase(queue.begin() + idx);
        }
        else
        {
            cerr << "Incorrect queue entered for Round Robin";
            exit(1);
        }
    }
}

// when the quantum of the queue is >= the burst stime
void completeProcess(vector<Process>& queue, int i)
{
    // increment stime remaining to finish process
    stime += queue[i].BT - queue[i].PT;
    // set the process stime to the burst
    queue[i].PT = queue[i].BT;
    // let user know what's happening
    commentator(queue[i], queue, queue[i].BT - queue[i].PT, true);
    // the process is complete, so move it to done
    done.push_back(queue[i]);
    // remove process from queue
    queue.erase(queue.begin() + i);
}

string pop_front(vector<string>& input)
{
    string temp = input[0];
    input.erase(input.begin());
    return temp;
}

void commentator(Process& p, vector<Process>& q, int length, bool done)
{
    string qname;
    if (q == Q0)
    {
        qname = "Queue 0";
    }
    else if (q == Q1)
    {
        qname = "Queue 1";
    }
    else
    {
        qname = "Queue 2";
    }

    if (done)
    {
        cout << p.ID << " is interrupted " << p.IT <<\
        " time(s) and completes on " << qname <<\
        ", TAT for " << p.ID << " is " << stime - p.AT << "\n";
    }
    else
    {
        cout << p.ID << " at " << qname << " it is executed for " << length << "\n";
    }
}
