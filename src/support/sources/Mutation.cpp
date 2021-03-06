#include <iterator>
#include <functional>
#include <algorithm>
#include <iostream>

#include "Mutation.hpp"
#include "Tour.hpp"
#include "TourUtils.hpp"
#include "Configs.hpp"


vector<int> Mutation::evaluateMutation(vector<int> tour){
    pair<vector<int>,bool> result;
    vector<int> initial = tour;
    int it=0;
    if(verifyList(tour)){
        return tour;
    }
    for (auto value: tour){
        auto moves = getMovesVector();
        for(auto move: moves){
            result=move(initial,it,value);
            if(result.second){
                return result.first;
            }
        }
        it++;
    }
    addList(tour);  
    //cout << "Tam Lista Mut: "<<sizeof(Configs::mutationMaxedList)<<endl;
    return tour;
}

    pair<vector<int>,bool> Mutation::firstMove(vector<int> tour,int it,int value){ 
    pair<vector<int>,bool> result=setInitialResult(tour);
    //cout<<"Fitness antes do movimento "<< iFitness <<endl;
    if(value!=Globals::customerMap.getDepotId()){ //Only does the move it isn't a depot   
        for(unsigned int i=0;i<result.first.size()-1;i++){
            if(result.first[i]==value){
                continue;
            }else{
                result.first.erase(find(result.first.begin(),result.first.end(),value));
                result.first.emplace(result.first.begin()+i,value);                
            }
            //printVector(result.first); //Debug
            result=basicFitnessEvaluation(Tour(tour),Tour(result.first));
            // printVector(result.first); //Debug
            if(result.second){
                break;
            }
        }                
    }else{
        result.second=false;
    }
    //cout<<"Fitness depois do movimento "<<bFitness<<endl;
    return result;
}



pair<vector<int>,bool> Mutation::secondAndThirdMove(vector<int> tour, int it,int value,int mode){
    //cout<<"iterator: " <<it <<endl;
    pair<vector<int>,bool> result=setInitialResult(tour);
    int valU=0;
    int valX=0;
    if(it==Globals::customerMap.getDepotId() || it==(int)tour.size()-1){
        return result;
    }else{
        for(unsigned int i=0;i<result.first.size()-1;i++){
                valU=result.first[i];
                valX=result.first[i+1];
                //cout<<"VALU: "<<valU << " VALX: "<< valX<< " IT: "<<it<<endl;
                if(valU==Globals::customerMap.getDepotId() || valX==Globals::customerMap.getDepotId() || valU==value || valX==value){
                    continue;
                }
                result.first.erase(find(result.first.begin(),result.first.end(),valU));
                result.first.erase(find(result.first.begin(),result.first.end(),valX));
                if(mode==2){
                    result.first.emplace(result.first.begin()+it,valU);
                    result.first.emplace(result.first.begin()+(it+1),valX);    
                }else{
                    result.first.emplace(result.first.begin()+it,valX);
                    result.first.emplace(result.first.begin()+(it+1),valU);    
                }
                
                // /printVector(result.first);
                result=basicFitnessEvaluation(Tour(tour),Tour(result.first));
                if(result.second){
                    return result;
                }
                result.first=tour;
        }
        result.second=false;
        return  result;
    }
}
pair<vector<int>,bool> Mutation::secondMove(vector<int> tour, int it,int value){
    return(secondAndThirdMove(tour,it,value,2));
}

pair<vector<int>,bool> Mutation::thirdMove(vector<int> tour, int it,int value){
    return(secondAndThirdMove(tour,it,value,3));
}
pair<vector<int>,bool> Mutation::fourthMove(vector<int> tour, int it,int value){
    pair<vector<int>,bool> result=setInitialResult(tour);
    int auxId=0;
    if(value!=Globals::customerMap.getDepotId()){
        for(unsigned int i=0; i<tour.size();i++){
            if(result.first[i]!=Globals::customerMap.getDepotId() && result.first[i]!=value){
                auxId=result.first[i];
                result.first[i]=value;
                result.first[it]=auxId;
                //printVector(result.first);
                result=basicFitnessEvaluation(Tour(tour),Tour(result.first));
                if(result.second){
                    return result;
                }
                result.first=tour;
            }
        
        }
    }
        return result;
}


