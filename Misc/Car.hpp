/*
 * Car.hpp
 *
 *  Created on: Jul 4, 2023
 *      Author: neiljohnson
 */

#ifndef CAR_HPP_
#define CAR_HPP_
#include <vector>
#include <string>
using namespace std;

class Car {
public:
	Car();
	Car(int, string, string, int, bool);
	virtual ~Car();
	const vector<string>& getAbsolutes() const;
	void setAbsolutes(const vector<string> &absolutes);
	const string& getDriver() const;
	void setDriver(const string &driver);
	const string& getName() const;
	void setName(const string &name);
	int getSeatsAvailable() const;
	void setSeatsAvailable(int seatsAvailable);
	int getNum() const;
	void setNum(int num);
	vector<int>& getOccupants();
	void setOccupants(const vector<int> &occupants);
	void addOccupantToCar(int);
	void addDriverToCar(int);
	void addServantToCar(int);
	void clearCar();
	int getOgSeats() const;
	void setOgSeats(int ogSeats);
	bool isServantSeat() const;
	void setServantSeat(bool servantSeat);
	bool isRunning() const;
	void setRunning(bool running);
	void removeOccupant(int person);

private:
	int num;
	string name;
	string driver;
	int seatsAvailable;
	bool running;
	int OGSeats;
	bool servantSeat;
	float avgFun;

	vector<int> occupants;
};

#endif /* CAR_HPP_ */