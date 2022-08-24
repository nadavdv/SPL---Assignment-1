#include "../include/Action.h"
#include "../include/Studio.h"
#include <string>


/*
** BaseAction
*/
// Constructor
BaseAction::BaseAction():errorMsg(""),comment(""), status(ERROR){}
// Destructor
BaseAction::~BaseAction(){}

// getStatus
ActionStatus BaseAction::getStatus() const{return status;}

// complete
void BaseAction::complete(){
    status = COMPLETED;
    // log action completed
    comment = " Completed";   
}

// error
void BaseAction::error(std::string errorMsg){
    status = ERROR;
    this->errorMsg = errorMsg;
    std::cout << "Error: " << getErrorMsg() << std::endl;
    // log error
    comment =  "Error: " +getErrorMsg();
}

// getErrorMsg
std::string BaseAction::getErrorMsg() const{return errorMsg;}

// getComment()
std::string BaseAction::getComment() const{return comment;}

/*
** OpenTrainer
*/

// Constructor
OpenTrainer::OpenTrainer(int id, std::vector<Customer *> &customersList):BaseAction(),trainerId(id),customersListStr(""),customers(customersList){}

// clone 
BaseAction* OpenTrainer::clone () const{
    return new OpenTrainer(*this);
}

// act
void OpenTrainer::act(Studio &studio){
    Trainer* trainer = studio.getTrainer(trainerId);
    //check if trainer is open
    if( trainer == nullptr || trainer->isOpen()){
        error("Workout session does not exist or is already open.");
        // free the memory of the new customers that werent added
        for (size_t i = 0; i < customers.size(); i++){
            delete customers[i];
            customers[i] = nullptr;
        }
        //clear customers vector
        customers.clear();
    }
    // trainer is open
    else{
        for(size_t i = 0; i < customers.size(); ++i){
            trainer->addCustomer(customers[i]);
            customersListStr += customers[i]->toString()+" ";
        }
        trainer->openTrainer();
        //clear customers vector
        customers.clear();
        complete();
    }
}
// toString
std::string OpenTrainer::toString() const{
    std::string str = "open ";
    str += std::to_string(trainerId) + " ";
    return str+customersListStr ;
}

/*
** Order
*/

// Constructor
Order::Order(int id):BaseAction(),trainerId(id){}

// clone 
BaseAction* Order::clone () const{
    return new Order(*this);
}

// act 
void Order::act(Studio &studio){
    Trainer* trainer = studio.getTrainer(trainerId);
    //check if trainer is open
    if(trainer == nullptr|| trainer->isOpen()== false ){
        error("Trainer does not exist or is not open.");
    }
    else{
        std::vector<Customer*> customersList = studio.getTrainer(trainerId)->getCustomers();
        for(size_t i = 0; i < customersList.size(); ++i){
            std::vector<Workout> workout_options = studio.getWorkoutOptions();
            std::vector<int> orderList = customersList[i]->order(workout_options);
            int customerId = customersList[i]->getId();
            trainer->order(customerId, orderList,workout_options);
            for (size_t j = 0; j < orderList.size(); j++){
                std::cout << customersList[i]->getName() << " Is Doing "<< workout_options[orderList[j]].getName()<< std::endl;
            }
            orderList.clear();
        }            
        customersList.clear();
        complete();
    }
}

// toString
std::string Order::toString() const{return "order "+std::to_string(trainerId);}

/*
** MoveCustomer
*/

// Constructor
MoveCustomer::MoveCustomer(int src, int dst, int customerId):BaseAction(),srcTrainer(src),dstTrainer(dst),id(customerId){}

// clone 
BaseAction* MoveCustomer::clone () const{
    return new MoveCustomer(*this);
}

// act 
void MoveCustomer::act(Studio &studio){
    Trainer* trainerSource = studio.getTrainer(srcTrainer);
    Trainer* trainerDest = studio.getTrainer(dstTrainer);
    Customer* c = trainerSource->getCustomer(id);
    //check if trainer is open
    if(trainerSource == nullptr || trainerSource->isOpen() == false || trainerDest == nullptr || trainerDest->isOpen() == false   || trainerSource->getCustomer(id) == nullptr){
        error("Cannot move customer.");
    }
    // try to add customer to destination trainer
    else{
        try{
            trainerDest->addCustomer(c);
            trainerSource->moveOrders(trainerDest,id);
            trainerSource->removeCustomer(id);
            complete();
        }catch(const char* msg){
            error("Cannot move customer.");
        }
    } 
}

//toString
std::string MoveCustomer::toString() const{return ("move "+ std::to_string(srcTrainer)+" "+std::to_string(dstTrainer)+" "+std::to_string(id)) ;}       

/*
** PrintWorkoutOptions
*/
// Constructor
PrintWorkoutOptions::PrintWorkoutOptions():BaseAction(){}

// clone 
BaseAction* PrintWorkoutOptions::clone () const{
    return new PrintWorkoutOptions(*this);
}


