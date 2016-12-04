#ifndef MICPARALLELEVOLUTIONENGINE_H
#define MICPARALLELEVOLUTIONENGINE_H

#include <SimpleEvolutionEngine.h>
#include "PlatformSpecific.h"
#include <omp.h>
#include "PinballEvaluator.h"
#include <sstream>

template <class T>
class MicParallelEvolutionEngine : public SimpleEvolutionEngine<T>
{
    protected:
        unsigned int numThreads;
        int islandIndex;
        Parameters* params;
    public:

        MicParallelEvolutionEngine(unsigned int populationSize,unsigned int eliteCount, CandidateFactory<T>* candidateFactory, EvolutionaryOperator<T>* evolutionScheme, SelectionStrategy<T>* selectionStrategy, Random* rng, unsigned int numThreads, int islandIndex, Parameters* params)
        :SimpleEvolutionEngine<T>(populationSize, eliteCount, candidateFactory, evolutionScheme, new PinballEvaluator(params, islandIndex), selectionStrategy, rng){
            this->numThreads = numThreads;
            if(this->numThreads > populationSize){
                this->numThreads = populationSize;
            }
            this->islandIndex=islandIndex;
            this->params=params;
        }

        MicParallelEvolutionEngine(unsigned int populationSize,unsigned int eliteCount, CandidateFactory<T>* candidateFactory, EvolutionaryOperator<T>* evolutionScheme, SelectionStrategy<T>* selectionStrategy, Random* rng, int islandIndex, Parameters* params)
        :SimpleEvolutionEngine<T>(populationSize, eliteCount, candidateFactory, evolutionScheme, new PinballEvaluator(params, islandIndex), selectionStrategy, rng){
            this->numThreads = getNumThreads();
            if(this->numThreads > populationSize){
                this->numThreads = populationSize;
            }
            this->islandIndex=islandIndex;
            this->params=params;
        }

        virtual ~MicParallelEvolutionEngine(){
        }

        virtual std::vector<EvaluatedCandidate<T>*> evaluatePopulation(std::vector<T*> population){
            EvaluatedCandidate<T>* ec;
            unsigned int popSize = population.size();
            unsigned int numThreads = this->numThreads;
            EvaluatedCandidate<T>** resultArray = new EvaluatedCandidate<T>*[popSize];
            char* filenames = new char[popSize*50];
            char* serialChromosomes = new char[popSize*(CANT_POINTS*34 + CANT_AI_PARAMETERS*21 + 21)];
            int frameCount = this->params->frameCount;
            double* fitnesses = new double[popSize];
            for(unsigned int i=0; i<popSize;i++){
                Solution* s = population[i];
                sprintf(&filenames[i*50], "/tmp/tempSols/%d%d%d.sol", i, this->islandIndex, this->params->evolutiveProcessID);
                serializeChromosome(s, &serialChromosomes[i*(CANT_POINTS*34 + CANT_AI_PARAMETERS*21 + 21)], this->params->instance);
            }

            #pragma offload target(mic) in(filenames[0 : popSize*50]) in(serialChromosomes[0 : popSize*(CANT_POINTS*34 + CANT_AI_PARAMETERS*21 + 21)]) in(popSize, numThreads, frameCount) out(fitnesses[0:popSize])
            {
                omp_set_dynamic(0);     // Explicitly disable dynamic teams
                omp_set_num_threads(numThreads);
               #pragma omp parallel shared(fitnesses,filenames,serialChromosomes,frameCount)
               {
                    #pragma omp for
                    for (unsigned int i = 0; i< popSize; i++){

                        FILE* file = fopen(&filenames[i*50], "w");
                        //escribimos el individuo al archivo
                        fputs(&serialChromosomes[i*(CANT_POINTS*34 + CANT_AI_PARAMETERS*21 + 21)], file);
                        fclose(file);
                        //invocar a fceux y obtener el resultado
                        char path[200];
                        //sprintf(path, "cd \"../Linux/Fceux Headless/bin/\"; ./fceux -l %d -e ../../../OwnPinballAE/%s Pinball.nes", frameCount, &filenames[i*50]);
                        sprintf(path, "LD_LIBRARY_PATH='/tmp/bin'; export LD_LIBRARY_PATH; cd \"/tmp/bin/\"; ./fceux -l %d -e %s Pinball.nes", frameCount, &filenames[i*50]);
                        char buff[15];
                        FILE *fp = popen(path,"r");
                        while(fgets(buff, 15, fp) != NULL);
                        pclose(fp);
                        buff[6] = '\000';

                        // read the output from the command
                        int score = atoi(buff);
                        int balls = atoi(&buff[7]);
                        fitnesses[i] = (double)(score + balls*10000);
                   }
                }
            }

            for(unsigned int i=0; i<popSize;i++){
                ec = new EvaluatedCandidate<T>;
                ec->candidate = population[i];
                ec->fitness = fitnesses[i];
                resultArray[i]=ec;
            }

            // delete my life
            delete[] filenames;
            delete[] serialChromosomes;
            delete[] fitnesses;

            //stop deleting my life

            std::vector<EvaluatedCandidate<T>*> evaluatedPopulation(resultArray, resultArray + popSize);
            delete[] resultArray;
            return evaluatedPopulation;
        }

        private:
            int serializeChromosome(Solution* sol, char* buff, int instance){
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

};

#endif // MICPARALLELEVOLUTIONENGINE_H
