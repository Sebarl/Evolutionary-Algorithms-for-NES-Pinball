#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <exception>

#include "Parameters.h"
#include "FinalStatistics.h"
#include "Solution.h"
#include "PinballFactory.h"
#include "EvolutionaryOperator.h"
#include "PinballCrossoverOperator.h"
#include "PinballMutationOperator.h"
#include "EvolutionPipeline.h"
#include "Constants.h"
#include "PinballEvaluator.h"
#include "SimpleEvolutionEngine.h"
#include "XorShift128.h"
#include "StochasticUniversalSampling.h"
#include "GenerationCount.h"
#include "GenerationObserver.h"
#include "ParallelEvolutionEngine.h"
#include "IslandsEvolutionEngine.h"
#include "RingMigration.h"
#include "MicParallelEvolutionEngine.h"

Parameters loadParams(char* configFilePath, int evolutiveProcessID){
    Parameters p;
    std::string line;
    ifstream configFile(configFilePath);
    if (configFile.is_open()){
        #define MAXIUM_COMMENT_LENGTH 100
        char* buff = new char[MAXIUM_COMMENT_LENGTH];
        configFile >> p.generations;
        configFile.getline(buff, MAXIUM_COMMENT_LENGTH);
        configFile >> p.populationSize;
        configFile.getline(buff, MAXIUM_COMMENT_LENGTH);
        configFile >> p.crossoverProbability;
        configFile.getline(buff, MAXIUM_COMMENT_LENGTH);
        configFile >> p.mutationProbability;
        configFile.getline(buff, MAXIUM_COMMENT_LENGTH);
        configFile >> p.elitismPercentage;
        configFile.getline(buff, MAXIUM_COMMENT_LENGTH);
        configFile >> p.enableIslands;
        configFile.getline(buff, MAXIUM_COMMENT_LENGTH);
        configFile >> p.islandCount;
        configFile.getline(buff, MAXIUM_COMMENT_LENGTH);
        configFile >> p.epochLength;
        configFile.getline(buff, MAXIUM_COMMENT_LENGTH);
        configFile >> p.migrationCount;
        configFile.getline(buff, MAXIUM_COMMENT_LENGTH);
        configFile >> p.frameCount;
        configFile.getline(buff, MAXIUM_COMMENT_LENGTH);
        configFile >> p.instance;
        configFile.getline(buff, MAXIUM_COMMENT_LENGTH);
        configFile >> p.islandThreads;
        configFile.getline(buff, MAXIUM_COMMENT_LENGTH);
        configFile >> p.islandEngineThreads;
        configFile.getline(buff, MAXIUM_COMMENT_LENGTH);
        configFile >> p.enableMic;
        configFile.getline(buff, MAXIUM_COMMENT_LENGTH);
        p.evolutiveProcessID = evolutiveProcessID;
        configFile.close();
        delete[] buff;
    }
    else{
        std::cout << "Unable to open config file";
        exit(-1);
    }
    return p;
}

