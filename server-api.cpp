#include "helper.h"
using namespace std;

// input Parser and call the related funtion
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
            cout << "ERROR1" << endl;
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
        else if (cmd == "shutdown")
        {
            shutdown(arg);
        }
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
            cout << "ERROR2" << endl;
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
                cout << "ERROR3" << endl;
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
                cout << "ERROR4" << endl;
                pthread_mutex_unlock(&parseLock);
                return NULL;
            }
            check_voter_status(stoi(arg1), stoi(arg2));
        }
        else
        {
            cout << "ERROR5" << endl;
            //pthread_exit(NULL);
        }
    }
    else
    {
        cout << "ERROR6" << endl;
    }
    //pthread_exit(NULL);
    
    pthread_mutex_unlock(&parseLock);
    
    return NULL; 
}

// MAIN //
int main(int argc, char *argv[])
{
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
            //cout << "password " << optarg << endl;
            break;
        case 'r':
            recover();
            // read file
            //cout << "recover " << endl;
            break;
        case 'p':
            //connect to port
            cout << "port " << optarg << endl;
            break;
        case '?':
            exit(0);
            break;
        }
    }
    if (0 != pthread_mutex_init(&parseLock, NULL))
        throw "Failed to initialize a mutex";
    
//     if (0 != pthread_mutex_init(&methodLock, NULL))
//         throw "Failed to initialize a mutex";
    
// //     if (0 != pthread_mutex_init(&userCmdsLock, NULL))
// //         throw "Failed to initialize a mutex";
    
//     if (0 != pthread_mutex_init(&candidatesLock, NULL))
//         throw "Failed to initialize a mutex";
    
//     if (0 != pthread_mutex_init(&votersLock, NULL))
//         throw "Failed to initialize a mutex";
    
//     if (0 != pthread_mutex_init(&inputLock, NULL))
//         throw "Failed to initialize a mutex";

    // 2. read input
    while (1)
    {
        // mutex?
  
        string input;
     
        // get line
        
        getline(cin, input);
        
        
   
        userCmds.push_back(input);  
     
        pthread_mutex_lock(&parseLock);
        pthread_create(&threads[running_thread], NULL, parseUserinput, &userCmds[userCmds.size() - 1]);
        running_thread++;
        pthread_mutex_unlock(&parseLock);
   
        

    }
    
    
    
    
    
    

    return 0;
}
