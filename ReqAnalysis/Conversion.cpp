#include "Conversion.h"


std::vector<int> convert(long input_seconds){
	int days = input_seconds / 60 / 60 / 24;
	int hours = (input_seconds / 60 / 60) % 24;
	int minutes = (input_seconds / 60) % 60;
	int seconds = input_seconds % 60;
	std::vector<int> date = { days, hours, minutes, seconds };
	return date;
}