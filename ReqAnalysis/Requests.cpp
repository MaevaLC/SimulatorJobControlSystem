#include "Requests.h"


std::default_random_engine Rgenerator;
std::uniform_int_distribution<int> nodeShort_distribution(1, 2);
std::uniform_int_distribution<int> nodeMedium_distribution(1, 13);
std::uniform_int_distribution<int> nodeLarge_distribution(1, 64);
std::exponential_distribution<double> time_distribution(1);


User *getRandomUser() {
	int r = std::rand() % 2;
	switch (r) {
	case 0: return new Student();
	case 1: return new Researcher();
	default: return NULL;
	}
}


Request::Request(){
	enquirer = getRandomUser();
	std::default_random_engine typeGenerator;
	std::discrete_distribution<int> typeDistribution{ 11, 3, 1};
	typeNodes = typeDistribution(typeGenerator);
}

shortR::shortR() : Request() {	
	typeRequest = 0;
	nbNodes = nodeShort_distribution(Rgenerator);
	nbCores = (nbNodes - 1)*16 + 1+ std::rand() % 15;
	timeCap = 3600 * 1;
	time = long(time_distribution(Rgenerator) * 3600 * 1);
	cost = 0.003 * nbCores * (time/3600);
}

mediumR::mediumR() : Request() {
	typeRequest = 1;
	nbNodes = nodeMedium_distribution(Rgenerator);
	nbCores = (nbNodes - 1)*16 + 1 + std::rand() % 15;
	timeCap = 3600 * 8;
	time = long(time_distribution(Rgenerator) * 3600 * 8);
	cost = 0.003 * nbCores * (time / 3600);
}

largeR::largeR() : Request() {
	typeRequest = 2;
	nbNodes = nodeLarge_distribution(Rgenerator);
	nbCores = (nbNodes - 1)*16 + 1 + std::rand() % 15;
	timeCap = 3600 * 16;
	time = long(time_distribution(Rgenerator) * 3600 * 16);
	cost = 0.003 * nbCores * (time / 3600);
}

hugeR::hugeR() : Request() {
	typeRequest = 3;
	nbNodes = 128;
	nbCores = 128*16;
	timeCap = 3600 * 64;
	time = long(time_distribution(Rgenerator) * 3600 * 64);
	cost = 0.003 * nbCores * (time / 3600);
}


long Request::getTime(){
	return time;
}

void Request::setTime(long time){
	this->time = time;
}

int Request::getNodes(){
	return nbNodes;
}

long Request::getTimeCap(){
	return timeCap;
}

int Request::getTypeNodes(){
	return typeNodes;
}