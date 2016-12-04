#ifndef EVALUATED_CANDIDATE_H
#define EVALUATED_CANDIDATE_H


template <class T>
class EvaluatedCandidate
{
    public:
        T* candidate;
        double fitness;
        EvaluatedCandidate(){}
        virtual ~EvaluatedCandidate(){
            delete candidate;
        }
};

#endif
