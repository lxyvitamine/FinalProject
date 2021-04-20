#include "helper.h"
#include <netinet/ip.h>
// #include <sys/socket.h>

using namespace std;

pthread_mutex_t sdLock;
MAX_MESSAGE = 1024;
char sendToClient[MAX_MESSAGE];
string errorMsg = "ERROR";

// void *parseUserinput(void *input);

void *threadHelper(void *sd)
{
    int threadSd = *(int *)sd;

    //Do we need while loop?

    //receive message 1023 chars with \n
    char cmdFromClient[MAX_MESSAGE];
    int numOfBytesRcvd = recv(threadSd, &cmdFromClient, sizeof(char) * MAX_MESSAGE, 0);

    if (numOfBytesRcvd == -1)
    {
        cout << "ERROR: receiving data" << endl;
    }
    //         else if (rec == 0)
    //         {
    //             //if client end:
    //             shutdown(newSD, SHUT_RDWR);
    //             break;
    //         }

    // change char to string
    string input(cmdFromClient);

    if (input == "shutdown " + password)
    {
        // pthread_mutex_lock(&parseLock);
        shutdown(password);
        // pthread_mutex_unlock(&parseLock);
        exit(0);
    }

    // create new thread
    // call parseuserinput
    userCmds[running_thread] = input;
    pthread_create(&threads[running_thread], NULL, parseUserinput, &userCmds[running_thread]);

    // how to catch the return value -- store in global variable or use join?
    // return string ....
    //

    // send feedback
    // send the message back to clients
    // < sizeof(message) ?
    if (send(threadSd, &sendToClient, sizeof(char) * strlen(sendToClient), 0) < sizeof(char) * strlen(sendToClient))
    {
        cout << "ERROR: sending data" << endl;
    }

    shutdown(threadSd, SHUT_RDWR);

    return NULL;
}

bool isNumber(const string &str)
{
    char *ptr;
    strtol(str.c_str(), &ptr, 10);
    return *ptr == '\0';
}

void *parseUserinput(void *input)
{
    string userinput = *(static_cast<string *>(input));
    vector<string> inputs = parseCmd(userinput, " ");

    // mutex
    // no argument
    if (inputs.size() == 1)
    {
        if (inputs[0] == "list_candidates")
        {
            pthread_mutex_lock(&parseLock);
            strcpy(sendToClient, list_candidtates().c_str());
            pthread_mutex_unlock(&parseLock);
        }
        else if (inputs[0] == "view_result")
        {
            pthread_mutex_lock(&parseLock);
            strcpy(sendToClient, view_result().c_str());
            pthread_mutex_unlock(&parseLock);
        }
        else
        {
            pthread_mutex_lock(&parseLock);
            strcpy(sendToClient, errorMsg.c_str());
            pthread_mutex_unlock(&parseLock);
            //pthread_exit(NULL);
        }
    }
    // 1 argument
    else if (inputs.size() == 2)
    {
        string cmd = inputs[0];
        string arg = inputs[1];

        if (cmd == "start_election")
        {
            pthread_mutex_lock(&parseLock);
            strcpy(sendToClient, start_election(arg).c_str());
            pthread_mutex_unlock(&parseLock);
        }
        else if (cmd == "end_election")
        {
            pthread_mutex_lock(&parseLock);
            strcpy(sendToClient, end_election(arg).c_str());
            pthread_mutex_unlock(&parseLock);
        }
        // else if (cmd == "shutdown")
        // {
        //     shutdown(arg);
        // }
        else if (cmd == "add_voter")
        {
            if (!isNumber(arg))
            {
                //end thread
                //pthread_exit(NULL);
                pthread_mutex_lock(&parseLock);
                strcpy(sendToClient, errorMsg.c_str());
                pthread_mutex_unlock(&parseLock);
                return NULL;
            }
            pthread_mutex_lock(&parseLock);
            strcpy(sendToClient, add_voter(stoi(arg)).c_str());
            pthread_mutex_unlock(&parseLock);
        }
        else if (cmd == "check_registration_status")
        {
            if (!isNumber(arg))
            {
                //end thread
                //pthread_exit(NULL);
                pthread_mutex_lock(&parseLock);
                strcpy(sendToClient, errorMsg.c_str());
                pthread_mutex_unlock(&parseLock);
                return NULL;
            }
            pthread_mutex_lock(&parseLock);
            strcpy(sendToClient, check_registration_status(stoi(arg)).c_str());
            pthread_mutex_unlock(&parseLock);
        }
        else if (cmd == "vote_count")
        {
            pthread_mutex_lock(&parseLock);
            strcpy(sendToClient, vote_count(arg).c_str());
            pthread_mutex_unlock(&parseLock);
        }
        else
        {
            pthread_mutex_lock(&parseLock);
            strcpy(sendToClient, errorMsg.c_str());
            pthread_mutex_unlock(&parseLock);
            //pthread_exit(NULL);
        }
    }
    // 2 argument
    else if (inputs.size() == 3)
    {
        string cmd = inputs[0];
        string arg1 = inputs[1];
        string arg2 = inputs[2];

        if (cmd == "add_candidate")
        {
            pthread_mutex_lock(&parseLock);
            strcpy(sendToClient, add_candidate(arg1, arg2).c_str());
            pthread_mutex_unlock(&parseLock);
        }
        else if (cmd == "vote_for")
        {
            if (!isNumber(arg2))
            {
                //pthread_exit(NULL);
                pthread_mutex_lock(&parseLock);
                strcpy(sendToClient, errorMsg.c_str());
                pthread_mutex_unlock(&parseLock);
                return NULL;
            }
            pthread_mutex_lock(&parseLock);
            strcpy(sendToClient, vote_for(arg1, stoi(arg2)).c_str());
            pthread_mutex_unlock(&parseLock);
        }
        else if (cmd == "check_voter_status")
        {
            if (!isNumber(arg1) || !isNumber(arg2))
            {
                //pthread_exit(NULL);
                pthread_mutex_lock(&parseLock);
                strcpy(sendToClient, errorMsg.c_str());
                pthread_mutex_unlock(&parseLock);
                return NULL;
            }
            pthread_mutex_lock(&parseLock);
            strcpy(sendToClient, check_voter_status(stoi(arg1), stoi(arg2)).c_str());
            pthread_mutex_unlock(&parseLock);
        }
        else
        {
            pthread_mutex_lock(&parseLock);
            strcpy(sendToClient, errorMsg.c_str());
            pthread_mutex_unlock(&parseLock);
            //pthread_exit(NULL);
        }
    }
    else
    {
        pthread_mutex_lock(&parseLock);
        strcpy(sendToClient, errorMsg.c_str());
        pthread_mutex_unlock(&parseLock);
    }
    //pthread_exit(NULL);

    return NULL;
}

