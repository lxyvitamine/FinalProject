#include "helper.h"
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string>
#include <netinet/ip.h>
#include <sys/socket.h>
using namespace std;

pthread_mutex_t sdLock;

void *parseUserinput(void *input);

void *threadHelper(void *socket)
{
    int newSD = *((int *)socket);

    //Do we need while loop?
    while (true)
    {

        //receive message 1023 chars with \n
        char message[1024];
        int rec = recv(newSD, &message, 1024, 0);

        if (rec < 0)
        {
            cout << "ERROR: receiving data" << endl;
        }
        else if (rec == 0)
        {
            //if client end:
            shutdown(newSD, SHUT_RDWR);
            break;
        }

        //change char to string
        string input(message);

        //if input == shutdown??

        //create new thread
        //call parseuserinput
        userCmds[running_thread] = input;
        pthread_create(&threads[running_thread], NULL, parseUserinput, &userCmds[running_thread]);

        //how to catch the return value -- store in global variable or use join?
        //send feedback
        //send the message back to clients
        // < sizeof(message) ?
        if (send(newSD, &message, sizeof(message), 0) < (int)sizeof(message))
        {
            cout << "ERROR: sending data" << endl;
        }
    }
    return NULL;
}

bool isNumber(const std::string &str)
{
    char *ptr;
    strtol(str.c_str(), &ptr, 10);
    return *ptr == '\0';
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

        if (connect < 0)
        {
            cout << "ERROR: connection" << endl;
            continue;
        }

        //create a thread
        //pass isConnect sd as arg
        int rt = pthread_create(&clientThreads[index], NULL, threadHelper, &isConnect);
        if (rt != 0)
        {
            cout << "ERROR: creating threads" << endl;
        }

        index++;
    }

    return 0;
}

void *parseUserinput(void *input)
{
    // mutex
    pthread_mutex_lock(&parseLock);

    string userinput = *(static_cast<string *>(input));
    vector<string> inputs = parseCmd(userinput, " ");

    // mutex
    // no argument
    if (inputs.size() == 1)
    {
        if (inputs[0] == "list_candidates")
        {
            list_candidtates();
        }
        else if (inputs[0] == "view_result")
        {
            view_result();
        }
        else
        {
            cout << "ERROR" << endl;
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
            start_election(arg);
        }
        else if (cmd == "end_election")
        {
            end_election(arg);
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
                cout << "[R]: ERROR" << endl;
                pthread_mutex_unlock(&parseLock);
                return NULL;
            }
            add_voter(stoi(arg));
        }
        else if (cmd == "check_registration_status")
        {
            if (!isNumber(arg))
            {
                //end thread
                //pthread_exit(NULL);
                cout << "ERROR" << endl;
                pthread_mutex_unlock(&parseLock);
                return NULL;
            }
            check_registration_status(stoi(arg));
        }
        else if (cmd == "vote_count")
        {
            vote_count(arg);
        }
        else
        {
            cout << "ERROR" << endl;
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
            add_candidate(arg1, arg2);
        }
        else if (cmd == "vote_for")
        {
            if (!isNumber(arg2))
            {
                //pthread_exit(NULL);
                cout << "ERROR" << endl;
                pthread_mutex_unlock(&parseLock);
                return NULL;
            }
            vote_for(arg1, stoi(arg2));
        }
        else if (cmd == "check_voter_status")
        {
            if (!isNumber(arg1) || !isNumber(arg2))
            {
                //pthread_exit(NULL);
                cout << "ERROR" << endl;
                pthread_mutex_unlock(&parseLock);
                return NULL;
            }
            check_voter_status(stoi(arg1), stoi(arg2));
        }
        else
        {
            cout << "ERROR" << endl;
            //pthread_exit(NULL);
        }
    }
    else
    {
        cout << "ERROR" << endl;
    }
    //pthread_exit(NULL);

    pthread_mutex_unlock(&parseLock);

    return NULL;
}