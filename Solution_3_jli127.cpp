
<!-- saved from url=(0077)http://10.182.198.199/problem3-20181112000000-20181125235959/jli127/code.html -->
<html><head><meta http-equiv="Content-Type" content="text/html; charset=windows-1252"><title>Solution.cpp</title>
<script src="./Solution_3_jli127_files/prettify.js.download" type="text/javascript"></script>
<link rel="stylesheet" type="text/css" href="./Solution_3_jli127_files/prettify.css">
</head><body onload="prettyPrint()">
<h2>Solution.cpp</h2><div><pre class="prettyprint linenums">#include &lt;fstream&gt;
#include &lt;sstream&gt;
#include &lt;vector&gt;
#include &lt;cstring&gt;
#include &lt;algorithm&gt;
using namespace std;

char inputstr[1000], teststr1[1000], teststr2[1000];
int inputp;
int testlinenum, usernum;

const int maxlen = 1024 + 5;
vector&lt;vector&lt;int&gt;&gt; map;
long long dp[20][maxlen];

bool getParam(char *infilename){
    FILE *f;
    f = fopen(infilename, "r");
    fscanf(f, "%d%*[^0-9]%d%*[^0-9]", &amp;testlinenum, &amp;usernum);

    map.resize(testlinenum);
    for(int i = 0; i &lt; testlinenum; i++)
        map.clear();
    for(int i = 0; i &lt; usernum; i++){
        fscanf(f, "%[^\n]\n", inputstr);
        inputp = 0;
        int id;
        while(inputp &lt; strlen(inputstr)){
            sscanf((inputstr + inputp), "%d%*[, ]", &amp;id);
            map[id - 1].push_back(i);
            sscanf((inputstr + inputp), "%[0-9]%[, ]", teststr1, teststr2);
            inputp += strlen(teststr1) + strlen(teststr2);
        }
    }
    return true;
}

int main(int argc, char **argv)
{
    if(argc != 2){
        printf("there are incorrect income file.\n");
        return 1;
    }

    if(!getParam(argv[1])){
        printf("there are incorrect income param.\n");
        return 2;
    }

    if(usernum == 0){
        printf("0\n");
        return 0;
    }

    int len = 1 &lt;&lt; usernum;
    for(int i = 0; i &lt; testlinenum; i++)
        for(int j = 0; j &lt; len; j++)
            dp[i][j] = 0;

    dp[0][0] = 1;
    for(int i = 0; i &lt; map[0].size(); i++)
        dp[0][1 &lt;&lt; map[0][i]] = 1;

    for(int i = 1; i &lt; testlinenum; i++)
        for(int j = 0; j &lt; len; j ++)
        {
            dp[i][j] += dp[i - 1][j];
            for(int k = 0; k &lt; map[i].size(); k++){
                if((j &amp; (1 &lt;&lt; map[i][k])) == 0)
                    continue;
                int frontid = j - (1 &lt;&lt; map[i][k]);
                dp[i][j] += dp[i - 1][frontid];
            }
        }

    long long ans = dp[testlinenum - 1][len - 1];
    printf("%lld\n", ans);
}</pre></div>
</body></html>