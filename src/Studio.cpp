#include "../include/Studio.h"
#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>
extern Studio *backup;
/*
** Studio
*/

// Default Constructor
Studio::Studio():open(true),trainers({}),workout_options({}),actionsLog({}){}

// Constructor
Studio::Studio(const std::string &configFilePath):open(true),trainers({}),workout_options({}),actionsLog({}){
    std::ifstream infile(configFilePath);
    std::string line;
    std::vector<int> trainersCapacities;
    std::vector<int> work;
    // get number of trainers
    while (std::getline(infile, line)) {
        // if line starts with #,or line is empty, skip line
        if((line.rfind('#',0) == 0 ) || line.empty()){continue;}
        // line is valid
        else{
            break;// break after one valid line
        }
    }
    // get trainers' capacities
    while (std::getline(infile, line)) {
        // if line starts with #, or line is empty, skip line
        if(line.rfind('#',0) == 0 || line.empty()){continue;}
        // line is valid
        else{
            std::istringstream ss(line);
            std::string trainerCapacity;
            while(std::getline(ss, trainerCapacity, ',')) {
                int capacity= stoi(trainerCapacity);
                trainers.push_back(new Trainer(capacity));
            }
            break;// break after one valid line
        }
    }
    int workoutId =0;
    // get workout options
    while (std::getline(infile, line)) {
        // if line starts with #, or line is empty, skip line
        if(line.rfind('#',0) == 0 || line.empty()){continue;}
        // line is valid
        else{
            // retrieve wname
            std::string wname = "";
            int i =0;
            while(line[i] != ','){
                wname+=line[i];
                i++;
            }
            i+=2;
            // retrieve type
            std::string wtype_str="";
            while(line[i] != ','&& line[i]){
                wtype_str+=line[i];
                i++;
            }
            i+=2;
            // map str to enum
            WorkoutType wtype;
            if (wtype_str == "Anaerobic"){wtype = ANAEROBIC;}
            else if (wtype_str == "Mixed"){wtype = MIXED;}
            else if (wtype_str == "Cardio"){wtype = CARDIO;}
            // retrieve price
            std::string price_str="";
            while(line[i]){
                price_str+=line[i];
                i++;
            }
            //change string to int
            int wprice = stoi(price_str);
            workout_options.push_back(Workout(workoutId, wname, wprice, wtype));
            workoutId+=1;
        }
    }
}

// Destructor
Studio::~Studio(){
    for(size_t i = 0; i < trainers.size(); ++i) {
        if(trainers[i])
            delete trainers[i];
    }
    
    for(size_t i = 0; i < actionsLog.size(); ++i) {
        if(actionsLog[i])
            delete actionsLog[i];
    }    
}

//Copy Constructor
Studio::Studio( Studio &other):open(other.open),trainers(),workout_options(other.getWorkoutOptions()),actionsLog(){
    //deep copy trainers
    for(size_t i = 0; i < other.trainers.size(); ++i) {
        trainers.push_back(new Trainer(*other.trainers[i]));
    }

    std::vector<BaseAction*> otherActionsLog=other.getActionsLog();
   
    for(size_t i = 0; i < otherActionsLog.size(); ++i) {
        actionsLog.push_back(otherActionsLog[i]->clone());
    }
    
}

// Copy Assignment
Studio& Studio::operator=(const Studio &other){
    // check for "self assignment" and do nothing in that case
    if(this == &other){
        return *this;
    }
    
    open = other.open;

    for(size_t i = 0; i < trainers.size(); ++i) {
        delete trainers[i];
    }
    trainers.clear();
    for(size_t i = 0; i < other.trainers.size(); ++i) {
        trainers.push_back(new Trainer(*other.trainers[i]));
    }

    workout_options.clear();
    for(size_t i = 0; i < other.workout_options.size(); ++i) {
        workout_options.push_back(other.workout_options[i]);
    }

    for(size_t i = 0; i < actionsLog.size(); ++i) {
        delete actionsLog[i];
    }
    
    actionsLog.clear();
    for(size_t i = 0; i < other.actionsLog.size(); ++i) {
        actionsLog.push_back(other.actionsLog[i]->clone());
    }

    return *this;
}
// Move Constructor
Studio::Studio( Studio &&other):open(other.open),trainers(other.trainers),workout_options(other.workout_options),actionsLog(other.actionsLog){
    other.open = false;
    other.trainers.clear();
    other.workout_options.clear();
    other.actionsLog.clear();
}

