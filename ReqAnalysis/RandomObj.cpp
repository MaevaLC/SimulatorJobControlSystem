#include "RandomObj.h"


User *getRandomUser() {
	int r = std::rand() % 2;
	switch (r) {
	case 0: return new Student();
	case 1: return new Researcher();
	default: return NULL;
	}
}