#include "helper2.h"

using namespace std;

// GLOBAL VARIABLE //
// dealing with threads
vector<pthread_t> clientThreads;
// int clientIndex = 0;
// const int MAX_LIMIT = 9999;
// pthread_t threads[MAX_LIMIT];
// int running_thread = 0;
// password
string password = "cit595";
vector<Candidate *> candidates;
vector<Voter *> voters;
unordered_set<int> generatedMagicNumbers;
// flag
bool isOngoing = false;
int highest_vote = 0;
bool changePassword = false;
int isShutdown = false;
// main() & mutex
// string userCmds[MAX_LIMIT];
pthread_mutex_t sdLock;
pthread_mutex_t isShutdownLock;
pthread_mutex_t parseUserinputLock;
pthread_mutex_t sendToClientLock;
pthread_mutex_t candidatesLock;
pthread_mutex_t votersLock;
// pthread_mutex_t inputLock;
// pthread_mutex_t magicNumLock;
int sd;
const int MAX_MESSAGE = 1024;
char sendToClient[MAX_MESSAGE];
string errorMsg = "ERROR";

// helper to view result of election
string view_result_helper()
{
    string feedback;
    int numOfWinners = 0;

    if (!isOngoing)
    {
        pthread_mutex_lock(&candidatesLock);
        for (auto it = candidates.begin(); it < candidates.end(); it++)
        {
            if ((*it)->getVotes() == highest_vote)
            {
                numOfWinners++;
            }

            feedback = feedback + (*it)->getName() + ": " + to_string((*it)->getVotes()) + "\n";
        }
        pthread_mutex_unlock(&candidatesLock);

        if (highest_vote == 0)
        {
            feedback = feedback + "No Winner";
            return feedback;
        }
        else
        {
            int commaCount = 0;
            if (numOfWinners == 1)
            {
                feedback = feedback + "Winner: ";
            }
            else
            {
                feedback = feedback + "Draw: ";
            }
            pthread_mutex_lock(&candidatesLock);
            for (auto it = candidates.begin(); it < candidates.end(); it++)
            {
                if ((*it)->getVotes() == highest_vote)
                {
                    commaCount++;
                    if (commaCount == numOfWinners)
                    {
                        feedback = feedback + (*it)->getName();
                        // cout << (*it)->getName() << endl;
                    }
                    else
                    {
                        feedback = feedback + (*it)->getName() + ", ";
                        //cout << (*it)->getName() << ", ";
                    }
                }
            }
            pthread_mutex_unlock(&candidatesLock);
        }
    }
    else
    {
        feedback = "[R]: ERROR";
        return feedback;
    }

    return feedback;
}

bool isNumber(const string &str)
{
    char *ptr;
    strtol(str.c_str(), &ptr, 10);
    return *ptr == '\0';
}

bool isValidPortNumber(const string &str)
{
    char *ptr;
    int ret = strtol(str.c_str(), &ptr, 10);
    return *ptr == '\0' && ret >= 0 && ret <= 65535;
}

// PARSER //
vector<string> parseCmd(const string &raw_line, const string &delim)
{
    vector<string> res;
    if (raw_line == "")
    {
#if PRINT
        cout << "empty string" << endl;
#endif
        return res;
    }
    else
    {
        char *strs = new char[raw_line.length() + 1];
        strcpy(strs, raw_line.c_str());

        char *d = new char[delim.length() + 1];
        strcpy(d, delim.c_str());

        char *p = strtok(strs, d);
        while (p)
        {
            string s = p;
            res.push_back(s);
            p = strtok(NULL, d);
        }

        delete[] strs;
        delete[] d;
        delete[] p;
        return res;
    }
}

