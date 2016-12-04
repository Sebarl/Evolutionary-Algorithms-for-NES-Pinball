#include "Genome.h"

Genome::Genome(){}

Genome::~Genome(){}

std::ostream& operator<<(std::ostream& os, Genome& genome) {
    genome.write(os);
    return os;
}

std::ofstream& operator<<(std::ofstream& os, Genome& genome) {
    genome.write(os);
    return os;
}
