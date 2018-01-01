/**
* \file      Simulator.h
* \author    m.leclech
* \version   1.0
* \date      08 January 2018
* \brief     Define the simulator behaviour
*
* \details   There is a single class, Simulator, which describe the behaviour of the scheduler.
*			 To test different scheduler algorithm, the IT support member can create a second
*			 "checkQueueAvailable" method, which is the method determining how to place the
*			 jobs waiting in the different queues in the processing system.
*/

#include "Conversion.h"
#include "Requests.h"
#include <iostream>
#include <iomanip>
#include <random>
#include <vector>

/**
* \class Simulator
* \brief Class which build the simulator, and make it run, to test different scheduler algorithm.
*
* A simulator is define by different types of attributes:
			- the hardware attributes, such as the number of nodes available per types, etc...
			- the operating attribute, such as the different hours of running on different tasks
			- vectors, defining the different queues, and also holding the jobs being processed
			- some attributes are used to store statistics about the simulation itself
*/
class Simulator{
protected:
	bool hugeRequestSpot; //!< a flag, to know is a huge request is being processed or scheduled for the WE
	bool processFull; //!< a flag, to know if the system is already full, in its number of nodes used.
	double totalPricePaid; //!< statistic - the total price paid by the users
	double machineHours; //!< statistic - the machine-hours amount used during the simulation
	int nbAcceleratedNodes; //!< hardware - number of Accelerated nodes of the system
	int nbSpecializedNodes; //!< hardware - nomber of Specialized nodes of the system
	int nbTraditionalNodes; //!< hardware - nomber of Traditional nodes of the system
	int nbUsersDenied; //!< statistic - number of users being rejected due to price
	long runningTime; //!< the time until the simulation must run
	long timeElapsed; //!< the time elapsed in the start of the simulation
	long timerNextRequest; //!< the timer between two request
	Request *hugeRProcessed; //!< the spot taken by the huge request being processed
	std::vector<int> beginWorkingWeek; //!< operating - the beginning of the week, where the system is accessible to everyone
	std::vector<int> endWorkingWeek; //!< operating - the end of the week, when the system is reserved for huge jobs
	std::vector<Request> *acc_requestProcessing; //!< the list of jobs being processed in the accelerated queue
	std::vector<Request> *acc_requestQueue; //!< the queue of jobs waiting for some accelerated nodes
	std::vector<Request> *spec_requestProcessing; //!< the list of jobs being processed in the specialized queue
	std::vector<Request> *spec_requestQueue; //!< the queue of jobs waiting for some specialized nodes
	std::vector<Request> *trad_requestProcessing; //!< the list of jobs being processed in the traditional queue
	std::vector<Request> *trad_requestQueue; //!< the queue of jobs waiting for some traditional nodes
	std::vector<Request> hugeR_queue; //!<  the queue of jobs waiting for the WE
	std::vector<int> numberJobEachQueueLength; //!< statistic - the number of jobs in each queues, queue per type of jobs
	std::vector<int> numberJobEachQueueType; //!< statistic - the number of jobs in each queues, queue per type of nodes
	std::vector<double> turnaroundLength; //!< statistic - turnabound-ratio average in each queues, queue per type of jobs
	std::vector<double> turnaroundType; //!< statistic - turnabound-ratio average in each queues, queue per type of nodes
	std::vector<long> waitEachQueueLength; //!< statistic - average waiting time in each queues, queue per type of jobs
	std::vector<long> waitEachQueueType; //!< statistic - average waiting time in each queues, queue per type of nodes
public:
	Simulator(long runningTime);
	~Simulator();
	std::vector<int> repartition();
	std::vector<double> averageStatistic();
	void addWaitTime(std::vector<Request> *ReqQueue);
	void checkNewRequest(double r1, double r2, double r3, double r4, double u1, double u2);
	void checkQueueAvailable(std::vector<Request> *ReqQueue, std::vector<Request> *ReqProcess, int nbNodes);
	void checkWorkOver(std::vector<Request> *ReqProcess);
	void completeHugeRQueue();
	void executionSimulation(double r1, double r2, double r3, double r4, double u1, double u2);
	void processCurentWork(std::vector<Request> *ReqProcess);
	void processHugeRQueue();
	void statistic(Request req);
};