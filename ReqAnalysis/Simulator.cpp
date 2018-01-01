#include "Simulator.h"

Request *getRandomRequest(double r1, double r2, double r3, double r4, double u1, double u2){
	std::default_random_engine requestGenerator(std::random_device{}());
	std::discrete_distribution<int> requestDistribution{ r1, r2, r3, r4 };
	int r = requestDistribution(requestGenerator);
	switch (r) {
	case 0: return new shortR(u1, u2);
	case 1: return new mediumR(u1, u2);
	case 2: return new largeR(u1, u2);
	case 3: return new hugeR(u1, u2);
	default: return NULL;
	}
}

Simulator::Simulator(long runningTime){
	acc_requestProcessing = new std::vector<Request>(0);
	acc_requestQueue = new std::vector<Request>(0);
	beginWorkingWeek = { 0, 9, 0, 0 };
	bool processFull = false;
	endWorkingWeek = { 4, 17, 0, 0 };
	hugeR_queue = std::vector<Request>(0);
	hugeRequestSpot = false;
	hugeRProcessed = new Request();
	machineHours = 0;
	nbAcceleratedNodes = 26;
	nbSpecializedNodes = 8;
	nbTraditionalNodes = 94;
	nbUsersDenied = 0;
	numberJobEachQueueLength = std::vector<int>(4);
	numberJobEachQueueType = std::vector<int>(4);
	spec_requestProcessing = new std::vector<Request>(0);
	spec_requestQueue = new std::vector<Request>(0);
	this->runningTime = runningTime;
	timeElapsed = 0;
	timerNextRequest = 0;
	totalPricePaid = 0;
	trad_requestProcessing = new std::vector<Request>(0);
	trad_requestQueue = new std::vector<Request>(0);
	turnaroundLength = std::vector<double>(4);
	turnaroundType = std::vector<double>(4);
	waitEachQueueLength = std::vector<long>(4);
	waitEachQueueType = std::vector<long>(4);
}

Simulator::~Simulator(){
	delete acc_requestProcessing;
	delete acc_requestQueue;
	delete hugeRProcessed;
	delete spec_requestProcessing;
	delete spec_requestQueue;
	delete trad_requestProcessing;
	delete trad_requestQueue;
}

void Simulator::executionSimulation(double r1, double r2, double r3, double r4, double u1, double u2){
	while (timeElapsed < runningTime) {
		checkNewRequest(r1, r2, r3, r4, u1, u2);
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

			addWaitTime(trad_requestQueue);
			addWaitTime(acc_requestQueue);
			addWaitTime(spec_requestQueue);
		}
		else {
			if (hugeRequestSpot == false){
				completeHugeRQueue();
			}
			else {
				processHugeRQueue();
			}
		}
		timeElapsed += 60;
	}
	std::cout << "\n" << "\n";
	std::cout << "All these statistics are given for jobs processed this week only." << "\n";
	std::cout << "It does not include jobs sumbmitted this week and queued but not processed yet." << "\n";
	std::cout << "X stand for Irrelevant." << "\n" << "\n";

	double maxMH = (nbAcceleratedNodes + nbSpecializedNodes + nbTraditionalNodes) * 24 * 7;
	std::cout << "Machine-Hours consumed: " << machineHours  << " (" << machineHours*100/(maxMH) << "% of the systeme)\n" << "\n";
	std::cout << "Total price paid by users: " << totalPricePaid << "\n" ;
	std::cout << "As the price of the system per week is estimated at 968 pounds sterling," << "\n"
		<< " the economic balance is: " << int(totalPricePaid - 968) << " pounds sterling.\n" << "\n";
	std::cout << "Number of users denied for insufficient funds: " << nbUsersDenied << "\n" << "\n";

	std::cout << "Number of job: " << "\n";
	std::cout << "per length: ";
	std::cout << " Short : " << numberJobEachQueueLength[0] << ", Medium : " << numberJobEachQueueLength[1]
		<< ", Large : " << numberJobEachQueueLength[2] << ", Huge : " << numberJobEachQueueLength[3] << "\n";
	std::cout << "per type: ";
	std::cout << "Traditional : " << numberJobEachQueueType[0] << ", Accelerated : " << numberJobEachQueueType[1]
		<< ", Specialized : " << numberJobEachQueueType[2] << ", Hybrid : " << numberJobEachQueueType[3] << "\n" << "\n";
	
	std::vector<double> answer = averageStatistic();

	std::cout << "Wait time in each queue (minutes): " << "\n";
	std::cout << "per length: ";
	std::cout << " Short : " << answer[0] << ", Medium : " << answer[1]
		<< ", Large : " << answer[2] << ", Huge : " << "X" << "\n";
	std::cout << "per type: ";
	std::cout << "Traditional : " << answer[4] << ", Accelerated : " << answer[5]
		<< ", Specialized : " << answer[6] << ", Hybrid : " << "X" << "\n" << "\n";

	std::cout << std::fixed << std::setprecision(1);

	std::cout << "Turnaround ratio (1 is the optimum ratio): " << "\n";
	std::cout << "per length: ";
	std::cout << " Short : " << answer[8] << ", Medium : " << answer[9]
		<< ", Large : " << answer[10] << ", Huge : " << "X" << "\n";
	std::cout << "per type : ";
	std::cout << "Traditional : " << answer[12] << ", Accelerated : " << answer[13]
		<< ", Specialized : " << answer[14] << ", Hybrid : " << "X" << "\n" << "\n";

	std::cin.get();
}

