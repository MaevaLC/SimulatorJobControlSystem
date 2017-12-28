#include "Users.h"


std::default_random_engine Ugenerator;
std::uniform_int_distribution<int> IDdistribution(10000, 65535);

std::uniform_int_distribution<long> instantCap_distribution(100, 200);
std::uniform_int_distribution<long> cumulCapLeft_distribution(0, 2000);

std::uniform_int_distribution<long> groupBudgetLeft_distribution(0, 100000);
std::uniform_int_distribution<long> specialGrantLeft_distribution(0, 20000);


User::User(){
	ID = IDdistribution(Ugenerator);
}

bool User::checkBudget(long price){
	return false; }


Student::Student() : User() {
	instantCap = instantCap_distribution(Ugenerator);
	cumulCapLeft = cumulCapLeft_distribution(Ugenerator);
}

bool Student::checkBudget(long price){
	bool granted;
	if ((price < instantCap) && (price < cumulCapLeft)){
		granted = true;
	}
	else {
		granted = false;
	}
	return granted;
}


Researcher::Researcher() : User() {
	groupBudgetLeft = groupBudgetLeft_distribution(Ugenerator);
	specialGrantLeft = specialGrantLeft_distribution(Ugenerator);
}

bool Researcher::checkBudget(long price){
	bool granted;
	if ((price < specialGrantLeft) || (price < groupBudgetLeft)){
		granted = true;
	}
	else {
		granted = false;
	}
	return granted;
}