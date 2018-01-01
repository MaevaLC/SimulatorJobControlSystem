/**
* \file      Users.h
* \author    m.leclech
* \version   1.0
* \date      08 January 2018
* \brief     Define the class to describe the users submiting jobs.
*
* \details   There is a mother class, User, which regroup all common elements of an user
*			 There are also the 2 sub classes, defining the different types of users.
*              - The Student
*              - The Researchers
*			 Each of them have dfferent type of budget, and so difference to check if they
*			 can afford the job.
*/

#include <random>

/**
* \class User
* \brief Base abstract class which define the attributes of an user
*
* A user is defin by different attributes, in common an ID, and an amount of
* how much they are already charged if they submitted different jobs in the queue.
* A user is temporarily charged when he submit a job, and definitely charged when
* the job is being processed.
*/
class User {
protected:
	int ID; //!< the ID of the user
	double temporaryCharged; //!< the amount the user is already temporary charged
public:
	User();
	double getTempCharged();
	virtual bool checkBudget(double price);
	virtual void setCharged(double price);
	void setTempCharged(double price);
};

/**
* \class Student
* \brief A sub class of User. It define the student user.
*
* A student is limited in his job submission by 2 factors. The first one is a
* instaneous cap, which a request cannot exceed. The second one is a cumulative
* cap, which represent a budget they can spend in the week/month/year. As an attribute,
* the non-used-yet amount is kept, for easier management. The amount allowed per
* student could be kept in a database, for a more precise simulation.
*/
class Student : public User{
protected:
	double instantCap; //!< the price cap the student can reach with one job.
	double cumulCapLeft; //!< the rest of the student budget
public:
	Student();
	bool checkBudget(double price) override;
	virtual void setCharged(double price);
};


/**
* \class Researcher
* \brief A sub class of User. It define the researcher user.
*
* A researcher is limited by 2 factors. They got 2 budget, a personnal on, and a grouped
* one. In this simulation, we only keep the non-used-yet amount for each attributes. The
* data could be hold in a database for a more accurate simulation, which would as well update
* the rest of the group.
*/
class Researcher : public User{
protected:
	double groupBudgetLeft; //!< the rest of the group budget
	double specialGrantLeft; //!< rest of the personnal budget
public:
	Researcher();
	bool checkBudget(double price) override;
	virtual void setCharged(double price);
};
