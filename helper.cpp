#include "helper.h"

using namespace std;

// GLOBAL VARIABLE //
// dealing with threads
const int MAX_LIMIT = 9999;
pthread_t threads[MAX_LIMIT];
int running_thread = 0;
// password
string password = "cit595";
vector<Candidate *> candidates;
vector<Voter *> voters;
vector<int> generatedValues;
// flag
bool isOngoing = false;
int highest_vote = 0;
bool changePassword = false;
// main() & mutex
string userCmds[MAX_LIMIT];
pthread_mutex_t parseLock;
// pthread_mutex_t userCmdsLock;
// pthread_mutex_t candidatesLock;
// pthread_mutex_t votersLock;
pthread_mutex_t methodLock;
// pthread_mutex_t inputLock;
// pthread_mutex_t magicNumLock;

// helper to view result of election
void view_result_helper()
{
    int numOfWinners = 0;
    cout << "[R]: ";

    if (!isOngoing)
    {
        for (auto it = candidates.begin(); it < candidates.end(); it++)
        {
            if ((*it)->getVotes() == highest_vote)
                numOfWinners++;
            cout << (*it)->getName() << ": " << (*it)->getVotes() << endl;
        }

        if (highest_vote == 0)
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
                if ((*it)->getVotes() == highest_vote)
                {
                    commaCount++;
                    if (commaCount == numOfWinners)
                    {
                        cout << (*it)->getName() << endl;
                    }
                    else
                    {
                        cout << (*it)->getName() << ", ";
                    }
                }
            }
        }
    }
    else
    {
        cout << "ERROR" << endl;
    }

    return;
}

bool isNumber(const string &str)
{
    char *ptr;
    strtol(str.c_str(), &ptr, 10);
    return *ptr == '\0';
}

// PARSER //
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

// ADMIN //
void start_election(string cmdpassword)
{
    // TODO
    // clean the backup.txt file
    ofstream ofs;
    ofs.open("backup.txt", ofstream::out | ofstream::trunc);
    ofs.close();

    cout << "[C]: start_election " << cmdpassword << endl;

    if (cmdpassword != password)
    {
        cout << "[R]: ERROR" << endl;
        return;
    }
    else
    {
        if (isOngoing)
        {
            cout << "[R]: EXISTS" << endl;
            return;
        }
        isOngoing = true;
        cout << "[R]: OK" << endl;
    }

    return;
}

void end_election(string cmdpassword)
{
    cout << "[C]: end_election " << cmdpassword << endl;

    if (!isOngoing || cmdpassword != password)
    {
        cout << "[R]: ERROR" << endl;
        return;
    }

    isOngoing = false;

    // end thread
	pthread_mutex_lock(&methodLock);
    for (int i = 0; i < running_thread; i++)
    {
        int rt = pthread_detach(threads[i]);

        if (rt != 0)
        {
            cout << "Fail to join " << endl;
        }
    }
	pthread_mutex_unlock(&methodLock);

    view_result_helper();

    return;
}

void add_candidate(string cmdpassword, string candiName)
{
    cout << "[C]: add_candidate " << cmdpassword << " " << candiName << endl;

    // check isOngoing flag and password
    if (cmdpassword != password || !isOngoing)
    {
        cout << "[R]: ERROR" << endl;
        return;
    }

    // check if candidate already exists
    for (int i = 0; i < (int)candidates.size(); i++)
    {
        if (candidates[i]->getName() == candiName)
        {
            cout << "[R]: EXISTS" << endl;
            return;
        }
    }

    // if not, create new candidate
    Candidate *c = new Candidate(candiName, 0);
    candidates.push_back(c);

    cout << "[R]: OK" << endl;

    return;
}

void shutdown(string cmdpassword)
{	
    cout << "[C]: shutdown " << cmdpassword << endl;

    // if password doesn't match, print error
    if (password != cmdpassword)
    {
        cout << "[R]: ERROR" << endl;
        return;
    }

    // end thread
//     pthread_mutex_lock(&methodLock);
    if (isOngoing)
    {
        for (int i = 0; i < running_thread; i++)
        {
            pthread_detach(threads[i]);
        }
    }
//     pthread_mutex_unlock(&methodLock);

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

        for (int i = 0; i < (int)candidates.size(); i++)
        {
            string candidateInfo = candidates[i]->getName() + " " + to_string(candidates[i]->getVotes());
            myfile << candidateInfo << endl;
        }

        // write voters
        myfile << "VOTER" << endl;
        for (int i = 0; i < (int)voters.size(); i++)
        {
            string voterInfo = to_string(voters[i]->getId()) + " " + to_string(voters[i]->getMagicNum());
            myfile << voterInfo << endl;
        }

        myfile.close();
    }

    // delete heap memory
    for (int i = 0; i < (int)candidates.size(); i++)
    {
        delete candidates[i];
    }

    for (int i = 0; i < (int)voters.size(); i++)
    {
        delete voters[i];
    }

    cout << "[R]: OK" << endl;

//     pthread_mutex_unlock(&parseLock);

    
}

