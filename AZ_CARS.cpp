#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_set>
#include <random>
#include <iomanip>
#include "Misc/Person.hpp"
#include "Misc/Car.hpp"

using namespace std;

vector<Person> getPeople(string fileName);
vector<Car> getCars(string fileName);
void makeNextCars(vector<vector<unsigned short> >& adjList, vector<Person>&
			people, vector<Car>& cars, vector<int>& absolutes, vector<int>& drivers,
				unordered_set<int>& adults, vector<int>& housingteam, vector<vector<unsigned short> >& comboCounter);
void printPeople(vector<Person>& p);
void printCars(vector<Car>& c);
int findBestCar(vector<Car>& c, vector<vector<unsigned short> >& adjList, int person, vector<Person>& people);
int findBestCarServant(vector<Car>& c, vector<vector<unsigned short> >& adjList, int person, vector<Person>& people);
void printCarOccupants(vector<Car>& c, vector<Person>& p);
int countCarAdjListSum(int person, vector<vector<unsigned short> >& adjList, Car& c);
vector<vector<unsigned short> > preventPair(vector<vector<unsigned short> >& adjList);
vector<vector<unsigned short> > favorPair(vector<vector<unsigned short> >& adjList);
vector<vector<unsigned short> > updateAdjList(vector<vector<unsigned short> >& adjList, vector<Car>& cars, vector<vector<unsigned short> >& comboCounter);
vector<vector<unsigned short> > loadFromFile(vector<vector<unsigned short> >& adjList, vector<vector<unsigned short> >& comboCounter);
void saveToFile(vector<vector<unsigned short> >& adjList, vector<vector<unsigned short> >& comboCounter);
void writeCarsToFile(vector<Car>& c, vector<Person>& p);
int getCarQuality(vector<Car>& c, vector<vector<unsigned short> >& adjList);
vector<vector<unsigned short> > removePairHistory(vector<vector<unsigned short> >& adjList, vector<vector<unsigned short> >& comboCounter);
vector<vector<unsigned short> > addPairHistory(vector<vector<unsigned short> >& adjList, vector<vector<unsigned short> >& comboCounter);

int main() {
	// welcome message
	cout << "welcome to my program!\n";

	// setup
	string peopleFile = "Misc/2023_PeopleFileAZ.txt";
	string carFile = "Misc/2023_CarFileAZ.txt";
	vector<Person> people;
	vector<Car> cars;
	bool loadedList = false;
	bool exit1 = false;
	try {
		people = getPeople(peopleFile);
		cars = getCars(carFile);
	}
	catch(const string& e) {
		cout << e << endl;
		exit1 = true;
	}



	vector<vector<unsigned short> > adjList(people.size(), vector<unsigned short>(people.size(), 60));
	vector<vector<unsigned short> > comboCounter(people.size(), vector<unsigned short>(people.size(), 0));

	vector<int> absolutes;
	for (int i = 0; i < people.size(); i++) {
		if (people[i].isAbsolute() && people[i].isAlive()) {
			absolutes.push_back(i);
		}
	}

	vector<int> drivers;
	for (int i = 0; i < people.size(); i++) {
		if (people[i].isDriver() && people[i].isAlive()) {
			drivers.push_back(i);
		}
	}

	unordered_set<int> adults;
	for (int i = 0; i < people.size(); i++) {
		if (people[i].isAdult() && people[i].isAlive()) {
			adults.insert(i);
		}
	}

	vector<int> housingteam;
	for (int i = 0; i < people.size(); i++) {
		if (people[i].isHousing() && people[i].isAlive()) {
			housingteam.push_back(i);
		}
	}


	while (exit1 == false) {
		// menu options
		cout << "\nhere are your options:\n";
		cout << "01) make the next cars\n";
		cout << "02) prevent a pair\n";
		cout << "03) favor a pair\n";
		cout << "04) manually enter cars for list\n";
		cout << "05) load in previous adjacency list (read in file)\n";
		cout << "06) save the results for next time (write to file)\n";
		cout << "07) print out all the people\n";
		cout << "08) print out all the cars\n";
		cout << "09) add pair history\n";
		cout << "10) remove pair history\n";

		cout << "99) exit the program! (make sure to save!)\n";
		cout << "please enter your choice: ";
		int menuOption;
		cin >> menuOption;

		switch(menuOption) {
		case 1:
			if (!loadedList) {
				cout << "you have a default list. do you wish to proceed (y/n)? ";
				char userInput;
				cin >> userInput;
				if (userInput != 'y') {
					break;
				}
			}
			makeNextCars(adjList, people, cars, absolutes, drivers, adults, housingteam, comboCounter);
			loadedList = true;
			break;
		case 2:
			adjList = preventPair(adjList);
			break;
		case 3:
			adjList = favorPair(adjList);
			break;
		case 4:
			// for every car in file
			for (int i = 0; i < cars.size(); i++) {
				// if car is dead skip over
				if (!cars[i].isRunning()) {
					continue;
				}

				// get all the people in every car
				cout << "CAR #" << i << " - " << cars[i].getName() << endl;
				cout << "driver (int): ";
				int userI;
				cin >> userI;
				cars[i].addDriverToCar(userI);
				cout << "servant seat: ";
				cin >> userI;
				cars[i].addServantToCar(userI);

				while (cars[i].getSeatsAvailable() > 0) {
					cout << "occupant (-1 to go to next car): ";
					cin >> userI;
					if (userI == -1) {
						break;
					}
					else {
						cars[i].addOccupantToCar(userI);
					}
				}
			}
			updateAdjList(adjList, cars, comboCounter);
			break;
		case 5:
			adjList = loadFromFile(adjList, comboCounter);
			loadedList = true;
			break;
		case 6:
			saveToFile(adjList, comboCounter);
			break;
		case 7:
			printPeople(people);
			break;
		case 8:
			printCars(cars);
			break;
		case 9:
			addPairHistory(adjList, comboCounter);
			break;
		case 10:
			removePairHistory(adjList, comboCounter);
			break;
		case 99:
			// make sure the user saved
			char userInput;
			cout << "are you sure you saved the adj list (y/n)? ";
			cin >> userInput;
			if (userInput == 'y') {
				exit1 = true;
			} else {}
			break;
		}
	}

	// goodbye message
	cout << "program ending, have a nice day" << endl;
	return 0;
}

