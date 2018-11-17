#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <queue>

#define DEBUG 0

typedef struct {
    int index;
    int  rowIndex; 
    int  colIndex;
    int  numOfPeople;
} BusStop;

class ShuttleBusCapacity
{
    private:    
        static const int      INIT_CAPACITY = -1;
        static const uint8_t  MAX_M = 100;
        static const uint8_t  MAX_N = 100;
        static const uint16_t MAX_BUS_STOPS = 10000;
        BusStop busStopSet[MAX_BUS_STOPS];
        bool addedBusStopSet[MAX_BUS_STOPS];
        int     maxCapacity[MAX_BUS_STOPS];
        int     busStopNum ;
        int     M_columnNumber;
        int     N_rowNumber;
        std::queue<int> busStopQueue;
    private:
        void updateBusStopQueue(int );
        void updateMaxCapacity(int , int );
    public:
        ShuttleBusCapacity();
        void calculateMaxNumberOfPeople();
        bool readInputData(char * fileName);
};

ShuttleBusCapacity::ShuttleBusCapacity() :addedBusStopSet(), 
       busStopSet(),maxCapacity(),busStopNum(0), M_columnNumber(0), N_rowNumber(0)
{
}

void ShuttleBusCapacity::updateMaxCapacity(int u, int v) {
    if( maxCapacity[u] + busStopSet[v].numOfPeople > maxCapacity[v] )
            maxCapacity[v] = maxCapacity[u] + busStopSet[v].numOfPeople;
}

void ShuttleBusCapacity::updateBusStopQueue(int u)
{
    int v;
    if (busStopSet[u].colIndex < M_columnNumber - 1 ) { // right direction.
        v = u + 1;
        updateMaxCapacity(u, v);
        if( !addedBusStopSet[v]) {
            busStopQueue.push(v);
            addedBusStopSet[v] = true;
        }
    }
    if( busStopSet[u].rowIndex < N_rowNumber - 1 ) { //down direction.
        v = u + M_columnNumber;
        updateMaxCapacity(u, v);
        if( !addedBusStopSet[v]) {
            busStopQueue.push(v);
            addedBusStopSet[v] = true;
        }
    } 
}

void ShuttleBusCapacity::calculateMaxNumberOfPeople()
{

    if ( busStopNum < 1 )
        return;
    
    for(int i = 0; i < busStopNum; i++) {
        maxCapacity[i] = busStopSet[i].numOfPeople;
        addedBusStopSet[i] = false;
    }
     int loop = 0;
    int u = 0;    
    while(1) {
        updateBusStopQueue(u);
        u = busStopQueue.front();
        busStopQueue.pop();

        
#if DEBUG
        for(int i = 0; i < busStopNum; i++) {
            std::cout << (int)maxCapacity[i] << " ";
            if((i+1) % M_columnNumber == 0)
                std::cout << '\n';
        }
        std::cout << '\n';        
#endif
        
        if (u == busStopNum - 1)
            break;
    }
    
        
    std::cout << maxCapacity[busStopNum - 1] << '\n';
}

bool ShuttleBusCapacity::readInputData(char * fileName) 
{
   
    std::ifstream fs(fileName);
    if ( fs ) {
        char c;
        int data;
        fs >> M_columnNumber >> c >> N_rowNumber;
        int row_index = 0;
        int col_index = 0;
        while(fs >> data) {
            fs.ignore(1, ',');
            busStopSet[busStopNum].index    =  busStopNum;
            busStopSet[busStopNum].rowIndex = row_index;
            busStopSet[busStopNum].colIndex = col_index;
            busStopSet[busStopNum].numOfPeople = data;                
            col_index++;
            if (col_index == M_columnNumber) {
                col_index = 0; 
                row_index += 1;
            }
            busStopNum++;
        }
        if (busStopNum != M_columnNumber * N_rowNumber)
            return false;

#if DEBUG
        std::cout << "(M:" << M_columnNumber << " N:" << N_rowNumber << " busStopNum:" << busStopNum << ")" << '\n';        
        int k = 0;    
        std::cout << "   ";
        for(int i = 0; i < M_columnNumber; i++) {//col
            std::cout.width(2);
            std::cout << i << " ";
        }
        std::cout << '\n';        
        for(int i = 0; i < N_rowNumber; i++) {//row        
            //std::cout.width(2);
            std::cout << i << ": ";
            for(int j = 0; j < M_columnNumber; j++) {//column
                std::cout.width(2);
                std::cout << busStopSet[k].numOfPeople << ' ';
                k++;
            }
            std::cout << '\n';
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
    
    ShuttleBusCapacity shuttleBus;
    if( shuttleBus.readInputData(argv[1]) ) {
        shuttleBus.calculateMaxNumberOfPeople();
        return 0;
    }
    else {
        std::cout << "Incorrect Data File.";
        return -1;
    }
}