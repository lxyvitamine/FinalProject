#include "helper2.h"
#include <netinet/ip.h>
#include <signal.h>
// #include <sys/socket.h>

using namespace std;

void get_http(string page, char *response);
string get_reply_page(string cmd, string reply);
void *parseUserinput(void *input);

void *threadHelper(void *argSd)
{
    int threadSd = *(int *)argSd;
    delete (int *)argSd;

 char buffer[30000] = {0};
    read(threadSd, buffer, 30000);

    //get the last line from request
    vector<string> v = parseCmd(buffer, "\r\n");
    int size = v.size();

    //get the content after "="
    string i = v[size - 1];
    vector<string> user = parseCmd(i, "=");

    //replace "+" with " "
    vector<string> noplus = parseCmd(user[1], "+");
    string res;
    for (int i = 0; i < (int) noplus.size(); i++)
    {
        if (i == (int) noplus.size() - 1)
        {
            res += noplus[i];
        }
        else
        {
            res += noplus[i] + " ";
        }
    }
    //printf("%s\n", res.c_str());

    string input = res;

    if (input.compare("shutdown " + password) == 0)
    { // equal
        pthread_mutex_lock(&isShutdownLock);
        isShutdown = true;
        pthread_mutex_unlock(&isShutdownLock);

        pthread_mutex_lock(&parseUserinputLock);
        shutdown(password);
        pthread_mutex_unlock(&parseUserinputLock);

         char shutdownOK[] = "[R]: OK";
        char response[2000];
        get_http(get_reply_page(res, string(shutdownOK)), response);

        if ((int)send(threadSd, response, sizeof(char) * MAX_MESSAGE, 0) < (int)(sizeof(char) * MAX_MESSAGE))
        {
            cout << "ERROR: sending data" << endl;
        }

        shutdown(sd, SHUT_RDWR);
        close(threadSd);
    }
    else
    {

        parseUserinput(&input);

        pthread_mutex_lock(&sendToClientLock);
        char response[2000];
        get_http(get_reply_page(res, string(sendToClient)), response);
        if ((int)send(threadSd, response, sizeof(char) * MAX_MESSAGE, 0) < (int)(sizeof(char) * MAX_MESSAGE))
        {
            cout << "ERROR: sending data" << endl;
        }
        memset(sendToClient, 0, MAX_MESSAGE);
        pthread_mutex_unlock(&sendToClientLock);
        close(threadSd);

    }

    return NULL;
}

void get_http(string page, char *response)
{
    //cout << page << endl;
    string header = "HTTP/1.1 200 OK\r\n";
    header.append("content-Type: text/html; charset=utf-8\r\n");
    header.append("Server: Voting System\r\n");
    header.append("content-Length: ");
    header.append(to_string(page.size()));
    header += "\r\n\r\n";
    header += page;
    // see whats to be send
//     cout << "page generated\r\n"
//          << header << endl;
//     sprintf(response, "%s", header.c_str());
}

string get_reply_page(string cmd, string reply)
{
    string temp = "<html><header><title>Voting System</title></header><h1><p>Feedback</p></h1>";
    temp.append("[C]: " + cmd + "\r\n" + reply);
    temp.append("</html>");
    return temp;
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

int main(int argc, char *argv[])
{
    // pid = getpid();
    // signal(SIGUSR1, handleSigusr1);

    //default to 10000
    unsigned short int port = 8080;

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
    //st.sin_port = port;
    st.sin_port = htons(port);

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


        int acceptSd = accept(sd, NULL, NULL);


        if (acceptSd == -1)
        {

            continue;
        }

        int *acceptSdTemp = new int();
        *acceptSdTemp = acceptSd;

        //create a thread

        clientThreads.push_back(1);
        if (pthread_create(&clientThreads.back(), NULL, threadHelper, acceptSdTemp) != 0)
        {
            cout << "ERROR: creating threads for clients" << endl;
        }
 
    }


    for (auto tid : clientThreads)
    {
        pthread_join(tid, NULL);
    }

    return 0;
}
