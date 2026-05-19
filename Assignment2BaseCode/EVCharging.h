/*
 * EVCharging.h
 *
 *  Created on: 26 Apr 2026
 *      Author: Dongmo
 */

#ifndef EVCHARGING_H_
#define EVCHARGING_H_

class EVCharging {// you may also declare this class as an extension of WeightedGraph
private:
	map<int, Location> locations;//It can also be unordered_map
	int numberOfLocations;
	WeightedGraphType* weightedGraph;
public:
	EVCharging();
	~EVCharging();
	void inputLocations();
	void printLocations();
	void printAdjacencyMatrix();
	void chargingStationAscending();
	void adjacentLocation();
	void nearestLocation();
	void shortestPath(int vertex);
};

EVCharging::EVCharging() {
	inputLocations();
	weightedGraph = new WeightedGraphType(numberOfLocations);
}

EVCharging::~EVCharging() {
	delete weightedGraph;
}
void EVCharging::inputLocations() {
	ifstream infile;
	char fileName[50] = "Locations.txt";

//	cout << "Enter charging location file name: ";
//	cin >> fileName;
//	cout << endl;

	infile.open(fileName);

	if (!infile) {
		cout << "Cannot open input file." << endl;
		return;
	}

	int locationIndex = 0;

	while (!infile.eof()) {
		Location s;
		string charger;
		string price;
		while (!infile.eof()) {
			getline(infile, s.locationName, ',');
			getline(infile, charger, ',');
			getline(infile, price);
			s.chargerInstalled = (stoi(charger) == 1) ? true : false;
			s.chargingPrice = stod(price);
			s.index = locationIndex;
			locations[locationIndex] = s;
			locationIndex++;
		}
	}

	numberOfLocations = locationIndex;
}

void EVCharging::printLocations() {
	cout << "List of locations and charging information " << endl;
	cout << setw (8) << "Index" << setw (20) << "Location name" << setw (20) <<"Charging station" << setw(20) << "Charging price" << endl;

	map<int, Location>::iterator it = locations.begin();

	for (; it != locations.end(); it++) {
		it->second.printLocation();
	}

    cout << endl;
}

void EVCharging::printAdjacencyMatrix() {
	cout << "Adjacency matrix (0 means no direct connection, non-zero value represents the distance of adjacent locations)\n" << endl;
	cout << setw(13) << " ";
	for (int i = 0; i < numberOfLocations; i++) {
		cout << setw(13) << locations[i].locationName;
	}
	cout << endl;
	for (int i = 0; i < numberOfLocations; i++) {
		cout << setw(13) << locations[i].locationName;
		for (int j = 0; j < numberOfLocations; j++) {
			cout << setw(13) << (weightedGraph->getWeight(i,j) == DBL_MAX ? 0.0 : weightedGraph->getWeight(i,j));
		}
		cout << endl;
	}
}

void EVCharging::chargingStationAscending() {
	vector<Location> list;
	// filling vector
	for(int i = 0; i < numberOfLocations; i++){
		if(locations[i].chargerInstalled){
			list.push_back(locations[i]);
		}
	}

	// error checking
	if (list.empty()) return;
	//	Local Definitions 
	int last = list.size() - 1;
	Location hold;
	int incre;
	int curr;
	int walker;

//	Statements 
	incre = last / 2;
	while (incre != 0)
	   { cout << "\nk=" << incre;
	    for (curr = incre; curr <= last; curr++) 
	       {
	        hold = list[curr];
	        walker = curr - incre;
	        while (walker >= 0 && hold.chargingPrice < list[walker].chargingPrice)
	           {
	            // Move larger element up in list 
	            list[walker + incre] = list[walker];
	            //  Fall back one partition 
	            walker = (walker - incre);
	           } //  while 
	        // Insert hold in proper relative position 
	        list[walker + incre] = hold;
	       } // for 
	    //  End of pass--calculate next increment. 
	    incre = incre / 2;
	   } // while 

	cout <<  "\nSorted array:   " ;
	for (size_t i = 0; i < list.size(); i++ ){
		list[i].printLocation();
	}
		
	
} //  shellSort 

void EVCharging::adjacentLocation(){
	int storedValue;
	string search;

	cout << "Enter a location: ";
	getline(cin, search);

	for(int i = 0; i < numberOfLocations; i++){
		if(locations[i].locationName == search){
			storedValue = i;
			break;
		}
	}
	
	if(!storedValue){
		cout << "Location could not be found.\n";
		return;
	}

	// grabs all adjacency locations from different class fun (using stored value)
	list<int> adjancentList = weightedGraph->getAdjancencyList(storedValue);
	
	// converting list into vector for search/iteration
	vector<int> adjancentLocations(adjancentList.begin(), adjancentList.end());
	
	for(size_t i = 0; i < adjancentLocations.size(); i++){
		int adjLocation = adjancentLocations[i]; 
		if(locations[adjLocation].chargerInstalled){
			cout << locations[adjLocation].locationName << "\n";
		}
	}
}

void EVCharging::nearestLocation(){
	int storedValue;
	string search;

	cout << "Enter a location: ";
	getline(cin, search);

	for(int i = 0; i < numberOfLocations; i++){
		if(locations[i].locationName == search){
			storedValue = i;
			break;
		}
	}
	
	if(!storedValue){
		cout << "Location could not be found.\n";
		return;
	}

	// grabs all adjacency locations from different class fun (using stored value)
	list<int> adjancentList = weightedGraph->getAdjancencyList(storedValue);
	
	// converting list into vector for search/iteration
	vector<int> adjancentLocations(adjancentList.begin(), adjancentList.end());

	double shortestDistance;

	for(size_t i = 0; i < adjancentLocations.size(); i++){

		double currentDistance = weightedGraph->shortestPath(i);

		if(currentDistance < shortestDistance){
			shortestDistance = currentDistance; 
		}
	}
	cout << "Shortest Distance is " << shortestDistance;
}

#endif /* EVCHARGING_H_ */