// function that gets people from a file
vector<Person> getPeople(string fileName) {
	vector<Person> thePeople;
	fstream peopleFile;
	peopleFile.open(fileName, ios::in);
	if (peopleFile.fail()) {
		throw string("could not open file");
	}

	for (int i = 0; !peopleFile.eof(); i++) {
		string currFirstName;
		string currLastName;
		bool currDriver;
		int driverCar = -1;
		bool currAdult;
		bool currHousing;
		bool currAbsolute;
		bool alive;
		int absoluteCar = -1;
		int funFac;
		peopleFile >> currFirstName >> currLastName >> currDriver;
		if (currDriver) {
			peopleFile >> driverCar;
		}
		peopleFile >> currAdult >> currHousing >> currAbsolute;
		if (currAbsolute) {
			peopleFile >> absoluteCar;
		}
		peopleFile >> alive;
		peopleFile >> funFac;
		string currName = currFirstName + " " + currLastName;

		Person currPerson(currName, currDriver, driverCar, currAdult, currHousing, 0, currAbsolute, i, absoluteCar, alive, funFac);
		thePeople.push_back(currPerson);
	}
	peopleFile.close();
	return thePeople;
}

// function that gets cars from a file
vector<Car> getCars(string fileName) {
	vector<Car> theCars;
	fstream carFile;
	carFile.open(fileName, ios::in);
	if (carFile.fail()) {
		throw string("could not open file");
	}

	for(int i = 0; !carFile.eof(); i++) {
		string currName;
		string currDriverFirstName;
		string currDriverLastName;
		bool running;
		int currSeatsAvailable;
		carFile >> currName >> currDriverFirstName>> currDriverLastName >> currSeatsAvailable >> running;
		string currDName = currDriverFirstName + " " + currDriverLastName;
		Car currCar(i, currName, currDName, currSeatsAvailable, running);
		if (!running) {
			currCar.setSeatsAvailable(0);
		}
		theCars.push_back(currCar);
	}
	carFile.close();
	return theCars;
}

// function that prints the people in the file
void printPeople(vector<Person>& p) {
	for (int i = 0; i < p.size(); i++) {
		cout << "#" << p[i].getID() << ": " << p[i].getName() << endl;
		if (p[i].isAdult()) {
			cout << "\tadult\n";
		}
		if (p[i].isHousing()) {
			cout << "\thousing team\n";
		}
		if (p[i].isAlive()) {
			cout << "\ton trip\n";
		}
		if (p[i].isAbsolute()) {
			cout << "\tabsolute car: ";
			if (p[i].getAbsoluteCar() == -1) {
				cout << "liu kid\n";
			}
			else if (p[i].getAbsoluteCar() == -2) {
				cout << "liu parent\n";
			}
			else {
				cout << p[i].getAbsoluteCar() << endl;
			}
		}
		cout << endl;
	}
}

// function that prints the cars NOT the occupants of the cars
void printCars(vector<Car>& c) {
	for (int i = 0; i < c.size(); i++) {
		cout << "#" << c[i].getNum() << ": " << c[i].getName() << "\n\tdriver: "
				<< c[i].getDriver() << "\n\tnumber of seats: " << c[i].getSeatsAvailable() << endl << endl;
	}
}

