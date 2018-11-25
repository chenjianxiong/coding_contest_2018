#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <set>
#include <algorithm>


#define DEBUG 0


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
    bool deleted;
    FeatureDep(int p, int d) {pos = p;depCount = d;deleted = false;}
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
        unsigned int depResolvedCound = 0;
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
    for(const auto& f: sortedFeatureDep) {
        std::cout << f.pos << "\t" << "deleted:" << f.deleted << "\t";        
        std::cout << "Cost\t" << features[f.pos].cost << "\t";
        std::cout << "fullDependencies:\t";
        for(auto & x : features[f.pos].fullDependencies) {
            std::cout << x << " ";
        }
        std::cout << "\n";
    }
#endif        
}


void FeatureSelector::allocate()
{
    int totalCost = 0;
    bool selectedFeatures[1000] = {false};
    displayFeatures();
    for(auto& f: features) {
        if(f.dependencies.size() > 0 && !f.isFullDependencisResolved) {
            resolveFeatureFullDependencies(f);            
        }
    }   
    
    displayFeatures();
    int iterator = featureNum;
    //while(iterator > 0) {    
    if(1) {
        for(auto& x : sortedFeatureDep) {
            if(!x.deleted) {
                if(features[x.pos].cost >= 0) {
                    int cost = features[x.pos].cost;
                    for(auto& dep: features[x.pos].fullDependencies) {
                        if(!selectedFeatures[dep]) {
                            cost += features[dep].cost;
                        }
                    }
                    if (cost > 0) {
                        for(auto& dep: features[x.pos].fullDependencies) {
                            selectedFeatures[dep] = true;
                        }
                        selectedFeatures[x.pos] = true;
                        x.deleted = true;
                        iterator--;
                    }
                    else {
                        x.deleted = true;
                        iterator--;
                    }                
                }
                else {
                    x.deleted = true;
                    iterator--;
                }  
            }
        }
    }
    
    displayFeatures();
    
    for(int i = 0; i < featureNum; i++) {
        if(selectedFeatures[i]) {
            totalCost += features[i].cost;
        }
    }
    
    std::cout << totalCost << "\n";
#if DEBUG    
    for(int i = 0; i < featureNum; i++) {
        if(selectedFeatures[i]) {
            std::cout << i + 1 << std::endl;
        }
    }
#else
    for(int i = 0; i < featureNum; i++) {
        if(selectedFeatures[i]) {
            std::cout << i << std::endl;
        }
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
               sortedFeatureDep.push_back(FeatureDep(index, 0));
           }
           else {
               feature.isFullDependencisResolved = false;
               sortedFeatureDep.push_back(FeatureDep(index, count));
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

