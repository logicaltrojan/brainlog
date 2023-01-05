#include <iostream>
#include <cstdlib>
#include <fstream>
#include <unordered_map>
#include <string>
#include <vector>

using namespace std;

unordered_map<string, long long> keyDir; 

vector<string> split (string s, string delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    string token;
    vector<string> res;

    while ((pos_end = s.find (delimiter, pos_start)) != string::npos) {
        token = s.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back (token);
    }

    res.push_back (s.substr (pos_start));
    return res;
}

const string delimiter = ",";
int main(){


    ifstream instream;
    ofstream outstream; 

    outstream.open("dataLog.log", ios_base::app| ios_base::out);
    instream.open("dataLog.log", ios_base::in);
    long long tail = 0;

    if(instream.is_open()){
	std::string s; 
	long long keyCount = 0;
	while(instream){
	    getline(instream, s);
	    string key = s.substr(0, s.find(delimiter));
	    keyDir[key] = keyCount++;
	    keyCount += s.size();
	    cout << key << keyDir[key] << endl;
	}
	tail = keyCount -1;
    }

    vector<string> argVec;
    while(true){
	cout << "GET / INSERT / UPDATE / DELETE " << endl;
	string args;
	getline(cin, args);

	argVec = split(args, " ");

	string cmd = argVec[0];
	string key; 
	string value;

	if(cmd == "GET"){
	    if(argVec.size() >= 2){

		instream.clear();
		if(keyDir.find(argVec[1]) != keyDir.end()){
		    string data;
		    //go to keyDir + key + delimiter(",") .size
		    instream.seekg(keyDir[argVec[1]] + argVec[1].size()  + delimiter.size());
		    getline(instream, data);

		    cout << data << endl;
		}
	    }
	}else if(cmd == "INSERT" || cmd == "UPDATE"){
	    if(argVec.size() >= 3){
		//append to file 
		key = argVec[1];
		value = argVec[2];
		outstream << key << ","<< value << endl;

		keyDir[key] = tail;
		//increase pointer 
		long long size = key.size() + value.size() + 2;
		tail += size;
		//add key - pointer to keyDir 
		cout << tail;
	    }
	}else if(cmd == "DELETE"){

	}

    }



}
