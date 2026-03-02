#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

//function simulates the aging algorithm
int aging(vector<int>& refs, int numFrames){
    vector<int> pages;
    vector<unsigned char> counts;
    vector<bool> pageUsed;

    int faults =0;

    for (int ref:refs){
        bool found=false;
        //find page
        for (int i=0; i<pages.size();i++){
            if (pages[i]==ref){
                pageUsed[i]=true;
                found=true;
                break;
            }
        }
        //adds page to frame if not used
        if(!found){
            faults++;

            if (pages.size()<numFrames){
                pages.push_back(ref);
                counts.push_back(0);
                pageUsed.push_back(true);
            }
            //find oldest unused page and replaces with new page
            else{
                int idx=0;
                unsigned char minCount=counts[0];

                for (int i=1;i<numFrames;i++){
                    if (counts[i]<minCount){
                        minCount = counts[i];
                        idx = i;
                    }
                }

                pages[idx]=ref;
                counts[idx]=0;
                pageUsed[idx]=true;
            }
        }
        //aging loop
        for (int i =0;i<pages.size();i++){
            counts[i] >>=1;

            if (pageUsed[i]) counts[i] |=128;
            pageUsed[i]=false;
        }
    }
    return faults;
}

int main(){
    //open file and store pages in vector
    ifstream file("task3.txt");

    vector<int> refs;
    int page;

    while (file>>page){
        refs.push_back(page);
    }
    file.close();

    int totalRefs =refs.size();
    int maxFrames;
    cout<<"Enter the maximum number of frames: ";
    cin>>maxFrames;
    //simulate aging with different number of frames
    for (int f=1;f<=maxFrames;f++){
        int faults =aging(refs, f);
        double faults1000 = ((double)faults/totalRefs) *1000;

        cout <<f<< " Frames: "<<faults1000<<" faults per 1000 references"<<endl;
    }
}