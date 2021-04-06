#include <string>
#include <vector>

using namespace std;
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