#include <string>
#include <vector>

using namespace std;
class Voter
{
private:
    int id;
    int magicNum;

public:
    Voter(int voterId);
    ~Voter();
    
    
    int getId();
    void setId(int id);

    int getMagicNum();
    void setMagicNum(int magic);
};