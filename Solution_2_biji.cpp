
<!-- saved from url=(0075)http://10.182.198.199/problem2-20181112000000-20181125235959/biji/code.html -->
<html><head><meta http-equiv="Content-Type" content="text/html; charset=windows-1252"><title>Solution.cpp</title>
<script src="./Solution_2_biji_files/prettify.js.download" type="text/javascript"></script>
<link rel="stylesheet" type="text/css" href="./Solution_2_biji_files/prettify.css">
</head><body onload="prettyPrint()">
<h2>Solution.cpp</h2><div><pre class="prettyprint linenums">// code contest 02
#pragma GCC optimize("-O3")

#include &lt;cstdio&gt;
#include &lt;cstdlib&gt;
#include &lt;fcntl.h&gt;
#include &lt;fstream&gt;
#include &lt;iostream&gt;
#include &lt;sys/stat.h&gt;
#include &lt;sys/types.h&gt;
#include &lt;unistd.h&gt;
#include &lt;vector&gt;

#define BUFF_SIZE (4096)
#define LEFT_MOVE1 (3)
#define LEFT_MOVE2 (1)

int main(int argc, const char* argv[])
{
    (void)argc;
    char buff[BUFF_SIZE];
    size_t m = 0;
    register uint32_t num = 0;
    register bool prevIsNum = false;
    register size_t pos = 0;
    std::vector&lt;uint32_t&gt; line;

    auto fd = open(argv[1], O_RDONLY);
    while(auto readBytes = ::read(fd, buff, BUFF_SIZE))
    {
        for(register int i = 0; i &lt; readBytes; ++i)
        {
            register auto ch = buff[i];
            // '\r', '\n', ' ', ',' are all &lt; '0'
            if(ch &gt;= '0')
            {
                if(num &gt; 0)
                {
                    // num x= 10
                    num = (num &lt;&lt; LEFT_MOVE1) + (num &lt;&lt; LEFT_MOVE2);
                }
                num += static_cast&lt;uint32_t&gt;(ch - '0');
                prevIsNum = true;
            }
            else
            {
                if(prevIsNum)
                {
                    if(pos == 0)
                    {
                        if(m == 0)
                        {
                            // first num in first line
                            m = num;
                            line = std::vector&lt;uint32_t&gt;(m + 1, 0);
                            num = 0;
                            prevIsNum = false;
                        }
                        else
                        {
                            // second num in first line
                            pos++;
                            num = 0;
                            prevIsNum = false;
                        }
                    }
                    else
                    {
                        // other nums
                        line[pos] = num + std::max(line[pos], line[pos - 1]);
                        if(pos == m)
                        {
                            pos = 1;
                        }
                        else
                        {
                            ++pos;
                        }
                        num = 0;
                        prevIsNum = false;
                    }
                }
            }
        }
    }
    if(prevIsNum)
    {
        line[pos] = num + std::max(line[pos], line[pos - 1]);
    }
    std::fprintf(stdout, "%u\n", line[m]);
    std::fflush(stdout);
    _exit(0);
    return 0;
}</pre></div>
</body></html>