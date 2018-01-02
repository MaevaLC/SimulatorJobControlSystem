/**
* \file      Requests.cpp
* \author    m.leclech
* \version   1.0
* \date      08 January 2018
* \brief     Define the concept of jobs being submited to the HPC.
*
* \details   There is a mother class, Request, which regroup all common elements of a request.
*			 There are also the 4 sub classes, defining the different types of jobs.
*              - The short requests (up to 2 nodes, up to 1h)
*              - The medium requests (up to 12 nodes, up to 8h)
*              - The large requests (up to 64 nodes, up to 16h)
*              - The huge requests (up to 128 nodes, up to 64h)
*/

#include "Requests.h"

// a random generator, able to work on different distributions
std::default_random_engine Rgenerator(std::random_device{}());
// these values defines the size of each request
std::uniform_int_distribution<int> nodeShort_distribution(1, 2);
std::uniform_int_distribution<int> nodeMedium_distribution(1, 12);
std::uniform_int_distribution<int> nodeLarge_distribution(1, 64);
// the lambda of the time distribution can be changed here
std::exponential_distribution<double> time_distribution(1);

/**
* \fn User *getRandomUser(double u1, double u2)
* \brief Fonction to generate an user randomly
*
* \param u1 ratio of Student
* \param u2 ratio of Researcher
* \return a new random-sub-class-of-User object.
*/
User *getRandomUser(double u1, double u2){
	std::default_random_engine userGenerator(std::random_device{}());
	std::discrete_distribution<int> userDistribution{ u1, u2 };
	int r = userDistribution(userGenerator);
	switch (r) {
	case 0: return new Student();
	case 1: return new Researcher();
	default: return NULL;
	}
}

/**
* \fn Request::Request()
* \brief Constructor of Request with a ratio of 1:1 for users
*/
Request::Request(){
	enquirer = getRandomUser(1, 1);
	std::default_random_engine typeGenerator(std::random_device{}());
	std::discrete_distribution<int> typeDistribution{ 11, 3, 1 };
	typeNodes = typeDistribution(typeGenerator);
	waitTime = 0;
}

/**
* \fn Request::Request(double u1, double u2)
* \brief Constructor of Request, with possibility to adjust the enquirer
*
* \param u1 ratio of Student
* \param u2 ratio of Researcher
*/
Request::Request(double u1, double u2){
	enquirer = getRandomUser(u1, u2);
	std::default_random_engine typeGenerator(std::random_device{}());
	std::discrete_distribution<int> typeDistribution{ 11, 3, 1 };
	typeNodes = typeDistribution(typeGenerator);
	waitTime = 0;
}

/**
* \fn shortR::shortR(double u1, double u2)
* \brief Constructor of shortR, with possibility to adjust the enquirer
*
* \param u1 ratio of Student
* \param u2 ratio of Researcher
*/
shortR::shortR(double u1, double u2) : Request(u1, u2){
	typeRequest = 0;
	nbNodes = nodeShort_distribution(Rgenerator);
	nbCores = (nbNodes - 1) * 16 + 1 + std::rand() % 15;
	timeCap = 3600 * 1;
	double generatedNumber = time_distribution(Rgenerator);
	while (generatedNumber > 1.0) generatedNumber = time_distribution(Rgenerator);
	time = long(generatedNumber * 3600 * 1);
	cost = 0.003 * nbCores * (time / 3600);
}

/**
* \fn mediumR::mediumR(double u1, double u2)
* \brief Constructor of mediumR, with possibility to adjust the enquirer
*
* \param u1 ratio of Student
* \param u2 ratio of Researcher
*/
mediumR::mediumR(double u1, double u2) : Request(u1, u2){
	typeRequest = 1;
	nbNodes = nodeMedium_distribution(Rgenerator);
	nbCores = (nbNodes - 1) * 16 + 1 + std::rand() % 15;
	timeCap = 3600 * 8;
	double generatedNumber = time_distribution(Rgenerator);
	while (generatedNumber > 1.0) generatedNumber = time_distribution(Rgenerator);
	time = long(generatedNumber * 3600 * 8);
	cost = 0.003 * nbCores * (time / 3600);
}