pair<vector<int>,bool> Mutation::fifthMove(vector<int> tour, int it,int value){
    pair<vector<int>,bool> result=setInitialResult(tour);
    if(it<(int)tour.size()-1){
        int valU=0;
        int valX=0;
        if(value!=Globals::customerMap.getDepotId()){
            for(unsigned int i=0;i<tour.size()-1;i++){
                valU=result.first[i];
                valX=result.first[i+1];
                if(valU == Globals::customerMap.getDepotId() || valX == Globals::customerMap.getDepotId() ||valU==value ||valX==value){
                 continue;
                }
                //cout<< "Value U: "<<valU << " Value X: "<<valX<<endl;
                //cout<< "Result: "<<result.first[it]<<endl;

                result.first[it]=-result.first[it];
                
                result.first.emplace(find(result.first.begin(),result.first.end(),valU),value);

                result.first.erase(find(result.first.begin(),result.first.end(),valU));
                result.first.erase(find(result.first.begin(),result.first.end(),valX));

                result.first.emplace(find(result.first.begin(),result.first.end(),-value),valU);
                result.first.emplace(find(result.first.begin(),result.first.end(),-value),valX);

                result.first.erase(find(result.first.begin(),result.first.end(),-value));
                //printVector(result.first);
                result=basicFitnessEvaluation(Tour(tour),Tour(result.first));
                if(result.second){
                    return result;
                }
                result.first=tour;
            }
        }
    }
    return result;
}

pair<vector<int>,bool> Mutation::sixthMove(vector<int> tour, int it,int value){
    pair<vector<int>,bool> result=setInitialResult(tour);
    int valU=0,valX=0,valV=0,valY=0;
    if(value!=Globals::customerMap.getDepotId() && it<(int)(tour.size()-1)){
        valU=result.first[it];
        valX=result.first[it+1];
        for(unsigned int i=0; i< (tour.size()-1);i++){
            valV=result.first[i];
            valY=result.first[i+1];
            if(valV!=Globals::customerMap.getDepotId() && valY!=Globals::customerMap.getDepotId() && valX!=Globals::customerMap.getDepotId() 
            && valU!=valV && valU!=valX && valU!=valY 
            && valX!=valV && valX!=valY
            && valY!=valV){
                result.first=doubleSwap(result.first,it,i,valV,valY,valU,valX);
                result=basicFitnessEvaluation(Tour(tour),Tour(result.first));
                if(result.second){
                   // cout << "ValU: "<<valU << " ValX: "<<valX<<" ValV: "<<valV<<" ValY: "<<valY<<endl; 
                   // printVector(result.first);
                    return result;
                }
                result.first=tour;
            }
        }
    }
    return result;
}

pair<vector<int>,bool> Mutation::seventhAndEighthAndNineMove(vector<int> tour, int it,int value, int move){
    pair<vector<int>,bool> result=setInitialResult(tour);
    int ValU=0,ValX=0,ValV=0,ValY=0;
        if(value==Globals::customerMap.getDepotId() || it==(int)tour.size()-1){
            return result;
        }else{
            ValU=result.first[it];
            ValX=result.first[it+1];
            if(ValX!=ValU && ValX!=Globals::customerMap.getDepotId()){
                for(unsigned int i=0; i<tour.size()-1;i++){
                    ValV=result.first[i];
                    ValY=result.first[i+1];
                    if(ValX==Globals::customerMap.getDepotId()||ValV==Globals::customerMap.getDepotId() || ValY==Globals::customerMap.getDepotId() || ValU==ValV 
                    ||!(ValU!=ValV && ValU!=ValX && ValU!=ValY && ValX!=ValV && ValX!=ValY && ValY!=ValV)){
                        continue;
                    }
                    if(move==7){
                        if(!checkInSameTour(tour,Globals::customerMap.getDepotId(),ValU,ValV)){
                            continue;
                        }
                     }
                     else if(move==8 || move==9){
                        if(checkInSameTour(tour,Globals::customerMap.getDepotId(),ValU,ValV)){
                            continue;
                        }
                    }
                    if(move==7 || move==8){
                        result.first=doubleSwap(result.first,it,i,ValU,ValV,ValX,ValY);
                    }
                    else if(move==9){
                        result.first=doubleSwap(result.first,it,i,ValU,ValY,ValX,ValV);
                    }
                    result=basicFitnessEvaluation(Tour(tour),Tour(result.first));
                    if(result.second){
                        return result;
                    }
                }
            }
        }

    return result;
}
pair<vector<int>,bool> Mutation::seventhMove(vector<int> tour, int it,int value){
    return seventhAndEighthAndNineMove(tour,it,value,7);
}
pair<vector<int>,bool> Mutation::eighthMove(vector<int> tour, int it,int value){
    return seventhAndEighthAndNineMove(tour,it,value,8);
}
pair<vector<int>,bool> Mutation::ninethMove(vector<int> tour, int it,int value){
    return seventhAndEighthAndNineMove(tour,it,value,9);
}

