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

using namespace std;

// GLOBAL VARIABLE //
// dealing with threads
extern const int MAX_LIMIT;
extern pthread_t threads[];
extern int running_thread;
// password
extern string password;
extern vector<Candidate *> candidates;
extern vector<Voter *> voters;
extern unordered_set<int> generatedMagicNumbers;
// flag
extern bool isOngoing;
extern int highest_vote;
extern bool changePassword;
// main() & mutex

extern string userCmds[];

extern pthread_mutex_t parseLock;
extern pthread_mutex_t runningThreadLock;
// extern pthread_mutex_t userCmdsLock;
// extern pthread_mutex_t candidatesLock;
// extern pthread_mutex_t votersLock;
// extern pthread_mutex_t inputLock;
// extern pthread_mutex_t magicNumLock;

// helper function
void view_result_helper();
bool isNumber(const string &str);
int generateUniqueMagicNumber();

// PARSER //
vector<string> parseCmd(const string &raw_line, const string &delim);

//ADMIN
void start_election(string cmdpassword);
void end_election(string cmdpassword);
void add_candidate(string cmdpassword, string candiName);
void shutdown(string cmdpassword);

// VOTER //
string add_voter(int voterId);
string vote_for(string name, int voterId);
string check_registration_status(int voterId);
string check_voter_status(int voterId, int magicNum);

// ANY USER //
string list_candidtates();
void vote_count(string name);
void view_result();

// optional argument: -r
void recover();
