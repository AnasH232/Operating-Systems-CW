#include <iostream>
#include <filesystem>
#include <vector>
#include <map>
#include <fstream>

using namespace std;
using namespace std::filesystem;

//function to iterate through files and stores file sizes in vector
void fileSizes(const path& dir, vector<int>& sizes){
    for (auto file: recursive_directory_iterator(dir)){
        if (is_regular_file(file)) sizes.push_back(file_size(file));
    }
}

//function to create histogram and store in map
map<int,int> createHistogram(vector<int> sizes, int binWidth){
    map<int,int> histogram;

    for (int size:sizes){
        int bin = size/binWidth;
        histogram[bin]++;
    }
    return histogram;
}

int main(){
    string dir;
    int binWidth;

    cout<<"Enter directory path: ";
    cin>>dir;
    cout<<"Enter bin width: ";
    cin>>binWidth;

    //store file sizes in vector
    vector<int> sz;
    fileSizes(dir, sz);

    map<int,int> hist = createHistogram(sz, binWidth);
    
    ofstream csv("t6results.csv");
    csv<<"Range,Count\n";

    cout<<"Histogram: "<<endl;
    cout<<"Range        Count"<<endl;

    //loop to output histogram and store results in csv
    for(auto pair:hist){
        int low = pair.first *binWidth;
        int high = low+binWidth-1;

        cout<<low<<"-"<<high<<"     "<<pair.second<<endl;
        csv<<low<<"-"<<high<<","<<pair.second<<"\n";
    }
    csv.close();
    cout<<"Results stored in csv file"<<endl;
}