vector<int> Mutation::doubleSwap(vector<int> tour,int currentValue,int iterationValue,int newValueU, 
    int newValueX,int newValueV,int newValueY){
    vector<int> swappedTour=tour;
    swappedTour[currentValue]=newValueU;
    swappedTour[currentValue+1]=newValueX;
    swappedTour[iterationValue]=newValueV;
    swappedTour[iterationValue+1]=newValueY;
    return swappedTour;
}

bool Mutation::checkInSameTour(vector<int> tour, int depotId, int customer1,int customer2){
    vector<vector<int>> tours=Tour(tour).explodeSubTours();
    bool flag1=false, flag2=false;
    for(auto tour: tours){
        for(auto customer:tour){
            if(customer==customer1){
                flag1=true;
            }
            if(customer==customer2){
                flag2=true;
            }
            if(flag1 && flag2){
                return true;
            }
        }
        flag1=false;
        flag2=false;
    }
    return false;
}

pair<vector<int>,bool> Mutation::basicFitnessEvaluation(Tour OldTour,Tour NewTour){
    pair<vector<int>,bool> result;
    result.first=OldTour.getRoute();
    result.second=false;
    if(NewTour.getFitness()<OldTour.getFitness()){
        result.first=NewTour.getRoute();
        result.second=true;
    }
    return result;
}

bool Mutation::validateTour(vector<int> tour, int depositNumber,int depositId){
    int valuesCont[tour.size()+1];
    for(unsigned i=0;i<tour.size();i++){
        valuesCont[i]=0;
    }
    for(auto value: tour){
        valuesCont[value]++;
    }

    for(unsigned i=0;i<=((tour.size()+1)-(depositNumber));i++){
        //cout<<"O valor "<<i<< " aparece "<< valuesCont[i] <<"vezes" <<endl;;
        if((i!=(unsigned)depositId && valuesCont[i]>1) ||(i!=0 && valuesCont[i]==0)){
            // cout <<"Dlaeu merda"<<endl;
            return false;
        }
    }
    return true;
}

pair<vector<int>,bool> Mutation::setInitialResult(vector<int> tour){
    pair<vector<int>,bool> result;
    result.first=tour;
    result.second=false;
    return result;
}

bool Mutation::verifyList(vector<int>& tour){
    sortList();
    Tour objTour= Tour(tour);
    for(auto listTour : MutationCtrl::mutationMaxedList){
        if(listTour==objTour){
            return true;
        }
    }
    return false;
}
void Mutation::addList(vector<int>& tour){
    MutationCtrl::mutationMaxedList.push_back(Tour(tour));
}

void Mutation::sortList(){
    std::sort(MutationCtrl::mutationMaxedList.begin(),MutationCtrl::mutationMaxedList.end(),
    [](Tour& a, Tour& b){
        return a.getFitness() > b.getFitness();
    });
}
vector<pair<vector<int>,bool> (*) (vector<int>,int,int)> Mutation::getMovesVector(){
    vector<pair<vector<int>,bool> (*) (vector<int>,int,int)> moves;
    moves.push_back(&firstMove);
    moves.push_back(&secondMove);
    moves.push_back(&thirdMove);
    moves.push_back(&fourthMove);
    moves.push_back(&fifthMove);
    moves.push_back(&sixthMove);
    moves.push_back(&seventhMove);
    moves.push_back(&eighthMove);
    moves.push_back(&ninethMove);
    return moves;
}
// Test functions used as sample presenting the TCC
vector<int> Mutation::testInsert(vector<int> tour){
    pair<vector<int>,bool> result;
    vector<int> initial = tour;
    int it=0;
    if(verifyList(tour)){
        return tour;
    }
    for (auto value: tour){
        vector<pair<vector<int>,bool> (*) (vector<int>,int,int)>  moves;
        moves.push_back(&firstMove);
        for(auto move: moves){
            result=move(initial,it,value);
            if(result.second){
                return result.first;
            }
        }
        it++;
    }
    addList(tour);  
    //cout << "Tam Lista Mut: "<<sizeof(Configs::mutationMaxedList)<<endl;
    return tour;
}

vector<int> Mutation::testSwap(vector<int> tour){
    pair<vector<int>,bool> result;
    vector<int> initial = tour;
    int it=0;
    if(verifyList(tour)){
        return tour;
    }
    for (auto value: tour){
        vector<pair<vector<int>,bool> (*) (vector<int>,int,int)>  moves;
        moves.push_back(&fourthMove);
        for(auto move: moves){
            result=move(initial,it,value);
            if(result.second){
                return result.first;
            }
        }
        it++;
    }
    addList(tour);  
    //cout << "Tam Lista Mut: "<<sizeof(Configs::mutationMaxedList)<<endl;
    return tour;
}