#ifndef AIPARAMETER_H
#define AIPARAMETER_H

#include <string>
#include <Genome.h>

class AIParameter : public Genome
{
    public:
        AIParameter(int value);
        virtual ~AIParameter();
        int getValue();
        void setValue(int val);

        //Genome
        void mutate(Random* rng);
        virtual std::ostream& write(std::ostream& os);
        virtual std::ofstream& write(std::ofstream& os);
        int serialize(char* buff);
        AIParameter* clone();
    private:
        int value;
};

#endif // AIPARAMETER_H
