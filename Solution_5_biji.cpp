
<!-- saved from url=(0075)http://10.182.198.199/problem5-20181112000000-20181125235959/biji/code.html -->
<html><head><meta http-equiv="Content-Type" content="text/html; charset=windows-1252"><title>Solution.cpp</title>
<script src="./Solution_5_biji_files/prettify.js.download" type="text/javascript"></script>
<link rel="stylesheet" type="text/css" href="./Solution_5_biji_files/prettify.css">
</head><body onload="prettyPrint()">
<h2>Solution.cpp</h2><div><pre class="prettyprint linenums">// code contest 05
#pragma GCC optimize("-O2")

#include &lt;cmath&gt;
#include &lt;cstdlib&gt;
#include &lt;fstream&gt;
#include &lt;functional&gt;
#include &lt;iostream&gt;
#include &lt;list&gt;
#include &lt;regex&gt;
#include &lt;set&gt;
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

    // load with delimiter, cb on each line, all words in vector.
    void load(const char delimiter, const std::function&lt;void(std::vector&lt;std::string&gt;&amp;)&gt;&amp; cb)
    {
        load([&amp;delimiter, &amp;cb](const std::string&amp; line) {
            std::vector&lt;std::string&gt; ret;
            std::istringstream iss(line);
            for(std::string token; std::getline(iss, token, delimiter);)
            {
                ret.push_back(std::move(token));
            }
            cb(ret);
        });
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

class Solution
{
public:
    void train(const std::string&amp; name, bool isMale)
    {
        if(isMale)
        {
            for(const auto&amp; pattern : getPatterns(name))
            {
                patternCountMale_.emplace(pattern, 0);
                patternCountMale_[pattern]++;
            }
            countMale_++;
        }
        else
        {
            for(const auto&amp; pattern : getPatterns(name))
            {
                patternCountFemale_.emplace(pattern, 0);
                patternCountFemale_[pattern]++;
            }
            countFemale_++;
        }
    }

    void trainComplete()
    {
        double freqSum = 0.0;
        double baseFreqSum = 0.0;
        for(const auto&amp; pair : patternCountMale_)
        {
            auto freq = 1.0 * pair.second / countMale_;
            freqMale_[pair.first] = freq;
            freqSum += freq;
            baseFreqSum += log(1 - freq);
        }
        baseFreqMale_ = log(1.0 * countMale_ / (countMale_ + countFemale_)) + baseFreqSum;

        freqSum = 0.0;
        baseFreqSum = 0.0;
        for(const auto&amp; pair : patternCountFemale_)
        {
            auto freq = 1.0 * pair.second / countFemale_;
            freqFemale_[pair.first] = freq;
            freqSum += freq;
            baseFreqSum += log(1 - freq);
        }
        baseFreqFemale_ = log(1.0 * countFemale_ / (countFemale_ + countMale_)) + baseFreqSum;
    }

    bool test(const std::string&amp; name)
    {
        auto freqMale = baseFreqMale_;
        auto freqFemale = baseFreqFemale_;
        auto patterns = getPatterns(name);
        for(size_t i = 0; i &lt; patterns.size(); ++i)
        {
            const auto&amp; pattern = patterns[i];
            freqMale += logP(pattern, freqMale_, countMale_);
            freqFemale += logP(pattern, freqFemale_, countFemale_);
        }
        return freqMale &gt; freqFemale;
    }

private:
    std::vector&lt;std::string&gt; getPatterns(const std::string&amp; name)
    {
        std::vector&lt;std::string&gt; ret;
        char buff[64] = {0};
        size_t buffLen = 0;
        std::stringstream ss;

        if(name.length() &gt;= sizeof(buff))
        {
            return ret;
        }
        for(const char&amp; ch : name)
        {
            if(buffLen &gt; 1 &amp;&amp; isVowel(ch) &amp;&amp; (!isVowel(buff[buffLen - 1])))
            {
                char temp = buff[buffLen - 1];
                buff[buffLen - 1] = 0;
                ret.emplace_back(std::string(buff));
                buff[buffLen - 1] = 0;
                ret.emplace_back(std::string(buff));
                for(size_t i = 1; i &lt; buffLen; ++i)
                {
                    buff[i] = 0;
                }
                buffLen = 1;
                buff[0] = temp;
            }
            buff[buffLen++] = ch;
        }
        if(buffLen &gt; 0)
        {
            // if last add a $ to identify this is a suffix
            ret.emplace_back(std::string(buff) + '$');
        }
        // first &amp; last character as pattern
        ret.emplace_back(name.substr(name.size() - 1, 1) + '$');
        ret.emplace_back(name);
        return ret;
    }

    bool inline isVowel(const char ch)
    {
        // w &amp; y alwasy sound like some Vowels, w -&gt; o|u; y -&gt; i
        switch(ch)
        {
        case 'w':
        case 'y':
        case 'a':
        case 'e':
        case 'i':
        case 'o':
        case 'u':
            return true;
        default:
            return false;
        }
    }

    double logP(const std::string&amp; pattern, std::map&lt;std::string, double&gt;&amp; freq, uint64_t total)
    {
        const double adjust = 1.0;
        auto it = freq.emplace(pattern, 0.0);
        auto count = it.first-&gt;second * total;
        auto freqSmooth = (count + adjust) / (total + freq.size() * adjust);
        return log(freqSmooth) - log(1 - freqSmooth);
    }

    uint64_t countMale_ = 0;
    uint64_t countFemale_ = 0;
    std::map&lt;std::string, uint64_t&gt; patternCountMale_;
    std::map&lt;std::string, uint64_t&gt; patternCountFemale_;
    std::map&lt;std::string, double&gt; freqMale_;
    std::map&lt;std::string, double&gt; freqFemale_;
    double baseFreqMale_;
    double baseFreqFemale_;
};

void handleTrain(const std::vector&lt;std::string&gt;&amp; words, Solution&amp; solution)
{
    solution.train(words.front(), words.back()[0] == 'm');
}

void handleTesting(const std::string&amp; name, Solution&amp; solution)
{
    if(solution.test(name))
    {
        std::fprintf(stdout, "%s,male\n", name.c_str());
    }
    else
    {
        std::fprintf(stdout, "%s,female\n", name.c_str());
    }
}
}

int main(int argc, const char* argv[])
{
    code::paramCheck(argc, argv);

    const std::string training = "/var/www/html/training_dataset.txt";
    const std::string testing(argv[1]);

    code::DataLoader loader(training);
    code::DataLoader tester(testing);

    code::Solution solution;
    loader.load(',', [&amp;solution](std::vector&lt;std::string&gt;&amp; words) { code::handleTrain(words, solution); });
    solution.trainComplete();

    tester.load([&amp;solution](const std::string&amp; name) { code::handleTesting(name, solution); });
    return 0;
}</pre></div>
</body></html>