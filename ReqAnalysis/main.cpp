/**
* \file      main.cpp
* \author    m.leclech
* \version   1.0
* \date      08 January 2018
* \brief     Launch the simulation with parameters choosen by the IT service
*
* \details   The IT support member need to specify the ratio needed for each jobs (short, medium,
*			 large, huge). Then they need to specify the ratio of student:researchers.  The 
*			 simulation run from Monday 00:00:00 to Sunday 23:59:59. Then the program return
*			 some statistics about the behaviour of the scheduler and the states of jobs.
*/

#include "Simulator.h"
#include "main.h"
#include "vld.h"

int main(){
	// the parameters are entered by the user of the program
	std::vector<int> jobRatio = std::vector<int>(0);
	std::vector<int> userRatio = std::vector<int>(0);
	int inputV;
	std::cout << "Enter the proportion of jobs you want to: " << "\n";
	std::cout << "(Exemple: I wish to have a ratio of 11 short jobs for 5 medium jobs, 2 large jobs et 1 huge job. I enter : 11 5 2 1)" << "\n";
	while (jobRatio.size() < 4) {
		if ((std::cin >> inputV) && (inputV > -1)) { jobRatio.push_back(inputV); }
		else {
			std::cout << "Please enter positive integers:" << "\n";
			std::cout << "So far, you have already input: ";
			for (int j = 0; j < int(jobRatio.size()); j++){
				std::cout << jobRatio[j] << " ";
			}
			std::cout << "\n";
			std::cin.clear();
			std::cin.ignore(999, '\n');
		}
	}
	std::cin.clear();
	std::cin.ignore(999, '\n');
	std::cout << "Enter the proportion of users you want to: " << "\n";
	std::cout << "(Exemple: I wish to have a ratio of 1 student for 1 researcher. I enter : 1 1)" << "\n";
	while (userRatio.size() < 2) {
		if ((std::cin >> inputV) && (inputV > -1)) { userRatio.push_back(inputV); }
		else {
			std::cout << "Please enter positive integers:" << "\n";
			std::cout << "So far, you have already input: ";
			for (int j = 0; j < int(userRatio.size()); j++){
				std::cout << userRatio[j] << " ";
			}
			std::cout << "\n";
			std::cin.clear();
			std::cin.ignore(999, '\n');
		}
	}
	std::cin.clear();
	std::cin.ignore(999, '\n');
	// once every parameters is input, and checked to be valid for our simulation, the simulation begin
	std::vector<int> endWeek = { 7, 0, 0, 0 };
	long runningTime = convert(endWeek);
	Simulator *simulationFCFS;
	simulationFCFS = new Simulator(runningTime);
	(*simulationFCFS).executionSimulation(jobRatio[0], jobRatio[1], jobRatio[2], jobRatio[3], userRatio[0], userRatio[1]); // default to 11 5 2 1 1 1
	delete simulationFCFS;
	return 0;
}