void makeNextCars(vector<vector<unsigned short> >& adjList, vector<Person>&
		people, vector<Car>& cars, vector<int>& absolutes, vector<int>& drivers,
			 	 unordered_set<int>& adults, vector<int>& housingteam, vector<vector<unsigned short> >& comboCounter) {
	bool house;
	cout << "does the housing team need to be together (y/n)? ";

	char userInput;
	cin >> userInput;
	if (userInput == 'y') {
		house = true;
	}
	else {
		house = false;
	}


	bool satisfied = false;
	while (!satisfied) {
		vector<vector<Car> > carVecVec;
		vector<int> carVecQuality;

		// the amount of combonations that will be made
		const int OPTIONS = 50;
		for (int q = 0; q < OPTIONS; q++) {
			// clear cars if still running
			for (int i = 0; i < cars.size(); i++) {
				if (cars[i].isRunning()) {
					cars[i].clearCar();
				}
			}

			// fill up set of people to assign with everyone still on trip
			unordered_set<int> peopleToAssign;
			for (int i = 0; i < people.size(); i++) {
				// liu exception
				if (people[i].isAlive() && i != 12 && i != 13 && i != 66) {
					peopleToAssign.insert(i);
				}
				else {}
			}

			// put drivers in their cars
			for (int i = 0; i < drivers.size(); i++) {
				if (!people[drivers[i]].isAlive()) {
					continue;
				}
				int currCar = people[drivers[i]].getDriverCar();
				int currID = people[drivers[i]].getID();
				cars[currCar].addDriverToCar(currID);
				peopleToAssign.erase(currID);
			}

			if (house) {
				cars[9].addServantToCar(housingteam[0]);
				peopleToAssign.erase(housingteam[0]);
				for (int i = 1; i < housingteam.size(); i++) {
					int currID = housingteam[i];
					// skip over tom sidebottom
					if (currID == 63) {
						continue;
					}
					else {
						cars[9].addOccupantToCar(currID);
					}
					peopleToAssign.erase(currID);
				}
				cars[9].setSeatsAvailable(0);
			}
			else {}

			// put absolutes in their car
			for (int i = 0; i < absolutes.size(); i++) {
				if (!people[absolutes[i]].isAlive()) {
					continue;
				}
				int currCar = people[absolutes[i]].getAbsoluteCar();
				// liu kid special case
				if (currCar == -1) {
					continue;
				}
				int currID = people[absolutes[i]].getID();
				if (people[absolutes[i]].isAdult() && !cars[currCar].isServantSeat()) {
					cars[currCar].addServantToCar(currID);
				}
				else {
					cars[currCar].addOccupantToCar(currID);
				}
				peopleToAssign.erase(currID);
			}


			// set the cars

			// set servant seats
			// count open servant seats
			int openServSeats = 0;
			for (int i = 0; i < cars.size(); i++) {
				if (cars[i].isRunning() && !cars[i].isServantSeat()) {
					openServSeats++;
				}
				else {}
			}

			// for every open servant seat
			for (int i = 0; i < openServSeats; i++) {
				int randomAdultIndex = 0;
				int randomAdult = -2;

				// get a random adult
				do {
					random_device rd;
					mt19937 gen(rd());
					uniform_int_distribution<> dis(0, adults.size() - 1);
					randomAdultIndex = dis(gen);
					auto it = adults.begin();
					for (int j = 0; j < randomAdultIndex; j++) {
						it++;
					}
					randomAdult = *it;
				} while (peopleToAssign.find(randomAdult) == peopleToAssign.end());

				// find best car for that adult
				int bestCar = findBestCarServant(cars, adjList, randomAdult, people);

				// add adult to car as servant
				cars[bestCar].addServantToCar(randomAdult);

				// erase person from people to assign set
				peopleToAssign.erase(randomAdult);

				// liu family special
				if (randomAdult == 54) {
					cars[bestCar].addOccupantToCar(12);
				}
				else if (randomAdult == 55) {
					cars[bestCar].addOccupantToCar(13);
				}
				else if (randomAdult == 67) {
					cars[bestCar].addOccupantToCar(66);
				}
			}


			// liu family special case AGAIN
			if (peopleToAssign.find(54) != peopleToAssign.end()) {
				int bestCar = findBestCar(cars, adjList, 54, people);
				cars[bestCar].addOccupantToCar(54);
				peopleToAssign.erase(54);
				cars[bestCar].addOccupantToCar(12);
			}

			if (peopleToAssign.find(67) != peopleToAssign.end()) {
				int bestCar = findBestCar(cars, adjList, 67, people);
				cars[bestCar].addOccupantToCar(67);
				peopleToAssign.erase(67);
				cars[bestCar].addOccupantToCar(66);
			}

			// liu family special case AGAIN
			if (peopleToAssign.find(55) != peopleToAssign.end()) {
				int bestCar = findBestCar(cars, adjList, 55, people);
				cars[bestCar].addOccupantToCar(55);
				peopleToAssign.erase(55);
				cars[bestCar].addOccupantToCar(13);
			}

			// fill rest
			while (!peopleToAssign.empty()) {
				int randomPersonIndex = 0;
				random_device rd;
				mt19937 gen(rd());
				uniform_int_distribution<> dis(0, peopleToAssign.size() - 1);
				randomPersonIndex = dis(gen);
				auto it = peopleToAssign.begin();
				for (int i = 0; i < randomPersonIndex; i++) {
					it++;
				}
				cars[findBestCar(cars, adjList, *it, people)].addOccupantToCar(*it);
				peopleToAssign.erase(it);
			}

			// make sure there's no more than 1 empty spot per car
			vector<int> carFullList;
			vector<int> carNeedList;

			// find every car that needs people and every car that is full and can give a person
			for (int i = 0; i < cars.size(); i++) {
				if (cars[i].getSeatsAvailable() == 0 && i != 4) {
					carFullList.push_back(i);
				}
				else if (cars[i].getSeatsAvailable() > 1) {
					carNeedList.push_back(i);
				}
			}

			// for every car that needs to fill a spot
			for (int i = 0; i < carNeedList.size(); i++) {
				// while there is more than 1 spot unfilled
				while (cars[carNeedList[i]].getSeatsAvailable() > 1) {
					int lowestPerson = -1;
					int lowest = INT_MAX;
					int carToPullFrom = -1;
					// for every full car
					for (int j = 0; j < carFullList.size(); j++) {
						// for every person in full car other than driver and servant
						for (int k = 2; k < cars[carFullList[j]].getOccupants().size(); k++) {
							// i dont even remember how this works
							int curr = countCarAdjListSum(cars[carFullList[j]].getOccupants()[k], adjList, cars[carNeedList[i]]);
							if (curr < lowest && !people[cars[carFullList[j]].getOccupants()[k]].isAbsolute()
									&& cars[carFullList[j]].getOccupants()[k] != 54 
										&& cars[carFullList[j]].getOccupants()[k] != 55 
											&& cars[carFullList[j]].getOccupants()[k] != 67 
												&& cars[carFullList[j]].getOccupants()[k] != 66 
													&& cars[carFullList[j]].getOccupants()[k] != 12 
														&& cars[carFullList[j]].getOccupants()[k] != 13) {
								lowest = curr;
								carToPullFrom = carFullList[j];
								lowestPerson = cars[carFullList[j]].getOccupants()[k];
							}
						}
					}
					cars[carToPullFrom].removeOccupant(lowestPerson);
					cars[carNeedList[i]].addOccupantToCar(lowestPerson);
					int curr = 0;
					while (carFullList[curr] != carToPullFrom) {
						curr += 1;
					}
					while (curr != carFullList.size() - 1) {
						carFullList[curr] = carFullList[curr + 1];
						curr++;
					}
					carFullList.pop_back();
				}
			}


			// // dan truck special case almost same as above i know this is inefficient whatever
			// if (cars[4].getSeatsAvailable() > 0) {
			// 	int lowestPerson = -1;
			// 	int lowest = INT_MAX;
			// 	int carToPullFrom = -1;
			// 	for (int j = 0; j < carFullList.size(); j++) {
			// 		for (int k = 2; k < cars[carFullList[j]].getOccupants().size(); k++) {
			// 			int curr = countCarAdjListSum(cars[carFullList[j]].getOccupants()[k], adjList, cars[carNeedList[4]]);
			// 			if (curr < lowest && !people[cars[carFullList[j]].getOccupants()[k]].isAbsolute()
			// 					&& cars[carFullList[j]].getOccupants()[k] != 54 
			// 						&& cars[carFullList[j]].getOccupants()[k] != 55 
			// 							&& cars[carFullList[j]].getOccupants()[k] != 12
			// 								&& cars[carFullList[j]].getOccupants()[k] != 13
			// 									&& cars[carFullList[j]].getOccupants()[k] != 67
			// 										&& cars[carFullList[j]].getOccupants()[k] != 66) {
			// 				lowest = curr;
			// 				carToPullFrom = carFullList[j];
			// 				lowestPerson = cars[carFullList[j]].getOccupants()[k];
			// 			}
			// 		}
			// 	}
			// 	cars[carToPullFrom].removeOccupant(lowestPerson);
			// 	cars[4].addOccupantToCar(lowestPerson);
			// 	int curr = 0;
			// 	while (carFullList[curr] != carToPullFrom) {
			// 		curr += 1;
			// 	}
			// 	while (curr != carFullList.size() - 1) {
			// 		carFullList[curr] = carFullList[curr + 1];
			// 	}
			// 	carFullList.pop_back();
			// }


			// push back current cars to vector of cars
			carVecVec.push_back(cars);

			// find current cars score from adj list and push it onto vector that holds quality
			int currQuality = getCarQuality(cars, adjList);
			carVecQuality.push_back(currQuality);
		}


		// find best cars generated
		int bestCarsIndex = -1;
		int lowest = INT_MAX;
		for (int i = 0; i < carVecQuality.size(); i++) {
			if (carVecQuality[i] < lowest) {
				lowest = carVecQuality[i];
				bestCarsIndex = i;
			}
		}
		// set cars to best
		cars = carVecVec[bestCarsIndex];

		// print the cars
		cout << "here is a preview of the cars: " << endl;
		printCarOccupants(cars, people);
		while (true) {
			// ask if the user is satisfied with the current cars
			cout << "would you like to keep the cars (y) or regenerate (n)? ";
			char userInput;
			cin >> userInput;
			if (userInput == 'y') {
				updateAdjList(adjList, cars, comboCounter);
				writeCarsToFile(cars, people);
				satisfied = true;
				break;
			}
			else if (userInput == 'n') {
				break;
			}
			else {}
		}
	}

	// clear cars if running
	for (int i = 0; i < cars.size(); i++) {
		if (cars[i].isRunning()) {
			cars[i].clearCar();
		}
	}
}

