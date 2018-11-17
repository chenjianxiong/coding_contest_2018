#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <cstring>
#include <algorithm>


#define DEBUG 0


class TestLineAllocation
{
    private:    
        static const int MAX_USER_NUM = 20;
        static const int MAX_TESTLINE_NUM = 20;
        int TESTLINEINDEX[20];
        int testLineNum;
        int userNum;
        bool userRequireSet[MAX_USER_NUM][MAX_TESTLINE_NUM];
    private:
    public:
        TestLineAllocation();
        void allocate();
        bool readConfiguration(char * fileName);
        void comb(int N, int K);
};

TestLineAllocation::TestLineAllocation() 
{
    memset(userRequireSet, false, sizeof(userRequireSet));
    memset(TESTLINEINDEX, 0, sizeof(TESTLINEINDEX));
}

void TestLineAllocation::comb(int N, int K){
    std::string bitmask(K, 1); // K leading 1's
    bitmask.resize(N, 0); // N-K trailing 0's

    // print integers and permute bitmask
    do {
        for (int i = 0; i < N; ++i) // [0..N-1] integers
        {
            if (bitmask[i]) std::cout << " " << i;
        }
        std::cout << std::endl;
    } while (std::prev_permutation(bitmask.begin(), bitmask.end()));
}


void TestLineAllocation::allocate()
{
    unsigned long goodNumber = 0;
    unsigned long loop = 0;
    char bitmask[20] = {0};
    memset(bitmask, 1, userNum);
    do {        
        int j = 0;
        for (int i = 0; i < testLineNum; ++i) // [0..N-1] integers
        {
            if (bitmask[i]) { 
                TESTLINEINDEX[j++] = i;
            }
        }
#if DEBUG
        std::cout << "Combination:" << "\n";
        for(int i = 0; i < userNum; i++) {
            std::cout << TESTLINEINDEX[i] << " ";
        }
        std::cout << "\n";
#endif        
        do {
            bool matched = true;
            for(int i = 0; i < userNum; i++) {
                if( !userRequireSet[i][TESTLINEINDEX[i]]) {
                    matched = false;
                    break;
                }
            }
            if( matched ) {
#if 0
                for(int i = 0; i < userNum; i++) {
                    std::cout << "Matched user:" << i + 1 << ", test line:" << TESTLINEINDEX[i] + 1 << " ";
                }
                std::cout << '\n';
#endif                
                goodNumber++;
            }
            
#if DEBUG
            std::cout << "LOOP:" << loop << " Number permutation:\n";
            for(int i = 0; i < userNum; i++)
                std::cout << TESTLINEINDEX[i]<< " ";
            std::cout << '\n';
#endif         
        }while ( std::next_permutation(TESTLINEINDEX,TESTLINEINDEX + userNum) );
    } while (std::prev_permutation(bitmask, bitmask + testLineNum));
    
    std::cout << goodNumber << '\n'; 
}

bool TestLineAllocation::readConfiguration(char * fileName) 
{
    std::ifstream fs(fileName);
    if ( fs ) {
        int data;        
        std::string line,x;
        getline(fs, line);
        std::istringstream ss( line );           
        getline( ss, x, ',' );
        testLineNum = std::stoi(x);
        getline( ss, x, ',' );
        userNum = std::stoi(x);
        int uIndex = 0;
        while(getline(fs, line)) {
            std::istringstream ss( line );
            while (getline( ss, x, ',' ))         
           {
               int tIndex = std::stoi(x) - 1; // convert data from 1-based to 0-based index.
               userRequireSet[uIndex][tIndex] = true;
           }
           uIndex++;
        }
            
#if DEBUG
        for(int i = 0; i < userNum; i++) {
            std::cout << i << ": ";
            for(int j = 0; j < testLineNum; j++) {                
                std::cout << userRequireSet[i][j]<< " ";
            }
            std::cout << "\n";
        }
#endif        
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
        allocation.allocate();
        //allocation.comb(10, 5);
        return 0;
    }
    else {
        std::cout << "Incorrect Data File.";
        return -1;
    }
}