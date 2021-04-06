#include "candidate.h"
#include <iostream>
#include <unistd.h>
#include <string>

Candidate::Candidate(std::string &name, int votes)
{
  this->name = name;
  this->votes = votes;
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

void Candidate::addVotes()
{
  this->votes++;
}
