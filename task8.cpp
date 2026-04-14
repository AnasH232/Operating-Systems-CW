#include <iostream>
#include <vector>
#include <openssl/des.h>
#include <cstdlib>
#include <ctime>
#include <cstring>

using namespace std;

//function converts encrypted password to hexadecimal for easier readability
string toHex(char* data, int len){
    string res="";
    char* hexDigits = "0123456789ABCDEF";

    for (int i=0; i <len; i++) {
        res += hexDigits[(data[i] >> 4) & 0xF];
        res += hexDigits[data[i] & 0xF];
    }
    return res;
}

//function performs encryption
void desEncrypt(char* input, char* key, char* output){
    DES_key_schedule schedule;
    DES_cblock keyBlock;

    memcpy(keyBlock, key, 8);
    DES_set_key_unchecked(&keyBlock, &schedule);
    DES_ecb_encrypt((DES_cblock*)input, (DES_cblock*)output, &schedule, DES_ENCRYPT);
}

//applies encryption 25 times
string DES25(string password, int salt){
    char data[8]={0};
    char key[8]={0};
    char output[8];

    string passAndSalt = password +to_string(salt);

    memcpy(data, passAndSalt.c_str(), min((size_t)8, passAndSalt.size()));
    memcpy(key, passAndSalt.c_str(), min((size_t)8, passAndSalt.size()));

    for (int i=0;i<25;i++){
        desEncrypt(data, key,output);
        memcpy(data, output, 8);
    }
    return toHex(data,8);
}

//function creates random 16 bit salt
int createSalt(){
    return rand()%65536;
}

//function encrypts and formats password
string createPassword(string password) {
    int salt = createSalt();
    string hash = DES25(password, salt);

    return to_string(salt) + ":" + hash;
}

//function checks if input password is same as one of stored ones
bool checkPassword(string input, string stored){
    int pos = stored.find(":");

    int salt = stoi(stored.substr(0,pos));
    string storedHash = stored.substr(pos+1);
    string newHash = DES25(input, salt);
    
    return newHash==storedHash;
}

int main(){
    srand(time(0));
    string input;

    vector<string> passwords={"pass1","hello","world","abcd","abcd123","test","admin","encrypt","admin","user"};
    vector<string> encryptedPass;

    cout<<"Generated encrypted passwords:"<<endl;
    for (string p:passwords){
        string encrypted = createPassword(p);
        encryptedPass.push_back(encrypted);
        cout <<encrypted<<endl;
    }

    cout<<"Enter password to check against stored ones: ";
    cin>>input;

    bool found=false;
    for(string p:encryptedPass){
        if(checkPassword(input,p)){
            cout << "Password matches "<<p<<endl;
            found=true;
            break;
        }
    }
    if (!found) cout<<"No match found"<<endl;
}
