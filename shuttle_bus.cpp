#include <fstream>
#include <climits>

static const int MAX_ROW      = 100;
static const int MAX_COL      = 100;
static const int LONGEST_DATA = 40;

static const int EOFOFFILE = INT_MAX;
static const int SKIPDATA  = INT_MIN;

int getData(FILE* fp, char inputData[]) 
{
    int i = 0;
    int isDigit = 0;
    char c;
    while((c = getc(fp)) != EOF) {
        if(( c >= '0') && (c <= '9')) {
            isDigit = 1;
            inputData[i++] = c;
        }
        else {
            if(isDigit) {
                inputData[i] = '\0';
                break;
            }
        }
    }
    if( isDigit ) {
        return std::stoi(inputData);
    }
    else {
        if( c == EOF ) {
            return EOFOFFILE;
        }
        else {
            return SKIPDATA;
        }
    }
}

void handleInputData(char * fileName) 
{
    int lastRow[MAX_COL] = {0};
    int currentRow[MAX_COL] = {0};
    char longestData[LONGEST_DATA] = {0};
    FILE * fp = fopen(fileName, "r");        
    if ( fp ) {
        int data = 0;
        int M_columnNumber = 0;
        int col_index = 0;            
        int lastData = 0;
        int bigData = 0;
        int newData = 0;
        int * pTemp = nullptr;
        int * pCurrentRow = currentRow;
        int * pLastRow = lastRow;
        M_columnNumber = getData(fp, longestData);
        (void)getData(fp, longestData);

        while((data = getData(fp, longestData)) != EOFOFFILE) {
            if (data == SKIPDATA)
                continue;
            bigData = ( lastData > pLastRow[col_index] ) ? lastData : pLastRow[col_index];
            newData = bigData + data;
            pCurrentRow[col_index] = newData;
          
            if( ++col_index == M_columnNumber) {
                col_index = 0;
                lastData = 0;
                pTemp = pLastRow;
                pLastRow = pCurrentRow;
                pCurrentRow = pTemp;                    
            }
            else {
                lastData = newData;
            }
        }
        printf("%d\n", newData);
        fclose(fp);
    }
}

int main(int argc, char **argv)
{
    if( argc != 2 ) {
        return -1;
    }    
    handleInputData(argv[1]);
    return 0;
}