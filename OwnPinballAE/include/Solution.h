#ifndef SOLUTION_H
#define SOLUTION_H

#include <vector>

#include "Genome.h"

using namespace std;

class Solution
{
    public:
        Solution(vector<Genome*>* solution);
        virtual ~Solution();
        Solution* clone();
        vector<Genome*>* getSolution() const;
        void setSolution(vector<Genome*>* solution);
        bool operator==(const Solution &other) const;
        bool operator!=(const Solution &other) const;
        void print(int instance);
        void print(std::ofstream& of, int instance);
    private:
        vector<Genome*>* solution;
};

#endif // SOLUTION_H
