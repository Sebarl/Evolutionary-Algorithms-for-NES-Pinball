#include <stdlib.h>
#include <cstring>
#include "PinballEvaluator.h"
#include "Constants.h"

PinballEvaluator::PinballEvaluator(Parameters* params, int islandIndex){
    this->params = params;
    this->islandIndex = islandIndex;
}

PinballEvaluator::~PinballEvaluator(){}

inline int serializeChromosome(Solution* sol, char* buff, int instance){
	std::vector<Genome*>* s = sol->getSolution();
	int length = 0;
	for(int i = 0; i < CANT_POINTS; i++){
		length += (*s)[i]->serialize(buff+length);
	}
    length +=sprintf(buff+length, "%d ", instance);
    for (int i=CANT_POINTS; i < CANT_POINTS+CANT_AI_PARAMETERS;i++){
        length += (*s)[i]->serialize(buff+length);
    }
	return length;
}


double PinballEvaluator::getFitness(const int index, vector<Solution*> population){
	char filename[50];
	Solution* s = population[index];
	sprintf(filename, "tempSols/%d%d%d.sol", index, this->islandIndex, this->params->evolutiveProcessID);
	char* serialChromosome = new char[CANT_POINTS*34 + CANT_AI_PARAMETERS*21 + 21];
	serializeChromosome(s, serialChromosome, this->params->instance);

	//saquemos de donde viene solution
	FILE* file = fopen(filename, "w");
	if(file != NULL){
		//escribimos el individuo al archivo
		fputs(serialChromosome, file);
		fclose(file);
	}
	else{
		printf("%s\n", "BOOOOOM!");
		//manejo de errores, que hacer si no puedo crear archivo
	}
	//invocar a fceux y obtener el resultado
	char path[200];
	sprintf(path, "cd \"../Linux/Fceux Headless/bin/\"; ./fceux -l %d -e ../../../OwnPinballAE/%s Pinball.nes", this->params->frameCount, filename);
	//sprintf(path, "cd bin; ./fceux -l %d -e ../%s Pinball.nes", this->params->frameCount, filename);
	char buff[15];
	FILE *fp = popen(path,"r");
	while(fgets(buff, 15, fp) != NULL);
	pclose(fp);
    buff[6] = '\000';

    // read the output from the command
    int score = atoi(buff);
    int balls = atoi(&buff[7]);
	delete[] serialChromosome;

	return score + balls*10000;
}

bool PinballEvaluator::isNatural(){
    return true;
}
