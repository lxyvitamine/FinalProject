#include <iostream>
#include <string>

using namespace std;

class Candidate
{
private:
    string name;
    int votes;

public:
    Candidate(string &name, int votes);

    ~Candidate();

    string getName();

    int getVotes();

    void addVotes();
};
