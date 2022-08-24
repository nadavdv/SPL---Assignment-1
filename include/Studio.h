#ifndef STUDIO_H_
#define STUDIO_H_

#include <vector>
#include <string>
#include "Workout.h"
#include "Trainer.h"
#include "Action.h"
#include <iostream>
#include <fstream>

class Studio{		
public:
	Studio();
    Studio(const std::string &configFilePath);
    ~Studio();
    Studio(Studio &other);
    Studio& operator=(const Studio &other);
    Studio(Studio &&other);// Move Constructor
    Studio& operator=(Studio &&other);// Move Assignment
    void start();
    int getNumOfTrainers() const;
    Trainer* getTrainer(int tid);
	const std::vector<BaseAction*>& getActionsLog() const; // Return a reference to the history of actions
    std::vector<Workout>& getWorkoutOptions();
    void addBackupTolog(BaseAction *b);
private:
    bool open;
    std::vector<Trainer*> trainers;
    std::vector<Workout> workout_options;
    std::vector<BaseAction*> actionsLog;
};

#endif