// VOTER //
void add_voter(int voterId)
{
    cout << "[C]: add_voter " << voterId << endl;

    if (!isOngoing || voterId > 9999 || voterId < 1000)
    {
        cout << "[R]: ERROR" << endl;
        return;
    }

    // check exists
    for (int i = 0; i < (int)voters.size(); i++)
    {
        if (voters[i]->getId() == voterId)
        {
            cout << "[R]: EXISTS" << endl;
            return;
        }
    }

    // create a new voter
    Voter *v = new Voter(voterId, 0);
    voters.push_back(v);

    cout << "[R]: OK" << endl;

    return;
}

// helper funtion to generate unique magic number
int generateUniqueInt()
{
    int num = rand() % 100000 + 1;
    //check whether the num is in this vector, while contains, keep creating new number

    while (find(generatedValues.begin(), generatedValues.end(), num) != generatedValues.end())
    {
        num = rand() % 100000 + 1;
    }

    return num;
}

void vote_for(string name, int voterId)
{
    cout << "[C]: vote_for " << name << " " << voterId << endl;

    if (!isOngoing || voterId > 9999 || voterId < 1000)
    {
        cout << "[R]: ERROR" << endl;
        return;
    }

    // check if voter exists, voterId innvalid: NOTAVOTER
    bool canVote = false;

    for (int i = 0; i < (int)voters.size(); i++)
    {
        if (voters[i]->getId() == voterId)
        {
            canVote = true; // voter exist
        }
    }

    if (!canVote)
    {
        cout << "[R]: NOTAVOTER" << endl;
        return;
    }

    // check if voter already voted, already voted: ALREADYVOTED
    for (int i = 0; i < (int)voters.size(); i++)
    {
        if (voters[i]->getId() == voterId)
        {
            if (voters[i]->getMagicNum() != 0)
            { // magic number != 0, voted
                cout << "[R]: ALREADYVOTED" << endl;
                return;
            }
        }
    }

    // check if candidate exists
    // candidate exists in system: EXISTS
    canVote = false;

    for (int i = 0; i < (int)candidates.size(); i++)
    {
        if (candidates[i]->getName() == name)
        {
            cout << "[R]: EXISTS" << endl;
            // increment vote count by 1
            candidates[i]->addVotes();
            // update highest_vote
            highest_vote = max(highest_vote, candidates[i]->getVotes());
            canVote = true;
            break;
        }
    }

    // candidate not exists in system: NEW
    if (!canVote)
    {
        Candidate *c = new Candidate(name, 1); // set vote to 1
        candidates.push_back(c);
        // update highest_vote
        highest_vote = max(highest_vote, c->getVotes());
        cout << "[R]: NEW" << endl;
    }

    // finish vote and return magic number
    int magicNumber;

    // initialize random seed and generate random number from 1 to 100000
    srand(time(NULL));
    magicNumber = generateUniqueInt();

    // print magicNumber of this voter
    for (int i = 0; i < (int)voters.size(); i++)
    {
        if (voters[i]->getId() == voterId)
        {
            voters[i]->setMagicNum(magicNumber);
            cout << magicNumber << endl;
            break;
        }
    }

    return;
}

void check_registration_status(int voterId)
{
    cout << "[C]: check_registration_status " << voterId << endl;

    // check isOngoing
    if (!isOngoing)
    {
        cout << "[R]: ERROR" << endl;
        return;
    }

    // id out of range
    if (voterId > 9999 || voterId < 1000)
    {
        cout << "[R]: INVALID" << endl;
        return;
    }

    // check if voter exists
    for (int i = 0; i < (int)voters.size(); i++)
    {
        if (voters[i]->getId() == voterId)
        {
            // voter exist
            cout << "[R]: EXISTS" << endl;
            return;
        }
        // voter not exist
        cout << "[R]: UNREGISTERED" << endl;
        return;
    }
}

void check_voter_status(int voterId, int magicNum)
{
    cout << "[C]: check_voter_status " << voterId << " " << magicNum << endl;

    if (!isOngoing || voterId > 9999 || voterId < 1000)
    {
        cout << "[R]: ERROR" << endl;
        return;
    }

    for (int i = 0; i < (int)voters.size(); i++)
    {
        if (voters[i]->getId() == voterId)
        {
            // wrong magicNum, print “UNAUTHORIZED”
            if (voters[i]->getMagicNum() != magicNum)
            {
                cout << "[R]: UNAUTHORIZED" << endl;
                return;
            }
            else
            {
                // “ALREADYVOTED” if voter has voted
                cout << "[R]: ALREADYVOTED" << endl;
                return;
            }
        }
    }

    // voter not in voters
    cout << "[R]: CHECKSTATUS" << endl;

    return;
}

// ANY USER //
void list_candidtates()
{
    cout << "[C]: list_candidtates" << endl;

    if (isOngoing)
    {
        cout << "[R]: ";

        for (auto it = candidates.begin(); it < candidates.end(); it++)
        {
            cout << (*it)->getName() << endl;
        }
    }
    else
    {
        cout << "[R]: " << endl;
    }
}

void vote_count(string name)
{
    cout << "[C]: vote_count " << name << endl;
    cout << "[R]: ";

    if (isOngoing)
    {
        for (auto it = candidates.begin(); it < candidates.end(); it++)
        {
            if ((*it)->getName() == name)
            {
                cout << (*it)->getVotes() << endl;
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
    cout << "[C]: view_result" << endl;
    view_result_helper();
    return;
}

// optional argument: -r
void recover()
{
    ifstream myfile("backup.txt"); // hard code to read "backup.txt"

    if (myfile.is_open())
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
