
<!-- saved from url=(0075)http://10.182.198.199/problem1-20181112000000-20181125235959/biji/code.html -->
<html><head><meta http-equiv="Content-Type" content="text/html; charset=windows-1252"><title>Solution.cpp</title>
<script src="./Solution_1_biji_files/prettify.js.download" type="text/javascript"></script>
<link rel="stylesheet" type="text/css" href="./Solution_1_biji_files/prettify.css">
</head><body onload="prettyPrint()">
<h2>Solution.cpp</h2><div><pre class="prettyprint linenums">// code contest 01
#pragma GCC optimize("-O2")

#include &lt;fstream&gt;
#include &lt;functional&gt;
#include &lt;iostream&gt;
#include &lt;list&gt;
#include &lt;regex&gt;
#include &lt;string&gt;
#include &lt;vector&gt;

namespace code
{

// data loader for code contest
class DataLoader
{
public:
    explicit DataLoader(const std::string&amp; filename) : fs(filename)
    {
    }

    // load content from file, callback on each line content
    // ignore \r for windows
    void load(const std::function&lt;void(const std::string&amp; line)&gt;&amp; cb)
    {
        for(std::string line; std::getline(fs, line);)
        {
            if(line.length() &gt; 0 &amp;&amp; *line.rbegin() == '\r')
            {
                *line.rbegin() = '\0';
            }
            cb(line);
        }
    }

    // load with pattern matching, cb on each line, all match line + sub matches
    // in vector.
    void load(const std::string&amp; pattern, const std::function&lt;void(std::vector&lt;std::string&gt;&amp;)&gt;&amp; cb)
    {
        std::vector&lt;std::string&gt; ret;
        load([&amp;ret, &amp;pattern](const std::string&amp; line) {
            std::regex reg(pattern);
            std::smatch matches;
            std::regex_match(line, matches, reg);
            for(const auto&amp; match : matches)
            {
                ret.emplace_back(match);
            }
        });
        cb(ret);
    }

private:
    std::ifstream fs;
};

// all code contest have one file input parameter
void paramCheck(int argc, const char* argv[])
{
    if(argc &lt;= 1)
    {
        std::fprintf(stdout, "usage %s &lt;input&gt;", argv[0]);
        std::exit(-1);
    }
}

const uint8_t invalid = 0xff;
const uint8_t atozNums = 26;
const uint8_t firstChar = 'A';
const uint8_t ten = 10;

class Solution
{
public:
    class Cache
    {
    public:
        Cache()
        {
            mapping.fill(invalid);
            nums.fill(0);
        }
        size_t a{0};
        size_t b{0};
        size_t r{0};
        std::array&lt;uint8_t, atozNums&gt; mapping;
        std::array&lt;uint8_t, ten&gt; nums;
        size_t depth{0};
        size_t modNum{1};
    };

    void processA(const std::string&amp; strA, size_t lenA, const Cache&amp; cache, std::vector&lt;Cache&gt;&amp; newCaches)
    {
        std::vector&lt;Cache&gt; newTempCaches{};
        newCaches.emplace_back(cache);
        for(const auto&amp; cacheA : newCaches)
        {
            if(cacheA.depth &lt; lenA)
            {
                auto ch = strA[lenA - cacheA.depth - 1] - firstChar;
                auto valA = cacheA.mapping[ch];
                if(valA != invalid)
                {
                    Cache newCacheA = cacheA;
                    auto added = valA * cacheA.modNum;
                    newCacheA.a += added;
                    newTempCaches.emplace_back(newCacheA);
                }
                else
                {
                    size_t start = 0;
                    if(lenA - cacheA.depth == 1)
                    {
                        start = 1;
                    }
                    for(size_t i = start; i &lt; ten; ++i)
                    {
                        if(cacheA.nums[i] == 0)
                        {
                            Cache newCacheA = cacheA;
                            newCacheA.mapping[ch] = i;
                            newCacheA.nums[i] = 1;
                            auto added = i * cacheA.modNum;
                            newCacheA.a += added;
                            newTempCaches.emplace_back(newCacheA);
                        }
                    }
                }
            }
            else
            {
                newTempCaches.emplace_back(cacheA);
            }
        }
        newCaches.swap(newTempCaches);
    }