// ADMIN //
string start_election(string cmdpassword)
{
    string feedback;

    if (cmdpassword != password)
    {
        feedback = "[R]: ERROR";
        return feedback;
    }
    else
    {
        if (isOngoing)
        {
            feedback = "[R]: EXISTS";
            return feedback;
        }
        else
        {
            // clean the backup.txt file
            ofstream backup;
            backup.open("backup.txt", ofstream::out | ofstream::trunc);
            backup.close();

            // clean the clientCommands.txt file and write the start_election command
            ofstream clientCommands;
            clientCommands.open("clientCommands.txt", ofstream::out | ofstream::trunc);
            if (clientCommands.is_open())
            {
                clientCommands << "start_election_ctrlc" << endl;
            }
            clientCommands.close();

            // clean the votersInfo.txt file
            ofstream votersInfo;
            votersInfo.open("votersInfo.txt", ofstream::out | ofstream::trunc);
            votersInfo.close();

            isOngoing = true;
            highest_vote = 0;
            pthread_mutex_lock(&candidatesLock);
            candidates.clear();
            pthread_mutex_unlock(&candidatesLock);
            pthread_mutex_lock(&votersLock);
            voters.clear();
            pthread_mutex_unlock(&votersLock);

            feedback = "[R]: OK";
            return feedback;
        }
    }
}

void start_election_ctrlc()
{
    isOngoing = true;
    highest_vote = 0;
    pthread_mutex_lock(&candidatesLock);
    candidates.clear();
    pthread_mutex_unlock(&candidatesLock);
    pthread_mutex_lock(&votersLock);
    voters.clear();
    pthread_mutex_unlock(&votersLock);

    ofstream backup;
    backup.open("backup.txt", ofstream::out | ofstream::trunc);
    backup.close();
}

string end_election(string cmdpassword)
{
    string feedback;

    if (!isOngoing || cmdpassword != password)
    {
        feedback = "[R]: ERROR";
        return feedback;
    }

    isOngoing = false;

    ofstream clientCommands;
    clientCommands.open("clientCommands.txt", ofstream::app);
    if (clientCommands.is_open())
    {
        clientCommands << "end_election_ctrlc" << endl;
    }
    clientCommands.close();

    // end threads
    //     pthread_mutex_lock(&runningThreadLock);
    //     for (int i = 0; i < running_thread; i++)
    //     {
    //         pthread_detach(threads[i]);
    //     }
    //     pthread_mutex_unlock(&runningThreadLock);

    feedback = view_result_helper();

    return feedback;
}

void end_election_ctrlc()
{
    isOngoing = false;
}

string add_candidate(string cmdpassword, string candiName)
{
    string feedback;
    //cout << "[C]: add_candidate " << cmdpassword << " " << candiName << endl;

    // check isOngoing flag and password
    if (cmdpassword != password || !isOngoing)
    {
        feedback = "[R]: ERROR";
        return feedback;
    }

    // check if candidate already exists
    pthread_mutex_lock(&candidatesLock);
    for (int i = 0; i < (int)candidates.size(); i++)
    {
        if (candidates[i]->getName() == candiName)
        {
            feedback = "[R]: EXISTS";
            pthread_mutex_unlock(&candidatesLock);
            return feedback;
        }
    }
    pthread_mutex_unlock(&candidatesLock);

    // if not, create new candidate
    Candidate *c = new Candidate(candiName, 0);
    pthread_mutex_lock(&candidatesLock);
    candidates.push_back(c);
    pthread_mutex_unlock(&candidatesLock);

    // write add_candidate command to clientCommands.txt file
    ofstream clientCommands;
    clientCommands.open("clientCommands.txt", ofstream::app);
    if (clientCommands.is_open())
    {
        clientCommands << "add_candidate_ctrlc " << candiName << endl;
    }
    clientCommands.close();

    feedback = "[R]: OK";
    return feedback;
}

void add_candidate_ctrlc(string candiName)
{
    Candidate *c = new Candidate(candiName, 0);
    pthread_mutex_lock(&candidatesLock);
    candidates.push_back(c);
    pthread_mutex_unlock(&candidatesLock);
}