void Simulator::checkNewRequest(double r1, double r2, double r3, double r4, double u1, double u2){
	std::vector<int> date = convert(timeElapsed);
	if ((date[1] > 8) && (date[1] < 17) && (date[0] < 5)){
		if (timerNextRequest < 1) {
			Request *newR;
			newR = getRandomRequest(r1, r2, r3, r4, u1, u2);
			double costNewR = (*newR).getCost();
			double tempChar = (*newR).getEnquirerTempCharged();
			bool budget = (*newR).checkUserCanPay(costNewR + tempChar);
			if (budget == true){
				(*newR).setEnquirerTempCharged(costNewR);
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
				nbUsersDenied += 1;
			}
			std::default_random_engine EDgenerator(std::random_device{}());
			std::exponential_distribution<double> EDdistribution(1);
			double EDnumber = EDdistribution(EDgenerator);
			while (EDnumber > 1.0) EDnumber = EDdistribution(EDgenerator);
			timerNextRequest = long(EDnumber * 3600);
		}
		else {
			timerNextRequest -= 60;
		}
	}
}

void Simulator::checkWorkOver(std::vector<Request> *ReqProcess){
	int sizeProcess = (*ReqProcess).size();
	for (int i = sizeProcess; i > 0; i--){
		if (((*ReqProcess)[i - 1]).getTime() < 0){
			(*ReqProcess).erase((*ReqProcess).begin() + i - 1);
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
					statistic((*ReqQueue)[0]);
					double cost = (*ReqQueue)[0].getCost();
					(*ReqQueue)[0].setEnquirerCharged(cost);
					(*ReqQueue)[0].setEnquirerTempCharged(-cost);
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
void Simulator::processCurentWork(std::vector<Request> *ReqProcess){
	int sizeTrad = (*ReqProcess).size();
	for (int i = sizeTrad; i > 0; i--){
		((*ReqProcess)[i - 1]).setTime(((*ReqProcess)[i - 1]).getTime() - 60);
	}
}

void Simulator::addWaitTime(std::vector<Request> *ReqQueue){
	int sizeTrad = (*ReqQueue).size();
	for (int i = sizeTrad; i > 0; i--){
		((*ReqQueue)[i - 1]).setWaitTime(((*ReqQueue)[i - 1]).getWaitTime() + 60);
	}
}

void Simulator::completeHugeRQueue(){
	std::vector<int> date = convert(timeElapsed);
	if ((hugeR_queue.size() > 0) && (date[0] == 5)  && (date[1] < 18)){
		*hugeRProcessed = hugeR_queue[0];
		statistic(*hugeRProcessed);
		double cost = hugeR_queue[0].getCost();
		hugeR_queue[0].setEnquirerCharged(cost);
		hugeR_queue[0].setEnquirerTempCharged(-cost);
		hugeR_queue.erase(hugeR_queue.begin());
		hugeRequestSpot = true;
	}
}

void Simulator::processHugeRQueue(){
	if ((*hugeRProcessed).getTime() > 0){
		(*hugeRProcessed).setTime((*hugeRProcessed).getTime() - 60);
	}
	else {
		hugeRequestSpot = false;
	}
}

std::vector<int> Simulator::repartition(){
	std::vector<int> repart = std::vector<int>(4);
	std::vector<std::vector<Request>> differentQueue = { *trad_requestProcessing, *acc_requestProcessing, *spec_requestProcessing };
	for (int q = 0; q < int(differentQueue.size()); q++){
		for (int i = 0; i < int(differentQueue[q].size()); i++){
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

void Simulator::statistic(Request req){
	int l = req.getTypeRequest();
	numberJobEachQueueLength[l] += 1;
	int t = req.getTypeNodes();
	numberJobEachQueueType[t] += 1;
	int n = req.getNodes();
	long s = req.getTime();
	machineHours += n * s / 3600;
	double c = req.getCost();
	totalPricePaid += c;
	long w = req.getWaitTime();
	waitEachQueueLength[l] += w;
	waitEachQueueType[t] += w;
	double turnaround = (w + s) / s;
	turnaroundLength[l] += turnaround;
	turnaroundType[t] += turnaround;
}

std::vector<double> Simulator::averageStatistic(){
	std::vector<double> answer = std::vector<double>(16);
	for (int i = 0; i < 4; i++){
		if (numberJobEachQueueLength[i] == 0){
			answer[i] = 0;
			answer[i + 8] = 0;
		}
		else {
			answer[i] = waitEachQueueLength[i] / numberJobEachQueueLength[i] / 60;
			answer[i + 8] = turnaroundLength[i] / numberJobEachQueueLength[i];
		}
		if (numberJobEachQueueType[i] == 0){
			answer[i + 4] = 0;
			answer[i + 12] = 0;
		}
		else {
			answer[i + 4] = waitEachQueueType[i] / numberJobEachQueueType[i] / 60;
			answer[i + 12] = turnaroundType[i] / numberJobEachQueueType[i];
		}
	}
	return answer;
}