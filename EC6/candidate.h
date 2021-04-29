#include <iostream>
#include <string>

using namespace std;

class Candidate
{
private:
    string name;
    int votes;

public:
    Candidate();

    Candidate(string &name, int votes);

    ~Candidate();

    string getName();

    int getVotes();

    void addVotes();

    // cereal
    template <class Archive>
    void serialize(Archive &archive)
    {
        // serialize things by passing them to the archive
        archive(name, votes);
    }
};
