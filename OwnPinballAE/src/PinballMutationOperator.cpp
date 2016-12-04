#include "PinballMutationOperator.h"
#include "Genome.h"

PinballMutationOperator::PinballMutationOperator(float probability){
    this->probability = probability;
}

PinballMutationOperator::~PinballMutationOperator(){}

std::vector<Solution*> PinballMutationOperator::apply(std::vector<Solution*> selectedCandidates, Random* rng){
    for(unsigned int i=0; i< selectedCandidates.size(); i++){
        std::vector<Genome*>* candidate = selectedCandidates[i]->getSolution();
        for(unsigned int j=0; j<candidate->size();j++){
            if(rng->nextFloat() < probability){
                //mutate the candidate
                (*candidate)[j]->mutate(rng);
            }
        }
    }
    return selectedCandidates;
}
