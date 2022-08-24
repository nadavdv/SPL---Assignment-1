#ifndef ACTION_H_
#define ACTION_H_

#include <string>
#include <iostream>
#include "Customer.h"


enum ActionStatus{
    COMPLETED, ERROR
};


//Forward declaration
class Studio;


extern Studio *backup;

class BaseAction{
public:
    BaseAction();
    virtual BaseAction* clone() const = 0;
    virtual~BaseAction();
    ActionStatus getStatus() const;
    virtual void act(Studio& studio)=0;
    virtual std::string toString() const=0;
    std::string getComment()const;
    //void log(Studio& studio,std::string userInput);
protected:
    void complete();
    void error(std::string errorMsg);
    std::string getErrorMsg() const;
private:
    std::string errorMsg;
    std::string comment;
    ActionStatus status;
    
};


class OpenTrainer : public BaseAction {
public:
    OpenTrainer(int id, std::vector<Customer *> &customersList);
    BaseAction* clone() const ;
    void act(Studio &studio);
    std::string toString() const;
private:
	const int trainerId;
    std::string customersListStr;
	std::vector<Customer *> customers;
};


class Order : public BaseAction {
public:
    Order(int id);
    BaseAction* clone() const ;
    void act(Studio &studio);
    std::string toString() const;
private:
    const int trainerId;
};


class MoveCustomer : public BaseAction {
public:
    MoveCustomer(int src, int dst, int customerId);
    BaseAction* clone() const ;
    void act(Studio &studio);
    std::string toString() const;
private:
    const int srcTrainer;
    const int dstTrainer;
    const int id;
};


class Close : public BaseAction {
public:
    Close(int id);
    BaseAction* clone() const ;
    void act(Studio &studio);
    std::string toString() const;
private:
    const int trainerId;
};


class CloseAll : public BaseAction {
public:
    CloseAll();
    BaseAction* clone() const ;
    void act(Studio &studio);
    std::string toString() const;
private:
};


class PrintWorkoutOptions : public BaseAction {
public:
    PrintWorkoutOptions();
    BaseAction* clone() const ;
    void act(Studio &studio);
    std::string toString() const;
private:
};


class PrintTrainerStatus : public BaseAction {
public:
    PrintTrainerStatus(int id);
    BaseAction* clone() const ;
    void act(Studio &studio);
    std::string toString() const;
private:
    const int trainerId;
};


class PrintActionsLog : public BaseAction {
public:
    PrintActionsLog();
    BaseAction* clone() const ;
    void act(Studio &studio);
    std::string toString() const;
private:
};


class BackupStudio : public BaseAction {
public:
    BackupStudio();
    BaseAction* clone() const ;
    void act(Studio &studio);
    std::string toString() const;
private:
};


class RestoreStudio : public BaseAction {
public:
    RestoreStudio();
    BaseAction* clone() const ;
    void act(Studio &studio);
    std::string toString() const;
};


#endif