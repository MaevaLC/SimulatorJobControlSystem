#include "Simulator.h"

int main(){
	std::vector<int> endWeek = { 7, 0, 0, 0 };
	long runningTime = convert(endWeek);
	Simulator *simulationFCFS;
	simulationFCFS = new Simulator(runningTime);
	(*simulationFCFS).executionSimulation(1, 1, 1, 30, 1, 1); // default to 11 5 2 1 1 1
	delete simulationFCFS;
	return 0;
}