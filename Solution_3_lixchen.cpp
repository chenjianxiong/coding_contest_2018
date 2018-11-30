
<!-- saved from url=(0078)http://10.182.198.199/problem3-20181112000000-20181125235959/lixchen/code.html -->
<html><head><meta http-equiv="Content-Type" content="text/html; charset=windows-1252"><title>Solution.cpp</title>
<script src="./Solution_3_lixchen_files/prettify.js.download" type="text/javascript"></script>
<link rel="stylesheet" type="text/css" href="./Solution_3_lixchen_files/prettify.css">
</head><body onload="prettyPrint()">
<h2>Solution.cpp</h2><div><pre class="prettyprint linenums">#include &lt;fstream&gt;
#include &lt;iostream&gt;
#include &lt;vector&gt;
#include &lt;string&gt;

unsigned long long count(int mask, int i, int lineNum, int allmask,
    std::vector&lt;std::vector&lt;unsigned long long&gt;&gt;&amp; dp,
    std::vector&lt;std::vector&lt;int&gt;&gt;&amp; lineList)
{
    if (mask == allmask)
    {
        return 1;
    }

    if (i &gt; lineNum)
    {
        return 0;
    }

    if (dp[mask][i] != -1)
    {
        return dp[mask][i];
    }
    unsigned long long ways = count(mask, i + 1, lineNum,
        allmask, dp, lineList);
    for (int j = 0; j &lt; lineList[i].size(); j++)
    {
        if (!(mask &amp; (1 &lt;&lt; lineList[i][j])))
        {
            ways += count(mask | (1 &lt;&lt; lineList[i][j]), i + 1, lineNum,
                allmask, dp, lineList);
        }
    }
    dp[mask][i] = ways;
    return dp[mask][i];
}

void parse(std::string&amp; str, const std::string&amp; delimiter,
    std::vector&lt;std::vector&lt;int&gt;&gt;&amp; lineList, int person)
{
    size_t start = 0;
    size_t pos = str.find(delimiter);
    while (pos != std::string::npos)
    {
        int line = std::stoi(str.substr(start, (pos - start)));
        lineList[line].push_back(person);
        start = pos + delimiter.size();
        pos = str.find(delimiter, start);
    }
    lineList[std::stoi(str.substr(start))].push_back(person);
}

int main(int argc, char **argv)
{
    std::ifstream infile(argv[1]);
    std::string delimiter = ", ";
    std::string str;
    std::getline(infile, str);
    size_t pos = str.find(delimiter);
    int lineNum = std::stoi(str.substr(0, pos));
    int totalUsersNum = std::stoi(str.substr(pos + delimiter.length()));
    std::vector&lt;std::vector&lt;int&gt;&gt; lineList(lineNum + 1);
    for (int person = 0; person &lt; totalUsersNum; person++)
    {
        std::getline(infile, str);
        parse(str, delimiter, lineList, person);
    }
    int allmask = (1 &lt;&lt; totalUsersNum) - 1;
    std::vector&lt;std::vector&lt;unsigned long long&gt;&gt; dp((allmask + 2),
        std::vector&lt;unsigned long long&gt;((lineNum + 1), -1));

    std::cout &lt;&lt; count(0, 1, lineNum, allmask, dp, lineList) &lt;&lt; std::endl;
    return 0;
}
</pre></div>
</body></html>