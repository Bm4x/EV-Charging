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
	void lowestTotalCost();
	void cheapestPathToDestination();
	void cheapestPathMultiDestination();
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
	while (incre != 0) { 
		cout << "\nk=" << incre;
		for (curr = incre; curr <= last; curr++) {
			hold = list[curr];
			walker = curr - incre;
			while (walker >= 0 && hold.chargingPrice < list[walker].chargingPrice) {
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

	cout <<  "\nSorted array (ascending order of charging price):   \n" ;
	for (size_t i = 0; i < list.size(); i++ ) {
		list[i].printLocation();
	}
} //  shellSort 

void EVCharging::adjacentLocation(){
	int storedValue = -1;
	string search;
	
	cout << "\nAll Adjacent Locations with charging station [Enter a location]: ";
	getline(cin, search);

	for(int i = 0; i < numberOfLocations; i++){
		if(locations[i].locationName == search){
			storedValue = i;
			break;
		}
	}
	
	if(storedValue == -1){
		cout << "Location could not be found.\n";
		return;
	}

	// grabs all adjacency locations from different class fun (using stored value)
	list<int> adjancentList = weightedGraph->getAdjancencyList(storedValue);
	
	// converting list into vector for search/iteration
	vector<int> adjancentLocations(adjancentList.begin(), adjancentList.end());
	
	for(int i = 0; i < adjancentLocations.size(); i++){
		int adjLocation = adjancentLocations[i]; 
		if(adjLocation == storedValue) continue;
		if(locations[adjLocation].chargerInstalled){
			cout << locations[adjLocation].locationName << "\n";
		}
	}
}

void EVCharging::nearestLocation(){
	int storedValue = -1;
	string search;

	cout << "\nNearest Location with available charging station [Enter a location]: ";
	getline(cin, search);

	for(int i = 0; i < numberOfLocations; i++){
		if(locations[i].locationName == search){
			storedValue = i;
			break;
		}
	}
	
	if(storedValue == -1){
		cout << "Location could not be found.\n";
		return;
	}

	weightedGraph->shortestPath(storedValue);

	double shortestDistance = DBL_MAX;
	int shortestIndex = -1;

	for(int i = 0; i < numberOfLocations; i++){
		if(locations[i].locationName == locations[storedValue].locationName || !locations[i].chargerInstalled) continue;

		double currentDistance = weightedGraph->smallestWeight[i];

		if(currentDistance < shortestDistance) {
			shortestDistance = currentDistance;
			shortestIndex = i;
		}
	}
	if(shortestIndex == -1 || shortestDistance == DBL_MAX){
		cout << "Could Not Locate Charging Station.\n";
	} else {
		cout << "The Closest Station to " << search << " is " << locations[shortestIndex].locationName << ": " << shortestDistance << "km\n";
	}
}

void EVCharging::lowestTotalCost(){
	int chargingAmount;
	int storedValue = -1;
	string search;

	cout << "\nNearest Available Charger with Lowest Total Cost (Travel & Charging) [Enter a location]: ";
	getline(cin, search);

	for(int i = 0; i < numberOfLocations; i++){
		if(locations[i].locationName == search){
			storedValue = i;
			break;
		}
	}
	
	if(storedValue == -1){
		cout << "Location could not be found.\n";
		return;
	}

	cout << "Enter Charging Amount Required [10kWh to 50kWh]: ";
	cin >> chargingAmount;

	if(chargingAmount < 10 || chargingAmount > 50){
		cout << "Invalid Number for Charging Amount";
		return;
	}

	weightedGraph->shortestPath(storedValue);

	double lowestCost = DBL_MAX;
	int lowestIndex = -1;
	double shortestDistance = DBL_MAX;


	for(int i = 0; i < numberOfLocations; i++){
		if(locations[i].locationName == locations[storedValue].locationName || !locations[i].chargerInstalled) continue;
		if(chargingAmount > 25 && locations[i].chargingPrice == 0) continue;

		double currentDistance = weightedGraph->smallestWeight[i];

    	double chargingCost = (currentDistance * 2 * 0.10) + (locations[i].chargingPrice * chargingAmount);

		if(chargingCost < lowestCost) {
			shortestDistance = currentDistance;
			lowestIndex = i;
			lowestCost = chargingCost;
		}
	}

	if(lowestIndex == -1 || shortestDistance == DBL_MAX){
		cout << "Could Not Locate Charging Station.\n";
	} else {
		cout << "Lowest Cost of Travel & Charging: $" << setprecision(4) << lowestCost << ", Location: " << locations[lowestIndex].locationName << " with Distance of " << shortestDistance << "km\n";
	}
}

void EVCharging::cheapestPathToDestination(){
	int chargingAmount;
	int startpoint = -1;
	int destination = -1;
	string startSearch, destinationSearch;
	cout << "\nTask 7: Most Efficient Path of travel with One Charge\n";

	// gathering array index and error checking startpoint and destination of trip
	cout << "\nStarting Location of Trip [Enter a location]: ";
	getline(cin, startSearch);

	cout << "\nDestination of Trip [Enter a location]: ";
	getline(cin, destinationSearch);

	for(int i = 0; i < numberOfLocations; i++){
		if(locations[i].locationName == startSearch){
			startpoint = i;
		}
		if(locations[i].locationName == destinationSearch){
			destination = i;
		}
	}

	// more error checking (if destination or start point couldnt be found)
	if(startpoint == -1){
		cout << "Starting Location could not be found.\n";
		return;
	}

	if(destination == -1){
		cout << "Destination Location could not be found.\n";
		return;
	}

	// getting charging amount
	cout << "\nEnter Charging Amount Required [10kWh to 50kWh]: ";
	cin >> chargingAmount;

	if(chargingAmount < 10 || chargingAmount > 50){
		cout << "Invalid Number for Charging Amount\n";
		return;
	}

	// runing dijkstra on all locations from starting point (saved into vector)
	weightedGraph->shortestPath(startpoint);
	vector<double> startpointDistance(numberOfLocations);
	for(int i = 0; i < numberOfLocations; i++){
		startpointDistance[i] = weightedGraph->smallestWeight[i];
	}

	// runing dijkstra on all locations from destination point (saved into vector)
	weightedGraph->shortestPath(destination);
	vector<double> destinationDistance(numberOfLocations);
	for(int i = 0; i < numberOfLocations; i++){
		destinationDistance[i] = weightedGraph->smallestWeight[i];
	}

	double lowestCost = DBL_MAX;
	int lowestIndex = -1;
	double shortestDistance = DBL_MAX;

	for(int i = 0; i < numberOfLocations; i++){
		if(!locations[i].chargerInstalled) continue;
		if(chargingAmount > 25 && locations[i].chargingPrice == 0) continue;

		double currentDistance = destinationDistance[i] + startpointDistance[i];

    	double chargingCost = (currentDistance  * 0.10) + (locations[i].chargingPrice * chargingAmount);

		if(chargingCost < lowestCost) {
			shortestDistance = currentDistance;
			lowestIndex = i;
			lowestCost = chargingCost;
		}
	}

	if(lowestIndex == -1){
		cout << "Could not find suitable path.\n";
	} else {
		cout << "Starting Point: " << locations[startpoint].locationName << "\nDestination: " << locations[destination].locationName;
		cout << "\nMost Optimal Charging Station: " << locations[lowestIndex].locationName << "\nTotal Cost (Charging & Travel): $" << setprecision(4) << lowestCost;
		cout << "\nTotal Distance: " << shortestDistance << "km\n";
	}
}

void EVCharging::cheapestPathMultiDestination(){
	int chargingAmount;
	int startpoint = -1;
	int destination =-1;
	string startSearch, destinationSearch;
	cout << "\nTask 8: Most Efficient Path of travel with Multiple Charges at Location\n";
	
	// gathering array index and error checking startpoint and destination of trip
	cout << "\nStarting Location of Trip [Enter a location]: ";
	getline(cin, startSearch);

	cout << "\nDestination of Trip [Enter a location]: ";
	getline(cin, destinationSearch);

	for(int i = 0; i < numberOfLocations; i++){
		if(locations[i].locationName == startSearch){
			startpoint = i;
		}
		if(locations[i].locationName == destinationSearch){
			destination = i;
		}
	}

	// more error checking (if destination or start point couldnt be found)
	if(startpoint == -1){
		cout << "Starting Location could not be found.\n";
		return;
	}

	if(destination == -1){
		cout << "Destination Location could not be found.\n";
		return;
	}

	// getting charging amount
	cout << "\nEnter Charging Amount Required [10kWh to 50kWh]: ";
	cin >> chargingAmount;

	if(chargingAmount < 10 || chargingAmount > 50){
		cout << "Invalid Number for Charging Amount\n";
		return;
	}

	// runing dijkstra on all locations from starting point (saved into vector)
	weightedGraph->shortestPath(startpoint);
	vector<double> startpointDistance(numberOfLocations);
	for(int i = 0; i < numberOfLocations; i++){
		startpointDistance[i] = weightedGraph->smallestWeight[i];
	}

	// runing dijkstra on all locations from destination point (saved into vector)
	weightedGraph->shortestPath(destination);
	vector<double> destinationDistance(numberOfLocations);
	for(int i = 0; i < numberOfLocations; i++){
		destinationDistance[i] = weightedGraph->smallestWeight[i];
	}

	double lowestCost = DBL_MAX;
	double totalDistance;

	int firstStation = -1;
	double firstCharge;

	int secondStation = -1;
	double secondCharge;

	for(int i = 0; i < numberOfLocations; i++){
		if(!locations[i].chargerInstalled) continue;

		// runs Dijkstra to on first station to get all other station distances
		weightedGraph->shortestPath(i);
		vector<double> secondStationSearch(numberOfLocations);
		for(int n = 0; n < numberOfLocations; n++){
			secondStationSearch[n] = weightedGraph->smallestWeight[n];
		}

		for(int j = 0; j < numberOfLocations; j++){
			// checking all conditions that would force an continue
			if(locations[i].locationName == locations[j].locationName) continue;
			if(!locations[i].chargerInstalled) continue;
			if(!locations[j].chargerInstalled) continue;

			if(locations[i].chargingPrice == 0 && locations[j].chargingPrice == 0) continue;
			
			// if the above condition did not go off there might possible be atleast 1 free station
			if(locations[i].chargingPrice == 0){
				firstCharge = min(25, chargingAmount);
				secondCharge = chargingAmount - firstCharge;
			} 
			else if (locations[j].chargingPrice == 0){
				secondCharge = min(25, chargingAmount);
				firstCharge = chargingAmount - secondCharge;
			} 
			else if (locations[i].chargingPrice >= locations[j].chargingPrice) {
				firstCharge = 0;
				secondCharge = chargingAmount;
			} 
			else {
				firstCharge = chargingAmount;
				secondCharge = 0;
			}

			// stops pointless calculations on non compatiable values 
			if(firstCharge < 0 || secondCharge < 0) continue;

			double currentDistance = startpointDistance[i] +  secondStationSearch[j] + destinationDistance[j] ;

    		double totalCost = (currentDistance  * 0.10) + ((locations[i].chargingPrice * firstCharge) + (locations[j].chargingPrice * secondCharge));

			if(totalCost < lowestCost) {
				firstStation = i;
				secondStation = j;
				lowestCost = totalCost;
				totalDistance = currentDistance;
			}
		}
	}

	if(firstStation == -1 && secondStation == -1){
		cout << "Could not find suitable path.\n";
	} else if (secondStation == -1) {
		cout << "Starting Point: " << locations[startpoint].locationName << "\nDestination: " << locations[destination].locationName;
		cout << "\nMost Optimal Charging Station: " << locations[firstStation].locationName << "\nTotal Cost (Charging & Travel): $" << lowestCost;
		cout << "\nTotal Distance: " << totalDistance << "km\n";
	} else {
		cout << "Starting Point: " << locations[startpoint].locationName << "\nDestination: " << locations[destination].locationName;
		cout << "\nFirst Most Optimal Charging Station: " << locations[firstStation].locationName << ", Charge Amount: " << firstCharge<< "kWh";
		cout << "\nSecond Most Optimal Charging Station: " << locations[secondStation].locationName<< ", Charge Amount: " << secondCharge << "kWh";
		cout << "\nTotal Cost (Charging & Travel): $" << setprecision(4) << lowestCost << "\nTotal Distance: " << totalDistance << "km\n";
	}
}

#endif /* EVCHARGING_H_ */
