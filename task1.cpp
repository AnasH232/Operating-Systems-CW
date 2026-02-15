#include <iostream>
#include <vector>
#include <string>
#include <unistd.h>
#include <dirent.h>
#include <cstdlib>
#include <limits.h>
#include <sys/wait.h>
#include <fstream>

using namespace std;

//separates input into individual words
vector<string> splitWords(string& input){
    vector<string> words;
    string word = "";

    for (char c : input){
        if (isspace(c)){
            if (!word.empty()){
                words.push_back(word);
                word="";
            }
        }
        else word += c;
    }
    if (!word.empty()) words.push_back(word);
    return words;
}

//cd built in command
void changeDir(vector<string>& inputs){
    //output current directory if no arguments
    if (inputs.size()==1){
        char cwd[PATH_MAX];
        getcwd(cwd, sizeof(cwd));
        cout << cwd<<endl;
    }
    else{
        if (chdir(inputs[1].c_str())!=0) cout<<"Directory not found"<<endl;
        else{
            char cwd[PATH_MAX];
            getcwd(cwd, sizeof(cwd));
            setenv("PWD",cwd,1);
        }
    }
}

//dir built in command
void directoryContents(vector<string>& inputs){
    string path;
    if (inputs.size()>1) path = inputs[1];
    else path = ".";

    DIR* dir = opendir(path.c_str());

    if(!dir){
        cout<<"Directory not found"<<endl;
        return;
    }

    struct dirent* entry;
    while ((entry = readdir(dir))!=nullptr){
        cout<<entry->d_name<<endl;
    }
    closedir(dir);
}

//environ built in command
extern char** environ;

void printEnv(){
    for (int i=0; environ[i]!=nullptr;i++){
        cout << environ[i]<<endl;
    }
}

//set built in command
void setEnv(vector<string>& inputs){
    if (inputs.size()<3){
        cout << "Variable and value required"<<endl;
        return;
    }
    
    if (setenv(inputs[1].c_str(),inputs[2].c_str(),1)!=0) cout <<"Could not set value"<<endl;
}

//echo built in command
void echo(vector<string>& inputs){
    for (int i=1; i<inputs.size();i++){
        cout << inputs[i]<<" ";
    }
    cout<<endl;
}

//help built in command
void help(){
    cout << "Shell Help"<<endl;
    cout << "Built-in commands:"<<endl;;
    cout << "cd DIR        - Change directory"<<endl;
    cout << "dir DIR       - Display directory contents"<<endl;;
    cout << "environ       - Display environment variables"<<endl;
    cout << "set VAR VALUE - Set environment variable"<<endl;;
    cout << "echo text     - Display text"<<endl;
    cout << "help          - Show help message"<<endl;;
    cout << "pause         - Wait for Enter key"<<endl;;
    cout << "quit          - Exit shell"<<endl;;
}

//pause built in command
void pauseBuiltIn(){
    cout<<"Press ENTER to continue: ";
    string temp;
    getline(cin, temp);
}

//function to execute any external commands
void externalCommand(vector<string> inputs){
    //checking if command should run in background
    bool isBackground = false;

    if (!inputs.empty()&&inputs.back()=="&"){
        isBackground = true;
        inputs.pop_back();
    }

    //array of string converted into of char so execvp can be used on it
    vector<char*> input;
    for (string s : inputs){
        input.push_back(const_cast<char*>(s.c_str()));
    }
    input.push_back(nullptr);

    pid_t pid = fork();

    //child process
    if (pid == 0){
        execvp(input[0], input.data());
        cout<<"Command not found"<<endl;
        exit(1);
    }
    else if (pid>0){
        if (!isBackground) waitpid(pid, nullptr, 0);
    }
    else cout << "Error creating fork"<<endl;
}

int main(int argc, char* argv[]){
    string input;
    string inputMethod = "cin";
    ifstream file;

    if (argc==2){
        file.open(argv[1]);
        if (!file){
            cout << "File not found"<<endl;
            return 0;
        }
        inputMethod = "file";
    }

    while(true){
        //display pathname when input method is manual
        if (inputMethod == "cin"){
            char cwd[PATH_MAX];
            getcwd(cwd, sizeof(cwd));
            cout<<cwd<<" ";
            getline(cin, input);
        }
        else{
            if (!getline(file, input)) break;
        }
        
        vector<string> inputs = splitWords(input);
        if (inputs.empty()) continue;

        string command = inputs[0];

        if (command=="cd") changeDir(inputs);
        else if (command=="dir") directoryContents(inputs);
        else if (command == "environ") printEnv();
        else if (command == "set") setEnv(inputs);
        else if (command == "echo") echo(inputs);
        else if (command == "help") help();
        else if (command == "pause") pauseBuiltIn();
        else if (command == "quit") break;
        else externalCommand(inputs);
    }

    return 0;
}