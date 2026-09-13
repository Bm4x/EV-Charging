#include <cfloat>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <list>
#include <map>
#include <queue>
#include <string>

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
    charging.nearestLocation();
    charging.lowestTotalCost();
    charging.cheapestPathToDestination();
    charging.cheapestPathMultiDestination();
    charging.fastestRoute();
    return 0;
}
