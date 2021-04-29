#include <string>
#include <vector>

class Voter
{
private:
    int id;
    int magicNum;

public:
    Voter(int voterId, int magic);

    ~Voter();

    int getId();

    int getMagicNum();

    void setMagicNum(int magic);
};