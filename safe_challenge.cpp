#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <set>
#include <algorithm>


#define DEBUG 1


struct Feature {
    int pos;
    int cost;
    int totalCost;
    bool isFullDependencisResolved;    
    std::vector<int> dependencies;
    std::set<int> fullDependencies;
};

struct FeatureDep {
    int pos;
    int depCount;
    FeatureDep(int p, int d) {pos = p;depCount = d;}
    bool operator < (const FeatureDep& dep) const
    {
        return depCount < dep.depCount;
    }
};


class FeatureSelector
{
    private:  
        int  featureNum;
        std::vector<Feature> features;  
        std::vector<FeatureDep> sortedFeatureDep;
    private:
        void resolveFeatureFullDependencies(Feature &);
        void displayFeatures();
    public:
        FeatureSelector();        
        void allocate();
        bool readConfiguration(char * fileName);
};

FeatureSelector::FeatureSelector() 
{

}

void FeatureSelector::resolveFeatureFullDependencies(Feature & f) {
    std::stack<Feature *> uncalculatedStack;
    uncalculatedStack.push(&f);
//    std::cout <<"push:\t"<< f.pos << '\n';
    while(!uncalculatedStack.empty()) {
        Feature * pf = uncalculatedStack.top();
//        std::cout << "processing feature:" << pf->pos << '\n';
        int depResolvedCound = 0;
        for(auto& i : pf->dependencies) {
            if( features[i].isFullDependencisResolved ) {
                for(auto& j : features[i].fullDependencies) {                
                    pf->fullDependencies.insert(j);                    
                }
                depResolvedCound++;
            }
            else {
                uncalculatedStack.push(&features[i]);
//                std::cout << "push:\t" << features[i].pos << '\n';
            }
        }
        if( depResolvedCound == pf->dependencies.size()) {
            pf->isFullDependencisResolved = true;
            sortedFeatureDep[pf->pos].depCount = depResolvedCound;
            uncalculatedStack.pop();
//            std::cout << "pop:\t" << pf->pos << '\n';
        }
    }
    for(auto& x : f.fullDependencies) {
        f.totalCost += features[x].cost;
    }        
}

void FeatureSelector::displayFeatures() {
#if DEBUG
    std::cout << "======Features as below:======\n";
    for(const auto& f: features) {
        std::cout << f.pos << "\t";
        std::cout << "Cost\t" << f.cost << "\t";
        std::cout << "totalCost\t" << f.totalCost << "\t";
        std::cout << "fullDependencies:\t";
        for(auto & x : f.fullDependencies) {
            std::cout << x << " ";
        }
        std::cout << "\n";
    }
#endif        
}


void FeatureSelector::allocate()
{
    std::set<int> selectedFeatures;
    displayFeatures();
    for(auto& f: features) {
        if(f.dependencies.size() > 0 && !f.isFullDependencisResolved) {
            resolveFeatureFullDependencies(f);            
        }
    }
    
    
    displayFeatures();    
    
    int totalCost = 0;
    for(auto& i: selectedFeatures) {
        totalCost += features[i].cost;
    }
    std::cout << totalCost << "\n";

#if DEBUG   
    for(auto& i: selectedFeatures) {
        std::cout << i + 0 << "\n";
    } 
#else
    for(auto& i: selectedFeatures) {
        std::cout << i + 1 << "\n";
    }
#endif    
}

bool FeatureSelector::readConfiguration(char * fileName) 
{
    std::ifstream fs(fileName);
    if ( fs ) {
        std::string line,x;
        getline(fs, x);
        featureNum = std::stoi(x);
        int index = 0;        
        while(getline(fs, line) && !line.empty()) {
//            std::cout << line << '\n';
            std::istringstream ss( line );
            int data;
            int count = 0;
            Feature feature;
            while (ss >> data)         
           {               
               if( count++ == 0) {
                   feature.cost = data;
               }
               else {
                   feature.dependencies.push_back(data - 1); //convert to 0-based.
                   feature.fullDependencies.insert(data - 1);                   
               }
           }
           if(count == 1) {        
               feature.isFullDependencisResolved = true;
               sortedFeatureDep.push_back(FeatureDep(index, 1));
           }
           else {
               feature.isFullDependencisResolved = false;
           }
           feature.totalCost = feature.cost;                      
           feature.pos = index++;
           features.push_back(feature);
        }
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
    
    FeatureSelector selector;
    if( selector.readConfiguration(argv[1]) ) {
        selector.allocate();
        return 0;
    }
    else {
        std::cout << "Incorrect Data File.";
        return -1;
    }
}

