#ifndef GENOME_H
#define GENOME_H

#define COEF 0.05
#include <string>
#include <iostream>
#include <fstream>
#include <Random.h>

class Genome{
    public:
        Genome();
        virtual ~Genome();
        virtual void mutate(Random* rng) =0;
        virtual Genome* clone() =0;
        virtual std::ostream& write(std::ostream& os) =0;
        virtual std::ofstream& write(std::ofstream& os) =0;
        virtual int serialize(char* buff) =0;

        friend std::ostream& operator<< (std::ostream& os, Genome& genome);
        friend std::ofstream& operator<< (std::ofstream& os, Genome& genome);
};

#endif // GENOME_H
