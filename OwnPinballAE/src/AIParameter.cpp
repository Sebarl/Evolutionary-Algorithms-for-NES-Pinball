#include "Random.h"
#include <time.h>
#include <string>
#include "OwnAEUtils.h"
#include "AIParameter.h"
#include <math.h>

AIParameter::AIParameter(int value){
    this->value = value;
}

AIParameter::~AIParameter(){}

int AIParameter::getValue() {
    return value;
}

void AIParameter::setValue(int value){
    this->value = value;
}

///////// Genome /////////

void AIParameter::mutate(Random* rng){
	float rnd;
    rnd = rng->nextFloat();

	value += round(30.0*rnd*COEF);
    if(value < 0){
        value = 0;
    }
}

std::ostream& AIParameter::write(std::ostream& os){
    os << value;
    return os;
}

std::ofstream& AIParameter::write(std::ofstream& os){
    os << value;
    return os;
}

int AIParameter::serialize(char* buff){
    return sprintf(buff, "%d ", value);
}

AIParameter* AIParameter::clone(){
    return new AIParameter(value);
}
