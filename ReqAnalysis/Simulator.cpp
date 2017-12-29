#include "Simulator.h"


Request *getRandomRequest() {
	std::default_random_engine requestGenerator(std::random_device{}());
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

	beginWorkingWeek = { 0, 9, 0, 0 };
	endWorkingWeek = { 4, 17, 0, 0 };

	hugeRequestSpot = false;
	hugeR_queue = std::vector<Request>(0);
	hugeRProcessed = new Request();
}

Simulator::~Simulator(){
	delete trad_requestProcessing;
	delete acc_requestProcessing;
	delete spec_requestProcessing;
	delete trad_requestQueue;
	delete acc_requestQueue;
	delete spec_requestQueue;
	delete hugeRProcessed;
}


void Simulator::executionSimulation(){
	while (timeElapsed < runningTime) {
		checkNewRequest();
		if ((timeElapsed > convert(beginWorkingWeek)) && (timeElapsed < convert(endWorkingWeek))){
			checkWorkOver(trad_requestProcessing);
			checkWorkOver(acc_requestProcessing);
			checkWorkOver(spec_requestProcessing);

			checkQueueAvailable(trad_requestQueue, trad_requestProcessing, nbTraditionalNodes);
			checkQueueAvailable(acc_requestQueue, acc_requestProcessing, nbAcceleratedNodes);
			checkQueueAvailable(spec_requestQueue, spec_requestProcessing, nbSpecializedNodes);

			processCurentWork(trad_requestProcessing);
			processCurentWork(acc_requestProcessing);
			processCurentWork(spec_requestProcessing);
		}
		else {
			if (hugeRequestSpot == false) {
				completeHugeRQueue();
			}
			else {
				processHugeRQueue();
			}	
		}
		timeElapsed += 1;
	}
	std::cout << "\n";
}


void Simulator::checkWorkOver(std::vector<Request> *ReqProcess){
	int sizeProcess = (*ReqProcess).size();
	for (int i = sizeProcess; i > 0; i--){
		if (((*ReqProcess)[i-1]).getTime() == 0){
			(*ReqProcess).erase((*ReqProcess).begin() + i-1);
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
			long timeLeft = convert(endWorkingWeek) - timeElapsed;
			if (((*ReqQueue)[0]).getTimeCap() > timeLeft){
				processFull = true;
			}
			else {
				bool reservedSpaceRespected = false;
				int lenghtRequest = ((*ReqQueue)[0]).getTypeRequest();
				std::vector<int> repart = repartition();
				switch (lenghtRequest){
				case 0: if (((repart[1] + repart[3] + ((*ReqQueue)[0]).getNodes()) / nbNodes) > (0.3)){ reservedSpaceRespected = true; }; break;
				case 1: if (((repart[0] + repart[3] + ((*ReqQueue)[0]).getNodes()) / nbNodes) > (0.1)){ reservedSpaceRespected = true; }; break;
				case 2: if (((repart[0] + repart[1] + repart[3] + ((*ReqQueue)[0]).getNodes()) / nbNodes) > (0.4)){ reservedSpaceRespected = true; }; break;
				default: exit(44);
				}
				if (reservedSpaceRespected == true){
					(*ReqProcess).push_back((*ReqQueue)[0]);
					double cost = (*ReqQueue)[0].getCost();
					(*ReqQueue)[0].getEnquirer().setCharged(cost);
					(*ReqQueue)[0].getEnquirer().setTempCharged(-cost);
					freeNodes -= ((*ReqQueue)[0]).getNodes();
					(*ReqQueue).erase((*ReqQueue).begin());
				}
				else {
					processFull = true;
				}
			}
		}
	}
}


void Simulator::checkNewRequest(){
	std::vector<int> date = convert(timeElapsed);
	if ((date[1] > 9) && (date[1] < 17) && (date[0]<6)){
		std::default_random_engine EDgenerator(std::random_device{}());
		std::exponential_distribution<double> EDdistribution(2);
		double EDnumber = EDdistribution(EDgenerator);
		if (EDnumber < 1) {
			Request *newR;
			newR = getRandomRequest();
			double costNewR = (*newR).getCost();
			if ((*newR).getEnquirer().checkBudget(costNewR + ((*newR).getEnquirer().getTempCharged())) == true){
				(*newR).getEnquirer().setTempCharged(costNewR);
				int lenghtNewR = (*newR).getTypeRequest();
				if (lenghtNewR == 3){
					hugeR_queue.push_back(*newR);
				}
				else {
					int r = (*newR).getTypeNodes();
					switch (r) {
					case 0: (*trad_requestQueue).push_back(*newR); break;
					case 1: (*acc_requestQueue).push_back(*newR); break;
					case 2: (*spec_requestQueue).push_back(*newR); break;
					default: exit(33);
					}
				}
			}
			else {
				// what to do if request denied ?
			}
		}
	}	
}


void Simulator::processCurentWork(std::vector<Request> *ReqProcess){
	int sizeTrad = (*ReqProcess).size();
	for (int i = sizeTrad; i > 0; i--){
		((*ReqProcess)[i-1]).setTime( ((*ReqProcess)[i-1]).getTime() - 1 );
	}
}


void Simulator::completeHugeRQueue(){
	if (hugeR_queue.size() > 0){
		*hugeRProcessed = hugeR_queue[0];
		double cost = hugeR_queue[0].getCost();
		hugeR_queue[0].getEnquirer().setCharged(cost);
		hugeR_queue[0].getEnquirer().setTempCharged(-cost);
		hugeR_queue.erase(hugeR_queue.begin());
		hugeRequestSpot = true;
	}	
}

void Simulator::processHugeRQueue(){
	if ((*hugeRProcessed).getTime() > 0){
		(*hugeRProcessed).setTime((*hugeRProcessed).getTime() - 1);
	}
	else {
		hugeRequestSpot = false;
	}
}

std::vector<int> Simulator::repartition(){
	std::vector<int> repart = std::vector<int>(4);
	std::vector<std::vector<Request>> differentQueue = { *trad_requestProcessing, *acc_requestProcessing, *spec_requestProcessing };
	for (int q = 0; q < differentQueue.size(); q++){
		for (int i = 0; i < differentQueue[q].size(); i++){
			int n = differentQueue[q][i].getNodes();
			int r = differentQueue[q][i].getTypeNodes();
			switch (r){
			case 0: repart[0] += n; break;
			case 1: repart[1] += n; break;
			case 2: repart[2] += n; break;
			default: exit(55);
			}
		}
	}
	int totalNodes = nbAcceleratedNodes + nbSpecializedNodes + nbTraditionalNodes;
	repart[3] = totalNodes - repart[0] - repart[1] - repart[2];
	return repart;
}
