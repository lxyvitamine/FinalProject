#include "helper2.h"
#include <netinet/ip.h>
#include <signal.h>
// #include <sys/socket.h>

using namespace std;

// pid_t pid;

// pthread_mutex_t mainLock;

//signal handler for SIGINT
// void handleSigusr1(int signo)
// {
//     //exit(0);
// }

void *parseUserinput(void *input);

void *threadHelper(void *argSd)
{
    int threadSd = *(int *)argSd;
    delete (int *)argSd;

    // Do we need while loop?
    // receive message 1023 chars with \n
    char cmdFromClient[MAX_MESSAGE];

    if (recv(threadSd, &cmdFromClient, sizeof(char) * MAX_MESSAGE, 0) == -1)
    {
        cout << "ERROR: receiving data" << endl;
    }

    // change char to string
    string input(cmdFromClient);

    if (input.compare("shutdown " + password) == 0)
    { // equal
        pthread_mutex_lock(&isShutdownLock);
        isShutdown = true;
        pthread_mutex_unlock(&isShutdownLock);

        pthread_mutex_lock(&parseUserinputLock);
        shutdown(password);
        pthread_mutex_unlock(&parseUserinputLock);

        char shutdownOK[MAX_MESSAGE] = "[R]: OK";

        if ((int)send(threadSd, &shutdownOK, sizeof(char) * MAX_MESSAGE, 0) < (int)(sizeof(char) * MAX_MESSAGE))
        {
            cout << "ERROR: sending data" << endl;
        }

        //pthread_mutex_lock(&sdLock);
        shutdown(sd, SHUT_RDWR);
        //pthread_mutex_unlock(&sdLock);

        close(threadSd);
        //pthread_detach(pthread_self());
        //kill(pid, SIGUSR1);
    }
    else
    {
        // create new thread
        // call parseuserinput
        //userCmds[running_thread] = input;
        //pthread_create(&threads[running_thread], NULL, parseUserinput, &userCmds[running_thread]);
        //pthread_t tid;
        //pthread_create(&tid, NULL, parseUserinput, &input);
        // send feedback
        //thread join
        //pthread_join(tid, NULL);
        parseUserinput(&input);
        //pthread_mutex_lock(&runningThreadLock);
        //running_thread++;
        //pthread_mutex_unlock(&runningThreadLock);

        pthread_mutex_lock(&sendToClientLock);
        if ((int)send(threadSd, &sendToClient, sizeof(char) * MAX_MESSAGE, 0) < (int)(sizeof(char) * MAX_MESSAGE))
        {
            cout << "ERROR: sending data" << endl;
        }
        memset(sendToClient, 0, MAX_MESSAGE);
        pthread_mutex_unlock(&sendToClientLock);

        close(threadSd);
        //pthread_detach(pthread_self());
        //pthread_exit(NULL);
    }
    // pthread_detach(pthread_self());
    // pthread_exit(0);
    return NULL;
}

