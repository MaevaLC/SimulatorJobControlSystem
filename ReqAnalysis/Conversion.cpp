#include "Conversion.h"


std::vector<int> convert(long input_seconds){
	int days = input_seconds / 60 / 60 / 24;
	int hours = (input_seconds / 60 / 60) % 24;
	int minutes = (input_seconds / 60) % 60;
	int seconds = input_seconds % 60;
	std::vector<int> date = { days, hours, minutes, seconds };
	return date;
}

long convert(std::vector<int> input_date){
	return ((input_date[0] * 24 + input_date[1]) * 60 + input_date[2]) * 60 + input_date[3];
}