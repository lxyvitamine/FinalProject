#include "candidate.h"
#include <iostream>
#include <unistd.h>
#include <string>

Candidate::Candidate(std::string &name)
{
  this->name = name;
  votes = 0;
}

Candidate::~Candidate()
{
}

std::string Candidate::getName()
{
  return this->name;
}

int Candidate::getVotes()
{
  return this->votes;
}

void Candidate::setVotes(int votes)
{
  this->votes = votes;
}