// act
void PrintWorkoutOptions::act(Studio &studio){
    std::vector<Workout> workout_options = studio.getWorkoutOptions();
    for (size_t i =0; i < workout_options.size(); ++i){
        WorkoutType wtype = workout_options[i].getType();
        std::string wtypeToString;
        if (wtype == ANAEROBIC) {wtypeToString = "Anaerobic";}
        else if (wtype == MIXED){wtypeToString = "Mixed";}
        else if (wtype == CARDIO){wtypeToString = "Cardio";}
        std::cout << workout_options[i].getName()+", "+ wtypeToString+ ", " +std::to_string(workout_options[i].getPrice())<<std::endl;
    }
    complete();
}
//toString
std::string PrintWorkoutOptions::toString() const{return "workout_options" ;}    

/*
** PrintTrainerStatus
*/

// Constructor
PrintTrainerStatus::PrintTrainerStatus(int id):BaseAction(),trainerId(id){}

// clone 
BaseAction* PrintTrainerStatus::clone () const{
    return new PrintTrainerStatus(*this);
}


// act
void PrintTrainerStatus::act(Studio &studio){
    Trainer* trainer = studio.getTrainer(trainerId);
    std::vector<Customer*> customersList = trainer->getCustomers();
    std::vector<OrderPair> orderList = trainer->getOrders();
    //check trainer's status
    std::string status="";
    if(trainer->isOpen()){
        status="open";
        //print trainer's status
        std::cout <<"Trainer " + std::to_string(trainerId) + " status: " + status << std::endl;
        //print customers 
        std::cout <<"Customers: " << std::endl;
        for(size_t i=0; i<customersList.size(); ++i){
            std::cout << std::to_string(customersList[i]->getId()) + " " + customersList[i]->getName() << std::endl; 
        }
        //print orders 
        std::cout <<"Orders:" << std::endl;
        for(size_t i=0; i<orderList.size(); ++i) {
            std::cout<< orderList[i].second.getName() << " " << orderList[i].second.getPrice() << "NIS " <<orderList[i].first << std::endl;
        }
        std::cout <<"Current Trainer’s Salary: " << trainer->getSalary() << " NIS" << std::endl;
    }
    else{
        status="closed";
        //print trainer's status
        std::cout <<"Trainer " + std::to_string(trainerId) + " status: " + status << std::endl;
    }
    complete();
}

// toString
std::string PrintTrainerStatus::toString() const{return "status"+ std::to_string(trainerId);}

/*
** PrintActionsLog
*/

// Constructor
PrintActionsLog::PrintActionsLog():BaseAction(){}

// clone 
BaseAction* PrintActionsLog::clone () const{
    return new PrintActionsLog(*this);
}

// act
void PrintActionsLog::act(Studio &studio){
    std::vector<BaseAction*> actionsLog = studio.getActionsLog();
    std::string comment = " ";
    for (size_t i = 0; i < actionsLog.size(); i++){
        std::cout << actionsLog[i]->toString() << " "<< actionsLog[i]->getComment()<<"\n";
    }
    

    complete();
}

// toString
std::string PrintActionsLog::toString() const{return "log" ;}

/*
** Close
*/

// Constructor 
Close::Close(int id):BaseAction(),trainerId(id){}

// clone 
BaseAction* Close::clone () const{
    return new Close(*this);
}

// act
void Close::act(Studio & studio){
    Trainer *trainer = studio.getTrainer(trainerId);
    if(trainer == nullptr||trainer->isOpen() == false){
        error("Trainer does not exist or is not open");
    }
    else{
        std::cout << "Trainer " + std::to_string(trainerId) + " closed. " + "Salary " + std::to_string(trainer->getSalary())+"NIS" << std::endl;
        trainer->closeTrainer();
        complete();
    }
}
// toString
std::string Close::toString() const{return "close "  +std::to_string(trainerId);}

/*
** CloseAll
*/
// Constructor
CloseAll::CloseAll():BaseAction(){}

// clone 
BaseAction* CloseAll::clone () const{
    return new CloseAll(*this);
}


// act
void CloseAll::act(Studio & studio){
    for(int i =0; i < studio.getNumOfTrainers();++i){
        Trainer *trainer = studio.getTrainer(i);
        if(trainer != nullptr && trainer->isOpen()){
            std::cout << "Trainer " + std::to_string(i) + " closed. " + "Salary " + std::to_string(trainer->getSalary())+"NIS" << std::endl;
        }
    }
    std::cout << "Studio is now closed!" << std::endl;
    complete();    
}
// toString
std::string CloseAll::toString() const{return "closeall" ;}

/*
** BackupStudio
*/

//Constructor
BackupStudio::BackupStudio():BaseAction(){}

// clone 
BaseAction* BackupStudio::clone () const{
    return new BackupStudio(*this);
}


//act
void BackupStudio::act(Studio & studio){
    if(backup!= nullptr){
        delete backup;
    }
    backup = new Studio(studio);
    complete();
}

//toString()
std::string BackupStudio::toString() const{return "backup" ;}

/*
** RestoreStudio
*/

//Constructor
RestoreStudio::RestoreStudio():BaseAction(){}

// clone 
BaseAction* RestoreStudio::clone () const{
    return new RestoreStudio(*this);
}


//act
void RestoreStudio::act(Studio & studio){
    if(backup){
        studio = *backup;
        complete();
    }
    else
        error("No backup available");
}

//toString()
std::string RestoreStudio::toString() const{return "restore" ;}