#include "Simulator.h"


Request *getRandomRequest() {
	std::default_random_engine requestGenerator;
	std::discrete_distribution<int> requestDistribution{ 10, 5, 2, 1};
	int r = requestDistribution(requestGenerator);
	switch (r) {
	case 0: return new shortR();
	case 1: return new mediumR();
	case 2: return new largeR();
	case 3: return new hugeR();
	default: return NULL;
	}
}


Simulator::Simulator(long runningTime){
	timeElapsed = 0;
	this->runningTime = runningTime;

	nbTraditionalNodes = 94;
	nbAcceleratedNodes = 26;
	nbSpecializedNodes = 8;

	trad_requestProcessing = new std::vector<Request>(0);
	acc_requestProcessing = new std::vector<Request>(0);
	spec_requestProcessing = new std::vector<Request>(0);

	trad_requestQueue = new std::vector<Request>(0);
	acc_requestQueue = new std::vector<Request>(0);
	spec_requestQueue = new std::vector<Request>(0);

	bool processFull = false;
}

Simulator::~Simulator(){
	delete trad_requestProcessing;
	delete acc_requestProcessing;
	delete spec_requestProcessing;
	delete trad_requestQueue;
	delete acc_requestQueue;
	delete spec_requestQueue;
}


void Simulator::executionSimulation(){
	while (timeElapsed < runningTime) {

		checkWorkOver(trad_requestProcessing);
		checkWorkOver(acc_requestProcessing);
		checkWorkOver(spec_requestProcessing);

		checkNewRequest();

		checkQueueAvailable(trad_requestQueue, trad_requestProcessing, nbTraditionalNodes);
		checkQueueAvailable(acc_requestQueue, acc_requestProcessing, nbAcceleratedNodes);
		checkQueueAvailable(spec_requestQueue, spec_requestProcessing, nbSpecializedNodes);

		processCurentWork(trad_requestProcessing);
		processCurentWork(acc_requestProcessing);
		processCurentWork(spec_requestProcessing);

		timeElapsed += 1;
	}
}


void Simulator::checkWorkOver(std::vector<Request> *ReqProcess){
	int sizeProcess = (*ReqProcess).size();
	for (int i = sizeProcess; i > 0; i--){
		if (((*ReqProcess)[i]).getTime() == 0){
			(*ReqProcess).erase((*ReqProcess).begin() + i);
			processFull = false;
		}
	}
}


void Simulator::checkQueueAvailable(std::vector<Request> *ReqQueue, std::vector<Request> *ReqProcess, int nbNodes){
	int freeNodes = nbNodes;
	int sizeProcess = (*ReqProcess).size();
	for (int i = 0; i < sizeProcess; i++){
		freeNodes -= ((*ReqProcess)[i]).getNodes();
	}
	while ((processFull == false) && ((*ReqQueue).size() > 0)){
		if (((*ReqQueue)[0]).getNodes() > freeNodes){
			processFull = true;
		}
		else {
			long timeLeft = runningTime - timeElapsed;
			if (((*ReqQueue)[0]).getTimeCap() > timeLeft){
				processFull = true;
			}
			else {
				(*ReqProcess).push_back((*ReqQueue)[0]);
				freeNodes -= ((*ReqQueue)[0]).getNodes();
				(*ReqQueue).erase((*ReqQueue).begin());				
			}
		}
	}
}


void Simulator::checkNewRequest(){
	std::default_random_engine EDgenerator;
	std::exponential_distribution<double> EDdistribution(1);
	double EDnumber = EDdistribution(EDgenerator) * timeElapsed;
	if (EDnumber < 0.1) {
		Request *newR;
		newR = getRandomRequest();
		int r = (*newR).getTypeNodes();
		switch (r) {
		case 0: (*trad_requestQueue).push_back(*newR); break;
		case 1: (*acc_requestQueue).push_back(*newR); break;
		case 2: (*spec_requestQueue).push_back(*newR); break;
		default: exit(33);
		}
	}
}


void Simulator::processCurentWork(std::vector<Request> *ReqProcess){
	int sizeTrad = (*ReqProcess).size();
	for (int i = sizeTrad; i > 0; i--){
		((*ReqProcess)[i]).setTime( ((*ReqProcess)[i]).getTime() - 1 );
	}
}
