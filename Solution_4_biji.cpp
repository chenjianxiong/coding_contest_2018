
<!-- saved from url=(0075)http://10.182.198.199/problem4-20181112000000-20181125235959/biji/code.html -->
<html><head><meta http-equiv="Content-Type" content="text/html; charset=windows-1252"><title>Solution.cpp</title>
<script src="./Solution_4_biji_files/prettify.js.download" type="text/javascript"></script>
<link rel="stylesheet" type="text/css" href="./Solution_4_biji_files/prettify.css">
</head><body onload="prettyPrint()">
<h2>Solution.cpp</h2><div><pre class="prettyprint linenums">// code contest 04
#pragma GCC optimize("-O2")

#include &lt;algorithm&gt;
#include &lt;array&gt;
#include &lt;ctime&gt;
#include &lt;fstream&gt;
#include &lt;functional&gt;
#include &lt;iostream&gt;
#include &lt;list&gt;
#include &lt;set&gt;
#include &lt;sstream&gt;
#include &lt;string&gt;
#include &lt;unordered_map&gt;
#include &lt;unordered_set&gt;
#include &lt;vector&gt;

namespace code
{
static const size_t infinity = SIZE_MAX;
static const size_t invalid = SIZE_MAX;
static const size_t extraNodeCount = 2;

// data loader for code contest
class DataLoader
{
public:
    explicit DataLoader(const std::string&amp; filename) : fs(filename)
    {
    }