// given a person and the cars, this function finds the best car with open spots that the person would fit into
// see findBestServantCar its almost the exact same
int findBestCar(vector<Car>& c, vector<vector<unsigned short> >& adjList, int person, vector<Person>& people) {
	// vector for the candidates--this will only have more than one car if there is a tie
	vector<int> carCandidates;

	// vector to count the talkability in each car
	vector<int> carFun;

	// fill the carFun vec with -1 so that there are no mistakes later
	for (int i = 0; i < c.size(); i++) {
		carFun.push_back(-1);
	}

	// set this to infinity so that we will guaranteed have a car with lower sum
	int currLowestAdjSum = INT_MAX;

	// for every car
	for (int i = 0; i < c.size(); i++) {
		// ints to keep track of the cars adj list sum and talkability sum
		int currSum = 0;
		int currFun = 0;

		// if the car is running and has an open seat
		if (c[i].isRunning() && c[i].getSeatsAvailable() > 0) {
			// for every occupant
			for (int j = 0; j < c[i].getOccupants().size(); j++) {
				// add to the adj list num
				currSum += adjList[person][c[i].getOccupants()[j]];

				// add to the talkability sum
				currFun += people[c[i].getOccupants()[j]].getFun();
			}

			// update the carFun vector
			carFun[i] = currFun;

			// if the current car is a better candidate than the previous lowest
			if (currSum < currLowestAdjSum) {
				// update lowest
				currLowestAdjSum = currSum;

				// clear the candidates vector
				carCandidates.clear();

				// put the new car in the candidates
				carCandidates.push_back(i);
			}
			// if it ties
			else if (currSum == currLowestAdjSum) {
				// push back this candidate
				carCandidates.push_back(i);
			}
		}
	}

	// if only one candidate return it
	if (carCandidates.size() == 1) {
		return carCandidates[0];
	}
	else {
		// check whether the current person would fit more into a talkable car or not talkable car
		bool funSide = people[person].getFun() == 1 ? 1 : 0;

		// this part is confusing im not commenting on it
		vector<int> candidatesFun;
		for (int i = 0; i < carCandidates.size(); i++) {
			candidatesFun.push_back(carFun[carCandidates[i]]);
		}
		int highest = -100;
		int lowest = 100;
		int highestIndex = -1;
		int lowestIndex = -1;
		for (int i = 0; i < candidatesFun.size(); i++) {
			if (candidatesFun[i] > highest) {
				highest = candidatesFun[i];
				highestIndex = i;
			}
			if (candidatesFun[i] < lowest) {
				lowest = candidatesFun[i];
				lowestIndex = i;
			}
		}

		if (funSide) {
			return carCandidates[lowestIndex];
		}
		else {
			return carCandidates[highestIndex];
		}
	}
}