FinalStatistics<Solution>* evolveSolutions(Parameters &params){
        //ENTREGA LAS ISLAS PIBE!

        EvolutionEngine<Solution>* engine;

        if(params.enableIslands){
            std::vector<EvolutionEngine<Solution>*> islands;
            EvolutionEngine<Solution>* island;

            for(int i =0; i <params.islandCount;i++){
                PinballFactory* factory = new PinballFactory();
                std::vector<EvolutionaryOperator<Solution>*> operators;
                operators.push_back(new PinballCrossoverOperator(CANT_POINTS + CANT_AI_PARAMETERS,params.crossoverProbability)); //uniform crossover
                operators.push_back(new PinballMutationOperator(params.mutationProbability));
                EvolutionaryOperator<Solution>* pipeline = new EvolutionPipeline<Solution>(operators);

                if(params.enableMic){
                    //FitnessEvaluator<Solution>* pe = new MicPinballEvaluator(&params,i);
                    island = new MicParallelEvolutionEngine<Solution>(params.populationSize, params.elitismPercentage, factory, pipeline, new StochasticUniversalSampling<Solution>(), new XorShift128(), params.islandThreads, i, &params);
                }else{
                    FitnessEvaluator<Solution>* pe = new PinballEvaluator(&params,i);
                    island = new ParallelEvolutionEngine<Solution>(params.populationSize, params.elitismPercentage, factory, pipeline, pe, new StochasticUniversalSampling<Solution>(), new XorShift128(), params.islandThreads);
                }
                char* algo = new char[40];
                //sprintf(algo, "Island %d:\nGeneration", i);
                //island->addEvolutionObserver(new GenerationObserver<Solution>(algo));
                islands.push_back(island);
            }

            engine = new IslandsEvolutionEngine<Solution>(islands,params.epochLength,params.migrationCount,new RingMigration<Solution>(), new XorShift128(),true,params.islandEngineThreads);

        }
        else{


            PinballFactory* factory = new PinballFactory();
            std::vector<EvolutionaryOperator<Solution>*> operators;
            operators.push_back(new PinballCrossoverOperator(CANT_POINTS + CANT_AI_PARAMETERS,params.crossoverProbability)); //uniform crossover
            operators.push_back(new PinballMutationOperator(params.mutationProbability));
            EvolutionaryOperator<Solution>* pipeline = new EvolutionPipeline<Solution>(operators);
            if(params.enableMic){
                //FitnessEvaluator<Solution>* pe = new MicPinballEvaluator(&params,i);
                engine = new MicParallelEvolutionEngine<Solution>(params.populationSize, params.elitismPercentage, factory, pipeline, new StochasticUniversalSampling<Solution>(), new XorShift128(), params.islandThreads,0, &params);
            }else{
                FitnessEvaluator<Solution>* pe = new PinballEvaluator(&params,0);
                engine = new ParallelEvolutionEngine<Solution>(params.populationSize, params.elitismPercentage, factory, pipeline, pe, new StochasticUniversalSampling<Solution>(), new XorShift128());
            }

        }
        std::vector<TerminationCondition<Solution>*> tc;
        tc.push_back(new GenerationCount<Solution>(params.generations));
        //char* algo= new char[40];
        //sprintf(algo, "Epoch");
        //engine->addEvolutionObserver(new GenerationObserver<Solution>(algo));
        FinalStatistics<Solution>* finalResult = engine->evolve(tc);
        delete engine;
        return finalResult;
    }

int main(int argc, char *argv[]){
    char* configFilePath = argv[1];
    int evolutiveProcessID = atoi(argv[2]);
    try {
        Parameters params = loadParams(configFilePath, evolutiveProcessID);
        FinalStatistics<Solution>* finalResult = evolveSolutions(params);
        Solution* result = finalResult->data->bestCandidate;
        printf("Best solution: \n\n");
        printf("Fitness: %lf\nSolution: ",finalResult->data->bestCandidateFitness);
        result->print(params.instance);

        char path[100];
        sprintf(path, "%dsol.txt", params.evolutiveProcessID);


        ofstream solutionFile(path, std::ofstream::out);
        solutionFile << "Instance: " << params.instance << endl
                   << "Solution: ";

        result->print(solutionFile, params.instance);

        solutionFile << endl
                   << "Fitness: " << finalResult->data->bestCandidateFitness << endl
                   << "Elapsed time: " << finalResult->data->elapsedTime << "s" << endl
                   << "Mean fitness: " << finalResult->data->meanFitness << endl
                   << "Fitness standard deviaton: " << finalResult->data->fitnessStandardDeviation << endl
                   << "First generation to find best candidate: " << finalResult->data->bestCandidateFirstGeneration + 1 << endl;

        solutionFile.close();

        delete finalResult;

    } catch (exception ex) {
        std::cout << "An exception occurred. Code: " << ex.what() << endl;
    }
    return 0;
}
