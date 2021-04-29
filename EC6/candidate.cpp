#include "candidate.h"
#include <iostream>
#include <unistd.h>
#include <string>

using namespace std;

Candidate::Candidate()
{
}

Candidate::Candidate(string &name, int votes)
{
    this->name = name;
    this->votes = votes;
}

Candidate::~Candidate()
{
}

string Candidate::getName()
{
    return this->name;
}

int Candidate::getVotes()
{
    return this->votes;
}

void Candidate::addVotes()
{
    this->votes++;
}
