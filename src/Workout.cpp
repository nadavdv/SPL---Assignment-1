#include "../include/Workout.h"

/*
** Workout
*/

// Constructor
Workout::Workout(int w_id, std::string w_name, int w_price, WorkoutType w_type):id(w_id),name(w_name),price(w_price),type(w_type){}

// getId
int Workout::getId() const{return id;}

// getName
std::string Workout::getName() const{return name;}

// getPrice
int Workout::getPrice() const {return price;}

// getType
WorkoutType Workout::getType() const{return type;};

