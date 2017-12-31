#include "Conversion.h"
#include "Requests.h"
#include <iostream>
#include <iomanip>
#include <random>
#include <vector>

class Simulator{
protected:
	bool hugeRequestSpot;
	bool processFull;
	double totalPricePaid;
	double machineHours;
	int nbAcceleratedNodes;
	int nbSpecializedNodes;
	int nbTraditionalNodes;
	int nbUsersDenied;
	long runningTime;
	long timeElapsed;
	long timerNextRequest;
	Request *hugeRProcessed;
	std::vector<int> beginWorkingWeek;
	std::vector<int> endWorkingWeek;
	std::vector<Request> *acc_requestProcessing;
	std::vector<Request> *acc_requestQueue;
	std::vector<Request> *spec_requestProcessing;
	std::vector<Request> *spec_requestQueue;
	std::vector<Request> *trad_requestProcessing;
	std::vector<Request> *trad_requestQueue;
	std::vector<Request> hugeR_queue;
	std::vector<int> numberJobEachQueueLength;
	std::vector<int> numberJobEachQueueType;
	std::vector<double> turnaroundLength;
	std::vector<double> turnaroundType;
	std::vector<long> waitEachQueueLength;
	std::vector<long> waitEachQueueType;
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