// given a person and the cars, this function finds the best car with an open servant seat that the person would fit into
int findBestCarServant(vector<Car>& c, vector<vector<unsigned short> >& adjList, int person, vector<Person>& people) {
	// vector for the candidates--this will only have more than one car if there is a tie
	vector<int> carCandidates;

	// vector to count the talkability in each car
	vector<int> carFun;

	// fill the carFun vec with -1 so that there are no mistakes later
	for (int i = 0; i < c.size(); i++) {
		carFun.push_back(-1);
	}

	// set this to infinity so that we will guaranteed have a car with lower sum
	int currLowestAdjSum = INT_MAX;

	// for every car
	for (int i = 0; i < c.size(); i++) {
		// ints to keep track of the cars adj list sum and talkability sum
		int currSum = 0;
		int currFun = 0;

		// if the car is running and has an open servant seat
		if (c[i].isRunning() && !c[i].isServantSeat()) {
			// for every occupant
			for (int j = 0; j < c[i].getOccupants().size(); j++) {
				// add to the adj list sum
				currSum += adjList[person][c[i].getOccupants()[j]];

				// add to the talkability sum
				currFun += people[c[i].getOccupants()[j]].getFun();
			}

			// update the carFun vector
			carFun[i] = currFun;

			// if the current car is a better candidate than the previous lowest
			if (currSum < currLowestAdjSum) {
				// update lowest
				currLowestAdjSum = currSum;

				// clear the candidates vector
				carCandidates.clear();

				// put the new car in the candidates
				carCandidates.push_back(i);
			}
			// if it ties
			else if (currSum == currLowestAdjSum) {
				// push back this candidate
				carCandidates.push_back(i);
			}
		}
	}

	// if only one candidate return it
	if (carCandidates.size() == 1) {
		return carCandidates[0];
	}
	else {
		// check whether the current person would fit more into a talkable car or not talkable car
		bool funSide = people[person].getFun() == 1 ? 1 : 0;

		// this part is confusing im not commenting on it
		vector<int> candidatesFun;
		for (int i = 0; i < carCandidates.size(); i++) {
			candidatesFun.push_back(carFun[carCandidates[i]]);
		}
		int highest = -100;
		int lowest = 100;
		int highestIndex = -1;
		int lowestIndex = -1;
		for (int i = 0; i < candidatesFun.size(); i++) {
			if (candidatesFun[i] > highest) {
				highest = candidatesFun[i];
				highestIndex = i;
			}
			if (candidatesFun[i] < lowest) {
				lowest = candidatesFun[i];
				lowestIndex = i;
			}
		}

		if (funSide) {
			return carCandidates[lowestIndex];
		}
		else {
			return carCandidates[highestIndex];
		}
	}
}

// given the cars and the people vecs, this function prints the cars to std::out
void printCarOccupants(vector<Car>& c, vector<Person>& p) {
	for (int i = 0; i < c.size(); i++) {
		cout << "=====================================\n";
		cout << "#" << i + 1 << " " << c[i].getName() << ":\n";
		if (c[i].isRunning()) {
			cout << "current available spots: " << c[i].getSeatsAvailable() << endl;

			if (c[i].getOccupants()[0] > 9) {
				cout << "#" << c[i].getOccupants()[0] << ": " << p[c[i].getOccupants()[0]].getName() << " (driver)\n";
			}
			else {
				cout << "#0" << c[i].getOccupants()[0] << ": " << p[c[i].getOccupants()[0]].getName() << " (driver)\n";
			}

			if (c[i].getOccupants()[1] > 9) {
				cout << "#" << c[i].getOccupants()[1] << ": " << p[c[i].getOccupants()[1]].getName() << " (servant seat)\n";
			}
			else {
				cout << "#0" << c[i].getOccupants()[1] << ": " << p[c[i].getOccupants()[1]].getName() << " (servant seat)\n";
			}

			for (int j = 2; j < c[i].getOccupants().size(); j++) {
				if (c[i].getOccupants()[j] > 9) {
					cout << "#" << c[i].getOccupants()[j] << ": " << p[c[i].getOccupants()[j]].getName() << endl;
				}
				else {
					cout << "#0" << c[i].getOccupants()[j] << ": " << p[c[i].getOccupants()[j]].getName() << endl;
				}
			}
		}
		else {
			cout << "DEAD\n";
		}
		cout << "=====================================\n";
	}
}

