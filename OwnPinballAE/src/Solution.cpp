#include "Solution.h"
#include <iostream>
#include "Constants.h"

Solution::Solution(std::vector<Genome*>* solution){
    this->solution = solution;
}

Solution::~Solution(){
    for(unsigned int i = 0; i < solution->size(); i++){
        delete (*solution)[i];
    }
    delete solution;
}

Solution* Solution::clone(){
    std::vector<Genome*>* otherSolution = new std::vector<Genome*>();
    for(unsigned int i = 0; i < solution->size(); i++){
        otherSolution->push_back((*solution)[i]->clone());
    }
    return new Solution(otherSolution);
}

std::vector<Genome*>* Solution::getSolution() const{
    return solution;
}

void Solution::setSolution(std::vector<Genome*>* solution){
    this->solution = solution;
}

bool Solution::operator==(const Solution &other) const{
    std::vector<Genome*>* otherSolution = other.getSolution();
    for(unsigned int i = 0; i < otherSolution->size(); i++){
        if((*otherSolution)[i] != (*solution)[i]){
            return false;
        }
    }
    return true;
}

bool Solution::operator!=(const Solution &other) const{
    return !(*this == other);
}

void Solution::print(int instance){
	for(int i = 0; i < CANT_POINTS; i++){
		std::cout << *(*solution)[i] << " ";
	}
	std::cout << instance;
    for (int i=CANT_POINTS; i < CANT_POINTS+CANT_AI_PARAMETERS;i++){
        std::cout << " " << *(*solution)[i];
    }
}

void Solution::print(std::ofstream& of, int instance){
	for(int i = 0; i < CANT_POINTS; i++){
		of << *(*solution)[i] << " ";
	}
	of << instance;
    for (int i=CANT_POINTS; i < CANT_POINTS+CANT_AI_PARAMETERS;i++){
        of << " " << *(*solution)[i];
    }
}
