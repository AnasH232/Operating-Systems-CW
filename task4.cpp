#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

int main(){
    ifstream file("task4.txt");

    int numProcess, numResource;
    file >> numProcess;
    file>>numResource;

    //resource vectors
    vector<int> E(numResource);
    vector<int> availableRes(numResource);

    for(int i=0;i<numResource;i++){
        file>>E[i];
    }

    //process matrices
    vector<vector<int>> C(numProcess, vector<int>(numResource));
    vector<vector<int>> R(numProcess, vector<int>(numResource));

    //loops to input data into matrices
    for (int i=0;i<numProcess;i++){
        for (int j=0; j<numResource;j++){
            file>>C[i][j];
        }
    }

    for (int i=0;i<numProcess;i++){
        for (int j=0; j<numResource;j++){
            file>>R[i][j];
        }
    }

    //loop to calculate available resources
    for (int i=0;i<numResource;i++){
        int sum=0;
        for (int j=0;j<numProcess;j++){
            sum+= C[j][i];
        }
        availableRes[i]=E[i]-sum;
    }

    vector<bool> isCompleted(numProcess);
    for (int i=0; i<numProcess;i++){
        isCompleted[i]=false;
    }
    bool cont=true;
    
    while (cont){
        cont=false;

        for (int i=0;i<numProcess;i++){
            if (isCompleted[i]) continue;

            bool canComplete =true;

            //check if process can be completed with available resources
            for(int j=0; j<numResource;j++){
                if(R[i][j]>availableRes[j]){
                    canComplete = false;
                    break;
                }
            }

            //return held resources to available
            if (canComplete){
                for (int j=0;j<numResource;j++){
                    availableRes[j] += C[i][j];
                }
                isCompleted[i]=true;
                cont=true;
            }
        }
    }

    bool deadlock=false;

    for(int i=0;i<numProcess;i++){
        if(!isCompleted[i]){
            deadlock=true;
            break;
        }
    }

    if(deadlock){
        cout << "Deadlock found:"<<endl;
        cout <<"Processes deadlocked: ";

        for (int i=0; i<numProcess;i++){
            if(!isCompleted[i]) cout<<"P"<<i<<" ";
        }
        cout<<endl;
    }
    else cout<<"No deadlocks found"<<endl;
}