#ifndef MICPINBALLEVALUATOR_H
#define MICPINBALLEVALUATOR_H

#include "Parameters.h"
#include "FitnessEvaluator.h"
#include "Solution.h"

class MicPinballEvaluator : public FitnessEvaluator<Solution>
{
   public:
        MicPinballEvaluator(Parameters* params, int islandIndex);
        ~MicPinballEvaluator();
        double getFitness(const int index, vector<Solution*> population);
        bool isNatural();

    private:
        int islandIndex;
        Parameters* params;
};

#endif // MICPINBALLEVALUATOR_H
