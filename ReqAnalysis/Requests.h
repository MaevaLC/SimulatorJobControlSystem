#include <random>
#include "Users.h"

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
	Request(double u1, double u2);
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
	shortR(double u1, double u2);
};

class mediumR : public Request{
public:
	mediumR(double u1, double u2);
};

class largeR : public Request{
public:
	largeR(double u1, double u2);
};

class hugeR : public Request{
public:
	hugeR(double u1, double u2);
};