string shutdown(string cmdpassword)
{
    string feedback;
    // if password doesn't match, print error
    if (password != cmdpassword)
    {
        feedback = "[R]: ERROR";
        return feedback;
    }

    // end thread
    //     if (isOngoing)
    //     {
    //         for (int i = 0; i < running_thread; i++)
    //         {
    //             pthread_detach(threads[i]);
    //         }
    //     }

    // write into backup.txt
    ofstream myfile("backup.txt");

    if (myfile.is_open())
    {
        // first line: is ongoing or not
        if (isOngoing)
        {
            myfile << "1" << endl;
        }
        else
        {
            myfile << "0" << endl;
        }

        //second line: add password
        myfile << password << endl;

        //third line: highest_vote
        myfile << highest_vote << endl;

        // write candidate
        myfile << "CANDIDATE" << endl;
        pthread_mutex_lock(&candidatesLock);
        for (int i = 0; i < (int)candidates.size(); i++)
        {
            string candidateInfo = candidates[i]->getName() + " " + to_string(candidates[i]->getVotes());
            myfile << candidateInfo << endl;
        }
        pthread_mutex_unlock(&candidatesLock);

        // write voters
        myfile << "VOTER" << endl;
        pthread_mutex_lock(&votersLock);
        for (int i = 0; i < (int)voters.size(); i++)
        {
            string voterInfo = to_string(voters[i]->getId()) + " " + to_string(voters[i]->getMagicNum());
            myfile << voterInfo << endl;
        }
        pthread_mutex_unlock(&votersLock);

        myfile.close();
    }

    // clean the clientCommands.txt file
    ofstream clientCommands;
    clientCommands.open("clientCommands.txt", ofstream::out | ofstream::trunc);
    clientCommands.close();

    // clean the votersInfo.txt file
    ofstream votersInfo;
    votersInfo.open("votersInfo.txt", ofstream::out | ofstream::trunc);
    votersInfo.close();

    // delete heap memory
    pthread_mutex_lock(&candidatesLock);
    for (int i = 0; i < (int)candidates.size(); i++)
    {
        delete candidates[i];
    }
    pthread_mutex_unlock(&candidatesLock);

    pthread_mutex_lock(&votersLock);
    for (int i = 0; i < (int)voters.size(); i++)
    {
        delete voters[i];
    }
    pthread_mutex_unlock(&votersLock);

    feedback = "[R]: OK";

    return feedback;
}

// VOTER //
string add_voter(int voterId)
{
    if (!isOngoing || voterId > 9999 || voterId < 1000)
    {
        return "[R]: ERROR";
    }

    // check exists
    pthread_mutex_lock(&votersLock);
    for (int i = 0; i < (int)voters.size(); i++)
    {
        if (voters[i]->getId() == voterId)
        {
            pthread_mutex_unlock(&votersLock);
            return "[R]: EXISTS";
        }
    }
    pthread_mutex_unlock(&votersLock);

    // create a new voter
    Voter *v = new Voter(voterId, 0);
    pthread_mutex_lock(&votersLock);
    voters.push_back(v);
    pthread_mutex_unlock(&votersLock);

    // write add_voter command to clientCommands.txt file
    ofstream clientCommands;
    clientCommands.open("clientCommands.txt", ofstream::app);
    if (clientCommands.is_open())
    {
        clientCommands << "add_voter_ctrlc " << to_string(voterId) << endl;
    }
    clientCommands.close();

    return "[R]: OK";
}

void add_voter_ctrlc(int voterId)
{
    Voter *v = new Voter(voterId, 0);
    pthread_mutex_lock(&votersLock);
    voters.push_back(v);
    pthread_mutex_unlock(&votersLock);
}

// helper funtion to generate unique magic number
int generateUniqueMagicNumber()
{
    int num = rand() % 100000 + 1;

    while (generatedMagicNumbers.find(num) != generatedMagicNumbers.end())
    {
        num = rand() % 100000 + 1;
    }

    generatedMagicNumbers.insert(num);

    return num;
}

