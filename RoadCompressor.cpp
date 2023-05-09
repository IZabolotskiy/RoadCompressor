#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

// Euclidean distance between two points
double distance(double x1, double y1, double x2, double y2) {
    return sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1));
}

// Compresses a line path to a specified length, with the ability to preserve specified segments
vector<pair<double,double>> compressLinePath(vector<pair<double,double>> linePath, double compressedLength, vector<pair<int,int>> preservedSegments) {
    // Calculate the length of the input line path
    double inputLength = 0;
    for (int i = 0; i < linePath.size()-1; i++) {
        inputLength += distance(linePath[i].first, linePath[i].second, linePath[i+1].first, linePath[i+1].second);
    }
    
    // Determine the compression ratio
    double compressionRatio = compressedLength / inputLength;
    
    // Initialize a new list for the compressed line path
    vector<pair<double,double>> compressedPath;
    
    // Iterate through the original line path
    double remainingLength = 0;
    for (int i = 0; i < linePath.size()-1; i++) {
        // Check if the current segment should be preserved
        bool preserveSegment = false;
        for (int j = 0; j < preservedSegments.size(); j++) {
            if (i >= preservedSegments[j].first && i < preservedSegments[j].second) {
                preserveSegment = true;
                break;
            }
        }
        if (preserveSegment) {
            // Add the endpoint of the preserved segment to the compressed line path
            compressedPath.push_back(linePath[i+1]);
            remainingLength = 0;
        } else {
            double segmentLength = distance(linePath[i].first, linePath[i].second, linePath[i+1].first, linePath[i+1].second);
            if (segmentLength + remainingLength >= compressionRatio*inputLength) {
                // Add a new point to the compressed line path
                double partialLength = compressionRatio*inputLength - remainingLength;
                double t = partialLength / segmentLength;
                double newX = linePath[i].first + t*(linePath[i+1].first - linePath[i].first);
                double newY = linePath[i].second + t*(linePath[i+1].second - linePath[i].second);
                compressedPath.push_back(make_pair(newX, newY));
                remainingLength = segmentLength - partialLength;
            } else {
                // Add the endpoint of the segment to the compressed line path
                compressedPath.push_back(linePath[i+1]);
                remainingLength += segmentLength;
            }
        }
    }
    
    // Scale the compressed path to the desired width (10 meters)
    for (int i = 0; i < compressedPath.size(); i++) {
        compressedPath[i].first *= 10.0 / 50.0;
    }
    
    return compressedPath;
}

int main() {
    // Example usage
    vector<pair<double,double>> linePath = {{0,0}, {1,2}, {3,4}, {5,6}, {7,8}, {9,10}};
    double compressedLength = 20.0;
    vector<pair<int,int>> preservedSegments = {{0,2}, {4,5}};
    vector<pair<double,double>> compressedPath = compressLinePath(linePath, compressedLength, preservedSegments);
// Print the compressed path
for (int i = 0; i < compressedPath.size(); i++) {
    cout << "(" << compressedPath[i].first << ", " << compressedPath[i].second << ")" << endl;
}

return 0;