    size_t getCount()
    {
        std::string token;
        std::getline(fs, token);
        return static_cast&lt;size_t&gt;(std::strtoul(token.c_str(), nullptr, 10));
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
    explicit Solution(size_t n)
    : nodeCount{n + extraNodeCount}, linkLimit{(nodeCount * nodeCount) &lt;&lt; 1}, linkHeads(nodeCount, invalid), links(linkLimit, {0, 0, 0, 0}), tIndex{n + 1}
    {
    }

    // save graph to s-graph-t, s:0 t:n+1
    void load(const std::vector&lt;std::string&gt;&amp; words)
    {
        if(words.size() &gt; 0)
        {
            auto it = words.begin();
            ++tempIndex;
            auto value = static_cast&lt;ssize_t&gt;(std::strtol(it-&gt;c_str(), nullptr, 10));
            it++;
            std::vector&lt;size_t&gt; deps;
            deps.reserve(words.size() - 1);

            while(it != words.end())
            {
                auto depIndex = static_cast&lt;size_t&gt;(std::strtoul(it-&gt;c_str(), nullptr, 10));
                deps.emplace_back(depIndex);
                ++it;
            }

            bool merged = false;
            for(auto&amp; n : nodes)
            {
                if(tryMerge(n.second, tempIndex, value, deps))
                {
                    merged = true;
                    break;
                }
            }
            if(!merged)
            {
                Node node;
                node.value = value;
                node.features.emplace(tempIndex);
                for(const auto dep : deps)
                {
                    node.deps.emplace(dep);
                }
                nodes.emplace(tempIndex, node);
            }
        }
    }

    void prepareLink()
    {
        for(const auto&amp; node : nodes)
        {
            if(node.second.value &gt; 0)
            {
                link(sIndex, node.first, static_cast&lt;size_t&gt;(node.second.value));
                valueSum += node.second.value;
            }
            else
            {
                link(node.first, tIndex, static_cast&lt;size_t&gt;(-node.second.value));
            }
            for(const auto dep : node.second.deps)
            {
                if(nodes.find(dep) != nodes.end())
                {
                    link(node.first, dep, infinity);
                }
            }
        }
    }

    void process()
    {
        prepareLink();
        auto minimalCut = resolveMinimalCut();
        if(valueSum &lt;= minimalCut)
        {
            std::fprintf(stdout, "0\n");
            return;
        }
        std::fprintf(stdout, "%lu\n", valueSum - minimalCut);

        std::set&lt;size_t&gt; selected;
        enumerateSelectedNodes(sIndex, selected);
        std::for_each(selected.begin(), selected.end(), [this](const auto&amp; v) {
            if(v != this-&gt;sIndex)
            {
                std::fprintf(stdout, "%lu\n", v);
            }
        });
    }

private:
    using Node = struct
    {
        ssize_t value;
        std::unordered_set&lt;size_t&gt; features;
        std::unordered_set&lt;size_t&gt; deps;
    };
    using Link = struct
    {
        size_t begin;
        size_t end;
        size_t capacity;
        size_t next;
    };

    bool tryMerge(Node&amp; a, size_t newFeature, ssize_t value, std::vector&lt;size_t&gt;&amp; deps)
    {
        bool match = false;
        for(const auto feature : a.features)
        {
            for(const auto dep : deps)
            {
                if(feature == dep)
                {
                    match = true;
                    break;
                }
            }
            if(match)
            {
                break;
            }
        }
        if(match &amp;&amp; (a.deps.find(newFeature) != a.deps.end()))
        {
            a.features.emplace(newFeature);
            a.value += value;
            if(a.deps.size() &lt; nodeCount - extraNodeCount)
            {
                std::for_each(deps.begin(), deps.end(), [&amp;a](const auto&amp; dep) { a.deps.emplace(dep); });
            }
            return true;
        }
        return false;
    }

    void link(size_t from, size_t to, size_t weight)
    {
        links[linkCount] = {from, to, weight, linkHeads.at(from)};
        linkHeads[from] = linkCount++;
        links[linkCount] = {to, from, 0, linkHeads.at(to)};
        linkHeads[to] = linkCount++;
    }

    bool search()
    {
        std::list&lt;size_t&gt; queue{sIndex};

        nodeDepthes.clear();
        nodeDepthes.emplace(sIndex, 0);

        while(!queue.empty())
        {
            auto tempBegin = queue.front();
            queue.pop_front();
            auto index = linkHeads[tempBegin];
            while(index != invalid)
            {
                auto&amp; lnk = links[index];
                auto tempEnd = lnk.end;
                if(lnk.capacity &gt; 0 &amp;&amp; nodeDepthes.find(tempEnd) == nodeDepthes.end())
                {
                    nodeDepthes.emplace(tempEnd, nodeDepthes[tempBegin] + 1);
                    queue.emplace_back(tempEnd);
                    if(tempEnd == tIndex)
                    {
                        return true;
                    }
                }
                index = links[index].next;
            }
        }
        return false;
    }

    size_t resolveMinimalCut()
    {
        size_t result{0};
        size_t top{0};
        std::vector&lt;size_t&gt; tempHeads(linkHeads);
        std::vector&lt;size_t&gt; cache(nodeCount, 0);
        while(search())
        {
            std::copy(linkHeads.begin(), linkHeads.end(), tempHeads.begin());
            auto currIndex = sIndex;
            top = 0;
            while(true)
            {
                size_t tempTop{0};
                auto minimalCapacity = infinity;
                if(currIndex == tIndex)
                {
                    for(size_t i = 0; i &lt; top; ++i)
                    {
                        auto cap = links[cache[i]].capacity;
                        if(minimalCapacity &gt; cap)
                        {
                            minimalCapacity = cap;
                            tempTop = i;
                        }
                    }
                    for(size_t i = 0; i &lt; top; ++i)
                    {
                        auto index = cache[i];
                        links[index].capacity -= minimalCapacity;
                        links[index ^ 1].capacity += minimalCapacity;
                    }
                    result += minimalCapacity;
                    currIndex = links[cache[tempTop]].begin;
                    top = tempTop;
                }
                auto index = tempHeads[currIndex];
                while(index != invalid)
                {
                    auto&amp; lnk = links[index];
                    if(lnk.capacity &gt; 0 &amp;&amp; nodeDepthes[currIndex] + 1 == nodeDepthes[lnk.end])
                    {
                        break;
                    }
                    index = lnk.next;
                    tempHeads[currIndex] = index;
                }
                if(tempHeads[currIndex] == invalid)
                {
                    if(top != 0)
                    {
                        top--;
                        nodeDepthes[currIndex] = invalid;
                        currIndex = links[cache[top]].begin;
                    }
                    else
                    {
                        break;
                    }
                }
                else
                {
                    cache[top] = tempHeads[currIndex];
                    currIndex = links[tempHeads[currIndex]].end;
                    top++;
                }
            }
        }
        return result;
    }

    void enumerateSelectedNodes(size_t curr, std::set&lt;size_t&gt;&amp; selected)
    {
        for(const auto f : nodes[curr].features)
        {
            selected.emplace(f);
        }
        auto index = linkHeads[curr];
        while(index != invalid)
        {
            auto linked = links[index].end;
            if(selected.find(linked) == selected.end() &amp;&amp; links[index].capacity &gt; 0)
            {
                enumerateSelectedNodes(linked, selected);
            }
            index = links[index].next;
        }
    }

    size_t nodeCount;
    size_t linkLimit;
    size_t linkCount{0};
    std::vector&lt;size_t&gt; linkHeads;
    std::unordered_map&lt;size_t, size_t&gt; nodeDepthes;
    std::vector&lt;Link&gt; links;
    std::unordered_map&lt;size_t, Node&gt; nodes;

    size_t tempIndex{0};
    size_t sIndex{0};
    size_t tIndex;
    size_t valueSum{0};
};
}

int main(int argc, const char* argv[])
{
    code::paramCheck(argc, argv);

    const std::string filename(argv[1]);
    code::DataLoader loader(filename);

    code::Solution solution{loader.getCount()};
    loader.load(' ', [&amp;solution](const std::vector&lt;std::string&gt;&amp; words) { solution.load(words); });
    solution.process();
    return 0;
}</pre></div>
</body></html>