#include "Simulator.h"



int main(){
	long runningTime = 104;
	Simulator *simulationFCFS;
	simulationFCFS = new Simulator(runningTime);
	(*simulationFCFS).executionSimulation();
	delete simulationFCFS;
	return 0;
}