void *parseUserinput(void *input)
{
    pthread_mutex_lock(&parseUserinputLock);

    string userinput = *(static_cast<string *>(input));
    vector<string> inputs = parseCmd(userinput, " ");

    // mutex
    // no argument
    if (inputs.size() == 1)
    {
        if (inputs[0] == "list_candidates")
        {
            pthread_mutex_lock(&sendToClientLock);
            strcpy(sendToClient, list_candidtates().c_str());
            pthread_mutex_unlock(&sendToClientLock);
        }
        else if (inputs[0] == "view_result")
        {
            pthread_mutex_lock(&sendToClientLock);
            strcpy(sendToClient, view_result().c_str());
            pthread_mutex_unlock(&sendToClientLock);
        }
        else if (inputs[0] == "start_election_ctrlc")
        { // for EC3
            start_election_ctrlc();
        }
        else if (inputs[0] == "end_election_ctrlc")
        { // for EC3
            end_election_ctrlc();
        }
        else
        {
            pthread_mutex_lock(&sendToClientLock);
            strcpy(sendToClient, errorMsg.c_str());
            pthread_mutex_unlock(&sendToClientLock);
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
            pthread_mutex_lock(&sendToClientLock);
            strcpy(sendToClient, start_election(arg).c_str());
            pthread_mutex_unlock(&sendToClientLock);
        }
        else if (cmd == "end_election")
        {
            pthread_mutex_lock(&sendToClientLock);
            strcpy(sendToClient, end_election(arg).c_str());
            pthread_mutex_unlock(&sendToClientLock);
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
                pthread_mutex_lock(&sendToClientLock);
                strcpy(sendToClient, errorMsg.c_str());
                pthread_mutex_unlock(&sendToClientLock);

                pthread_mutex_unlock(&parseUserinputLock);
                return NULL;
            }
            pthread_mutex_lock(&sendToClientLock);
            strcpy(sendToClient, add_voter(stoi(arg)).c_str());
            pthread_mutex_unlock(&sendToClientLock);
        }
        else if (cmd == "check_registration_status")
        {
            if (!isNumber(arg))
            {
                //end thread
                //pthread_exit(NULL);
                pthread_mutex_lock(&sendToClientLock);
                strcpy(sendToClient, errorMsg.c_str());
                pthread_mutex_unlock(&sendToClientLock);

                pthread_mutex_unlock(&parseUserinputLock);
                return NULL;
            }
            pthread_mutex_lock(&sendToClientLock);
            strcpy(sendToClient, check_registration_status(stoi(arg)).c_str());
            pthread_mutex_unlock(&sendToClientLock);
        }
        else if (cmd == "vote_count")
        {
            pthread_mutex_lock(&sendToClientLock);
            strcpy(sendToClient, vote_count(arg).c_str());
            pthread_mutex_unlock(&sendToClientLock);
        }
        else if (cmd == "add_candidate_ctrlc")
        { // for EC3
            add_candidate_ctrlc(arg);
        }
        else if (cmd == "add_voter_ctrlc")
        { // for EC3
            add_voter_ctrlc(stoi(arg));
        }
        else
        {
            pthread_mutex_lock(&sendToClientLock);
            strcpy(sendToClient, errorMsg.c_str());
            pthread_mutex_unlock(&sendToClientLock);
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
            pthread_mutex_lock(&sendToClientLock);
            strcpy(sendToClient, add_candidate(arg1, arg2).c_str());
            pthread_mutex_unlock(&sendToClientLock);
        }
        else if (cmd == "vote_for")
        {
            if (!isNumber(arg2))
            {
                //pthread_exit(NULL);
                pthread_mutex_lock(&sendToClientLock);
                strcpy(sendToClient, errorMsg.c_str());
                pthread_mutex_unlock(&sendToClientLock);

                pthread_mutex_unlock(&parseUserinputLock);
                return NULL;
            }
            pthread_mutex_lock(&sendToClientLock);
            strcpy(sendToClient, vote_for(arg1, stoi(arg2)).c_str());
            pthread_mutex_unlock(&sendToClientLock);
        }
        else if (cmd == "check_voter_status")
        {
            if (!isNumber(arg1) || !isNumber(arg2))
            {
                //pthread_exit(NULL);
                pthread_mutex_lock(&sendToClientLock);
                strcpy(sendToClient, errorMsg.c_str());
                pthread_mutex_unlock(&sendToClientLock);

                pthread_mutex_unlock(&parseUserinputLock);
                return NULL;
            }
            pthread_mutex_lock(&sendToClientLock);
            strcpy(sendToClient, check_voter_status(stoi(arg1), stoi(arg2)).c_str());
            pthread_mutex_unlock(&sendToClientLock);
        }
        else if (cmd == "vote_for_ctrlc")
        { // for EC3
            vote_for_ctrlc(arg1, stoi(arg2));
        }
        else
        {
            pthread_mutex_lock(&sendToClientLock);
            strcpy(sendToClient, errorMsg.c_str());
            pthread_mutex_unlock(&sendToClientLock);
            //pthread_exit(NULL);
        }
    }
    else
    {
        pthread_mutex_lock(&sendToClientLock);
        strcpy(sendToClient, errorMsg.c_str());
        pthread_mutex_unlock(&sendToClientLock);
    }

    pthread_mutex_unlock(&parseUserinputLock);
    return NULL;
}

