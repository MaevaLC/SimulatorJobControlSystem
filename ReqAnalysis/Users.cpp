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