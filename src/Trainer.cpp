#include "../include/Trainer.h"

/*
** Trainer
*/

// Constructor
Trainer::Trainer():capacity(-1),salary(0),open(false),customersList(),orderList(){}

// Constructor
Trainer::Trainer(int t_capcitiy):capacity(t_capcitiy),salary(0),open(false),customersList(),orderList(){}

// Destructor
Trainer::~Trainer(){
    closeTrainer();
    customersList.clear();
    orderList.clear();
}

// Copy Constructor
Trainer::Trainer(Trainer &other):capacity(other.capacity),salary(other.salary),open(other.open),customersList(),orderList(){
    for(size_t i = 0; i < other.customersList.size(); ++i){
        customersList.push_back(other.customersList[i]->clone());
    }
    for(size_t i = 0; i < other.orderList.size(); ++i){
        OrderPair op (other.orderList[i].first,other.orderList[i].second);
        orderList.push_back(op);
    }
}

// Copy Assignment
Trainer& Trainer::operator=(const Trainer &other){
    capacity = other.capacity;
    salary = other.capacity;
    open = other.open;
    for(size_t i = 0; i < other.customersList.size(); ++i){      
        customersList.push_back(other.customersList[i]->clone());
    }
    for(size_t i = 0; i < other.orderList.size(); ++i){
        OrderPair op (other.orderList[i].first,other.orderList[i].second);
        orderList.push_back(op);
    }
    return *this;
}

// Move Constructor
Trainer::Trainer( Trainer &&other):capacity(other.capacity),salary(other.salary),open(other.open),customersList(other.customersList),orderList(other.orderList){
    other.capacity = -1;
    other.salary= -1;
    other.open = false;
    other.customersList.clear();
    other.orderList.clear();
}

// Move Assignment
Trainer& Trainer::operator=(Trainer &&other){
    if (this != &other)
    {
        
        capacity = other.capacity;
        salary = other.salary;
        open = other.open;
        customersList = other.customersList;
        // orderList = other.orderList;
        for (size_t i = 0; i < other.orderList.size(); i++){
            orderList.push_back(other.orderList[i]);
        }
        other.capacity = -1;
        other.salary= -1;
        other.open = false;
        other.customersList.clear();
        other.orderList.clear();
    }    
    return *this;
}

// getCapacity
int Trainer::getCapacity()const{return capacity;}

// addCustomer
void Trainer::addCustomer(Customer* customer){
    if(capacity > static_cast<int>(customersList.size()))
        customersList.push_back(customer);
    else{
        throw "Trainer is Full";
    }
}

// removeCustomer
void Trainer::removeCustomer(int id){
    for (size_t i = 0; i < customersList.size(); ++i) {
        if(customersList[i]->getId() == id){
            customersList.erase(customersList.begin() + i);
            break;
        }
    }
    // remove customer's active orders
    std::vector<OrderPair> newOrderList;
    for (size_t i = 0; i < orderList.size(); i++){
        if(orderList[i].first != id){
            newOrderList.push_back(orderList[i]);
        }
        else{
            salary-= orderList[i].second.getPrice();
        }
    }
    orderList.clear();
    for (size_t j = 0; j < newOrderList.size(); j++){
        orderList.push_back(newOrderList[j]);
    }
    // close trainer if no customers left
    if(customersList.size()==0)
        closeTrainer();
}

// getCustomer
Customer* Trainer::getCustomer(int id){
    for (size_t i = 0; i < customersList.size(); ++i) {
        if(customersList[i]->getId() == id){
            return customersList[i];
        }
    }
    return nullptr;
}

// getCustomers
std::vector<Customer*>& Trainer::getCustomers(){return customersList;}

// getOrders
std::vector<OrderPair>& Trainer::getOrders(){return orderList;}

// order
void Trainer::order(const int customer_id, const std::vector<int> workout_ids, const std::vector<Workout>& workout_options){
    for(size_t i=0; i < workout_ids.size();++i){
        OrderPair op  (customer_id,workout_options[workout_ids[i]]);
        salary+= op.second.getPrice();
        orderList.push_back(op);
    }
    
}

// openTrainer
void Trainer::openTrainer(){open = true;}

// closeTrainer
void Trainer::closeTrainer(){
    open = false;
    for(size_t i = 0; i < customersList.size(); i++){
        delete customersList[i];
    }
    customersList.clear();
    orderList.clear();
}
// getSalary
int Trainer::getSalary(){return salary;}

// isOpen
bool Trainer::isOpen(){return open;}

//move Orders
void Trainer::moveOrders(Trainer* dst, int cId){
    for(size_t i=0; i< orderList.size(); i++) {
        if(orderList[i].first==cId){
            dst->orderList.push_back(orderList[i]);
            dst->salary+= orderList[i].second.getPrice();
        }
    }
};