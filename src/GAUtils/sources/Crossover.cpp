#include "Crossover.hpp"
#include "HamiltonianCycle.hpp"
#include "GPX2.hpp"
#include "Extra.hpp"
#include "Configs.hpp"


void Crossover::crossoverGPX(Population& pop){
    unsigned size{(unsigned)pop.getPop().size()};
    /* Population aux;
    for(unsigned i=0; i<size; i++) {
        Tour auxT{crossover(pop.getPop()[i], pop.getPop()[(i+1)%size])};
        aux.addNewTour(auxT);
    }
    aux.sortPop();
    return(aux); */
    Population fullPop;

    int test{0};
    for(unsigned i{0}; i<size; i++) {
        Tour nowT{pop.getPop()[i]};
        for(unsigned j{0}; j<size; j++) {
            if(i != j) {
                Tour offspring{Crossover::crossoverGPX(nowT, pop.getPop()[j])};
                nowT = offspring;
                test++;
            }
        }
        fullPop.addNewTour(nowT);
    }
    fullPop.sortPop();
    pop = fullPop;
}

Tour Crossover::crossoverGPX(Tour& red, Tour& blue){
    HamiltonianCycle::parentsHamiltonian parents{HamiltonianCycle::toHamiltonianCycle(red, blue)};
    Tour offspring{GPX2::crossover(parents.first, parents.second)};
    return(offspring);
}
void Crossover::crossoverOX(Population &pop){
    auto tours= OXCrossover::returnEletism(pop);
        for(unsigned i=0; i<pop.getPop().size(); i++){
        if(i==pop.getPop().size()-1){
            crossoverOX(pop.getPop()[i], pop.getPop()[0]);
        }else{
            crossoverOX(pop.getPop()[i], pop.getPop()[i+1]);
        }
    }    
    for(vector<int> i : tours){
        pop.getPop().push_back(i);
    }
}

void Crossover::crossoverOX(Tour& parent1, Tour& parent2){
    vector<int> tmp1{parent1.getRoute()}, tmp2{parent2.getRoute()};
    vector<int> aux1, aux2;
    int cutPoint{(int) parent1.getRoute().size()/2};
    int infLimit{(int) parent1.getRoute().size()};

    if(parent1.getRoute().size() != parent2.getRoute().size()){
        cutPoint = ((parent1.getRoute().size() + parent2.getRoute().size())/4);
        if(parent1.getRoute().size() > parent2.getRoute().size()){
            infLimit = parent2.getRoute().size();
        }
        if(infLimit < cutPoint){
            cutPoint = infLimit;
        }
    }

    for(int i=cutPoint; i<infLimit; i++){
        OXCrossover::swap(tmp1, i, OXCrossover::findElement(tmp1, parent2.getRoute()[i]));
        tmp1[i] = parent2.getRoute()[i];
        OXCrossover::swap(tmp2, i, OXCrossover::findElement(tmp2, parent1.getRoute()[i]));
        tmp2[i] = parent1.getRoute()[i];
    }
    parent1.getRoute() = tmp1;
    parent2.getRoute() = tmp2;    
}
//OX functions
void OXCrossover::swap(vector<int>& vector, const int a, const int b){
    int tmp = vector[a];
    vector[a]=vector[b];
    vector[b]=tmp;
}

int OXCrossover::findElement(vector<int>& vector, const int element){
    for(unsigned i=0; i<vector.size(); i++){
        if(vector[i] == element) return (i);
    }
    return(-1);
}

vector<vector<int>> OXCrossover::returnEletism(Population& pop){
    vector<vector<int>> elitismTours;
    unsigned i=0;
    while(i != Configs::elitismNumber){
        //int bestPos = getBestTour(pop, map).first;
        elitismTours.push_back(pop.getPop()[i].getRoute());
        pop.getPop().erase(pop.getPop().begin()+i);
        i++;
    }

    return(elitismTours);
}
//--------End OX Functions---------