#ifndef CONFIGS_HPP
#define CONFIGS_HPP
#include "CustomerMap.hpp"
#include "Tour.hpp"
namespace Configs {
    //Basic Settings
    //Truck number to be used
    extern unsigned truckNumber;
    //Size of the populations used
    extern unsigned popSize;
    //Customer map to be used among the entire program
    extern CustomerMap customerMap;

    //Fitness params
    //Best of the first pop
    extern double initialBest;
    //Max number of iterations
    extern unsigned maxIterations;
    //Fitness mode, value must be 1 or 2
    extern int fitnessMode;
    //Iteration counter to be used by the fitness function
    extern unsigned currentIteration;
    
    //Initial population settings
    //Quantity of iterations to be realized on the initial population
    extern unsigned InitialPopmutIterations;
    //Percentage of initial population that will receive mutation
    extern unsigned InitialPopMutRate;
    
    //Loop Mutation settings
    //Amount of populations that will receive mutation
    extern unsigned mutationRate;
    //true applies mutation to the best, false applies mutation to the worst
    extern bool applyWorst; 

    //Mutation improvements settings
    //Store tours that can't be improved anymore
    extern vector<Tour> mutationMaxedList;

    //Reset Settings
    //Maintain the n best tours during reset
    extern int nBestToKeep;
    //Defines how many iterations of mutation will be applied during reset
    extern unsigned resetMutIterations;
}

#endif