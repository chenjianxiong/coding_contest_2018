#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

#define DEBUG 0

static const int MAX_ROW = 100;
static const int MAX_COL = 100;
void handleInputData(char * fileName) 
{
    try {        
        int lastRow[MAX_COL] = {0};
        int currentRow[MAX_COL] = {0};
        std::ifstream fs(fileName);
        if ( fs ) {
            char c;
            int data;
            int M_columnNumber = 0;
            int N_rowNumber = 0;
            
            fs >> M_columnNumber >> c >> N_rowNumber;
            int col_index = 0;
            
            int lastData = 0;
            int bigData = 0;
            int newData;
            int * pTemp = nullptr;
            int * pCurrentRow = currentRow;
            int * pLastRow = lastRow;
            while(fs >> data) {
                fs.ignore(1, ',');

               
                bigData = ( lastData > pLastRow[col_index] ) ? lastData : pLastRow[col_index];
                newData = bigData + data;
                pCurrentRow[col_index] = newData;
#if DEBUG                  
                std::cout << "Col:" << col_index << "\tdata:" << data << "\tnew Data:" << newData  << "\t Big data:" << bigData << "\n";
#endif                
                if( ++col_index == M_columnNumber) {
#if DEBUG                    
                    std::cout << "Last Row:\t" << pLastRow << "\t";
                    for( int i = 0; i < M_columnNumber; i++)
                        std::cout << pLastRow[i] << "\t";
                    std::cout << "\n";
                    std::cout << "Current Row:\t" << pCurrentRow << "\t";
                    for( int  i = 0; i < M_columnNumber; i++)
                        std::cout << pCurrentRow[i] << "\t";
                    std::cout << "\n";
#endif                    
                    col_index = 0;
                    lastData = 0;
                    pTemp = pLastRow;
                    pLastRow = pCurrentRow;
                    pCurrentRow = pTemp;                    
                    
#if DEBUG                    
                    std::cout << "New Last Row:\t" << pLastRow << "\t";
                    for( int i = 0; i < M_columnNumber; i++)
                        std::cout << pLastRow[i] << "\t";
                    std::cout << "\n";
#endif                    
                    
                }
                else {
                    lastData = newData;
                }
            }
            std::cout << newData << std::endl; 
        }            
    }
    catch (const std::exception& e)
    {
        std::cout << "Preocessing data exception." << std::endl;        
    }


}

int main(int argc, char **argv)
{
    if( argc != 2 ) {
        std::cout << "Incorrect parameters. Please give your file name." << '\n';
        return -1;
    }   
    
    handleInputData(argv[1]);
    return 0;
}