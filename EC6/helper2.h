#include "candidate.h"
#include "voter.h"
#include <iostream>
#include <vector>
#include <unordered_set>
#include <string>
#include <cstring>
#include <stdio.h>
#include <unistd.h>
#include <fstream>
#include <stdlib.h>
#include <time.h>
#include <algorithm>

#include <cereal/types/string.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/memory.hpp>

using namespace std;

class helper2
{
public:
    helper2();
    ~helper2();

    // GLOBAL VARIABLE //
    // dealing with threads
    vector<pthread_t> clientThreads;
    // password
    string password = "cit595";
    vector<shared_ptr<Candidate>> candidates;
    vector<shared_ptr<Voter>> voters;
    unordered_set<int> generatedMagicNumbers;
    // flag
    bool isOngoing = false;
    int highest_vote = 0;
    bool changePassword = false;
    int isShutdown = false;
    // main() & mutex
    pthread_mutex_t sdLock;
    pthread_mutex_t isShutdownLock;
    pthread_mutex_t parseUserinputLock;
    pthread_mutex_t sendToClientLock;
    pthread_mutex_t candidatesLock;
    pthread_mutex_t votersLock;

    int sd;
    static const int MAX_MESSAGE = 1024;
    char sendToClient[MAX_MESSAGE];
    string errorMsg = "ERROR";

    // helper function
    string view_result_helper();
    bool isNumber(const string &str);
    bool isValidPortNumber(const string &str);
    int generateUniqueMagicNumber();

    // PARSER //
    vector<string> parseCmd(const string &raw_line, const string &delim);

    //ADMIN
    string start_election(string cmdpassword);
    string end_election(string cmdpassword);
    string add_candidate(string cmdpassword, string candiName);
    string shutdown(string cmdpassword);

    // VOTER //
    string add_voter(int voterId);
    string vote_for(string name, int voterId);
    string check_registration_status(int voterId);
    string check_voter_status(int voterId, int magicNum);

    // ANY USER //
    string list_candidtates();
    string vote_count(string name);
    string view_result();

    // optional argument: -r
    void recover();

    // cereal
    template <class Archive>
    void serialize(Archive &archive)
    {
        // serialize things by passing them to the archive
        archive(isOngoing, password, highest_vote, candidates, voters);
    }
};
