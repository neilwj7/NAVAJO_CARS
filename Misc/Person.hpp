/*
 * Person.hpp
 *
 *  Created on: Jul 4, 2023
 *      Author: neiljohnson
 */

#ifndef PERSON_HPP_
#define PERSON_HPP_
#include <string>
using namespace std;

class Person {
public:
	//Person();
	Person(string, bool, int, bool, bool, bool, bool, int, int, bool, int);
	virtual ~Person();
	bool isAdult() const;
	void setAdult(bool adult);
	bool isDevoteam() const;
	void setDevoteam(bool devoteam);
	bool isHousing() const;
	void setHousing(bool housing);
	int getID() const;
	void setID(int id);
	const string& getName() const;
	void setName(const string &name);
	bool isAbsolute() const;
	void setAbsolute(bool absolute);
	int getAbsoluteCar() const;
	void setAbsoluteCar(int absoluteCar);
	bool isDriver() const;
	void setDriver(bool driver);
	int getDriverCar() const;
	void setDriverCar(int driverCar);
	bool isAlive() const;
	void setAlive(bool alive);
	float getFun() const;
	void setFun(float fun);
	int getId() const;
	void setId(int id);

private:
	string name;
	bool driver;
	int driverCar;
	bool adult;
	bool housing;
	bool devoteam;
	bool absolute;
	int id;
	int absoluteCar;
	bool alive;
	int fun;
};

#endif /* PERSON_HPP_ */