// this function takes a person and a car to count how well that person would fit into the car
int countCarAdjListSum(int person, vector<vector<unsigned short> >& adjList, Car& c) {
	int sum = 0;
	// for every occupant in the car
	for (int i = 0; i < c.getOccupants().size(); i++) {
		sum += adjList[person][c.getOccupants()[i]];
	}
	return sum;
}

// this function can be used to try to prevent a pair (almost guaranteed)
vector<vector<unsigned short> > preventPair(vector<vector<unsigned short> >& adjList) {
	// get the people from the user by their IDs
	int x, y;
	do {
		cout << "who is person 1 to prevent(int, -99 to cancel)? ";
		cin >> x;
		if (x == -99) {
			return adjList;
		}
	} while (!(x < adjList.size() && x >= 0));

	do {
		cout << "who is person 2 (int)? ";
		cin >> y;
	} while (!(x < adjList.size() && x >= 0));

	// add to their adj list slots to prevent them in the coming car assignments
	adjList[x][y] += 1000;
	adjList[y][x] += 1000;

	return adjList;
}

// this function can be used to favor a certain pair of people
vector<vector<unsigned short> > favorPair(vector<vector<unsigned short> >& adjList) {
	// get the people from the user by their IDs
	int x, y;
	do {
		cout << "who is person 1 to favor (int, -99 to cancel)? ";
		cin >> x;
		if (x == -99) {
			return adjList;
		}
	} while (!(x < adjList.size() && x >= 0));

	do {
		cout << "who is person 2 (int)? ";
		cin >> y;
	} while (!(x < adjList.size() && x >= 0));

	// subtract 40 from those
	adjList[x][y] -= 30;
	adjList[y][x] -= 30;

	return adjList;
}

// given a vector of cars, this function updates the adj list and the combo counting list
vector<vector<unsigned short> > updateAdjList(vector<vector<unsigned short> >& adjList, vector<Car>& cars, vector<vector<unsigned short> >& comboCounter) {
	// marking list
	vector<vector<bool> > markingList(adjList.size(), vector<bool>(adjList.size(), 0));

	// for every car
	for (int i = 0; i < cars.size(); i++) {
		if (!cars[i].isRunning()) {
			continue;
		}
		// for every occupant in every car
		for (int j = 0; j < cars[i].getOccupants().size(); j++) {
			// for every other occupant in that car
			for (int k = j + 1; k < cars[i].getOccupants().size(); k++) {
				// marking list update
				markingList[cars[i].getOccupants()[j]][cars[i].getOccupants()[k]] = true;
				markingList[cars[i].getOccupants()[k]][cars[i].getOccupants()[j]] = true;

				// adj list update
				adjList[cars[i].getOccupants()[j]][cars[i].getOccupants()[k]] += 50;
				adjList[cars[i].getOccupants()[k]][cars[i].getOccupants()[j]] += 50;

				// combo counting list update
				comboCounter[cars[i].getOccupants()[k]][cars[i].getOccupants()[j]]++;
				comboCounter[cars[i].getOccupants()[j]][cars[i].getOccupants()[k]]++;

			}
		}
	}

	// if we want adj list to fade over time
	for (int i = 0; i < adjList.size(); i++) {
		for (int j = 0; j < adjList.size(); j++) {
			if (!markingList[i][j] && adjList[i][j] > 9) {
				adjList[i][j] -= 5;
			}
		}
	}
	return adjList;
}

// remove 50 and 1 from list and combo counter of 2 people
vector<vector<unsigned short> > removePairHistory(vector<vector<unsigned short> >& adjList, vector<vector<unsigned short> >& comboCounter) {
	int p1, p2;
	cout << "who is person 1? ";
	cin >> p1;
	cout << "who is person 2? ";
	cin >> p2;

	// adj list update
	if (adjList[p1][p2] > 50) {
		adjList[p1][p2] -= 50;
		adjList[p2][p1] -= 50;
	}
	else {
		adjList[p1][p2] = 0;
		adjList[p2][p1] = 0;
	}


	// combo counting list update
	comboCounter[p1][p2]--;
	comboCounter[p2][p1]--;

	return adjList;
}

// add 50 and 1 from list and combo counter of 2 people
vector<vector<unsigned short> > addPairHistory(vector<vector<unsigned short> >& adjList, vector<vector<unsigned short> >& comboCounter) {
	int p1, p2;
	cout << "who is person 1? ";
	cin >> p1;
	cout << "who is person 2? ";
	cin >> p2;

	// adj list update
	adjList[p1][p2] += 50;
	adjList[p2][p1] += 50;


	// combo counting list update
	comboCounter[p1][p2]++;
	comboCounter[p2][p1]++;

	return adjList;
}


