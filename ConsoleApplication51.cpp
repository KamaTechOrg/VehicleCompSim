#include <chrono>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <unordered_map>
#include <deque>
#include <memory> // Add this for shared_ptr

//#include "open_cv.h"
#include "yolov3.h"
#include "MakeCollage.h"
//=============================================================================
using namespace cv;
using namespace std;
namespace fs = std::filesystem;
//=============================================================================
unordered_map<string, int> readCSV(const string& filename);
int findNumberByName(const unordered_map<string, int>& data, const string& name);
//=============================================================================
int main()
{
    string images_folder_path = R"(C:\Users\dmaro\Downloads\Cars3)";

    std::ofstream resultsFile("results.csv");
    resultsFile << "Image,Model,Time(sec),precision,real number,succes presentage\n";

    for (const auto& entry : fs::directory_iterator(images_folder_path))
    {
        string image_name = entry.path().filename().string();
        //Mat image = imread(entry.path().string());
        Mat img = imread(entry.path().string());
        int screenWidth = 1000;
        int screenHeight = 500;
        cv::Mat image;
        cv::resize(img, image, cv::Size(screenWidth, screenHeight));
        int result = 0;
        auto start = std::chrono::high_resolution_clock::now();
        std::vector<cv::Rect> diff_boxes = run_yolov3(image);//Elimelech

        cv::Mat crroped_collage = optimizeCollageGreedy(image, diff_boxes);

        cv::imshow("crroped_collage", crroped_collage);
        waitKey(0);

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        resultsFile << image_name << "," << "yoloV3" << "," <<
            (double)duration.count() / 1000000 << "," << result <<
            ", " << /*findNumberByName(cars_number, image_name) <<*/ ","
            << /*(double)result / findNumberByName(cars_number, image_name) <<*/
            "\n";
    }

    resultsFile.close();
    return 0;
}
//=============================================================================
// Function to read a CSV file into a map of names to numbers
unordered_map<string, int> readCSV(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file\n";
        return {};
    }

    unordered_map<string, int> data;
    string line;

    // Read header
    if (getline(file, line)) {
        // Optionally process the header if needed
    }

    // Read data lines
    while (getline(file, line)) {
        stringstream ss(line);
        string name;
        string number_str;

        if (getline(ss, name, ',') && getline(ss, number_str)) {
            try {
                int number = stod(number_str);
                data[name] = number;
            }
            catch (const invalid_argument& e) {
                cerr << "Invalid number format in line: " << line << "\n";
            }
        }
    }

    file.close();
    return data;
}
//=============================================================================
// Function to find a number by name
int findNumberByName(const unordered_map<string, int>& data, const string& name) {
    auto it = data.find(name);
    if (it != data.end()) {
        return it->second;
    }
    else {
        cerr << "Name not found: " << name << "\n";
        return -1; // Return an appropriate error value or handle as needed
    }
}
//=============================================================================
