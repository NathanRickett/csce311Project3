using namespace std;
#include <iostream>
#include <fstream>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <sstream>
#include <string.h>

string search_text(string file_path, string search_string) {
    string line;
    ifstream myfile (file_path);
    string return_string = "";
    if (myfile.is_open()) {
        while ( getline (myfile,line) ) {
            if (line.find(search_string) != std::string::npos) {
                //cout << line << endl;
                return_string += line + "\n";
            }
        }
        myfile.close();
        clog << "\tFILE CLOSED";
    }
  else {
      clog << "INVALID FILE\n";
  }

  return return_string;
}

int main(int argc, char const *argv[]) {
    cout << "SERVER STARTED" << endl;
    //GENERATING KEY FOR THE SHARED MEMORY
    key_t key = ftok("shmfile",65);
  
     // GETTING IDENTIFIER FOR SHARED MEMORY SEGMENT
    int shmid = shmget(key,1024,0666|IPC_CREAT);

    //IF GETTING IDENTIFIER UNSUCCESSFUL, EXIT 
    if (shmid == -1) {
      perror("Shared memory");
      return 1;
   }
    
    //while(true) {

    // SERVER ATTACHING ITSELF TO SHARED MEMORY
    char *buf = (char*) shmat(shmid,(void*)0,0);
    //TODO handle error if shmat fails
    
    string received_string(buf);
    while (received_string.empty()) { //loop until there is a request
        
    }
    clog << "CLIENT REQUEST RECEIVED" << endl;
    clog << "\tMEMORY OPEN";
    //START PARSING DATA FROM SHARED MEMORY
    string file_path = "";
    string search_string = "";
    string token;
    stringstream ss(received_string);
    while (getline(ss, token, ',')) {
        if (file_path == "") {
            file_path = token;
        } else {
            search_string = token;
        }
    }

    clog << "\tOPENING: " <<  file_path;
    //SEARCHING THE TEXT USING THE FILE PATH AND SEARCH STRING PROVIDED IN SHARED MEMORY
    string return_string = search_text(file_path, search_string);

    //WRITING CONTENTS OF FILE TO THE CLIENT
    strcpy(buf, return_string.c_str());
    
    //DETACHING FROM SHARED MEMORY
    shmdt(buf);
    clog << "\tMEMORY CLOSED\n";
    //}
}
