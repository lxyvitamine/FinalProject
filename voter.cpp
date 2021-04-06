#include "voter.h"

Voter::Voter(int voterId)
{
    id = voterId;
    magicNum = 0;
}

Voter::~Voter()
{
}


int Voter::getId()
{
    return this->id;
}
void Voter::setId(int id)
{
    this->id = id;
}

int Voter::getMagicNum()
{
    return this->magicNum;
}
void Voter::setMagicNum(int magic)
{
    this->magicNum = magic;
}