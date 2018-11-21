#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <set>
#include <unordered_map>
#include <regex>
#include <algorithm>

#define DEBUG 0


class EquationPuzzle
{
    private:
        char DIGITS[10];
        std::string inputEquation;
        char* firstMultiplier;
        char* secondMultiplier;
        char* product;        
        char alphabetMap[255];
        char alphabetNum;
        char firstLetters[3]; // first letter of multipler and product.
        char lastLetters[3]; // last letter of multipler and product.
        int firstMultiplierLength;
        int secondMultiplierLength;
        int productLength;
        char lastDigitOfProduct[10][10];        
    private:
        int word2Num(const char* s, int& N);
        void buildAlphabetMap();
    public:
        EquationPuzzle();
        ~EquationPuzzle();
        bool readInputData(char * fileName);
        void lookupNumberSet();
};


EquationPuzzle::EquationPuzzle():alphabetNum(0),firstMultiplierLength(0),
        secondMultiplierLength(0), productLength(0),
        firstMultiplier(NULL), secondMultiplier(NULL), product(NULL)
{
    memset(alphabetMap, 0xFF, sizeof(alphabetMap));
    for(int i = 0; i < 10; i++)
        for(int j = 0; j < 10; j++)
            lastDigitOfProduct[i][j] = (i * j) % 10;
}

EquationPuzzle::~EquationPuzzle()
{
    free(firstMultiplier);
    free(secondMultiplier); 
    free(product);
}

int EquationPuzzle::word2Num(const char * inputStr, int& N)
{
    int ret = 0;
    for(int i = 0; i < N; i++) {
        ret = ret* 10 + DIGITS[alphabetMap[inputStr[i]]];
    }
    return ret;
}

void EquationPuzzle::buildAlphabetMap()
{
    for(char c: inputEquation) {
        if( alphabetMap[c] < 10U) //the alphabet had been added before.
            continue;
        
        if(( c != ' ') && (c != '*') && (c != '=')) {
            alphabetMap[c] = alphabetNum;
            alphabetNum++;
        }
        else {
            alphabetMap[c] = c;
        }
    }
    firstMultiplierLength = strlen(firstMultiplier);
    secondMultiplierLength = strlen(secondMultiplier);
    productLength = strlen(product);  
    
    firstLetters[0] = alphabetMap[firstMultiplier[0]];
    firstLetters[1] = alphabetMap[secondMultiplier[0]];
    firstLetters[2] = alphabetMap[product[0]];
    
    lastLetters[0] = alphabetMap[firstMultiplier[firstMultiplierLength - 1]];
    lastLetters[1] = alphabetMap[secondMultiplier[secondMultiplierLength - 1]];
    lastLetters[2] = alphabetMap[product[productLength - 1]];
    

#if DEBUG    
    for(char c : inputEquation) {
        printf("%c => %d ", c, alphabetMap[c]);
    }
    printf("alphabet Num:%d\n", alphabetNum);
#endif        
}

void EquationPuzzle::lookupNumberSet()
{
    buildAlphabetMap();
  
    int m1 = 0,m2 = 0,p = 0;
    char value;
    char bitmask[10] = {0};
    memset(bitmask, 1, alphabetNum);
    do {        
        int j = 0;
        for (int i = 0; i < 10; ++i) // [0..N-1] integers
        {
            if (bitmask[i]) { 
                DIGITS[j++] = i;                
            }
        }
        do {
            //skip in case of the first letters is zero.
            if( (DIGITS[firstLetters[0]] == 0) || 
                (DIGITS[firstLetters[1]] == 0) ||
                (DIGITS[firstLetters[2]] == 0)) {
                continue;
            }
                
            if( lastDigitOfProduct[DIGITS[lastLetters[0]]] \
                [DIGITS[lastLetters[1]]] != DIGITS[lastLetters[2]]) {
                continue;
            }
                
            m1 = word2Num(firstMultiplier, firstMultiplierLength);
            m2 = word2Num(secondMultiplier, secondMultiplierLength);
            
            p = word2Num(product, productLength);

#if DEBUG
            std::cout << "LOOP:" << loop << " Number permutation:\n";
            for(int i = 0; i < 10; i++)
                printf("%d ", DIGITS[i]);
            printf("\n");
            std::cout << m1 << " * " << m2 << " = " << p << '\n';        
#endif        
            
            if( p == m1 * m2 ) {
                for(char c: inputEquation) {
                    value = alphabetMap[c];                
                    if(value < 10U)
                        value = DIGITS[value] + '0';
                    std::cout << value;
                }
                std::cout << '\n';
            }
        } while ( std::next_permutation(DIGITS,DIGITS + alphabetNum) );
    } while (std::prev_permutation(bitmask, bitmask + 10));
}


bool EquationPuzzle::readInputData(char * fileName) 
{   
    std::ifstream fs(fileName);
    if ( fs ) {
        getline(fs, inputEquation);
        firstMultiplier = (char *)malloc(inputEquation.size());
        secondMultiplier = (char *)malloc(inputEquation.size());
        product = (char *)malloc(inputEquation.size());
        std::smatch smatch;
        static const std::regex regex("^\\s*([A-Za-z0-9]+)\\s*\\*\\s*([A-Za-z0-9]+)\\s*=\\s*([A-Za-z0-9]+)\\s*$");
        if(std::regex_search(inputEquation, smatch, regex)  && smatch.size() == 4 ) {            
            strcpy(firstMultiplier, smatch.str(1).c_str());
            strcpy(secondMultiplier, smatch.str(2).c_str());
            strcpy(product, smatch.str(3).c_str());
        }
        else {
            return false;
        }
        
#if DEBUG
        std::cout << firstMultiplier << " * " << secondMultiplier << " = " << product << '\n';
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
    
    EquationPuzzle puzzle;
    try {
        if( puzzle.readInputData(argv[1]) ) {
            puzzle.lookupNumberSet();        
            return 0;
        }
        else {
            std::cout << "Incorrect Data File.";
            return -1;
        }
    }
    catch (const std::exception& e)
    {
        return -1;        
    }
}