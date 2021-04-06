#include "candidate.h"
#include "voter.h"
#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>
#include <unistd.h>
#include <fstream> 

// map<candidate, num_votes>;
// map<Fakecandidate, num_votes>; // if not real candidate, negative
// map<voterId, List<magicNum>>;
// flag ongoing;

using namespace std;

//global variable
//
//dealing with threads
static const int MAX_LIMIT = 9999;
pthread_t threads[MAX_LIMIT];
int running_thread = 0;
//
//password
string password = "cit595";
vector<string> inputs;
vector<Candidate> candidates;
vector<Voter> voters;

bool isOngoing;
bool isShut = false;
int highest_vote = 0;



std::vector<std::string> parseCmd(const std::string &raw_line, const std::string &delim)
{
    std::vector<std::string> res;
    if (raw_line == "")
    {
#if PRINT
        std::cout << "empty string" << std::endl;
#endif
        return res;
    }
    else
    {
        char *strs = new char[raw_line.length() + 1];
        std::strcpy(strs, raw_line.c_str());

        char *d = new char[delim.length() + 1];
        std::strcpy(d, delim.c_str());

        char *p = std::strtok(strs, d);
        while (p)
        {
            std::string s = p;
            res.push_back(s);
            p = std::strtok(NULL, d);
        }
        delete[] strs;
        delete[] d;
        delete[] p;
        return res;
    }
}



//  ADMIN //
void start_election(string cmdpassword)
{
    //TODO
    //clean the backup.txt file

    
   ofstream ofs;
   ofs.open("backup.txt", std::ofstream::out | std::ofstream::trunc);
   ofs.close();
    
    cout<<"[C]: start_election " << cmdpassword << endl;
    if(cmdpassword != password){
        cout<<"[R]: ERROR" << endl;
    }else{
        
      if(isOngoing == true){
        cout<<"[R]: EXISTS" << endl;
      }
    
        isOngoing = true;
        cout<<"[R]: OK" << endl;
        
    }

    return;
}

void end_election(string cmdpassword)
{
    cout << "[C]: end_election " << cmdpassword << endl;
    if (isOngoing == false || cmdpassword != password)
    {
        cout << "[R]: ERROR" << endl;
        return;
    }

    isOngoing = false;
    
    //end thread
    for (int i = 0; i < running_thread; i++)
    {
        pthread_join(threads[i], NULL);
    }

    for (int i = 0; i < candidates.size(); i++)
    {
        cout << candidates[i]->getName() << ":" << candidates[i]->getVotes() << endl;
    }

    
    view_result();
    
    
//     if(highest_vote == 0){
//         cout<<"No Winner" << endl;
//         return;
//     }
    
//     vector<Candidate*> winner;
//     //TODO find the winner and draw
//     for(int i = 0; i < candidates.size(); i++){
//         if(candidates[i] -> getVotes() == highest_vote){
//             winner.push_back(candidates[i]);
//         }
//     }
    
//     cout<<"Draw: ";
//     for(int i = 0; i < winner.size(); i++){
//         cout<< winner[i] -> getName();
        
//         if(i != winner.size() - 1){
//             cout<<", ";
//         }
//     }
//     cout<<endl;
    
    return;

}

void add_candidate(string cmdpassword, string candiName)
{
    //check if ongoing

    cout << "[C]: add_candidate " << cmdpassword << candiName << endl;

    if(cmdpassword != password || isOngoing == false){

        cout << "[R]: ERROR" << endl;
        return;
    }

    for (int i = 0; i < candidates.size(); i++)
    {
        if (candidates[i]->getName() == candiName)
        {
            cout << "[R]: EXISTS" << endl;
            return;
        }
    }

    Candidate* c = new Candidate(candiName, 0);
    

    candidates.push_back(c);
    cout << "[R]: OK" << endl;

    return;

    //If an early voter
    //writes-in a candidate before the store is populated, the candidate vote should not be overridden.
}

void shutdown(string cmdpassword)
{
    cout << "[C]: shutdown " << cmdpassword << endl;

    //if password doesn't match || election ends
    if (isOngoing == false || password != cmdpassword)
    {
        cout << "[R]: ERROR" << endl;
    }

    //shutdown server
    shutdown = true;

    //end thread
    for (int i = 0; i < running_thread; i++)
    {
        pthread_join(threads[i], NULL);
    }


        //TODO write into backup.txt
     ofstream myfile("backup.txt");

     if(myfile.is_open()){
        //write condition
        if(isOngoing){
            myfile<<"1"<<endl;
        }else{
            myfile<<"0"<<endl;
        }


        //write candidate
        myfile<<"CANDIDATE"<<endl;
        for(int i = 0; i < candidates.size(); i++){
            string candidateInfo = candidates[i] -> getName() + " " + to_string(candidates[i] -> getVotes());
            myfile<<candidateInfo<<endl;
        }

        //write Voters
        myfile<<"VOTER"<<endl;
        for(int i = 0; i < voters.size(); i++){
            string voterInfo = to_string(voters[i]->getId()) +" "+ to_string(voters[i] -> getMagicNum());
            myfile<<voterInfo<<endl;
        }

        myfile.close();
     }

}

// // VOTE VOTER//

