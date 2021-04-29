#include <string>
#include <vector>

class Voter
{
private:
    int id;
    int magicNum;

public:
    Voter();

    Voter(int voterId, int magic);

    ~Voter();

    int getId();

    int getMagicNum();

    void setMagicNum(int magic);

    // cereal
    template <class Archive>
    void serialize(Archive &archive)
    {
        // serialize things by passing them to the archive
        archive(id, magicNum);
    }
};
