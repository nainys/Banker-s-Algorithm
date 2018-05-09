#include <iostream>
#include <bits/stdc++.h>
#include <fstream>
using namespace std;

list<int> *adj;
vector<int> pid,arrival;  // process id's and arrival time
unsigned int timenow = 0;
priority_queue<int, vector<int>, greater<int> >pq;
stack<int> st;
std::vector<int> safe;  // Safe sequence

int main(int arg,char *args[])
{
  ifstream fi;  // Input file stream
  ofstream fo;  // Output file stream
  fi.open(args[1]);
  fo.open(args[2]);
  int n,r,q;  // No of processes, resources and queries respectively
  fi>>n>>r>>q;
  list<int>::iterator it;

  int avail[r]; // No of available resources
  int maximum[n][r], alloc[n][r]; // Max no of resources required by each process and no of resources currently allocated

  map<int,int> no_q;  // No of  queries for a process



  for(int i=0;i<n;i++)
  {
    int x;
    fi>>x;

    pid.push_back(x);
    no_q.insert(pair<int,int>(x,0));
  }

  int max_arrtime=0;
  int min_time = INT_MAX;

  for(int i=0;i<n;i++)
  {
    int x;
    fi>>x;
    if(x>max_arrtime)
    max_arrtime = x;
    if(x<min_time)
    min_time  = x;
    arrival.push_back(x);
  }
  timenow = min_time;
  adj = new list<int> [max_arrtime+1];

  for(int i=0;i<n;i++)
  {
    adj[arrival[i]].push_back(pid[i]);
  }

  for(int i=0;i<r;i++)
  fi>>avail[i];

  for(int i=0;i<n;i++)
  {
    for(int j=0;j<r;j++)
    {
      fi>>maximum[pid[i]][j]; // Maximum no of resources that can be requested by a process
    }
  }


  for(int i=0;i<n;i++)
  {
    for(int j=0;j<r;j++)
    {
      fi>>alloc[pid[i]][j]; // No of resources currently allocated to the process
    }
  }


// Query struct corresponding to resources requested by a process
  struct query
  {
    int pid;
    vector<int> res;
  };
  query qu;
  std::vector<query> v;
  std::vector<query>::iterator itr;


  while(q--)
  {

    int a;
    fi>>a;
    vector<int> local;
    for(int i=0;i<r;i++)
    {
      int x;
      fi>>x;
      local.push_back(x);

    }
    qu.res=local;
    qu.pid = a;


    v.push_back(qu);
    no_q[a]++;  // Increment no of queries for the process
  }

  list<int>::iterator j;

  for(j=adj[timenow].begin();j!=adj[timenow].end();j++)
  pq.push(*j);

  while(!pq.empty() )
  {
    int process = pq.top();
    pq.pop();
    for(itr = v.begin();itr!=v.end();itr++)
    {
      if((*itr).pid == process && no_q[process]>0)
      {


        int flag = 1;

        for(int i=0;i<r;i++)
        {
          if((avail[i]-(*itr).res[i])<0)  // If need cannot be fulfilled
          flag = 0;
        }

        // Requested resources can be granted
        if(flag)
        {
          fo<<"Grant"<<endl;
          safe.push_back(process);
          no_q[process]--;  // Decrease no of queries corresponding to the process

          for(int i=0;i<r;i++)
          {
            alloc[process][i]+=(*itr).res[i]; // Increase allocated resources for the processes
          }
          for(int i=0;i<r;i++)
          {
            avail[i]-=(*itr).res[i];  // Decrease currently available resources
          }

          timenow++;
          // Once process finished release acquired resources
          if(no_q[process]==0)
          {
            for(int i=0;i<r;i++)
            {
              avail[i]+= alloc[process][i];
            }
          }
          while(!st.empty())
          {
            pq.push(st.top());
            st.pop();
          }
          v.erase(itr);
          if(timenow<=max_arrtime)
          {
            for(j=adj[timenow].begin();j!=adj[timenow].end();j++)
            pq.push(*j);

          }
          if(pq.top() < process  )
          {
            if(no_q[process]>0)
            pq.push(process);
            break;
          }

        }
        //grant if ends

        // Requested resources cannot be granted
        else
        {
          fo<<"Deny"<<endl;
          st.push(process);

          if(pq.empty() == true)
          {
            timenow++;
            if(timenow<=max_arrtime)
            {
              for(j=adj[timenow].begin();j!=adj[timenow].end();j++)
              pq.push(*j);
            }
            else
            {
              fo<<"Not safe";
              return 0;
            }
            while(!st.empty())
            {
              pq.push(st.top());
              st.pop();
            }
          }

          break;
        }


      }

      if(v.size() == 0)
      break;
    }

    if(pq.size()==0 && timenow<max_arrtime)
    {
      int done = 0;
      while(!done)
      {
        timenow++;
        for(j=adj[timenow].begin();j!=adj[timenow].end();j++)
        {
          pq.push(*j);
          done = 1;

        }
      }
    }

  }
  for(int i=0;i<safe.size()-1;i++)
  fo<<safe[i]<<" ";

  fo<<safe[safe.size()-1];


  return 0;
}
