/**
* \file      Requests.h
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

#include "Users.h"
#include <random>

/**
* \class Request
* \brief Base abstract class which define the attributes of a request
*
* A request is composed of different attributes, such as: the cost of a request (cost),
* a number of cores needed (nbCores), a number of nodes needed (nbNodes), the type of 
* node needed (0 for Traditional, 1 for Accelerated, 2 for Specialized, 3 for Hybrid which is
* a mix of those) (typeNodes), the type of the request (0 for short, 1 for medium, 2 for large
* and 3 for huge) (typeRequest), a time while the simulation which run (time), a time cap which
* is determined by the type of request (timeCap), a wait time which is a counter of the time the
* request spend in the queue (waitTime), and finally an user which will be submitting the job and
* paying for it (enquirer).
*/
class Request {
protected:
	double cost; //!< cost of the job
	int nbCores; //!< number of core asked
	int nbNodes; //!< the number of nodes asked
	int typeNodes; //!< the type of nodes asked
	int typeRequest; //!< the type of request asked
	long time; //!< the time the job will be running - time randomly choosen to simulate the real time of the job. The sim. doesn't have accessed to this value when evaluating if there is enough time left
	long timeCap; //!< the cap the time can reach for a job, it's the time which will be used to check if there is place in the processing queue
	long waitTime; //!< the timer which will be incremented while the job is in the queue, waiting to be processed
	User *enquirer; //!< the user submitting this job
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
	User* getEnquirer();
	void setEnquirerCharged(double price);
	void setEnquirerTempCharged(double price);
	void setTime(long time);
	void setWaitTime(long time);
};

/**
* \class shortR
* \brief sub class of Request
*
* The constructor of the class is the only method of this sub class. It set some attributes according to
* certain specificity of the job
*/
class shortR : public Request{
public:
	shortR(double u1, double u2);
};

/**
* \class mediumR
* \brief sub class of Request
*
* The constructor of the class is the only method of this sub class. It set some attributes according to
* certain specificity of the job
*/
class mediumR : public Request{
public:
	mediumR(double u1, double u2);
};

/**
* \class largeR
* \brief sub class of Request
*
* The constructor of the class is the only method of this sub class. It set some attributes according to
* certain specificity of the job
*/
class largeR : public Request{
public:
	largeR(double u1, double u2);
};

/**
* \class hugeR
* \brief sub class of Request
*
* The constructor of the class is the only method of this sub class. It set some attributes according to
* certain specificity of the job
*/
class hugeR : public Request{
public:
	hugeR(double u1, double u2);
};