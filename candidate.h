#include <iostream>
#include <string>
class Candidate
{
private:
    std::string name;
    int votes;

public:
    Candidate(std::string &name, int votes);

    ~Candidate();

    std::string getName();

    int getVotes();
    
    void setVotes(int votes);
};