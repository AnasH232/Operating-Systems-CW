#include <iostream>
#include <pthread.h>
#include <fstream>
#include <vector>
#include <map>

using namespace std;

//global variables
vector<string> lines;
vector<map<string,int>> threadRes;
int numThreads;

//Function each thread will perform to count word frequency
void* countFreq(void* arg){
    int* id = (int*) arg;

    int totalLines = lines.size();
    int linesPerThread = totalLines/numThreads;

    int firstLine = (*id) * linesPerThread;
    int endLine = firstLine + linesPerThread;
    //loop to separate each word in a line then increment counter of word
    for (int i=firstLine; i<endLine; i++){
        string word = "";

        for (char c: lines[i]){
            if(isspace(c)){
                if(!word.empty()){
                    threadRes[*id][word]++;
                    word = "";
                }
            }
            else word += tolower(c);
        }
        if (!word.empty()) threadRes[*id][word]++;
    }
    pthread_exit(NULL);
}

int main(){
    string line;
    map<string, int> totalCount;

    cout << "Enter the number of threads: ";
    cin >> numThreads;

    //open text file and store lines in 'lines' vector
    ifstream file("words.txt");
    while (getline(file, line)) lines.push_back(line);
    file.close();

    threadRes.resize(numThreads);
    vector<pthread_t> threads(numThreads);
    vector<int> ids(numThreads);

    //loop to create thread and then loop to join them together
    for (int i =0; i<numThreads; i++){
        ids[i] = i;
        pthread_create(&threads[i], NULL, countFreq, &ids[i]);
    }

    for (int i=0; i<numThreads; i++) pthread_join(threads[i], NULL);

    //loop to sum up all frequencies and store in totalCount
    for (int i=0; i<numThreads; i++){
        for (auto pair : threadRes[i]){
            totalCount[pair.first] += pair.second;
        }
    }

    //outputting results
    cout << "Word Frequencies: "<<endl;
    for (auto pair : totalCount) cout << pair.first << " : "<<pair.second<<endl;
}