// Move Assignment
Studio& Studio::operator=(Studio &&other){
    if (this != &other)
    {
        open = other.open;
        trainers = other.trainers;
        // workout_options = other.workout_options;
        for (size_t i = 0; i < other.workout_options.size(); i++){
            workout_options.push_back(other.workout_options[i]);
        }
        actionsLog = other.actionsLog;

        other.trainers.clear();
        other.workout_options.clear();
        other.actionsLog.clear();
    }    
    return *this;
}


//start
void Studio::start(){
    std::cout << "Studio is now open!" << std::endl;

    int customerId = 0; 
    while(1){
        std::string line;
        std::getline(std::cin ,line);
        std::istringstream ss (line);
        std::string actionName;
        ss >> actionName;

        //OpenTrainer
        if(actionName=="open"){
            //get trainer ID
            int trainerId;
            ss >> trainerId;
            //get trainer Capacity
            int trainerCapacity = getTrainer(trainerId)->getCapacity();
            //create new customerlist
            std::vector<Customer *> customerList;           
            // get <customerName,customerStrategy>
            std::string pair;
            while(ss>> pair && trainerCapacity > 0){
                std::string customerName =  pair.substr(0, pair.find(","));
                std::string customerStrategy = pair.substr(pair.find(",")+1,pair.size()-1);
                //check customer strategy
                // Sweaty Customer
                Customer *c;
                if(customerStrategy == "swt"){
                    c = new SweatyCustomer(customerName,customerId);
                    customerList.push_back(c);
                }
                // Cheap Customer
                else if(customerStrategy == "chp"){
                    c = new CheapCustomer(customerName,customerId);
                    customerList.push_back(c);
                }
                // Heavy Muscle Customer
                else if(customerStrategy == "mcl"){
                    c = new HeavyMuscleCustomer(customerName,customerId);
                    customerList.push_back(c);
                }
                // Full Body Customer
                else if(customerStrategy == "fbd"){
                    c = new FullBodyCustomer(customerName,customerId);
                    customerList.push_back(c);
                }
                customerId+=1;
                trainerCapacity-=1;
                c=nullptr;
            }
            OpenTrainer *t=  new OpenTrainer(trainerId, customerList);
            t->act(*this);
            actionsLog.push_back(t);
        }

        //Order
        else if(actionName=="order"){
            std::string id;
            ss>>id;
            Order *o = new Order(std::stoi(id));
            o->act(*this);
            actionsLog.push_back(o);
        }

        //MoveCustomer
        else if(actionName=="move"){
            std::string srcTrainer;
            std::string dstTrainer;
            std::string id;
            ss>>srcTrainer;
            ss>>dstTrainer;
            ss>>id;
            MoveCustomer *m = new MoveCustomer(stoi(srcTrainer),stoi(dstTrainer),stoi(id));
            m->act(*this);
            actionsLog.push_back(m);
        }

        //Close
        else if(actionName=="close"){
            int trainerId;
            ss >> trainerId;
            Close *c = new Close (trainerId);
            c->act(*this);
            actionsLog.push_back(c);
        }

        //CloseAll
        else if(actionName=="closeall"){
            CloseAll *c = new CloseAll();
            //actionsLog.push_back(c);
            c->act(*this);
            delete c;
            break;
        }

        //PrintWorkoutOptions
        else if(actionName=="workout_options"){
            PrintWorkoutOptions *w = new PrintWorkoutOptions();
            w->act(*this);
            actionsLog.push_back(w);   
        }

        //PrintTrainerStatus
        else if(actionName=="status"){
            int trainerId;
            ss >> trainerId;
            PrintTrainerStatus *ts= new PrintTrainerStatus (trainerId);
            ts->act(*this);
            actionsLog.push_back(ts);
        }

        //PrintActionsLog
        else if(actionName=="log"){
            PrintActionsLog *p = new PrintActionsLog();
            p->act(*this);
            actionsLog.push_back(p);
        }

        //BackupStudio
        else if(actionName=="backup"){
            BackupStudio *b = new BackupStudio();
            b->act(*this);
            actionsLog.push_back(b);
            backup->addBackupTolog(b->clone());
        }

        //RestoreStudio
        else if(actionName=="restore"){
            RestoreStudio *r = new RestoreStudio();
            r->act(*this);
            actionsLog.push_back(r);
        } 
    }
}

//getNumOfTrainers
int Studio::getNumOfTrainers() const{return trainers.size();}

//getTrainer
Trainer* Studio::getTrainer(int tid){
    if(tid >= getNumOfTrainers()){
       return nullptr;
    }
    return trainers[tid];
}

//getActionsLog
const std::vector<BaseAction*>& Studio::getActionsLog() const{return actionsLog;}

//getWorkoutOptions
std::vector<Workout>& Studio::getWorkoutOptions(){return workout_options;}

void Studio::addBackupTolog(BaseAction *b){
    actionsLog.push_back(b);
}


