#include <random>
#include "Users.h"

User *getRandomUser();

class Request {
protected:
	double cost;
	int nbCores;
	int nbNodes;
	int typeNodes;
	int typeRequest;
	long time;
	long timeCap;
	long waitTime;
	User *enquirer;
public:
	Request();
	bool checkUserCanPay(double price);
	double getCost();
	double getEnquirerTempCharged();
	int getNodes();
	int getTypeNodes();
	int getTypeRequest();
	long getTime();
	long getTimeCap();
	long getWaitTime();
	void setEnquirerCharged(double price);
	void setEnquirerTempCharged(double price);
	void setTime(long time);
	void setWaitTime(long time);
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