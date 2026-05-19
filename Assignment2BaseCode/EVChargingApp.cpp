#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <list>
#include <queue>
#include <map>

using namespace std;

#include "Location.h"
#include "WeightedGraph.h"
#include "EVCharging.h"

int main() {
	EVCharging charging;
	charging.printLocations();
	charging.printAdjacencyMatrix();
	charging.chargingStationAscending();
	charging.adjacentLocation();

	return 0;
}
