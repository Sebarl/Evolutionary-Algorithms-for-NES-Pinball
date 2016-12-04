#include "PinballCrossoverOperator.h"
#include "Genome.h"
#include <stdlib.h>
#include <string.h>

PinballCrossoverOperator::PinballCrossoverOperator(unsigned int crossoverPoints):AbstractCrossover(crossoverPoints){}

PinballCrossoverOperator::PinballCrossoverOperator(unsigned int crossoverPoints, float crossoverProbability):AbstractCrossover(crossoverPoints, crossoverProbability){}

PinballCrossoverOperator::~PinballCrossoverOperator(){}

inline void swap(Genome** g1, Genome** g2, unsigned int lower, unsigned int upper){
    int length = upper - lower + 1;
    int start=sizeof(Genome*)*lower;
    void* temp = malloc(length*sizeof(Genome*));
    memcpy(temp, g1+start, sizeof(Genome*)*length);
    memcpy(g1+start, g2+start, sizeof(Genome*)*length);
    memcpy(g2+start, temp, sizeof(Genome*)*length);
}

void PinballCrossoverOperator::mate(Solution* parent1, Solution* parent2, Random* rng){
    std::vector<Solution*> result;
    std::vector<Genome*>* p1 = parent1->getSolution();
    std::vector<Genome*>* p2 = parent2->getSolution();
    Genome** ap1 = &(*p1)[0];
    Genome** ap2 = &(*p2)[0];
    if(crossoverPoints>=p1->size()-1){
        Genome* temp;
        for(unsigned int i=0; i<p1->size(); i+=2){
            temp = ap1[i];
            ap1[i] = ap2[i];
            ap2[i]=temp;
        }
    }
    else{
        int* arr = new int[p1->size()-1];
        int j;
        for(unsigned int i = 1; i < p1->size();i++){
            j = rng->nextInt(i);
            arr[i-1] = arr[j];
            arr[j] = i;
        }
        for(unsigned int i = 0; i<crossoverPoints;i++){
            swap(ap1,ap2, 0, arr[i]);
        }
        delete [] arr;
    }
}
