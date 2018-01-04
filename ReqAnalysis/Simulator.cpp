/**
* \file      Simulator.cpp
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

#include "Simulator.h"

/**
* \fn Request *getRandomRequest(double r1, double r2, double r3, double r4, double u1, double u2)
* \brief Fonction to generate a request randomly
*
* \param r1 ratio of short job
* \param r2 ratio of medium job
* \param r3 ratio of large job
* \param r4 ratio of huge job
* \return a new random-sub-class-of-Request object.
*/
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

/**
* \fn Simulator::Simulator(long runningTime)
* \brief Constructor of Request with a ratio of 1:1 for users
*
* \param runningTime the time the simulation will be running
*/
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

/**
* \fn Simulator::Simulator(long runningTime)
* \brief destructor of Simulator
*/
Simulator::~Simulator(){
	delete acc_requestProcessing;
	delete acc_requestQueue;
	delete hugeRProcessed;
	delete spec_requestProcessing;
	delete spec_requestQueue;
	delete trad_requestProcessing;
	delete trad_requestQueue;
}

/**
* \fn void Simulator::executionSimulation(double r1, double r2, double r3, double r4, double u1, double u2)
* \brief Central function of Simulor: describe the general behaviour of the simulator
*
* \param r1 ratio of short job
* \param r2 ratio of medium job
* \param r3 ratio of large job
* \param r4 ratio of huge job
* \param u1 ratio of student
* \param u2 ratio of researcher
*/
void Simulator::executionSimulation(double r1, double r2, double r3, double r4, double u1, double u2){
	while (timeElapsed < runningTime) {
		// check if a user has submitted a new job, and queue it
		checkNewRequest(r1, r2, r3, r4, u1, u2);
		// if we are during the week
		if ((timeElapsed > convert(beginWorkingWeek)) && (timeElapsed < convert(endWorkingWeek))){
			// check if some work has finished processing - delete the job from the processing list if over
			checkWorkOver(trad_requestProcessing);
			checkWorkOver(acc_requestProcessing);
			checkWorkOver(spec_requestProcessing);

			// check if some job from the wait queue can fit in the processed list
			checkQueueAvailable(trad_requestQueue, trad_requestProcessing, nbTraditionalNodes);
			checkQueueAvailable(acc_requestQueue, acc_requestProcessing, nbAcceleratedNodes);
			checkQueueAvailable(spec_requestQueue, spec_requestProcessing, nbSpecializedNodes);

			// process the job in the processing list
			processCurentWork(trad_requestProcessing);
			processCurentWork(acc_requestProcessing);
			processCurentWork(spec_requestProcessing);

			// increase the waiting timer of jobs in the wating queue
			addWaitTime(trad_requestQueue);
			addWaitTime(acc_requestQueue);
			addWaitTime(spec_requestQueue);
		}
		// if we are the WE - same but with the huge jobs queue
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
	//once the week is over, statistics show up
	std::cout << "\n" << "\n";
	std::cout << "All these statistics are given for jobs processed this week only." << "\n";
	std::cout << "It does not include jobs submitted this week and queued but not processed yet." << "\n";
	std::cout << "X stand for Irrelevant." << "\n" << "\n";

	double maxMH = (nbAcceleratedNodes + nbSpecializedNodes + nbTraditionalNodes) * 24 * 7;
	std::cout << "Machine-Hours consumed: " << machineHours;
	std::cout << std::fixed << std::setprecision(2);
	std::cout << " (" << machineHours * 100 / (maxMH) << "% of the systeme)\n" << "\n";
	std::cout << "Total price paid by users: " << totalPricePaid << "\n";
	std::cout << "As the price of the system per week is estimated at 968 pounds sterling," << "\n"
		<< " the economic balance is: " << int(totalPricePaid - 968) << " pounds sterling.\n" << "\n";
	std::cout << "Number of users denied for insufficient funds: " << nbUsersDenied << "\n" << "\n";

	std::cout << std::fixed << std::setprecision(0);
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

	std::cout << "Press Enter to terminate this simulation.";
	std::cin.get();
}

/**
* \fn void Simulator::checkNewRequest(double r1, double r2, double r3, double r4, double u1, double u2)
* \brief Function which check if there is a new request to be added in a waiting queue
*
* \param r1 ratio of short job
* \param r2 ratio of medium job
* \param r3 ratio of large job
* \param r4 ratio of huge job
* \param u1 ratio of student
* \param u2 ratio of researcher
*/
void Simulator::checkNewRequest(double r1, double r2, double r3, double r4, double u1, double u2){
	std::vector<int> date = convert(timeElapsed);
	// to match a "normal" behaviour, I took the decision to produce request between 9AM and 17PM only
	// during the week only as well
	if ((date[1] > 8) && (date[1] < 17) && (date[0] < 5)){
		// check if the timer between each request has reach zero, to input a new request
		if (timerNextRequest < 1) {
			Request *newR;
			newR = getRandomRequest(r1, r2, r3, r4, u1, u2);
			bool possible = false;
			int t = (*newR).getTypeNodes();
			int n = (*newR).getNodes();
			switch (t) {
			case 0: if (n < nbTraditionalNodes + 1) possible = true; break;
			case 1: if (n < nbAcceleratedNodes + 1) possible = true; break;
			case 2: if (n < nbSpecializedNodes + 1) possible = true; break;
			case 3: if (n < nbTraditionalNodes + nbAcceleratedNodes + nbSpecializedNodes + 1) possible = true; break;
			}
			// check if the hardware have the capacity to deal such request
			if (possible == true){
				double costNewR = (*newR).getCost();
				double tempChar = (*newR).getEnquirerTempCharged();
				bool budget = (*newR).checkUserCanPay(costNewR + tempChar);
				// check if the user can pay, and get temporaly charged and queued if yes
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
				// once a request has been produced, the timer until next request is generated
				std::default_random_engine EDgenerator(std::random_device{}());
				std::exponential_distribution<double> EDdistribution(1);
				double EDnumber = EDdistribution(EDgenerator);
				while (EDnumber > 1.0) EDnumber = EDdistribution(EDgenerator);
				timerNextRequest = long(EDnumber * 3600);
			}
		}
		else {
			timerNextRequest -= 60;
		}
	}
}

/**
* \fn void Simulator::checkWorkOver(std::vector<Request> *ReqProcess)
* \brief Function to parse the list of processed jobs, and take them off if complete
*
* \param *ReqProcess the pointer to the vector containing the jobs being processed currently
*/
void Simulator::checkWorkOver(std::vector<Request> *ReqProcess){
	int sizeProcess = (*ReqProcess).size();
	for (int i = sizeProcess; i > 0; i--){
		if (((*ReqProcess)[i - 1]).getTime() < 0){
			(*ReqProcess).erase((*ReqProcess).begin() + i - 1);
			// if the process is freed from somes jobs, it's flagged as not full anymore
			processFull = false;
		}
	}
}

/**
* \fn void Simulator::checkQueueAvailable(std::vector<Request> *ReqQueue, std::vector<Request> *ReqProcess, int nbNodes)
* \brief Function to check if the system is full, and complete it with jobs waiting in the queue if not
*
* \param *ReqQueue the pointer to the vector containing the waiting queue
* \param *ReqProcess the pointer to the vector containing the jobs being processed currently
* \param nbNodes the number of nodes for this kind of job
*/
void Simulator::checkQueueAvailable(std::vector<Request> *ReqQueue, std::vector<Request> *ReqProcess, int nbNodes){
	int freeNodes = nbNodes;
	int sizeProcess = (*ReqProcess).size();
	// Calculation of the not used nodes at this instant in this processing list
	for (int i = 0; i < sizeProcess; i++){
		freeNodes -= ((*ReqProcess)[i]).getNodes();
	}
	// if the process hasn't been flagged as full yet, and the waiting queue is not empty
	while ((processFull == false) && ((*ReqQueue).size() > 0)){
		// here is the first come, first served:
		// we test if the first job in the queue fit or not - if not the process is flagged a full
		// and will be unflagged once the processed list is freed from one job in "Simulator::checkWorkOver"
		if (((*ReqQueue)[0]).getNodes() > freeNodes){
			processFull = true;
		}
		// if there are enough nods left, we check the time
		// no job can go over Friday 5PM as the system is reserved to huge request
		// else it get flagged as full - no need to retry as the system hasn't finish to process at least one job
		else {
			long timeLeft = convert(endWorkingWeek) - timeElapsed;
			if (((*ReqQueue)[0]).getTimeCap() > timeLeft){
				processFull = true;
			}
			else {
				// in these line, we calculate if including the job will still respect the reserved amount for
				// each queue, ie 10% of all nodes should be for short jobs, 30% for medium jobs.
				bool reservedSpaceRespected = false;
				int lenghtRequest = ((*ReqQueue)[0]).getTypeRequest();
				std::vector<int> repart = repartition();
				int totalNodes = nbAcceleratedNodes + nbSpecializedNodes + nbTraditionalNodes;
				switch (lenghtRequest){
				case 0: if ((double(repart[1]) + double(freeNodes) - double(((*ReqQueue)[0]).getNodes()) / double(totalNodes)) > (0.3)){ reservedSpaceRespected = true; }; break;
				case 1: if ((double(repart[0]) + double(freeNodes) - double(((*ReqQueue)[0]).getNodes()) / double(totalNodes)) > (0.1)){ reservedSpaceRespected = true; }; break;
				case 2: if ((double(repart[0]) + double(repart[1]) + double(freeNodes) - double(((*ReqQueue)[0]).getNodes()) / double(totalNodes)) > (0.4)){ reservedSpaceRespected = true; }; break;
				default: exit(44);
				}
				// if the job respect all these condition, then it's put in the processing list
				// the user get charged and the statistic are update, the job is taken off the queue
				// else it get flagged as full - no need to retry as the system hasn't finish to process at least one job
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

/**
* \fn void Simulator::processCurentWork(std::vector<Request> *ReqProcess)
* \brief Function to "process" the job list
*
* \param *ReqProcess the pointer to the vector containing the jobs being processed currently
*/
void Simulator::processCurentWork(std::vector<Request> *ReqProcess){
	int sizeTrad = (*ReqProcess).size();
	for (int i = sizeTrad; i > 0; i--){
		((*ReqProcess)[i - 1]).setTime(((*ReqProcess)[i - 1]).getTime() - 60);
	}
}

/**
* \fn void Simulator::processCurentWork(std::vector<Request> *ReqProcess)
* \brief Function to increase the waiting timer for the request waiting in the queue
*
* \param *ReqQueue the pointer to the vector containing the waiting queue
*/
void Simulator::addWaitTime(std::vector<Request> *ReqQueue){
	int sizeTrad = (*ReqQueue).size();
	for (int i = sizeTrad; i > 0; i--){
		((*ReqQueue)[i - 1]).setWaitTime(((*ReqQueue)[i - 1]).getWaitTime() + 60);
	}
}

/**
* \fn void Simulator::completeHugeRQueue()
* \brief Function to put the first job of the huge queue in the processed list
*/
void Simulator::completeHugeRQueue(){
	std::vector<int> date = convert(timeElapsed);
	// simple test to just put in the system on the Friday evening (charge the user, update statistics)
	if ((hugeR_queue.size() > 0) && (date[0] == 4) && (date[1] < 18) && (date[2] < 2)){
		*hugeRProcessed = hugeR_queue[0];
		statistic(*hugeRProcessed);
		double cost = hugeR_queue[0].getCost();
		hugeR_queue[0].setEnquirerCharged(cost);
		hugeR_queue[0].setEnquirerTempCharged(-cost);
		hugeR_queue.erase(hugeR_queue.begin());
		// for the huge queue, it's not a list being pocessed but just a single spot with a flag taken or not
		hugeRequestSpot = true;
	}
}

/**
* \fn void Simulator::processHugeRQueue()
* \brief Process the huge job while in the system and free the system one finished
*/
void Simulator::processHugeRQueue(){
	if ((*hugeRProcessed).getTime() > 0){
		(*hugeRProcessed).setTime((*hugeRProcessed).getTime() - 60);
	}
	else {
		hugeRequestSpot = false;
	}
}

/**
* \fn std::vector<int> Simulator::repartition()
* \brief Calculte the repartition of the different jobs being processed
*
* \return repart vector with the number of nodes being working on [short, medium, large] jobs
*/
std::vector<int> Simulator::repartition(){
	std::vector<int> repart = std::vector<int>(3);
	std::vector<std::vector<Request>> differentQueue = { *trad_requestProcessing, *acc_requestProcessing, *spec_requestProcessing };
	// for each processing list
	for (int q = 0; q < int(differentQueue.size()); q++){
		// for each jobs of each list
		for (int i = 0; i < int(differentQueue[q].size()); i++){
			int n = differentQueue[q][i].getNodes();
			int r = differentQueue[q][i].getTypeRequest();
			// the adequate counter is increased from the number of nodes dedicated to this task
			switch (r){
			case 0: repart[0] += n; break;
			case 1: repart[1] += n; break;
			case 2: repart[2] += n; break;
			default: exit(55);
			}
		}
	}
	return repart;
}

/**
* \fn void Simulator::statistic(Request req)
* \brief Collect some statisics about the request
*
* \param req the request which will be processed
*/
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
	// these stats are an average, here we add the numerators, and the division part
	// is done at the end when Simulator::averageStatistic() is called
	long w = req.getWaitTime();
	waitEachQueueLength[l] += w;
	waitEachQueueType[t] += w;
	double turnaround = (w + s) / s;
	turnaroundLength[l] += turnaround;
	turnaroundType[t] += turnaround;
}

/**
* \fn std::vector<double> Simulator::averageStatistic()
* \brief Process some statistics
*
* \return answer vector with [ 0-3 average wait per length, 4 - 7 average wait per type,
*							8 - 11 average turnaround per length, 12 - 15 average turnaround per type]
*/
std::vector<double> Simulator::averageStatistic(){
	std::vector<double> answer = std::vector<double>(16);
	// continuity of Simulator::statistic(Request req)
	// the numerators have been added, and now we divide at the end
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