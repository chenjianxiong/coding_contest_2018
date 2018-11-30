
<!-- saved from url=(0078)http://10.182.198.199/problem4-20181112000000-20181125235959/jianywu/code.html -->
<html><head><meta http-equiv="Content-Type" content="text/html; charset=windows-1252"><title>Solution.cpp</title>
<script src="./Solution_1_jianywu_files/prettify.js.download" type="text/javascript"></script>
<link rel="stylesheet" type="text/css" href="./Solution_1_jianywu_files/prettify.css">
</head><body onload="prettyPrint()">
<h2>Solution.cpp</h2><div><pre class="prettyprint linenums">#include &lt;cstring&gt;
#include &lt;string&gt;
#include &lt;iostream&gt;
#include &lt;fstream&gt;
#include &lt;sstream&gt;
#include &lt;vector&gt;
#include &lt;algorithm&gt;
#include &lt;list&gt;

#define INPUT_PARAM_COUNT    (2)
#define MIN_ITEM_NUM       (0)
#define MIN_PROFIT        (0)
#define MAX_ITEM_NUM       (1001)
#define MAX_VALUE         (1000000)

namespace
{
    std::vector&lt;std::string&gt; linesOfText; 
    std::list&lt;int&gt; gDependenceList[MAX_ITEM_NUM];
    int gItemNum = 0;
    int gProfit  = 0;
    int gItemValue[MAX_ITEM_NUM] = {0};
    int gItemUsed[MAX_ITEM_NUM] = {0};
    int gItemAddedValue[MAX_ITEM_NUM] = {0};
    int gItemUsedBak[MAX_ITEM_NUM] = {0};
    int gItemAddedValueBak[MAX_ITEM_NUM] = {0};
    
    typedef std::vector&lt;std::string&gt;::size_type lineNo;
    
    int getItemAddedValue(int itemNum)
    {
        for (std::list&lt;int&gt;::iterator it = gDependenceList[itemNum].begin();
                                      it != gDependenceList[itemNum].end();
                                      ++it)
        {
            if (gItemUsed[*it] == 0)
            {
                gItemUsed[*it] = 1;
                gItemAddedValue[itemNum] += getItemAddedValue(*it);
            }
        }
        return gItemAddedValue[itemNum];
    }
    
    void selectItem()
    {
        for (int i = 1; i &lt;= gItemNum; i++)
        {
            if ((!gItemUsed[i]) &amp;&amp; (gItemValue[i] &gt; 0))
            {
                int addedVal = 0;
                std::memcpy(&amp;gItemUsedBak[1],
                        &amp;gItemUsed[1],
                        sizeof(gItemUsed[0])*gItemNum);
                std::memcpy(&amp;gItemAddedValueBak[1],
                        &amp;gItemAddedValue[1],
                        sizeof(gItemAddedValue[0])*gItemNum);

                gItemUsed[i] = 1;
                addedVal = getItemAddedValue(i);
                if (addedVal &gt; 0)
                {
                    gProfit += addedVal;
                }
                else
                {
                    std::memcpy(&amp;gItemUsed[1],
                            &amp;gItemUsedBak[1],
                            sizeof(gItemUsed[1])*gItemNum);
                    std::memcpy(&amp;gItemAddedValue[1],
                            &amp;gItemAddedValueBak[1],
                            sizeof(gItemAddedValue[1])*gItemNum);
                }
            }
        }
    }
    
    void printResults()
    {
        int i;
        int profit = 0;
    
        for (i = 1; i &lt;= gItemNum; i++)
        {
            if (gItemUsed[i] == 1)
            {
                profit += gItemValue[i];
            }
        }
        std::fprintf(stdout, "%d\n", gProfit);
    
        for (i = 1; i &lt;= gItemNum; i++)
        {
            if (gItemUsed[i] == 1)
            {
                std::fprintf(stdout, "%d\n", i);
            }
        }
    }

    void getData()
    {
        for (lineNo lineNum = 0;
             lineNum != linesOfText.size();
             ++lineNum)
        {
            std::istringstream line(linesOfText[lineNum]);
            int itemDependNum;
            int itemNum = lineNum + 1;
            line &gt;&gt; gItemValue[itemNum];
            gItemAddedValue[itemNum] = gItemValue[itemNum];

            if (gItemValue[itemNum] &lt; -MAX_VALUE)
            {
                gItemValue[itemNum] = -MAX_VALUE;
            }
            if (gItemValue[itemNum] &gt; MAX_VALUE)
            {
                gItemValue[itemNum] = MAX_VALUE;
            }
            while (line &gt;&gt; itemDependNum)
            {
                gDependenceList[itemNum].push_back(itemDependNum);
            }
        }
    }
}

int main(int argc, char* argv[]) 
{ 
    std::ifstream inputFile;
    std::string line;

    if (INPUT_PARAM_COUNT != argc)
    {
        std::fprintf(stdout, "ERROR: Provide File name as input command line.\n");
        return (EXIT_FAILURE);
    }
    
    inputFile.open(argv[1]);
    if (inputFile)
    {
        getline(inputFile, line);
        gItemNum = stoi(line);
        if (gItemNum == MIN_ITEM_NUM)
        {
            std::fprintf(stdout, "%d\n", MIN_PROFIT);
            return (EXIT_SUCCESS);
        }

        if (gItemNum &gt; MAX_ITEM_NUM)
        {
            gItemNum = MAX_ITEM_NUM;
        }
 
        while (getline(inputFile, line))
        {
            linesOfText.push_back(line);
        }
        getData();
        inputFile.close();
    }
    
    selectItem();
    printResults();
  
    return (EXIT_SUCCESS);
}</pre></div>
</body></html>