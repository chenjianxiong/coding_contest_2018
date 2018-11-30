#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <set>
#include <unordered_map>
#include <algorithm>
#include <string.h>

#define DEBUG 0


class EquationPuzzle
{
    private:
        unsigned char DIGITS[10];
        std::string inputEquation;
        char* firstMultiplier;
        char* secondMultiplier;
        char* product;        
        unsigned char alphabetMap[255];
        char alphabetNum;
        unsigned char firstLetters[3]; // first letter of multipler and product.
        unsigned char lastLetters[3]; // last letter of multipler and product.
        int firstMultiplierLength;
        int secondMultiplierLength;
        int productLength;
        char lastDigitOfProduct[10][10];        
        int word2Num(const char* s, int& N);
        void buildAlphabetMap();
        int getData(const char* , char* );
		void doFullPermutation();
    public:
        EquationPuzzle();
        ~EquationPuzzle();
        bool readInputData(char * fileName);
        void lookupNumberSet();
};


EquationPuzzle::EquationPuzzle():alphabetNum(0),firstMultiplierLength(0),
        secondMultiplierLength(0), productLength(0)
        
{
    memset(alphabetMap, 0xFF, sizeof(alphabetMap));
    memset(DIGITS, 0, sizeof(DIGITS));
    memset(firstLetters, 0, sizeof(firstLetters));
    memset(lastLetters, 0, sizeof(lastLetters));
    firstMultiplier = NULL;
    secondMultiplier = NULL;
    product = NULL;
        
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
        ret = ret* 10 + DIGITS[alphabetMap[(unsigned char)inputStr[i]]];
    }
    return ret;
}

void EquationPuzzle::buildAlphabetMap()
{
    for(char& c: inputEquation) {
        if( alphabetMap[(unsigned char)c] < 10U) //the alphabet had been added before.
            continue;
        
        if(( c != ' ') && (c != '*') && (c != '=')) {
            alphabetMap[(unsigned char)c] = alphabetNum;
            alphabetNum++;
        }
        else {
            alphabetMap[(unsigned char)c] = c;
        }
    }
    firstMultiplierLength = strlen(firstMultiplier);
    secondMultiplierLength = strlen(secondMultiplier);
    productLength = strlen(product);  
    
    firstLetters[0] = alphabetMap[(unsigned char)firstMultiplier[0]];
    firstLetters[1] = alphabetMap[(unsigned char)secondMultiplier[0]];
    firstLetters[2] = alphabetMap[(unsigned char)product[0]];
    
    lastLetters[0] = alphabetMap[(unsigned char)firstMultiplier[firstMultiplierLength - 1]];
    lastLetters[1] = alphabetMap[(unsigned char)secondMultiplier[secondMultiplierLength - 1]];
    lastLetters[2] = alphabetMap[(unsigned char)product[productLength - 1]];
    
}

void EquationPuzzle::doFullPermutation()
{
	int m1 = 0;
	int m2 = 0;
	int p = 0;
	unsigned char value = 0;
	do {
		//skip in case of the first letters is zero.
		if ((DIGITS[firstLetters[0]] == 0) ||
			(DIGITS[firstLetters[1]] == 0) ||
			(DIGITS[firstLetters[2]] == 0)) {
			continue;
		}

		if (lastDigitOfProduct[DIGITS[lastLetters[0]]] \
			[DIGITS[lastLetters[1]]] != DIGITS[lastLetters[2]]) {
			continue;
		}

		m1 = word2Num(firstMultiplier, firstMultiplierLength);
		m2 = word2Num(secondMultiplier, secondMultiplierLength);

		p = word2Num(product, productLength);



		if (p == m1 * m2) {
			for (char& c : inputEquation) {
				value = alphabetMap[(unsigned char)c];
				if (value < 10U)
					value = DIGITS[value] + '0';
				std::cout << value;
			}
			std::cout << '\n';
            Log().Get(logINFO) << "My Message";
		}
	} while (std::next_permutation(DIGITS, DIGITS + alphabetNum));

}

void EquationPuzzle::lookupNumberSet()
{
    buildAlphabetMap();
  

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
		doFullPermutation();
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
        int length = 0;
        const char * pChar = inputEquation.c_str();
        length = getData(pChar, firstMultiplier);
        pChar += length;
        length = getData(pChar, secondMultiplier);
        pChar += length;
        length = getData(pChar, product);
        
   
        return true;
    }
    else {
        return false;
    }

}

int EquationPuzzle::getData(const char* pSrc, char* pDest) 
{
    bool hasAlphabet = false;
    int length = 0;
    do {
        length++;
        if( isalnum( *pSrc ) ) {
            hasAlphabet = true;
            *pDest++ = *pSrc++;
        }
        else {
            if( hasAlphabet ) {
                *pDest = '\0';
                break;
            }
            else {
                pSrc++;
            }
        }
    } while(*pSrc != '\n');
    return length;
}

int main(int argc, char **argv)
{
    if( argc != 2 ) {
        return -1;
    }   
    
    EquationPuzzle puzzle;
        if( puzzle.readInputData(argv[1]) ) {
            puzzle.lookupNumberSet();        
            return 0;
        }
        else {
            return -1;
        }
}