    void processB(const std::string&amp; strB, size_t lenB, std::vector&lt;Cache&gt;&amp; newCaches)
    {

        std::vector&lt;Cache&gt; newTempCaches{};
        for(const auto&amp; cacheB : newCaches)
        {
            if(cacheB.depth &lt; lenB)
            {
                auto ch = strB[lenB - cacheB.depth - 1] - firstChar;
                auto valB = cacheB.mapping[ch];
                if(valB != invalid)
                {
                    Cache newCacheB = cacheB;
                    auto added = valB * cacheB.modNum;
                    newCacheB.b += added;
                    newTempCaches.emplace_back(newCacheB);
                }
                else
                {
                    size_t start = 0;
                    if(lenB - cacheB.depth == 1)
                    {
                        start = 1;
                    }
                    for(size_t i = start; i &lt; ten; ++i)
                    {
                        if(cacheB.nums[i] == 0)
                        {
                            Cache newCacheB = cacheB;
                            newCacheB.mapping[ch] = i;
                            newCacheB.nums[i] = 1;
                            auto added = i * cacheB.modNum;
                            newCacheB.b += added;
                            newTempCaches.emplace_back(newCacheB);
                        }
                    }
                }
            }
            else
            {
                newTempCaches.emplace_back(cacheB);
            }
        }
        newCaches.swap(newTempCaches);
    }

    void process(const std::vector&lt;std::string&gt;&amp; words)
    {
        auto it = words.begin();
        ++it;
        auto&amp; strA = *it;
        ++it;
        auto&amp; strB = *it;
        ++it;
        auto&amp; strR = *it;
        ++it;
        auto lenA = strA.size();
        auto lenB = strB.size();
        auto lenR = strR.size();

        std::list&lt;Cache&gt; caches{};
        caches.emplace_back(Cache());

        while(caches.size() &gt; 0)
        {
            Cache cache = caches.front();
            caches.pop_front();

            std::vector&lt;Cache&gt; newCaches{};
            processA(strA, lenA, cache, newCaches);
            processB(strB, lenB, newCaches);
            for(const auto&amp; cacheR : newCaches)
            {
                if(cacheR.depth &lt; lenR)
                {
                    auto ch = strR[lenR - cacheR.depth - 1] - firstChar;
                    auto r = cacheR.a * cacheR.b % (cacheR.modNum * 10);
                    auto rHead = r / cacheR.modNum;

                    if(rHead != 0 || lenR - cacheR.depth &gt; 1)
                    {

                        auto valR = cacheR.mapping[ch];
                        if(valR == invalid &amp;&amp; cacheR.nums[rHead] == 0)
                        {
                            auto newCacheR = cacheR;
                            newCacheR.mapping[ch] = rHead;
                            newCacheR.nums[rHead] = 1;
                            newCacheR.r = r;
                            newCacheR.depth++;
                            newCacheR.modNum *= 10;
                            caches.emplace_back(newCacheR);
                        }
                        else
                        {
                            if(cacheR.mapping[ch] == rHead)
                            {
                                auto newCacheR = cacheR;
                                newCacheR.r = r;
                                newCacheR.depth++;
                                newCacheR.modNum *= 10;
                                caches.emplace_back(newCacheR);
                            }
                        }
                    }
                }
                else
                {
                    if(cacheR.a * cacheR.b == cacheR.r)
                    {

                        std::fprintf(stdout, "%lu * %lu = %lu\n", cacheR.a, cacheR.b, cacheR.r);
                    }
                }
            }
        }
    }
};

}

int main(int argc, const char* argv[])
{
    code::paramCheck(argc, argv);

    const std::string filename(argv[1]);
    auto loader = std::make_unique&lt;code::DataLoader&gt;(filename);
    code::Solution solution;
    loader-&gt;load(std::string("([A-Z]+) \\* ([A-Z]+) = ([A-Z]+)\n?"),
                 [&amp;solution](const std::vector&lt;std::string&gt;&amp; words) { solution.process(words); });
    return 0;
}</pre></div>
</body></html>