#include "Requests.h"

std::default_random_engine Rgenerator(std::random_device{}());

std::uniform_int_distribution<int> nodeShort_distribution(1, 2);
std::uniform_int_distribution<int> nodeMedium_distribution(1, 13);
std::uniform_int_distribution<int> nodeLarge_distribution(1, 64);

std::exponential_distribution<double> time_distribution(1);

User *getRandomUser(){
	int r = std::rand() % 2;
	switch (r) {
	case 0: return new Student();
	case 1: return new Researcher();
	default: return NULL;
	}
}

Request::Request(){
	enquirer = getRandomUser();
	std::default_random_engine typeGenerator(std::random_device{}());
	std::discrete_distribution<int> typeDistribution{ 11, 3, 1 };
	typeNodes = typeDistribution(typeGenerator);
	waitTime = 0;
}

shortR::shortR() : Request(){
	typeRequest = 0;
	nbNodes = nodeShort_distribution(Rgenerator);
	nbCores = (nbNodes - 1) * 16 + 1 + std::rand() % 15;
	timeCap = 3600 * 1;
	time = long(time_distribution(Rgenerator) * 3600 * 1);
	cost = 0.003 * nbCores * (time / 3600);
}

mediumR::mediumR() : Request(){
	typeRequest = 1;
	nbNodes = nodeMedium_distribution(Rgenerator);
	nbCores = (nbNodes - 1) * 16 + 1 + std::rand() % 15;
	timeCap = 3600 * 8;
	time = long(time_distribution(Rgenerator) * 3600 * 8);
	cost = 0.003 * nbCores * (time / 3600);
}

largeR::largeR() : Request(){
	typeRequest = 2;
	nbNodes = nodeLarge_distribution(Rgenerator);
	nbCores = (nbNodes - 1) * 16 + 1 + std::rand() % 15;
	timeCap = 3600 * 16;
	time = long(time_distribution(Rgenerator) * 3600 * 16);
	cost = 0.003 * nbCores * (time / 3600);
}

hugeR::hugeR() : Request(){
	typeNodes = 3;
	typeRequest = 3;
	nbNodes = 128;
	nbCores = 128 * 16;
	timeCap = 3600 * 64;
	time = long(time_distribution(Rgenerator) * 3600 * 64);
	cost = 0.003 * nbCores * (time / 3600);
}

int Request::getTypeNodes(){
	return typeNodes;
}

int Request::getTypeRequest(){
	return typeRequest;
}

int Request::getNodes(){
	return nbNodes;
}

long Request::getTimeCap(){
	return timeCap;
}

long Request::getTime(){
	return time;
}

long Request::getWaitTime(){
	return waitTime;
}

void Request::setTime(long time){
	this->time = time;
}

void Request::setWaitTime(long time){
	waitTime = time;
}

double Request::getCost(){
	return cost;
}

bool Request::checkUserCanPay(double price){
	return (*enquirer).checkBudget(price);
}

void Request::setEnquirerCharged(double price){
	(*enquirer).setCharged(price);
}

void Request::setEnquirerTempCharged(double price){
	(*enquirer).setTempCharged(price);
}

double Request::getEnquirerTempCharged(){
	return (*enquirer).getTempCharged();
}