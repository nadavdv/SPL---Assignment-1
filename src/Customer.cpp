#include "../include/Customer.h"
#include <algorithm>

/*
** Customer
*/

// Constructor
Customer::Customer(std::string c_name, int c_id):name(c_name), id(c_id){}

// Destructor
Customer::~Customer() {}

//getName
std::string Customer::getName()const{return name;}

//getId
int Customer::getId()const{return id;}

//Workout Comparator - expansive first
bool expensiveFirst( const Workout *a, const Workout *b){
    return (a->getPrice() > b->getPrice());
}

// get the ID of Cheapest workout with minimal ID of WorkoutType t
int minId(const std::vector<Workout> &workout_options, WorkoutType t){
    int LowestPrice=-1;
    int CheapestWorkoutId=-1;
    for (size_t i = 0; i < workout_options.size(); ++i) {
        if(workout_options[i].getType()==t){
            if(LowestPrice==-1){
                LowestPrice= workout_options[i].getPrice();
                CheapestWorkoutId = workout_options[i].getId();
            }
            else
                if (workout_options[i].getPrice() < LowestPrice) {
                    LowestPrice = workout_options[i].getPrice();
                    CheapestWorkoutId = workout_options[i].getId();
                }

        }
    }
    return CheapestWorkoutId;
}
// get the ID of Most Expensive workout with minimal ID of WorkoutType t
int maxId(const std::vector<Workout> &workout_options, WorkoutType t){
    int HighestPrice=-1;
    int expensiveWorkoutId=-1;
    for (size_t i = 0; i < workout_options.size(); ++i) {
        if(workout_options[i].getType()==t){
            if(HighestPrice==-1){
                HighestPrice= workout_options[i].getPrice();
                expensiveWorkoutId = workout_options[i].getId();
            }
            else
            if (workout_options[i].getPrice() > HighestPrice) {
                HighestPrice = workout_options[i].getPrice();
                expensiveWorkoutId = workout_options[i].getId();
            }

        }
    }
    return expensiveWorkoutId;
}
/*
** SweatyCustomer
*/

// Constructor
SweatyCustomer::SweatyCustomer(std::string c_name, int c_id):Customer(c_name, c_id){}

// Destructor
SweatyCustomer::~SweatyCustomer(){}

// clone 
Customer* SweatyCustomer::clone () const{
    return new SweatyCustomer(*this);
}

// order
std::vector<int> SweatyCustomer::order(const std::vector<Workout> &workout_options){
    std::vector<int> orders;
    for (size_t i = 0; i < workout_options.size(); ++i) {
        if(workout_options[i].getType() == CARDIO){
            orders.push_back(workout_options[i].getId());
        }
    }
    return orders;
}

//toString
std::string SweatyCustomer::toString() const{return getName()+",swt";}

/*
** CheapCustomer
*/

//Constructor
CheapCustomer::CheapCustomer(std::string c_name, int c_id):Customer(c_name, c_id){}

//Destructor
CheapCustomer::~CheapCustomer(){}

// clone 
Customer* CheapCustomer::clone () const{
    return new CheapCustomer(*this);
}

// order
std::vector<int> CheapCustomer::order(const std::vector<Workout> &workout_options){
    std::vector<int> orders;
    int LowestPrice=workout_options[0].getPrice();
    int CheapestWorkoutId=workout_options[0].getId();
    for (size_t i = 0; i < workout_options.size(); ++i) {
        if(workout_options[i].getPrice()<LowestPrice){
            LowestPrice = workout_options[i].getPrice();
            CheapestWorkoutId = workout_options[i].getId();
        }
    }
    orders.push_back(CheapestWorkoutId);
    return orders;
}

//toString
std::string CheapCustomer::toString() const{return getName()+",chp";}

/*
** HeavyMuscleCustomer
*/

//Constructor
HeavyMuscleCustomer::HeavyMuscleCustomer(std::string c_name, int c_id):Customer(c_name, c_id){}

//Destructor
HeavyMuscleCustomer::~HeavyMuscleCustomer(){}

// clone 
Customer* HeavyMuscleCustomer::clone () const{
    return new HeavyMuscleCustomer(*this);
}

// order
std::vector<int> HeavyMuscleCustomer::order(const std::vector<Workout> &workout_options){
   
    std::vector<Workout*> workout_options_copy;
     
    for(size_t i = 0; i < workout_options.size(); ++i){
        workout_options_copy.push_back(new Workout(workout_options[i]));
    }
    
    std::stable_sort(workout_options_copy.begin(), workout_options_copy.end(),expensiveFirst);
    
    std::vector<int> orders;
    for (size_t i = 0; i < workout_options_copy.size(); ++i) {
        if (workout_options_copy[i]->getType() == ANAEROBIC) {
            orders.push_back(workout_options_copy[i]->getId());
        }
    }
    // freeing the copy
    for(size_t i = 0; i < workout_options.size(); ++i){
        delete workout_options_copy[i];
        workout_options_copy[i] = nullptr;
    }
    workout_options_copy.clear();
    return orders;
}

//toString
std::string HeavyMuscleCustomer::toString() const{
    return getName()+",mcl";
}

/*
** FullBodyCustomer
*/

// Constructor
FullBodyCustomer::FullBodyCustomer(std::string c_name, int c_id):Customer(c_name, c_id){}

// Destructor
FullBodyCustomer::~FullBodyCustomer(){}

// clone 
Customer* FullBodyCustomer::clone () const{
    return new FullBodyCustomer(*this);
}

// order
std::vector<int> FullBodyCustomer::order(const std::vector<Workout> &workout_options){
    std::vector<int> orders;
    orders.push_back(minId(workout_options,CARDIO));
    orders.push_back(maxId(workout_options,MIXED));
    orders.push_back(minId(workout_options,ANAEROBIC));
    return orders;
}

//toString
std::string FullBodyCustomer::toString() const{return getName()+",fbd";}