void recover_from_ctrlc()
{
    ifstream clientCommands("clientCommands.txt");

    if (clientCommands.is_open() && clientCommands.peek() != ifstream::traits_type::eof())
    {
        string command;
        while (getline(clientCommands, command))
        {
            parseUserinput(&command);
        }
    }

    clientCommands.close();
}

int main(int argc, char *argv[])
{
    // pid = getpid();
    // signal(SIGUSR1, handleSigusr1);

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
            recover_from_ctrlc();
            break;
        case 'p':
            //connect to port
            if (!isValidPortNumber(optarg))
            {
                cout << "ERROR: Invalid port number" << endl;
                exit(0);
                break;
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
    sd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sd == -1)
    {
        cout << "ERROR: create server socket" << endl;
        shutdown(sd, SHUT_RDWR);
        return -1;
    }

    int reuse_address = 1;

    if ((setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &reuse_address, sizeof(int))) == -1)
    {
        cerr << "ERROR: set socket reuseaddress" << endl;
    }

    //bind
    if (bind(sd, (const struct sockaddr *)&st, sizeof(st)) == -1)
    {
        cout << "ERROR: bind" << endl;
        shutdown(sd, SHUT_RDWR);
        return -1;
    }

    //listen
    if (listen(sd, 20) == -1)
    {
        cout << "ERROR: listen" << endl;
        shutdown(sd, SHUT_RDWR);
        return -1;
    }

    //establish connection
    //wait for clients in an infinite loop

    //create an array for threads
    //     pthread_t clientThreads[9999];
    //     int index = 0;

    if (0 != pthread_mutex_init(&sdLock, NULL))
    {
        throw "Failed to initialize a mutex";
    }

    if (0 != pthread_mutex_init(&isShutdownLock, NULL))
    {
        throw "Failed to initialize a mutex";
    }

    // if (0 != pthread_mutex_init(&runningThreadLock, NULL))
    // {
    //     throw "Failed to initialize a mutex";
    // }

    if (0 != pthread_mutex_init(&parseUserinputLock, NULL))
    {
        throw "Failed to initialize a mutex";
    }

    if (0 != pthread_mutex_init(&sendToClientLock, NULL))
    {
        throw "Failed to initialize a mutex";
    }

    if (0 != pthread_mutex_init(&candidatesLock, NULL))
    {
        throw "Failed to initialize a mutex";
    }

    if (0 != pthread_mutex_init(&votersLock, NULL))
    {
        throw "Failed to initialize a mutex";
    }

    while (1)
    {
        pthread_mutex_lock(&isShutdownLock);
        if (isShutdown)
        {
            pthread_mutex_unlock(&isShutdownLock);
            break;
        }
        pthread_mutex_unlock(&isShutdownLock);

        //pthread_mutex_lock(&sdLock);
        int acceptSd = accept(sd, NULL, NULL);
        //pthread_mutex_unlock(&sdLock);

        if (acceptSd == -1)
        {
            //cout << "ERROR: connection" << endl;
            //pthread_mutex_unlock(&sdLock);
            continue;
        }

        int *acceptSdTemp = new int();
        *acceptSdTemp = acceptSd;

        //create a thread
        //pass acceptSd sd as arg
        //pthread_mutex_lock(&mainLock);
        clientThreads.push_back(1);
        if (pthread_create(&clientThreads.back(), NULL, threadHelper, acceptSdTemp) != 0)
        {
            cout << "ERROR: creating threads for clients" << endl;
        }
        //clientIndex++;
        //pthread_mutex_unlock(&mainLock);
    }

    // pthread_exit(0);
    // sleep(2);
    for (auto tid : clientThreads)
    {
        pthread_join(tid, NULL);
    }

    return 0;
}