string vote_for(string name, int voterId)
{
    if (!isOngoing || voterId > 9999 || voterId < 1000)
    {
        return "[R]: ERROR";
    }

    // check if voter exists, voterId innvalid: NOTAVOTER
    bool canVote = false;

    pthread_mutex_lock(&votersLock);
    for (int i = 0; i < (int)voters.size(); i++)
    {
        if (voters[i]->getId() == voterId)
        {
            canVote = true; // voter exist
        }
    }
    pthread_mutex_unlock(&votersLock);

    if (!canVote)
    {
        return "[R]: NOTAVOTER";
    }

    // check if voter already voted, already voted: ALREADYVOTED
    pthread_mutex_lock(&votersLock);
    for (int i = 0; i < (int)voters.size(); i++)
    {
        if (voters[i]->getId() == voterId)
        {
            if (voters[i]->getMagicNum() != 0)
            { // magic number != 0, voted
                pthread_mutex_unlock(&votersLock);
                return "[R]: ALREADYVOTED";
            }
        }
    }
    pthread_mutex_unlock(&votersLock);

    // check if candidate exists
    // candidate exists in system: EXISTS
    canVote = false;
    // returned string
    string res;
    pthread_mutex_lock(&candidatesLock);
    for (int i = 0; i < (int)candidates.size(); i++)
    {
        if (candidates[i]->getName() == name)
        {
            //cout << "[R]: EXISTS" << endl;
            res = "[R]: EXISTS";
            // increment vote count by 1
            candidates[i]->addVotes();
            // update highest_vote
            highest_vote = max(highest_vote, candidates[i]->getVotes());
            canVote = true;
            break;
        }
    }
    pthread_mutex_unlock(&candidatesLock);

    // candidate not exists in system: NEW
    if (!canVote)
    {
        Candidate *c = new Candidate(name, 1); // set vote to 1

        pthread_mutex_lock(&candidatesLock);
        candidates.push_back(c);
        pthread_mutex_unlock(&candidatesLock);

        // update highest_vote
        highest_vote = max(highest_vote, c->getVotes());
        //cout << "[R]: NEW" << endl;
        res = "[R]: NEW";
    }

    // finish vote and return magic number
    int magicNumber;

    // initialize random seed and generate random number from 1 to 100000
    srand(time(NULL));
    magicNumber = generateUniqueMagicNumber();

    // print magicNumber of this voter
    pthread_mutex_lock(&votersLock);
    for (int i = 0; i < (int)voters.size(); i++)
    {
        if (voters[i]->getId() == voterId)
        {
            voters[i]->setMagicNum(magicNumber);
            res = res + "\n" + to_string(magicNumber);
            //cout << magicNumber << endl;
            break;
        }
    }
    pthread_mutex_unlock(&votersLock);

    // write vote_for command into clientCommands.txt file
    ofstream clientCommands;
    clientCommands.open("clientCommands.txt", ofstream::app);
    if (clientCommands.is_open())
    {
        clientCommands << "vote_for_ctrlc " << name << " " << to_string(voterId) << endl;
    }
    clientCommands.close();

    // write voter info into votersInfo.txt file (for magic number)
    ofstream votersInfo;
    votersInfo.open("votersInfo.txt", ofstream::app);
    if (votersInfo.is_open())
    {
        votersInfo << to_string(voterId) << " " << to_string(magicNumber) << endl;
    }
    votersInfo.close();

    return res;
}

void vote_for_ctrlc(string name, int voterId)
{
    // check if candidate exists
    // candidate exists in system: EXISTS
    bool canVote = false;

    pthread_mutex_lock(&candidatesLock);
    for (int i = 0; i < (int)candidates.size(); i++)
    {
        if (candidates[i]->getName() == name)
        {
            // increment vote count by 1
            candidates[i]->addVotes();
            // update highest_vote
            highest_vote = max(highest_vote, candidates[i]->getVotes());
            canVote = true;
            break;
        }
    }
    pthread_mutex_unlock(&candidatesLock);

    // candidate not exists in system: NEW
    if (!canVote)
    {
        Candidate *c = new Candidate(name, 1); // set vote to 1

        pthread_mutex_lock(&candidatesLock);
        candidates.push_back(c);
        pthread_mutex_unlock(&candidatesLock);

        // update highest_vote
        highest_vote = max(highest_vote, c->getVotes());
    }

    bool voterIsUpdated = false;
    pthread_mutex_lock(&votersLock);
    for (int i = 0; i < (int)voters.size(); i++)
    {
        if (voters[i]->getId() == voterId)
        {
            ifstream votersInfo("votersInfo.txt");
            if (votersInfo.is_open() && votersInfo.peek() != ifstream::traits_type::eof())
            {
                string idAndNumber;
                while (getline(votersInfo, idAndNumber))
                {
                    vector<string> splits = parseCmd(idAndNumber, " ");
                    int id = stoi(splits[0]);
                    int number = stoi(splits[1]);
                    if (id == voterId)
                    {
                        voters[i]->setMagicNum(number);
                        voterIsUpdated = true;
                        break;
                    }
                }
            }
            votersInfo.close();
        }
        if (voterIsUpdated)
        {
            break;
        }
    }
    pthread_mutex_unlock(&votersLock);
}

