#include <iostream>
#include <vector>
#include <fstream>
#include <queue>

using namespace std;

//first come first serve
double fcfs(vector<int> at, vector<int> bt){
    int sz = at.size();

    //sort vectors by arrival time
    for (int i=0; i<sz-1;i++){
        for (int j=i+1;j<sz;j++){
            if (at[i]>at[j]){
                swap(at[i],at[j]);
                swap(bt[i],bt[j]);
            }
        }
    }

    int t=0;
    double totalWait=0;

    for (int i=0;i<sz;i++){
        if (t<at[i]) t=at[i];

        totalWait += t-at[i];
        t+=bt[i];
    }

    return totalWait/sz;
}

//shortest job first
double sjf(vector<int> at, vector<int> bt){
    int sz = at.size();
    vector<bool> finished(sz);
    for (int i=0;i<sz;i++){
        finished[i]=false;
    }
    int numFinished =0, t=0;
    double totalWait=0;

    while (numFinished<sz){
        int minIdx=-1;
        int minBT=100000;

        //find shortest available process
        for (int i=0; i<sz;i++){
            if (!finished[i]&& at[i]<=t && bt[i]<minBT){
                minBT = bt[i];
                minIdx = i;
            }
        }

        if (minIdx==-1){
            t++;
            continue;
        }

        totalWait += t - at[minIdx];
        t += bt[minIdx];
        finished[minIdx]= true;
        numFinished++;
    }
    return totalWait/sz;
}

//round robin
double roundRobin(vector<int> at, vector<int> bt, int quantum){
    int sz = at.size();
    vector<int> remaining =bt;
    vector<int> finishTimes(sz);
    for (int i=0; i<sz; i++){
        finishTimes[i] = 0;
    }

    queue<int> q;
    int t=0, nextProc=0;
    double totalWait=0;

    //sort vectors by arrival time
    for (int i=0; i<sz-1;i++){
        for (int j=i+1;j<sz;j++){
            if (at[i]>at[j]){
                swap(at[i],at[j]);
                swap(bt[i],bt[j]);
                swap(remaining[i], remaining[j]);
            }
        }
    }

    q.push(0);
    t = at[0];
    nextProc=1;

    while(!q.empty()){
        int idx = q.front();
        q.pop();

        //check if process will be completed this round
        if (remaining[idx]>quantum){
            t+=quantum;
            remaining[idx]-=quantum;
        }
        else{
            t+=remaining[idx];
            remaining[idx]=0;
            finishTimes[idx]=t;
        }

        while(nextProc<sz && at[nextProc]<=t){
            q.push(nextProc);
            nextProc++;
        }

        if (remaining[idx]>0) q.push(idx);

        if (q.empty()&& nextProc<sz){
            t = at[nextProc];
            q.push(nextProc);
            nextProc++;
        }
    }
     //calculate total waiting time
    for (int i=0; i<sz; i++){
        int tat = finishTimes[i]-at[i];
        totalWait += tat -bt[i];
    }
    return totalWait/sz;
}

int main(){
    int n;
    cout << "Enter number of processes: ";
    cin >> n;
    vector<int> at(n), bt(n);

    for (int i=0; i<n;i++){
        cout << "Enter arrival time and burst time of process: ";
        cin >> at[i] >>bt[i];
    }
    
    double fcfsWait = fcfs(at, bt);
    double sjfWait = sjf(at,bt);
    double rrWait = roundRobin(at,bt,2);

    ofstream out("t5results.csv");
    out << "Algorithm,Average Waiting Time\n";
    out << "FCFS," << fcfsWait << "\n";
    out << "SJF," << sjfWait << "\n";
    out << "Round Robin," << rrWait << "\n";
    out.close();

    cout << "FCFS: " << fcfsWait << endl;
    cout << "SJF: " << sjfWait << endl;
    cout << "Round Robin: " << rrWait << endl;
    cout << "Results stored in t5results.csv"<<endl;
}