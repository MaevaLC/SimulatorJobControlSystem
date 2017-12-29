#include <random>
#include "Users.h"


User *getRandomUser();


class Request {
protected:
	User *enquirer;
	int typeRequest;
	int nbNodes;
	int typeNodes;
	int nbCores;
	long timeCap;
	long time;
	double cost;
public:
	Request();
	long getTime();
	void setTime(long time);
	int getNodes();
	long getTimeCap();
	int getTypeNodes();
	User getEnquirer();
	double getCost();
	int getTypeRequest();
};

class shortR : public Request{
public:
	shortR();
};

class mediumR : public Request{
public:
	mediumR();
};

class largeR : public Request{
public:
	largeR();
};

class hugeR : public Request{
public:
	hugeR();
};