string check_registration_status(int voterId)
{
    // check isOngoing
    if (!isOngoing)
    {
        return "[R]: ERROR";
    }

    // id out of range
    if (voterId > 9999 || voterId < 1000)
    {
        return "[R]: INVALID";
    }

    // check if voter exists
    pthread_mutex_lock(&votersLock);
    for (int i = 0; i < (int)voters.size(); i++)
    {
        if (voters[i]->getId() == voterId)
        { // voter exist
            pthread_mutex_unlock(&votersLock);
            return "[R]: EXISTS";
        }
    }
    pthread_mutex_unlock(&votersLock);

    // voter not exist
    return "[R]: UNREGISTERED";
}

string check_voter_status(int voterId, int magicNum)
{
    if (!isOngoing || voterId > 9999 || voterId < 1000)
    {
        return "[R]: ERROR";
    }

    pthread_mutex_lock(&votersLock);
    for (int i = 0; i < (int)voters.size(); i++)
    {
        if (voters[i]->getId() == voterId)
        {
            // wrong magicNum, print “UNAUTHORIZED”
            if (voters[i]->getMagicNum() != magicNum || magicNum == 0)
            {
                pthread_mutex_unlock(&votersLock);
                return "[R]: UNAUTHORIZED";
            }
            else
            { // “ALREADYVOTED” if voter has voted
                pthread_mutex_unlock(&votersLock);
                return "[R]: ALREADYVOTED";
            }
        }
    }
    pthread_mutex_unlock(&votersLock);

    // voter not in voters
    return "[R]: CHECKSTATUS";
}

// ANY USER //
string list_candidtates()
{
    // cout << "[C]: list_candidtates" << endl;
    string res = "[R]: ";

    if (isOngoing)
    {
        // cout << "[R]: ";
        pthread_mutex_lock(&candidatesLock);
        for (auto it = candidates.begin(); it < candidates.end(); it++)
        {
            if (it != candidates.end() - 1)
            {
                res = res + (*it)->getName() + "\n";
            }
            else
            {
                res = res + (*it)->getName();
            }

            //cout << (*it)->getName() << endl;
        }
        pthread_mutex_unlock(&candidatesLock);

        return res;
    }
    else
    {
        return res;
    }
}

string vote_count(string name)
{
    string feedback;
    feedback += "[R]: ";

    if (isOngoing)
    {
        pthread_mutex_lock(&candidatesLock);
        for (auto it = candidates.begin(); it < candidates.end(); it++)
        {
            if ((*it)->getName() == name)
            {
                feedback += to_string((*it)->getVotes());
                pthread_mutex_unlock(&candidatesLock);
                return feedback;
            }
        }
        pthread_mutex_unlock(&candidatesLock);

        return "-1";
    }
    else
    {
        return "-1";
    }
}

string view_result()
{
    return view_result_helper();
}

// optional argument: -r
void recover()
{
    ifstream myfile("backup.txt"); // hard code to read "backup.txt"

    if (myfile.is_open() && myfile.peek() != ifstream::traits_type::eof())
    {
        // check condition
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

        // check password
        string filePassword;
        getline(myfile, filePassword);

        if (!changePassword)
        {
            password = filePassword;
        }

        //update highest_vote
        string highest_vote_read;
        getline(myfile, highest_vote_read);
        highest_vote = stoi(highest_vote_read);

        // check Candidate
        string candidate;
        getline(myfile, candidate);

        string candidateInfo;
        while (getline(myfile, candidateInfo) && candidateInfo != "VOTER")
        {
            vector<string> store = parseCmd(candidateInfo, " ");
            Candidate *c = new Candidate(store[0], stoi(store[1]));

            pthread_mutex_lock(&candidatesLock);
            candidates.push_back(c);
            pthread_mutex_unlock(&candidatesLock);
        }

        string voterInfo;
        while (getline(myfile, voterInfo))
        {
            vector<string> vstore = parseCmd(voterInfo, " ");
            Voter *v = new Voter(stoi(vstore[0]), stoi(vstore[1]));

            pthread_mutex_lock(&votersLock);
            voters.push_back(v);
            pthread_mutex_unlock(&votersLock);
        }

        myfile.close();
    }
}
