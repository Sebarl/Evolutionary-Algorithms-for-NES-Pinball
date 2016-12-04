#include <stdlib.h>
#include <cstring>
#include "MicPinballEvaluator.h"
#include "Constants.h"

MicPinballEvaluator::MicPinballEvaluator(Parameters* params, int islandIndex){
    this->params = params;
    this->islandIndex = islandIndex;
}

MicPinballEvaluator::~MicPinballEvaluator(){}

int serializeChromosome(Solution* sol, char* buff, int instance){
	std::vector<Genome*> s = sol->getSolution();
	int length = 0;
	for(int i = 0; i < CANT_POINTS; i++){
		length += s[i]->serialize(buff+length);
	}
    length +=sprintf(buff+length, "%d ", instance);
    for (int i=CANT_POINTS; i < CANT_POINTS+CANT_AI_PARAMETERS;i++){
        length += s[i]->serialize(buff+length);
    }
	return length;
}

double offloadEvaluation(char* filename, char* serialChromosome, int frameCount){
    double fitValue;
    #pragma offload target(mic) in(serialChromosome, frameCount, filename) out(fitValue)
     {

        //saquemos de donde viene solution
        FILE* file = fopen(filename, "w");
        if(file != NULL){
            //escribimos el individuo al archivo
            fputs(serialChromosome, file);
            fclose(file);
        }
        else{
            FILE* file = fopen("/tmp/tempSols/ERRORLOG.TXT", "a");
            if(file != NULL){
                fprintf(file, "Candidate file write failed.");
                fclose(file);
            }
            //manejo de errores, que hacer si no puedo crear archivo
        }
        //invocar a fceux y obtener el resultado
        char path[200];
        sprintf(path, "LD_LIBRARY_PATH=/tmp/bin;export LD_LIBRARY_PATH; cd \"/tmp/bin/\"; ./fceux -l %d -e %s Pinball.nes", frameCount, filename);
        char buff[15];
        FILE *fp = popen(path,"r");
        while(fgets(buff, 15, fp) != NULL);
        pclose(fp);
        // read the output from the command
        int score = atoi(strtok(buff, " "));
        int balls = atoi(strtok(NULL, " "));
        fitValue = (double)(score + balls*10000);
    }
    return fitValue;
}

double MicPinballEvaluator::getFitness(const int index, vector<Solution*> population){
	char* filename= new char[50];
	Solution* s = population[index];
	sprintf(filename, "/tmp/tempSols/%d%d%d.sol", index, this->islandIndex, this->params->evolutiveProcessID);
	char* serialChromosome = new char[CANT_POINTS*34 + CANT_AI_PARAMETERS*21 + 21];
	serializeChromosome(s, serialChromosome, this->params->instance);
    int frameCount = this->params->frameCount;
    double fitValue = offloadEvaluation(filename, serialChromosome, frameCount);
    delete[] serialChromosome;
    delete[] filename;
    return fitValue;
}

bool MicPinballEvaluator::isNatural(){
    return true;
}
