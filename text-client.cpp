using namespace std;
#include <string>
#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
   if (argc > 3) {
        std::clog << "too many arguments\nArguments must be 'file path', 'search string'\n";
        return 0;
    } else if (argc < 3) {
        std::clog << "not enough arguments\nArguments must be 'file path', 'search string'\n";
        return 0;
    }
    // 1) -------------------------------

    const size_t BUFFER_SIZE = 1024;
    //char storage_buf[BUFFER_SIZE];
    const string SEND_STRING = string(argv[1]) + "," + string(argv[2]);

    //GENERATING KEY FOR THE SHARED MEMORY
    key_t key = ftok("shmfile",65);

    // GETTING IDENTIFIER FOR SHARED MEMORY SEGMENT
    int shmid = shmget(key,BUFFER_SIZE,0666|IPC_CREAT);
  
    // CLIENT ATTACHING ITSELF TO SHARED MEMORY
    char * transfer_buf = (char *) shmat(shmid, NULL, 0);
    //  -------------------------------


    // 2) -------------------------------
    //copying our file_path to the buffer
    strcpy (transfer_buf, SEND_STRING.c_str());
    //  -------------------------------


    string old_data(transfer_buf);
    while (true) {
        string data(transfer_buf);
        if (data != old_data) {
            break;
        }
    }
    cout << transfer_buf << endl;

    //DETATCHING FROM SHARED MEMORY
    shmdt(transfer_buf);

        
    // DESTROYING SHARED MEMORY
    shmctl(shmid,IPC_RMID,NULL);

    return 0;
}
