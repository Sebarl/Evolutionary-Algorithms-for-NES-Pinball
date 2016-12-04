#ifndef PINBALLFACTORY_H
#define PINBALLFACTORY_H

#include <CandidateFactory.h>
#include "Solution.h"
#include "Random.h"

class PinballFactory : public CandidateFactory<Solution>
{
    public:
        PinballFactory();
        virtual ~PinballFactory();
        virtual Solution* generateRandomCandidate(Random* rng);
    private:
};

#endif // PINBALLFACTORY_H
