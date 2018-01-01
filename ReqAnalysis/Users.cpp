/**
* \file      Users.cpp
* \author    m.leclech
* \version   1.0
* \date      08 January 2018
* \brief     Define the class to describe the users submiting jobs.
*
* \details   There is a mother class, User, which regroup all common elements of an user
*			 There are also the 2 sub classes, defining the different types of users.
*              - The Student
*              - The Researchers
*			 Each of them have dfferent type of budget, and so difference to check if they
*			 can afford the job.
*/

#include "Users.h"

std::default_random_engine Ugenerator(std::random_device{}());

std::uniform_int_distribution<int> IDdistribution(10000, 65535);

std::uniform_int_distribution<long> instantCap_distribution(10, 20);
std::uniform_int_distribution<long> cumulCapLeft_distribution(0, 100);

std::uniform_int_distribution<long> groupBudgetLeft_distribution(0, 1000);
std::uniform_int_distribution<long> specialGrantLeft_distribution(0, 500);

User::User(){
	ID = IDdistribution(Ugenerator);
	temporaryCharged = 0;
}

bool User::checkBudget(double price){
	return false;
}

void User::setTempCharged(double price){
	temporaryCharged += price;
}

double User::getTempCharged(){
	return temporaryCharged;
}

void User::setCharged(double price){}

Student::Student() : User() {
	instantCap = instantCap_distribution(Ugenerator);
	cumulCapLeft = cumulCapLeft_distribution(Ugenerator);
}

bool Student::checkBudget(double price){
	bool granted;
	if ((price < instantCap) && (price < cumulCapLeft)){
		granted = true;
	}
	else {
		granted = false;
	}
	return granted;
}

void Student::setCharged(double price){
	cumulCapLeft -= price;
}

Researcher::Researcher() : User() {
	groupBudgetLeft = groupBudgetLeft_distribution(Ugenerator);
	specialGrantLeft = specialGrantLeft_distribution(Ugenerator);
}

bool Researcher::checkBudget(double price){
	bool granted;
	if (price < specialGrantLeft + groupBudgetLeft){
		granted = true;
	}
	else {
		granted = false;
	}
	return granted;
}

void Researcher::setCharged(double price){
	if (price < specialGrantLeft){
		specialGrantLeft -= price;
	}
	else {
		price -= specialGrantLeft;
		specialGrantLeft = 0;
		groupBudgetLeft -= price;
	}
}