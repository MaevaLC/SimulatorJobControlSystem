/**
* \file      Conversion.cpp
* \author    m.leclech
* \version   1.0
* \date      08 January 2018
* \brief     Make time conversion between secondes and date
*
* \details   In this project, the time is represented under 2 differents forms:
- a long , which reprents seconds
- a vector, which represent the date as this: [day, hour, minute, second]
The 2 fonctions make the conversion between each forms.
*/
#include "Conversion.h"

/**
* \fn std::vector<int> convert(long input_seconds)
* \brief Fonction to convert seconds in date
*
* \param input_seconds the time in second
* \return a vector containing the date under the format [day, hour, minute, second]
*/
std::vector<int> convert(long input_seconds){
	int days = input_seconds / 60 / 60 / 24;
	int hours = (input_seconds / 60 / 60) % 24;
	int minutes = (input_seconds / 60) % 60;
	int seconds = input_seconds % 60;
	std::vector<int> date = { days, hours, minutes, seconds };
	return date;
}

/**
* \fn long convert(std::vector<int> input_date)
* \brief Fonction to convert a date into seconds
*
* \param input_date a vector which contains the date
* \return a long containing the time in seconds
*/
long convert(std::vector<int> input_date){
	return ((input_date[0] * 24 + input_date[1]) * 60 + input_date[2]) * 60 + input_date[3];
}