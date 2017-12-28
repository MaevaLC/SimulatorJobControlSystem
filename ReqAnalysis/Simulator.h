#include "Conversion.h"
#include "Requests.h"
#include <iostream>
#include <random>
#include <vector>


class Simulator{
protected:
	long timeElapsed;
	long runningTime;
	int nbTraditionalNodes;
	std::vector<Request> *trad_requestProcessing;
	std::vector<Request> *trad_requestQueue;
	int nbAcceleratedNodes;
	std::vector<Request> *acc_requestProcessing;
	std::vector<Request> *acc_requestQueue;
	int nbSpecializedNodes;
	std::vector<Request> *spec_requestProcessing;
	std::vector<Request> *spec_requestQueue;
	bool processFull;
public:
	Simulator(long runningTime);
	~Simulator();
	void executionSimulation();
	void checkWorkOver(std::vector<Request> *ReqProcess);
	void checkQueueAvailable(std::vector<Request> *ReqQueue, std::vector<Request> *ReqProcess, int nbNodes);
	void checkNewRequest();
	void processCurentWork(std::vector<Request> *ReqProcess);
};