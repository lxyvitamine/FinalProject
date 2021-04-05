#include "candidate.h"


vector<int> tids;

//candidate: name / real(fake) / votes
//voters: id / magicNum /

//vector<candidate>
//vector<voters>

map<candidate, num_votes>;
map<Fakecandidate, num_votes>; // if not real candidate, negative
map<voterId, List<magicNum>>;
flag ongoing;

//  ADMIN //
start_election(String password){
    //[c]: start_election password
    //[R]: OK / EXISTS(check if map is empty) / ERROR(incorrect password)
    //change flag
}

end_election(String password){
    //check flag
    //[c]: 
    //
    //join      
    for(int i ...){
            
    }
    //[R]: print map and winner
    //if there is a draw
    // if there is no winner
    //ERROR
}

add_candidate(String password, String candidate){
    //check flag
    //[c]: 
    //[R]: OK / EXISTS /ERROR
    
    //add_candiate vote --> 0
}

shutdown(String password){
    //check flag
    //[c]: 
    //[R]: OK
    //flag == true:     
    //join      
    for(int i ...){
            
    }
    
    //print OK
    
    //save date
    //write into backup.txt
    
}

// VOTE VOTER//

add_voter(int voterId){
    //check flag
    //[c]
    //[R] check id range and check if exists
}


vote_for(String name, int voterId){
    //check flag
    
    //if not voted, add votes
    //If the named candidate is not already present in the system, add the candidate to the system with a vote count of 1
    //if the candidate is already present in the system, increment their vote count by 1
    
    //[R]
    //if candidate exists in system: EXISTS
    //if not: NEW
    //voterId innvalid: NOTAVOTER
    //already vote: ALREADYVOTED
    //ERROR
    
    //returns a random number on a new line EXISTS + magicNum (random)
    
}


check_registration_status(int voterId){
    //check flag
    //
    //[R] EXISTS// INVALID // UNREGISTERED //ERROR
}

check_voter_status(int voterId, int magicNum){
    //check flag
    
    //[R] 
    //“ALREADYVOTED” if voter has voted
    //“CHECKSTATUS” if voter isn’t registered
    //“UNAUTHORIZED” if magicno doesn’t match records 
    //“ERROR” if there is any error both incorrect
}


//ANY USER
list_candidtate(){
    //check flag
    //
    //[R] print a list of candidates
}

vote_count(String name){
     //check flag
    //Returns the vote total for the candidate in string format referred to by name, or “-1” if
    //the candidate isn't in the system or the election hasn’t started yet.
}

view_result(){
    //check flag
    /* Returns the list of candidates with their vote count as well as the winner of the
       election. Return ERROR if the election has not ended or there’s any other error. Refer to the end_election command for return format.*/
     
}

method(string){
    //parse string
    //check the num of args(vector size)
    
    if(size == 2){
        //determine string
    }
    
    
    switch(string)
        
        string == add_vote:
        //call add_vote();
    
        string == xx:
    
       //
}

int main(int argc, char *argv[])
{   
    //1. parse command
    //-r -p -a
    
    String password = "cit 595";
    
    //-r 
    //read text file
    
    
    
    //2. read input
    while(1){
        //read user input
        
        //read a enter key -- make a thread
        
        

        int tid = 0;
        tid++;
        tids.push_back(tid);
        thread(tid, method(), arg(string))
            
    }
    
//     candidate* c = new candidate();
    
//     c->print();
    
    return 0;
}