int main(int argc, char *argv[])
{
    //default to 10000
    unsigned short int port = 10000;
    // 1. parse command
    // -r -p -a
    int option;
    while ((option = getopt(argc, argv, "a:rp:")) != -1)
    {
        switch (option)
        {
        case 'a':
            password = optarg;
            changePassword = true;
            break;
        case 'r':
            // read file
            recover();
            break;
        case 'p':
            //connect to port
            if (!isNumber(optarg))
            {
                cout << "ERROR: port must be a number" << endl;
            }
            port = stoi(optarg);
            break;
        case '?':
            exit(0);
            break;
        }
    }

    //address filling out the structure
    struct sockaddr_in st;
    st.sin_family = AF_INET;
    st.sin_addr.s_addr = INADDR_ANY; // bind to the current IP of this machine
    st.sin_port = port;

    //set-up socket
    int sd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sd == -1)
    {
        cout << "ERROR: create server socket" << endl;
        shutdown(sd, SHUT_RDWR);
        return -1;
    }

    //bind
    if (bind(sd, (const struct sockaddr *)&st, sizeof(st)) == -1)
    {
        cout << "ERROR: bind" << endl;
        shutdown(sd, SHUT_RDWR);
        return -1;
    }

    //listen
    if (listen(sd, 20) < 0)
    {
        cout << "ERROR: listen" << endl;
        shutdown(sd, SHUT_RDWR);
        return -1;
    }

    //establish connection
    //wait for clients in an infinite loop

    //create an array for threads
    pthread_t clientThreads[9999];
    int index = 0;

    if (0 != pthread_mutex_init(&sdLock, NULL))
        throw "ERROR: Failed to initialize a mutex";

    while (1)
    {
        int isConnect = accept(sd, NULL, NULL);
        //cout<< "accept sd is "<<isConnect <<endl;

        if (isConnect < 0)
        {
            cout << "ERROR: connection" << endl;
            continue;
        }

        //create a thread
        //pass isConnect sd as arg
        if (pthread_create(&clientThreads[index], NULL, threadHelper, &isConnect) != 0)
        {
            cout << "ERROR: creating threads" << endl;
        }

        index++;
    }

    return 0;
}
