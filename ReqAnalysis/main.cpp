#include "Simulator.h"



int main(){
	std::vector<int> endWeek = { 7, 0, 0, 0 };
	long runningTime = convert(endWeek);
	Simulator *simulationFCFS;
	simulationFCFS = new Simulator(runningTime);
	(*simulationFCFS).executionSimulation();
	delete simulationFCFS;
	return 0;
}