/**
* \fn largeR::largeR(double u1, double u2)
* \brief Constructor of largeR, with possibility to adjust the enquirer
*
* \param u1 ratio of Student
* \param u2 ratio of Researcher
*/
largeR::largeR(double u1, double u2) : Request(u1, u2){
	typeRequest = 2;
	nbNodes = nodeLarge_distribution(Rgenerator);
	nbCores = (nbNodes - 1) * 16 + 1 + std::rand() % 15;
	timeCap = 3600 * 16;
	double generatedNumber = time_distribution(Rgenerator);
	while (generatedNumber > 1.0) generatedNumber = time_distribution(Rgenerator);
	time = long(generatedNumber * 3600 * 16);
	cost = 0.003 * nbCores * (time / 3600);
}

/**
* \fn hugeR::hugeR(double u1, double u2)
* \brief Constructor of hugeR, with possibility to adjust the enquirer
*
* \param u1 ratio of Student
* \param u2 ratio of Researcher
*/
hugeR::hugeR(double u1, double u2) : Request(u1, u2){
	typeNodes = 3;
	typeRequest = 3;
	nbNodes = 128;
	nbCores = 128 * 16;
	timeCap = 3600 * 64;
	double generatedNumber = time_distribution(Rgenerator);
	while (generatedNumber > 1.0) generatedNumber = time_distribution(Rgenerator);
	time = long(generatedNumber * 3600 * 64);
	cost = 0.003 * nbCores * (time / 3600);
}

/**
* \fn int Request::getTypeNodes()
* \brief Accessor of Request
*
* \return the type of nodes (traditional, accelerated, specialized, hybrid) of the Request
*/
int Request::getTypeNodes(){
	return typeNodes;
}

/**
* \fn int Request::getTypeRequest()
* \brief Accessor of Request
*
* \return the type (short, medium, large, huge) of the Request
*/
int Request::getTypeRequest(){
	return typeRequest;
}

/**
* \fn int Request::getNodes()
* \brief Accessor of Request
*
* \return the number of nodes of the Request
*/
int Request::getNodes(){
	return nbNodes;
}

/**
* \fn long Request::getTimeCap()
* \brief Accessor of Request
*
* \return the time cap of the Request
*/
long Request::getTimeCap(){
	return timeCap;
}

/**
* \fn long Request::getTime()
* \brief Accessor of Request
*
* \return the processing time the Request will take
*/
long Request::getTime(){
	return time;
}

/**
* \fn long Request::getWaitTime()
* \brief Accessor of Request
*
* \return the time passed in the queue for a Request
*/
long Request::getWaitTime(){
	return waitTime;
}

/**
* \fn void Request::setTime(long time)
* \brief Accessor of Request
*
* \param time the time the request will take to be processed
*/
void Request::setTime(long time){
	this->time = time;
}

/**
* \fn void Request::setWaitTime(long time)
* \brief Accessor of Request
*
* \param time the time the request is waiting
*/
void Request::setWaitTime(long time){
	waitTime = time;
}

/**
* \fn double Request::getCost()
* \brief Accessor of Request
*
* \return the cost of the Request
*/
double Request::getCost(){
	return cost;
}

/**
* \fn bool Request::checkUserCanPay(double price)
* \brief Check if the enquirer submitting the job can afford it
*
* \param price the price of the request
* \return true or false, depending if the user can pay or not
*/
bool Request::checkUserCanPay(double price){
	return (*enquirer).checkBudget(price);
}

/**
* \fn void Request::setEnquirerCharged(double price)
* \brief Charge the user
*
* \param price the price the user should be charged
*/
void Request::setEnquirerCharged(double price){
	(*enquirer).setCharged(price);
}

/**
* \fn void Request::setEnquirerTempCharged(double price)
* \brief Charge the user temporarily
*
* \param price the price the user should be charged temporarily
*/
void Request::setEnquirerTempCharged(double price){
	(*enquirer).setTempCharged(price);
}

/**
* \fn double Request::getEnquirerTempCharged()
* \brief Fonctiun to know how much an user is temporarily charged
*
* \return the price the user is already temporarily charged
*/
double Request::getEnquirerTempCharged(){
	return (*enquirer).getTempCharged();
}