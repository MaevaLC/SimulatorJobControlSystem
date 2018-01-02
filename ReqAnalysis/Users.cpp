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

// a random generator, able to work on different distributions
std::default_random_engine Ugenerator(std::random_device{}());
std::uniform_int_distribution<int> IDdistribution(10000, 65535);
// these values should be adjusted to grant money to the different users
std::uniform_int_distribution<long> instantCap_distribution(10, 20);
std::uniform_int_distribution<long> cumulCapLeft_distribution(0, 100);
std::uniform_int_distribution<long> groupBudgetLeft_distribution(0, 1000);
std::uniform_int_distribution<long> specialGrantLeft_distribution(0, 500);

/**
* \fn User::User()
* \brief Constructor of an User
*/
User::User(){
	ID = IDdistribution(Ugenerator);
	temporaryCharged = 0;
}

/**
* \fn bool User::checkBudget(double price)
* \brief To test if the user can affor to pay such price
*
* \param price the price we need to check
* \return a boolean, depending if the user can pay or not (true = can afford)
*/
bool User::checkBudget(double price){
	return false;
}

/**
* \fn void User::setTempCharged(double price)
* \brief To add to the amount the user is already temporarily charged
*
* \param price the price the user will be tempCharged (it will be added)
*/
void User::setTempCharged(double price){
	temporaryCharged += price;
}

/**
* \fn double User::getTempCharged()
* \brief Accessor of User
*
* \return the amount the user is already temporary charged
*/
double User::getTempCharged(){
	return temporaryCharged;
}

/**
* \fn void User::setCharged(double price)
* \brief Accessor of User - virtual fonction
*
* \param price the amount the user need to be charged
*/
void User::setCharged(double price){}

/**
* \fn Student::Student()
* \brief Constructor of Student
*/
Student::Student() : User() {
	instantCap = instantCap_distribution(Ugenerator);
	cumulCapLeft = cumulCapLeft_distribution(Ugenerator);
}

/**
* \fn bool Student::checkBudget(double price)
* \brief Check if the student can afford this price
*
* \param price the amount the student need to pay
* \return boolean depending if the student can pay (true = can afford it)
*/
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

/**
* \fn void Student::setCharged(double price)
* \brief Adjust how much the student is charged
*
* \param price the amount the student need to be charged
*/
void Student::setCharged(double price){
	cumulCapLeft -= price;
}

/**
* \fn Researcher::Researcher()
* \brief Constructor of researcher
*/
Researcher::Researcher() : User() {
	groupBudgetLeft = groupBudgetLeft_distribution(Ugenerator);
	specialGrantLeft = specialGrantLeft_distribution(Ugenerator);
}

/**
* \fn bool Researcher::checkBudget(double price)
* \brief Check if the researcher can afford this price
*
* \param price the amount the researcher need to pay
* \return boolean depending if the researcher can pay (true = can afford it)
*/
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

/**
* \fn void Researcher::setCharged(double price)
* \brief Adjust how much the researcher is charged
*
* \param price the amount the researcher need to be charged
*/
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