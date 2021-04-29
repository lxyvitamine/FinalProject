#include "helper2.h"
#include <netinet/ip.h>
#include <signal.h>

#include <cereal/archives/binary.hpp> // for binary serialization

using namespace std;

helper2 h2;

void *parseUserinput(void *input);

void *threadHelper(void *argSd)
{
    int threadSd = *(int *)argSd;
    delete (int *)argSd;

    // receive message 1023 chars with \n
    char cmdFromClient[helper2::MAX_MESSAGE];

    if (recv(threadSd, &cmdFromClient, sizeof(char) * helper2::MAX_MESSAGE, 0) == -1)
    {
        cout << "ERROR: receiving data" << endl;
    }

    // change char to string
    string input(cmdFromClient);

    if (input.compare("shutdown " + h2.password) == 0)
    { // equal
        pthread_mutex_lock(&h2.isShutdownLock);
        h2.isShutdown = true;
        pthread_mutex_unlock(&h2.isShutdownLock);

        {
            // cereal serialization
            // create a file stream
            ofstream binOut("cereal.bin", ios::binary);
            // create a serializable archive
            cereal::BinaryOutputArchive binOutArchive(binOut);
            // serialize the classes we want
            pthread_mutex_lock(&h2.parseUserinputLock);
            binOutArchive(h2);
            pthread_mutex_unlock(&h2.parseUserinputLock);
            binOut.close();
        }

        char shutdownOK[helper2::MAX_MESSAGE] = "[R]: OK";

        if ((int)send(threadSd, &shutdownOK, sizeof(char) * helper2::MAX_MESSAGE, 0) < (int)(sizeof(char) * helper2::MAX_MESSAGE))
        {
            cout << "ERROR: sending data" << endl;
        }

        shutdown(h2.sd, SHUT_RDWR);
        close(threadSd);
    }
    else
    {
        parseUserinput(&input);

        pthread_mutex_lock(&h2.sendToClientLock);
        if ((int)send(threadSd, &h2.sendToClient, sizeof(char) * helper2::MAX_MESSAGE, 0) < (int)(sizeof(char) * helper2::MAX_MESSAGE))
        {
            cout << "ERROR: sending data" << endl;
        }
        memset(h2.sendToClient, 0, helper2::MAX_MESSAGE);
        pthread_mutex_unlock(&h2.sendToClientLock);

        close(threadSd);
    }

    return NULL;
}

