#include <random>
#include "Users.h"

User *getRandomUser();


class Request {
protected:
	User *enquirer;
	int nbNodes;
	int typeNodes;
	int nbCores;
	long time;
	double cost;
public:
	Request();
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