// loads the adj list and combo counter from a file that was previously saved to
vector<vector<unsigned short> > loadFromFile(vector<vector<unsigned short> >& adjList, vector<vector<unsigned short> >& comboCounter) {
	string fileName;
	string garbage;
	cout << "enter the filename to load list from ('exit' to quit): ";
	cin >> fileName;
	if (fileName == "exit") {
		return adjList;
	}

	fstream theFile;
	theFile.open(fileName, ios::in);
	if (theFile.fail()) {
		cout << "file failed to open\n";
		return adjList;
	}
	else {
		for (int i = 0; i < adjList.size(); i++) {
			for (int j = 0; j < adjList.size(); j++) {
				// throw away the readability string
				theFile >> garbage;
				theFile >> adjList[i][j];
			}
		}
	}

	cout << "loaded!\n";
	theFile.close();

	string fileName2;
	cout << "enter the filename to load combos from ('exit' to quit): ";
	cin >> fileName2;
	if (fileName2 == fileName) {
		cout << "file names cannot be the same\n";
		return adjList;
	}
	if (fileName == "exit") {
		return adjList;
	}

	fstream theFile2;
	theFile2.open(fileName2, ios::in);
	if (theFile2.fail()) {
		cout << "file failed to open\n";
		return adjList;
	}
	else {
		for (int i = 0; i < adjList.size(); i++) {
			for (int j = 0; j < adjList.size(); j++) {
				theFile2 >> garbage;
				theFile2 >> comboCounter[i][j];
			}
		}
	}

	cout << "loaded!\n";
	theFile2.close();

	return adjList;
}

// takes the adj list and the combo counter array and saves them to a file for later use
void saveToFile(vector<vector<unsigned short> >& adjList, vector<vector<unsigned short> >& comboCounter) {
	// get file name from user to save adj list to
	string fileName;
	cout << "enter the filename to save list to ('exit' to quit): ";
	cin >> fileName;
	if (fileName == "exit") {
		return;
	}

	fstream theFile;
	theFile.open(fileName, ios::out | ios::trunc);
	if (theFile.fail()) {
		cout << "file failed to open\n";
		return;
	}
	else {
		for (int i = 0; i < adjList.size(); i++) {
			for (int j = 0; j < adjList.size(); j++) {
				// this makes it easier to read the file
				string temp = to_string(i) + ":" + to_string(j);

				// store the current value
				theFile << setw(6) << left << temp << setw(6) << left << adjList[i][j] << " ";
			}
			theFile << "\n";
		}
	}

	cout << "saved!\n";
	theFile.close();

	// same thing with combo counter

	string fileName2;
	cout << "enter the filename to save combos to ('exit' to quit): ";
	cin >> fileName2;
	if (fileName2 == fileName) {
		cout << "file names cannot be the same\n";
		return;
	}
	if (fileName == "exit") {
		return;
	}

	fstream theFile2;
	theFile2.open(fileName2, ios::out | ios::trunc);
	if (theFile2.fail()) {
		cout << "file failed to open\n";
		return;
	}
	else {
		for (int i = 0; i < comboCounter.size(); i++) {
			for (int j = 0; j < comboCounter.size(); j++) {
				string temp = to_string(i) + ":" + to_string(j);
				theFile2 << setw(6) << left << temp << setw(6) << left << comboCounter[i][j] << " ";
			}
			theFile2 << "\n";
		}
	}

	cout << "saved!\n";
	theFile2.close();
}

// writes finished cars to a file
void writeCarsToFile(vector<Car>& c, vector<Person>& p) {
	fstream theFile;
	// open the file
	theFile.open("currentCars.txt", ios::out | ios::trunc);
	if (theFile.fail()) {
		cout << "failed to open file\n";
		return;
	}

	// for each car in the list, print the occupants
	for (int i = 0; i < c.size(); i++) {
		theFile << "=====================================\n";
		theFile << "#" << i + 1 << " " << c[i].getName() << ":\n";
		if (c[i].isRunning()) {
			theFile << "current available spots: " << c[i].getSeatsAvailable() << endl;

			if (c[i].getOccupants()[0] > 9) {
				theFile << "#" << c[i].getOccupants()[0] << ": " << p[c[i].getOccupants()[0]].getName() << " (driver)\n";
			}
			else {
				theFile << "#0" << c[i].getOccupants()[0] << ": " << p[c[i].getOccupants()[0]].getName() << " (driver)\n";
			}

			if (c[i].getOccupants()[1] > 9) {
				theFile << "#" << c[i].getOccupants()[1] << ": " << p[c[i].getOccupants()[1]].getName() << " (servant seat)\n";
			}
			else {
				theFile << "#0" << c[i].getOccupants()[1] << ": " << p[c[i].getOccupants()[1]].getName() << " (servant seat)\n";
			}

			for (int j = 2; j < c[i].getOccupants().size(); j++) {
				if (c[i].getOccupants()[j] > 9) {
					theFile << "#" << c[i].getOccupants()[j] << ": " << p[c[i].getOccupants()[j]].getName() << endl;
				}
				else {
					theFile << "#0" << c[i].getOccupants()[j] << ": " << p[c[i].getOccupants()[j]].getName() << endl;
				}
			}
		}
		else {
			theFile << "DEAD\n";
		}
		theFile << "=====================================\n";
	}
	// close the file
	theFile.close();
}