void *parseUserinput(void *input)
{
    pthread_mutex_lock(&h2.parseUserinputLock);

    string userinput = *(static_cast<string *>(input));
    vector<string> inputs = h2.parseCmd(userinput, " ");

    // mutex
    // no argument
    if (inputs.size() == 1)
    {
        if (inputs[0] == "list_candidates")
        {
            pthread_mutex_lock(&h2.sendToClientLock);
            strcpy(h2.sendToClient, h2.list_candidtates().c_str());
            pthread_mutex_unlock(&h2.sendToClientLock);
        }
        else if (inputs[0] == "view_result")
        {
            pthread_mutex_lock(&h2.sendToClientLock);
            strcpy(h2.sendToClient, h2.view_result().c_str());
            pthread_mutex_unlock(&h2.sendToClientLock);
        }
        else
        {
            pthread_mutex_lock(&h2.sendToClientLock);
            strcpy(h2.sendToClient, h2.errorMsg.c_str());
            pthread_mutex_unlock(&h2.sendToClientLock);
        }
    }
    // 1 argument
    else if (inputs.size() == 2)
    {
        string cmd = inputs[0];
        string arg = inputs[1];

        if (cmd == "start_election")
        {
            pthread_mutex_lock(&h2.sendToClientLock);
            strcpy(h2.sendToClient, h2.start_election(arg).c_str());
            pthread_mutex_unlock(&h2.sendToClientLock);
        }
        else if (cmd == "end_election")
        {
            pthread_mutex_lock(&h2.sendToClientLock);
            strcpy(h2.sendToClient, h2.end_election(arg).c_str());
            pthread_mutex_unlock(&h2.sendToClientLock);
        }
        else if (cmd == "add_voter")
        {
            if (!h2.isNumber(arg))
            {
                pthread_mutex_lock(&h2.sendToClientLock);
                strcpy(h2.sendToClient, h2.errorMsg.c_str());
                pthread_mutex_unlock(&h2.sendToClientLock);

                pthread_mutex_unlock(&h2.parseUserinputLock);
                return NULL;
            }
            pthread_mutex_lock(&h2.sendToClientLock);
            strcpy(h2.sendToClient, h2.add_voter(stoi(arg)).c_str());
            pthread_mutex_unlock(&h2.sendToClientLock);
        }
        else if (cmd == "check_registration_status")
        {
            if (!h2.isNumber(arg))
            {
                pthread_mutex_lock(&h2.sendToClientLock);
                strcpy(h2.sendToClient, h2.errorMsg.c_str());
                pthread_mutex_unlock(&h2.sendToClientLock);

                pthread_mutex_unlock(&h2.parseUserinputLock);
                return NULL;
            }
            pthread_mutex_lock(&h2.sendToClientLock);
            strcpy(h2.sendToClient, h2.check_registration_status(stoi(arg)).c_str());
            pthread_mutex_unlock(&h2.sendToClientLock);
        }
        else if (cmd == "vote_count")
        {
            pthread_mutex_lock(&h2.sendToClientLock);
            strcpy(h2.sendToClient, h2.vote_count(arg).c_str());
            pthread_mutex_unlock(&h2.sendToClientLock);
        }
        else
        {
            pthread_mutex_lock(&h2.sendToClientLock);
            strcpy(h2.sendToClient, h2.errorMsg.c_str());
            pthread_mutex_unlock(&h2.sendToClientLock);
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
            pthread_mutex_lock(&h2.sendToClientLock);
            strcpy(h2.sendToClient, h2.add_candidate(arg1, arg2).c_str());
            pthread_mutex_unlock(&h2.sendToClientLock);
        }
        else if (cmd == "vote_for")
        {
            if (!h2.isNumber(arg2))
            {
                pthread_mutex_lock(&h2.sendToClientLock);
                strcpy(h2.sendToClient, h2.errorMsg.c_str());
                pthread_mutex_unlock(&h2.sendToClientLock);

                pthread_mutex_unlock(&h2.parseUserinputLock);
                return NULL;
            }
            pthread_mutex_lock(&h2.sendToClientLock);
            strcpy(h2.sendToClient, h2.vote_for(arg1, stoi(arg2)).c_str());
            pthread_mutex_unlock(&h2.sendToClientLock);
        }
        else if (cmd == "check_voter_status")
        {
            if (!h2.isNumber(arg1) || !h2.isNumber(arg2))
            {
                pthread_mutex_lock(&h2.sendToClientLock);
                strcpy(h2.sendToClient, h2.errorMsg.c_str());
                pthread_mutex_unlock(&h2.sendToClientLock);

                pthread_mutex_unlock(&h2.parseUserinputLock);
                return NULL;
            }
            pthread_mutex_lock(&h2.sendToClientLock);
            strcpy(h2.sendToClient, h2.check_voter_status(stoi(arg1), stoi(arg2)).c_str());
            pthread_mutex_unlock(&h2.sendToClientLock);
        }
        else
        {
            pthread_mutex_lock(&h2.sendToClientLock);
            strcpy(h2.sendToClient, h2.errorMsg.c_str());
            pthread_mutex_unlock(&h2.sendToClientLock);
        }
    }
    else
    {
        pthread_mutex_lock(&h2.sendToClientLock);
        strcpy(h2.sendToClient, h2.errorMsg.c_str());
        pthread_mutex_unlock(&h2.sendToClientLock);
    }

    pthread_mutex_unlock(&h2.parseUserinputLock);
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
            h2.password = optarg;
            h2.changePassword = true;
            break;
        case 'r':
        {
            // make a file stream
            ifstream binIn("cereal.bin", ios::binary);
            // make an input archive
            cereal::BinaryInputArchive binInArchive(binIn);
            // read the archive into variables
            pthread_mutex_lock(&h2.parseUserinputLock);
            binInArchive(h2.isOngoing);
            binInArchive(h2.password);
            binInArchive(h2.highest_vote);
            binInArchive(h2.candidates);
            binInArchive(h2.voters);
            pthread_mutex_unlock(&h2.parseUserinputLock);
            binIn.close();
        }
        break;
        case 'p':
            //connect to port
            if (!h2.isValidPortNumber(optarg))
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
    h2.sd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (h2.sd == -1)
    {
        cout << "ERROR: create server socket" << endl;
        shutdown(h2.sd, SHUT_RDWR);
        return -1;
    }

    int reuse_address = 1;

    if ((setsockopt(h2.sd, SOL_SOCKET, SO_REUSEADDR, &reuse_address, sizeof(int))) == -1)
    {
        cerr << "ERROR: set socket reuseaddress" << endl;
    }

    //bind
    if (bind(h2.sd, (const struct sockaddr *)&st, sizeof(st)) == -1)
    {
        cout << "ERROR: bind" << endl;
        shutdown(h2.sd, SHUT_RDWR);
        return -1;
    }

    //listen
    if (listen(h2.sd, 20) == -1)
    {
        cout << "ERROR: listen" << endl;
        shutdown(h2.sd, SHUT_RDWR);
        return -1;
    }

    if (0 != pthread_mutex_init(&h2.sdLock, NULL))
    {
        throw "Failed to initialize a mutex";
    }

    if (0 != pthread_mutex_init(&h2.isShutdownLock, NULL))
    {
        throw "Failed to initialize a mutex";
    }

    if (0 != pthread_mutex_init(&h2.parseUserinputLock, NULL))
    {
        throw "Failed to initialize a mutex";
    }

    if (0 != pthread_mutex_init(&h2.sendToClientLock, NULL))
    {
        throw "Failed to initialize a mutex";
    }

    if (0 != pthread_mutex_init(&h2.candidatesLock, NULL))
    {
        throw "Failed to initialize a mutex";
    }

    if (0 != pthread_mutex_init(&h2.votersLock, NULL))
    {
        throw "Failed to initialize a mutex";
    }

    while (1)
    {
        pthread_mutex_lock(&h2.isShutdownLock);
        if (h2.isShutdown)
        {
            pthread_mutex_unlock(&h2.isShutdownLock);
            break;
        }
        pthread_mutex_unlock(&h2.isShutdownLock);

        int acceptSd = accept(h2.sd, NULL, NULL);

        if (acceptSd == -1)
        {
            continue;
        }

        int *acceptSdTemp = new int();
        *acceptSdTemp = acceptSd;

        //create a thread
        //pass acceptSd sd as arg
        h2.clientThreads.push_back(1);
        if (pthread_create(&h2.clientThreads.back(), NULL, threadHelper, acceptSdTemp) != 0)
        {
            cout << "ERROR: creating threads for clients" << endl;
        }
    }

    for (auto tid : h2.clientThreads)
    {
        pthread_join(tid, NULL);
    }

    return 0;
}
