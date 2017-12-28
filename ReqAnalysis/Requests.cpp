#include "Requests.h"



std::default_random_engine Rgenerator;
std::uniform_int_distribution<int> nodeShort_distribution(1, 2);
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

}

shortR::shortR() : Request() {
	enquirer = getRandomUser();
	nbNodes = nodeShort_distribution(Rgenerator);
	typeNodes = std::rand() % 3;
	nbCores = (nbNodes - 1) + 1+ std::rand() % 15;
	time = time_distribution(Rgenerator) * 3600 * 1;
	cost;
}