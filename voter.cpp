#include "voter.h"

Voter::Voter(int voterId, int magic)
{
    id = voterId;
    magicNum = magic;
}


Voter::~Voter()
{
}


int Voter::getId()
{
    return this->id;
}


int Voter::getMagicNum()
{
    return this->magicNum;
}
void Voter::setMagicNum(int magic)
{
    this->magicNum = magic;
}