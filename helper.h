#include "candidate.h"
#include "voter.h"
#include <iostream>
#include <vector>
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
extern set<int> generatedValues;
// flag
extern bool isOngoing;
extern int highest_vote;
extern bool changePassword;
// main() & mutex
extern vector<string> userCmds;
extern pthread_mutex_t parseLock;
// extern pthread_mutex_t userCmdsLock;
// extern pthread_mutex_t candidatesLock;
// extern pthread_mutex_t votersLock;
// extern pthread_mutex_t methodLock;
// extern pthread_mutex_t inputLock;
// extern pthread_mutex_t magicNumLock;

// helper function
void view_result_helper();
bool isNumber(const string &str);
int generateUniqueInt();

// PARSER //
std::vector<std::string> parseCmd(const std::string &raw_line, const std::string &delim);

//ADMIN
void start_election(string cmdpassword);
void end_election(string cmdpassword);
void add_candidate(string cmdpassword, string candiName);
void shutdown(string cmdpassword);

// VOTER //
void add_voter(int voterId);
void vote_for(string name, int voterId);
void check_registration_status(int voterId);
void check_voter_status(int voterId, int magicNum);

// ANY USER //
void list_candidtates();
void vote_count(string name);
void view_result();

// optional argument: -r
void recover();
