#include <filesystem>
#include <iostream>
#include "yolov3.h"
using namespace std;
using namespace cv;
namespace fs = std::filesystem;

void max_object_area_detection(Mat& image,string& image_name) {
    try {
        double ratio_of_max_object = run_yolov3(image, image_name);
    }
	catch (const std::exception& e) {
		cerr << "Error | max_object_area_detection: " << e.what() << endl;
		return;
	}
}

void iterate_over_images(const std::string& images_folder_path) {
    try {
        for (const auto& entry : fs::directory_iterator(images_folder_path))
        {
            try {
                string image_name = entry.path().filename().string();
                Mat image = imread(entry.path().string());
                if (image.empty()) {
                    cerr << "Filesystem error | iterate_over_images: Could not open or find the image at: " << entry.path() << endl;
                    return;
                }
                max_object_area_detection(image, image_name);
            }
            catch (const fs::filesystem_error& e) {
                cerr << "Filesystem error | iterate_over_images: " << e.what() << endl;
                return;
            }
        }
    }
    catch (const std::exception& e) {
        cerr << "Error |iterate_over_images: " << e.what() << endl;
        return;
    }
}

int main()
{
    const std::string images_folder_path = DATA_DIR "\\images"; 

    iterate_over_images(images_folder_path);
  
    return 0;
}
