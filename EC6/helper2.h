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

using namespace std;

// GLOBAL VARIABLE //
// dealing with threads
extern vector<pthread_t> clientThreads;
extern int clientIndex;
// extern const int MAX_LIMIT;
// extern pthread_t threads[];
// extern int running_thread;
// password
extern string password;
extern vector<Candidate *> candidates;
extern vector<Voter *> voters;
extern unordered_set<int> generatedMagicNumbers;
// flag
extern bool isOngoing;
extern int highest_vote;
extern bool changePassword;
extern int isShutdown;
// main() & mutex
extern string userCmds[];

extern pthread_mutex_t sdLock;
extern pthread_mutex_t isShutdownLock;
extern pthread_mutex_t parseUserinputLock;
extern pthread_mutex_t sendToClientLock;
extern pthread_mutex_t candidatesLock;
extern pthread_mutex_t votersLock;
// extern pthread_mutex_t inputLock;
// extern pthread_mutex_t magicNumLock;
extern int sd;
extern const int MAX_MESSAGE;
extern char sendToClient[];
extern string errorMsg;

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
