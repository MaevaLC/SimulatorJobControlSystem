#include <random>


class User {
protected:
	int ID;
public:
	User();
	virtual bool checkBudget(long price);
};


class Student : public User{
protected:
	long instantCap;
	long cumulCapLeft;
public:
	Student();
	virtual bool checkBudget(long price);
};


class Researcher : public User{
protected:
	long groupBudgetLeft;
	long specialGrantLeft;
public:
	Researcher();
	virtual bool checkBudget(long price);
};

