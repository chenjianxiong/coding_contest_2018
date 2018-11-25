#include <fstream>
#include <sstream>
#include <iostream>
#include <string.h>
#include <vector>
#include <algorithm> 



#define DEBUG 0

struct User {
    int userIndex;
    int count;    
    int userRequireBitSet;
    std::vector<int> requiredSet;
    User(int u = 0, int c = 0) {
        userIndex = u;
        count = c;
        userRequireBitSet = 0;
    };
           
    bool operator < (const User& user) const
    {
        return ( count < user.count);
    }           
};



class TestLineAllocation
{
    private:    
        static const int MAX_USER_NUM = 20;
        static const int MAX_TESTLINE_NUM = 20;
        int selectedTestLineSet;
        //int totalTestLineNum;
        int totalUserNum;
        int userRequireSet[MAX_USER_NUM][MAX_TESTLINE_NUM];
        int userRequireBitSet[MAX_USER_NUM];
        int userRequireCount[MAX_USER_NUM];
        unsigned long totalAllocationNum;
        std::vector<User> usersSortedByTestLineCount;
    private:        
        int lookup(int selectedTestLine, int nextUserNum);
    public:
        TestLineAllocation();
        void optimizeData(); 
        void allocate();
        bool readConfiguration(const char * fileName);
};

TestLineAllocation::TestLineAllocation()
{
    totalAllocationNum = 1;
    selectedTestLineSet = 0;
    memset(userRequireSet,    0, sizeof(userRequireSet));
    memset(userRequireBitSet, 0, sizeof(userRequireBitSet));
    memset(userRequireCount,    0, sizeof(userRequireCount));
}

int TestLineAllocation::lookup(int selectedTestLine, int userIterator)
{
    unsigned long totalCount = 1;
    int nextUserNum = usersSortedByTestLineCount[userIterator].userIndex;
    int currentUserBitSet = userRequireBitSet[nextUserNum];
    int bits = 0;
    int new_possibilities = 0;
    bits = ~(1 << selectedTestLine);
    new_possibilities = bits & currentUserBitSet;
    if( !new_possibilities ) {
        return 0;
    }
    else {
        if(nextUserNum == usersSortedByTestLineCount[totalUserNum - 1].userIndex) {            
            // Reach the last user. Clear selectedTestLineSet.
            //selectedTestLineSet = 0;
            int count = 0;
            for(int i = 0; i < userRequireCount[nextUserNum]; i++) {                
                bool canSelect = !((1 << userRequireSet[nextUserNum][i]) & selectedTestLineSet);
                if(  canSelect ) {
                    count++;
#if DEBUG                    
                    std::cout << "Found: ";
                    int j = 0;
                    while(j < 32) {
                        if( (selectedTestLineSet & (1 << j)))
                            std::cout << j + 1 << " ";
                        j++;
                    }
                    std::cout << userRequireSet[nextUserNum][i] + 1;
                    std::cout << "\n";
#endif                    
                }
            }
            //printf("To the last User. Return lookup: userNum:%d, selectedTestLineSet:%08X with count:%d\n", nextUserNum, selectedTestLineSet, count);
            return count;
        }        
        else {            
            int count = 0;
            for(int i = 0; i < userRequireCount[nextUserNum]; i++) {
                int testLine = userRequireSet[nextUserNum][i];
                bool canSelect = !((1 << testLine) & selectedTestLineSet);                
                if( canSelect ) {                    
                    selectedTestLineSet |= (1 << testLine);
                    count += lookup(testLine, userIterator + 1);
                    selectedTestLineSet &= ~(1 << testLine);                 
                }                
            }
            totalCount *= count;
            //printf("Return lookup: nextUserNum:%d, selectedTestLineSet:%08X with count:%d, totalAllocationNum:%lu\n", nextUserNum, selectedTestLineSet, count, totalCount);
            return count;
        }
    }
}

void TestLineAllocation::optimizeData() 
{
    std::sort(usersSortedByTestLineCount.begin(), usersSortedByTestLineCount.end()); 
#if DEBUG    
    for(const auto& user: usersSortedByTestLineCount) {
        int index = user.userIndex;
        std::cout << "userIndex:" << user.userIndex << "\tCount:" << user.count << "\tuserRequireCount:" << userRequireCount[index] << " Require:";
        for(int i = 0; i < userRequireCount[index]; i++)
            std::cout << userRequireSet[index][i] << "  ";
        std::cout << "\n";
    }
#endif    
}

void TestLineAllocation::allocate()
{
    totalAllocationNum = lookup(MAX_TESTLINE_NUM + 1, 0);
    std::cout << totalAllocationNum << "\n";
}

bool TestLineAllocation::readConfiguration(const char * fileName) 
{
    std::ifstream fs(fileName);
    if ( fs ) {
        std::string line,x;
        getline(fs, line);
        std::istringstream ss( line );           
        getline( ss, x, ',' );
        //totalTestLineNum = std::stoi(x);
        (void)std::stoi(x);
        getline( ss, x, ',' );
        totalUserNum = std::stoi(x);
        int uIndex = 0;
        while(getline(fs, line) && !line.empty()) {
            std::istringstream ss( line );
            int bitmap = 0;
            int count = 0;
            int testLineIndex = 0;
            while (getline( ss, x, ',' ))         
           {               
               testLineIndex = std::stoi(x) - 1; // convert data from 1-based to 0-based index.
               bitmap |= (1 << testLineIndex);               
               userRequireSet[uIndex][count++] = testLineIndex;               
           }
           userRequireBitSet[uIndex] = bitmap;
           usersSortedByTestLineCount.push_back(User(uIndex, count));
           userRequireCount[uIndex++] = count;          
        }
            
        return true;
    }
    else {
        std::cout << "Unable to open file." << '\n';
        return false;
    }

}

int main(int argc, char **argv)
{
    if( argc != 2 ) {
        std::cout << "Incorrect parameters. Please give your file name." << '\n';
        return -1;
    }   
    TestLineAllocation allocation;
    if( allocation.readConfiguration(argv[1]) ) {
        allocation.optimizeData();
        allocation.allocate();
        return 0;
    }
    else {
        std::cout << "Incorrect Data File.";
        return -1;
    }
}
