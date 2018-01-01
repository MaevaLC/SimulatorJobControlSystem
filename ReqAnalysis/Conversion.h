/**
* \file      Conversion.h
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

#include <vector>

std::vector<int> convert(long input_seconds);
long convert(std::vector<int> input_date);