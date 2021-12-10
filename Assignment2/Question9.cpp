//Question 9
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
#include <thread>
#include <cmath>
#include <stdio.h>
#include <math.h>
#include <cstdlib>
using namespace std;
int pages =  0;
int page_size = 0;
int ramsize = 0;
int phys_address_size = 0;

mutex mtx;

void *ramresult(void *tid);
void *physaddresscalc(void *tid);

int main(int argc, char *argv[])
{
  pthread_t ramcalc;
  pthread_t physcalc;
  if(argc<=1)
  {
    cout << "Invalid input try again." << endl;
    exit(1);
  }
  pages= atoi(argv[1]);
  page_size= atoi(argv[3]);

  pthread_create(&ramcalc, NULL, ramresult,(void *)&ramcalc);
  pthread_create(&physcalc, NULL, physaddresscalc,(void *)&physcalc);

  pthread_join(ramcalc, NULL);
  pthread_join(physcalc, NULL);
  cout << "Suppose RAM contains " << pages << " pages, with " << page_size << " bytes page size. What is the total RAM size? \n" << endl;
  cout << ramsize <<" bytes.\n";

  cout << "How many bits are needed to each physical address? \n" << endl;
  cout << phys_address_size << " bytes." << endl;


  return 0;
}

void *ramresult(void *tid)
{
  mtx.lock();
  ramsize = pages * page_size;
  mtx.unlock();
  return 0;
}

void *physaddresscalc(void *tid)
{
  mtx.lock();
  phys_address_size = log2 (ramsize);
  mtx.unlock();
  return 0;
}
