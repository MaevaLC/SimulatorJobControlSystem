#include "Conversion.h"
#include "Requests.h"
#include <iostream>
#include <random>
#include <vector>


std::default_random_engine EDgenerator;
std::exponential_distribution<double> EDdistribution(1);




void execution(long runningTime){
	long t = 0;
	while (t < runningTime) {
		//fais des chooooooose
		double EDnumber = EDdistribution(EDgenerator);
		if (EDnumber < 0.3) {

			User *Mr;
			Mr = getRandomUser();

			std::vector<int> date = convert(t);
			std::cout << "Day : " << date[0] << ", Time : " << date[1] << ":" << date[2] << ":" << date[3] << "\n";

			bool granted = (*Mr).checkBudget(1000);
			std::cout << granted << "\n";
		}
		

		t = t + 1;
	}
}


int main(){
	long runningTime = 846; //one day
	execution(runningTime);
	system("PAUSE");
	return 0;
}