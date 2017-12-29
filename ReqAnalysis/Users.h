#include <random>


class User {
protected:
	int ID;
	double temporaryCharged;
public:
	User();
	virtual bool checkBudget(double price);
	void setTempCharged(double price);
	virtual void setCharged(double price);
	double getTempCharged();
};


class Student : public User{
protected:
	double instantCap;
	double cumulCapLeft;
public:
	Student();
	virtual bool checkBudget(double price);
	virtual void setCharged(double price);
};


class Researcher : public User{
protected:
	double groupBudgetLeft;
	double specialGrantLeft;
public:
	Researcher();
	virtual bool checkBudget(double price);
	virtual void setCharged(double price);
};

