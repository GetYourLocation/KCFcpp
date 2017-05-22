#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iomanip>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "kcftracker.hpp"

using namespace std;
using namespace cv;

template<typename T>
std::string toString(const T &obj) {
    std::ostringstream oss;
    oss << std::setiosflags(std::ios::fixed) << obj;
    return oss.str();
}

int main(int argc, char* argv[]) {

    if (argc > 5) return -1;

    bool HOG = true;
    bool FIXEDWINDOW = false;
    bool MULTISCALE = true;
    bool SILENT = true;
    bool LAB = false;

    for(int i = 0; i < argc; i++) {
        if (strcmp(argv[i], "hog") == 0) {
            HOG = true;
        }
        if (strcmp(argv[i], "fixed_window") == 0) {
            FIXEDWINDOW = true;
        }
        if (strcmp(argv[i], "singlescale") == 0) {
            MULTISCALE = false;
        }
        if (strcmp(argv[i], "show") == 0) {
            SILENT = false;
        }
        if (strcmp(argv[i], "lab") == 0) {
            LAB = true;
            HOG = true;
        }
        if (strcmp (argv[i], "gray") == 0) {
            HOG = false;
        }
    }

    // Read config file
    ifstream configFile;
    string config = "../config.txt";
    configFile.open(config);
    string configLine;
    getline(configFile, configLine);
    configFile.close();
    
    // Read config like a dumb
    istringstream ss(configLine);
    int totFrames;
    float xMin, yMin, xMax, yMax;
    string label;
    ss >> totFrames >> label >> xMin >> yMin >> xMax >> yMax;

    // Write Results
    ofstream resultsFile;
    string resultsPath = "result.txt";
    resultsFile.open(resultsPath);

    // Create KCFTracker object
    KCFTracker tracker(HOG, FIXEDWINDOW, MULTISCALE, LAB);

    for (int i = 1; i <= totFrames; ++i) {
        cout << "Computing frame " << i << endl;

        string frameName = "../frames/" + toString(i) + ".jpg";
        Mat frame = imread(frameName, CV_LOAD_IMAGE_COLOR);

        if (i == 1) {  // First frame, give the groundtruth to the tracker
            tracker.init(Rect(xMin, yMin, xMax - xMin, yMax - yMin), frame);
            rectangle(frame, Point(xMin, yMin), Point(xMax, yMax), Scalar(0, 255, 255), 1, 8);
            resultsFile << toString(i) << ".jpg "
                        << label << " "
                        << xMin << " "
                        << yMin << " "
                        << xMax << " "
                        << yMax << " ";
        } else {  // Update
            Rect result = tracker.update(frame);
            rectangle(frame, Point(result.x, result.y), Point(result.x + result.width, result.y + result.height), Scalar( 0, 255, 255 ), 1, 8);
            resultsFile << toString(i) << ".jpg "
                        << label << " "
                        << result.x << " "
                        << result.y << " "
                        << result.x + result.width << " "
                        << result.y + result.height << " ";
        }

        if (i < totFrames) {
            resultsFile << endl;
        }

        if (!SILENT) {
            imshow("Frame", frame);
            waitKey(50);
        }
    }
    cout << "Results are stored in \"" + resultsPath + "\"." << endl;
    resultsFile.close();
}
