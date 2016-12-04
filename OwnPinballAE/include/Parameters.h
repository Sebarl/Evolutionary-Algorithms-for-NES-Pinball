#ifndef PARAMETERS_H
#define PARAMETERS_H

struct Parameters{
    int generations;
    int populationSize;
    double crossoverProbability;
    double mutationProbability;
    int elitismPercentage;
    int islandCount;
    int epochLength;
    int migrationCount;
    int frameCount;
    int instance;
    int evolutiveProcessID;
    int islandThreads;
    int islandEngineThreads;
    bool enableIslands;
    bool enableMic;
};

#endif // PARAMETERS_H
