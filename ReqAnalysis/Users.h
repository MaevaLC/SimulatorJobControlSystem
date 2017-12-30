#include <random>

class User {
protected:
	int ID;
	double temporaryCharged;
public:
	User();
	double getTempCharged();
	virtual bool checkBudget(double price);
	virtual void setCharged(double price);
	void setTempCharged(double price);
};

class Student : public User{
protected:
	double instantCap;
	double cumulCapLeft;
public:
	Student();
	bool checkBudget(double price) override;
	virtual void setCharged(double price);
};

class Researcher : public User{
protected:
	double groupBudgetLeft;
	double specialGrantLeft;
public:
	Researcher();
	bool checkBudget(double price) override;
	virtual void setCharged(double price);
};
