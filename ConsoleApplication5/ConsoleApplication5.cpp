// ConsoleApplication1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <chrono>
#include <fstream>
#include <iostream>
#include <numeric>
#include <filesystem>
#include <unordered_map>
#include "Open_cv.h"
#include "Haar_cascade.h"
#include "MobileNet.h"
#include "yolov3.h"
#include "yoloV4-tiny.h"
#include "HOG&SVM.h"


using namespace cv;
using namespace std;
namespace fs = std::filesystem;

unordered_map<string, int> readCSV(const string& filename);
int findNumberByName(const unordered_map<string, int>& data, const string& name);

int main()
{
	const int NUM_TEST_PICTURE = 50;
	// Define the folder path
	string images_folder_path = R"(C:\Users\Dell\Downloads\Cars3)";
	string cars_number_path = R"(C:\Users\Dell\Downloads\number_of_cars.csv)";
	unordered_map<string, int> cars_number = readCSV(cars_number_path);

	vector<pair<string, function<int(Mat&)>>> models = { 
		{ "cascade", run_haar_cascade},
		{"yolov4-Tiny", run_yoloV4Tiny},
		{"yolov3",run_yolov3},
		{"MobileNet",run_MobileNet },
		{"HOG", run_HOG}
	};
	std::ofstream resultsFile("results.csv");
	resultsFile << "every picture tested in every nodel " << NUM_TEST_PICTURE << " times\n";
	resultsFile << "Image,Model,Time(sec),found,real number,succes presentage\n";

	// Iterate over all image files in the folder
	for (const auto& model : models)
	{
		for (const auto& entry : fs::directory_iterator(images_folder_path))
		{
			string image_name = entry.path().filename().string();
			Mat image = imread(entry.path().string());

			vector<int> qualityResults(NUM_TEST_PICTURE);
			vector<chrono::microseconds> durationResults(NUM_TEST_PICTURE);

			for (int i = 0; i < NUM_TEST_PICTURE; i++) {
				auto start = chrono::high_resolution_clock::now();

				qualityResults[i] = model.second(image);

				auto end = chrono::high_resolution_clock::now();
				durationResults[i] = chrono::duration_cast<std::chrono::microseconds>(end - start);

			}

			double result = accumulate(qualityResults.begin(), qualityResults.end(), 0.0) / qualityResults.size();
			chrono::microseconds duration = accumulate(durationResults.begin(), durationResults.end(), chrono::microseconds(0)) / durationResults.size();

			resultsFile << image_name << "," << model.first << "," <<
				(double)duration.count() / 1000000 << "," << result <<
				", " << findNumberByName(cars_number, image_name) << ","
				<< result / findNumberByName(cars_number, image_name) <<
				"\n";
			//imshow("ing", image);
			//waitKey(0);
		}
	}

	resultsFile.close();
	return 0;
}




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

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file



//#pragma once
//#include <chrono>
//#include <fstream>
//#include <iostream>
//#include <filesystem>
//#include "Open_cv.h"
//#include "Haar_cascade.h"
//#include "yolov3.h"
//#include "yoloV4-tiny.h"
//#include "MobileNet.h"
//#include "HOG&SVM.h"
//
////#include "squeezenet.h"
//
//
//
//using namespace cv;
//using namespace std;
//namespace fs = std::filesystem;
//
//int main()
//{
//    // Define the folder path
//    string images_folder_path = R"(C:\Users\dmaro\Downloads\test)";
//    //vector<string> models = { "cascade", "yolov3", "squeezeNet" };
//    vector<pair<string, function<int(Mat&)>>> models = { { "cascade", run_haar_cascade},
//                                                        {"yolov4-Tiny", run_yoloV4Tiny},
//                                                         // {"yolov3",run_yolov3}, 
//                                                        {"MobileNet",run_MobileNet },
//                                                         {"HOG", run_HOG}
//    };
//
//    std::ofstream resultsFile("results.csv");
//    resultsFile << "Image,Model,Time(sec),precision\n";
//
//    // Iterate over all image files in the folder
//    for (const auto& model : models)
//    {
//
//
//        for (const auto& entry : fs::directory_iterator(images_folder_path))
//        {
//            string image_name = entry.path().filename().string();
//            Mat image = imread(entry.path().string());
//            int result = 0;
//            auto start = std::chrono::high_resolution_clock::now();
//
//            result = model.second(image);
//
//            auto end = std::chrono::high_resolution_clock::now();
//            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
//            resultsFile << image_name << "," << model.first << "," << (double)duration.count() / 1000000 << "," << result << "\n";
//            //imshow("ing", image);
//            //waitKey(0);
//        }
//    }
//
//    resultsFile.close();
//    return 0;
//}
//
//
