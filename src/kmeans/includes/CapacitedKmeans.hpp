#ifndef CAPACITEDKMEANS_HPP
#define CAPACITEDKMEANS_HPP

#include "Tour.hpp"
#include <vector>
using std::vector;
namespace Structure {

struct Centroid {
    int id;
    double x;
    double y;
};
}

namespace CapacitedCentroidCalc {
Structure::Centroid calcRouteCentroid(vector<int>& route);
vector<Structure::Centroid> getAllCentroids(Tour& tour);
bool compareCentroids(vector<Structure::Centroid> before, vector<Structure::Centroid> after);
vector<Structure::Centroid> heavierCustomersAsCentroids(Tour& tour);
}
namespace CapacitedClassifier {
vector<int> getNearestCentroids(vector<Structure::Centroid> centroids, int customer);
Tour capacitedKmeansBasic(Tour& originalTour, vector<Structure::Centroid> centroids);
}
namespace CapacitedKmeans {
Tour run(Tour& originalTour);
}
#endif