void add_voter(int voterId){
     cout << "[C]: add_voter " << voterId << endl;
    if(isOngoing == false || voterId > 9999 || voterId < 1000){
      cout << "[R]: ERROR " << endl;
      return;
    }

    //check exists
    for (int i = 0; i < voters.size(); i++)
    {
        if (voters[i]->getId() == voterId)
        {
            cout << "[R]: EXISTS " << endl;
            return;
        }
    }

    //register a new user

    Voter* v = new Voter(voterId, 0);
    voters.push_back(v);
    cout << "[R]: OK" << endl;

    return;
}

// vote_for(String name, int voterId){
//     //check flag

//     //if not voted, add votes
//     //If the named candidate is not already present in the system, add the candidate to the system with a vote count of 1
//     //if the candidate is already present in the system, increment their vote count by 1

//     //[R]
//     //if candidate exists in system: EXISTS
//     //if not: NEW
//     //voterId innvalid: NOTAVOTER
//     //already vote: ALREADYVOTED
//     //ERROR


//     //returns a random number on a new line EXISTS + magicNum (random)

// }

// check_registration_status(int voterId){
//     //check flag
//     //
//     //[R] EXISTS// INVALID // UNREGISTERED //ERROR
// }

// check_voter_status(int voterId, int magicNum){
//     //check flag

//     //[R]
//     //“ALREADYVOTED” if voter has voted
//     //“CHECKSTATUS” if voter isn’t registered
//     //“UNAUTHORIZED” if magicno doesn’t match records
//     //“ERROR” if there is any error both incorrect
// }

//ANY USER
void list_candidtate()
{
    cout << "[C]: list_candidtate" << endl;
    cout << "[R]: ";

    if (isOngoing)
    {
        for (auto it = candidates.begin(); it < candidates.end(); it++)
        {
            cout << it->getName() << endl;
        }
    }
}

void vote_count(String name)
{
    cout << "[C]: vote_count " << name << endl;
    cout << "[R]: ";

    if (isOngoing)
    {
        for (auto it = candidates.begin(); it < candidates.end(); it++)
        {
            if (it->getName() == name)
            {
                cout << it->getVotes() << endl;
                return;
            }
        }
        cout << "-1" << endl;
        return;
    }
    else
    {
        cout << "-1" << endl;
        return;
    }
}


void view_result()
{
    int numOfWinners = 0;

    cout << "[C]: view_result" << endl;
    cout << "[R]: ";

    if (!isOngoing)
    {
        for (auto it = candidates.begin(); it < candidates.end(); it++)
        {
            if (it->getVotes() == highestVote)
                numOfWinners++;
            cout << it->getName() << ": " << it->getVotes() << endl;
        }

        if (highestVote == 0)
        {
            cout << "No Winner" << endl;
        }
        else
        {
            int commaCount = 0;

            if (numOfWinners == 1)
            {
                cout << "Winner: ";
            }
            else
            {
                cout << "Draw: ";
            }
            for (auto it = candidates.begin(); it < candidates.end(); it++)
            {
                if (it->getVotes() == highestVote)
                {
                    commaCount++;
                    if (commaCount == numOfWinners)
                    {
                        cout << it->getName() << endl;
                    }
                    else
                    {
                        cout << it->getName() << ", ";
                    }
                }
            }
        }
    }
    else
    {
        cout << "ERROR" << endl;
    }
}



void recover()
{
    ifstream myfile("backup.txt");

    if (myfile.is_open())
    {
        //check condition
        string ongoing;
        getline(myfile, ongoing);
        if (ongoing == "1")
        {
            isOngoing = true;
        }
        else
        {
            isOngoing = false;
        }

        //check Candidate
        string candidate;
        getline(myfile, candidate);

        string candidateInfo;
        while (getline(myfile, candidateInfo) && candidateInfo != "VOTER")
        {
            vector<string> store = parseCmd(candidateInfo, " ");
            Candidate *c = new Candidate(store[0], stoi(store[1]));
            candidates.push_back(c);
        }

        string voterInfo;
        while (getline(myfile, voterInfo))
        {
            vector<string> vstore = parseCmd(voterInfo, " ");
            Voter *v = new Voter(stoi(vstore[0]), stoi(vstore[1]));
            voters.push_back(v);
        }

        myfile.close();
    }

}

void *parseUserinput(void *input)
{
    //TODO parse input string
    //mutex
    string userinput = *(static_cast<std::string *>(input));

    sleep(3);
    cout << "input is" << userinput << endl;

    return NULL;
}

int main(int argc, char *argv[])
{
    //1. parse command
    //-r -p -a
    int option;
    while ((option = getopt(argc, argv, "a:r:p:")) != -1)
    {
        switch (option)
        {
        case 'a':
            password = optarg;
            cout << "password " << optarg << endl;
            break;
        case 'r':
            isShut = false;
            recover();
            //read file
            cout << "recover " << optarg << endl;
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

    //2. read input
    while (1)
    {
        if(isShut  == true){
            exit(0);
            break;
        }
        //mutex?
        string input;
        //get line
        getline(cin, input);
        fflush(stdin);

        inputs.push_back(input);
        int size = inputs.size();

        pthread_t tid1;

        pthread_create(&threads[running_thread], NULL, parseUserinput, &inputs[size - 1]);
        running_thread++;
    }

    //TODO join all threads

    return 0;
}