// vector that takes a vector of filled cars and returns the quality of that list of cars based
// on how much each person in every car has been with everyone else in their car
int getCarQuality(vector<Car>& c, vector<vector<unsigned short> >& adjList) {
	int returnVal = 0;

	// loop through each car
	for (int i = 0; i < c.size(); i++) {
		if (!c[i].isRunning()) {
			continue;
		}
		// loop through every occupant in every car
		for (int j = 0; j < c[i].getOccupants().size(); j++) {
			// add to return value the amount that person has been with everyone else in the car
			for (int k = j + 1; k < c[i].getOccupants().size(); k++) {
				returnVal += adjList[c[i].getOccupants()[j]][c[i].getOccupants()[k]];
			}
		}
	}

	return returnVal;
}

Car::Car() : num(0), name(""), driver(""), seatsAvailable(0), running(1) {
	OGSeats = 0;
	servantSeat = 0;
	avgFun = 0.0;
}

Car::Car(int num_, string name_, string driver_, int seatsAvail, bool run)
		: num(num_), name(name_), driver(driver_), seatsAvailable(seatsAvail), running(run) {
	OGSeats = seatsAvail;
	servantSeat = 0;
	avgFun = 0.0;
	occupants.push_back(-1);
	occupants.push_back(-1);
	if (!run) {
		seatsAvailable = 0;
		servantSeat = 1;
	}
}

Car::~Car() {}

const string& Car::getDriver() const {
	return driver;
}

void Car::setDriver(const string &driver) {
	this->driver = driver;
}

const string& Car::getName() const {
	return name;
}

void Car::setName(const string &name) {
	this->name = name;
}

int Car::getSeatsAvailable() const {
	return seatsAvailable;
}

void Car::setSeatsAvailable(int seatsAvailable) {
	this->seatsAvailable = seatsAvailable;
}

int Car::getNum() const {
	return num;
}

void Car::setNum(int num) {
	this->num = num;
}

vector<int>& Car::getOccupants() {
	return occupants;
}

void Car::setOccupants(const vector<int> &occupants) {
	this->occupants = occupants;
}

void Car::addOccupantToCar(int num) {
	if (seatsAvailable > 0) {
		this->occupants.push_back(num);
		seatsAvailable--;
	}
	else {
		cout << "not enough space in car\n";
	}
}

void Car::addDriverToCar(int num) {
	occupants[0] = num;
	seatsAvailable--;
}

void Car::addServantToCar(int num) {
	occupants[1] = num;
	servantSeat = true;
	seatsAvailable--;
}

void Car::clearCar() {
	this->occupants.clear();
	occupants.push_back(-1);
	occupants.push_back(-1);
	seatsAvailable = OGSeats;
	servantSeat = false;
}

int Car::getOgSeats() const {
	return OGSeats;
}

void Car::setOgSeats(int ogSeats) {
	OGSeats = ogSeats;
}

bool Car::isServantSeat() const {
	return servantSeat;
}

void Car::setServantSeat(bool servantSeat) {
	this->servantSeat = servantSeat;
}

bool Car::isRunning() const {
	return running;
}

void Car::setRunning(bool running) {
	this->running = running;
}

void Car::removeOccupant(int person) {
	int curr = 0;
	while (occupants[curr] != person) {
		curr++;
	}
	while (curr != occupants.size() - 1) {
		occupants[curr] = occupants[curr + 1];
		curr++;
	}
	occupants.pop_back();
	seatsAvailable++;
}


//Person::Person() : name(""), driver(0), driverCar(-1), adult(0), housing(0), devoteam(0), absolute(0), id(-1), absoluteCar(-1), alive(1), fun(0) {}

Person::Person(string n, bool driver_, int driverCar_, bool adult_, bool housing_, bool devoteam_, bool absolute_, int id_, int abs, bool live, int funny)
	: name(n), driver(driver_), driverCar(driverCar_), adult(adult_), housing(housing_), devoteam(devoteam_), absolute(absolute_), id(id_), absoluteCar(abs), alive(live), fun(funny) {}

Person::~Person() {}

bool Person::isAdult() const {
	return adult;
}

void Person::setAdult(bool adult) {
	this->adult = adult;
}

bool Person::isDevoteam() const {
	return devoteam;
}

void Person::setDevoteam(bool devoteam) {
	this->devoteam = devoteam;
}

bool Person::isHousing() const {
	return housing;
}

void Person::setHousing(bool housing) {
	this->housing = housing;
}

int Person::getID() const {
	return id;
}

void Person::setID(int id) {
	this->id = id;
}

const string& Person::getName() const {
	return name;
}

void Person::setName(const string &name) {
	this->name = name;
}

bool Person::isAbsolute() const {
	return absolute;
}

void Person::setAbsolute(bool absolute) {
	this->absolute = absolute;
}

int Person::getAbsoluteCar() const {
	return absoluteCar;
}

void Person::setAbsoluteCar(int absoluteCar) {
	this->absoluteCar = absoluteCar;
}

bool Person::isDriver() const {
	return driver;
}

void Person::setDriver(bool driver) {
	this->driver = driver;
}

int Person::getDriverCar() const {
	return driverCar;
}

void Person::setDriverCar(int driverCar) {
	this->driverCar = driverCar;
}

bool Person::isAlive() const {
	return alive;
}

void Person::setAlive(bool alive) {
	this->alive = alive;
}

float Person::getFun() const {
	return fun;
}

void Person::setFun(float fun) {
	this->fun = fun;
}

int Person::getId() const {
	return id;
}

void Person::setId(int id) {
	this->id = id;
}