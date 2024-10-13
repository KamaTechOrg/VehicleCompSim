#pragma once
//#ifndef DATA_DIR
//#error "DATA_DIR is not defined!"
//#endif

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <opencv2/opencv.hpp>
#include "util.h"
#include "Poligon.h"
#include <functional>
#include "SafeQueue.h"


const std::string CFG_FILE = DATA_DIR "\\weights\\yolov3.cfg";
const std::string WEIGHTS_FILE = DATA_DIR "\\weights\\yolov3.weights";
const std::string CATEGORIES_FILE = DATA_DIR "\\weights\\coco.names";


const std::string DEPARTURE_RIGHT_WARNING = "Lane Departure Right Detected!";
const std::string DEPARTURE_LEFT_WARNING = "Lane Departure Left Detected!";

constexpr bool SAVE_FIRST_IMG = false;
constexpr int WAIT_KEY_SEC_DETECTION = 1;
constexpr int WAIT_KEY_SEC_TRACKERS = 1;

constexpr bool RUN_VERSION_1_OF_MAPS = true;
//FOR VISUALIZATION:
constexpr bool ENABLE_DRAWING = true;
constexpr bool DRAW_RELATIVE_SPEED = false;
constexpr bool DRAW_RELATIVE_SPEED_WITH_DISTANCE = false;
constexpr bool DRAW_WARNINGS_MSG = true;


namespace OBJECT_CATEGORY
{
    inline const std::string CAR = "car";
    inline const std::string PERSON = "person";
    inline const std::string TRUCK = "truck";
    inline const std::string BICYCLE = "bicycle";
    inline const std::string MOTORBIKE = "motorbike";
    inline const std::string BUS = "bus";
    inline const std::string TRAIN = "train";
    inline const std::string TRAFFIC_LIGHT = "traffic light";
}

const std::string VIDEO_DIR = DATA_DIR "\\videos";
const std::string VIDEO_PATH = VIDEO_DIR + "\\01c71072-718028b8.mov";
//const std::string VIDEO_PATH = VIDEO_DIR + "\\01f2bc5a-5a845805.mov";
//const std::string VIDEO_PATH = VIDEO_DIR+"\\0000f77c-6257be58.mov";PROVING DAVID'S HYPOTESYS
constexpr int FRAME_BEGIN = 0;// 580;


constexpr int FRAMES_NUM_UNTIL_INIT_TRACKERS = 18;


// Constants
constexpr float FOCAL_LENGTH = 700.0f;

constexpr float SQUER_RATIO_LIMIT = 1.2f;
constexpr float CARMERA_TO_CAR_BOUNDARIS = 2.5f;

constexpr float VIDEO_FPS = 30.18f;
constexpr float CENTER_DISTANCE_THRESHOLD = 50.f;
constexpr float MIN_INTERSECTION_AREA = 0.5f;
const std::pair<int, int> IMAGE_BORDERS = std::pair<int, int>(1280, 720);//cols, rows

const std::unordered_map<std::string, float> WARNING_PERCENTAGE_MAP = {
    {"car", 0.15},
    {"person", 0.03},
    {"traffic light", 100.0},
    {"truck", 0.3},
    {"bicycle", 0.1},
    {"motorbike", 0.1},
    {"bus", 0.3},
    {"train", 0.2}
};

const std::unordered_map<std::string, std::pair<float, float>> OBJECTS_HEIGHT_WIDTH = {
    {"car", {1.55f, 1.2f}},
    {"person", {1.7f, 0.4f}},
    {"truck", {3.2f, 2.5f}},
    {"bus", {3.2f, 2.5f}}
};

namespace TRAPEZOID
{
    namespace CRITICAL_ZONE_2
    {
        inline const cv::Point DL(390, 450);
        inline const cv::Point DR(735, 450);
        inline const cv::Point UR(650, 370);
        inline const cv::Point UL(540, 370);
    }
    namespace CRITICAL_ZONE_1
    {
        inline const cv::Point DL(111, 590);
        inline const cv::Point DR(880, 590);
        inline const cv::Point UR(735, 450);
        inline const cv::Point UL(390, 450);
    }
    namespace CAR_HOOD
    {
        inline const cv::Point DL(0, 780);
        inline const cv::Point DR(1280, 780);
        inline const cv::Point UR(1277, 590);
        inline const cv::Point UL(3, 590);
    }
    namespace TRACKED_ZONE
    {
        const int base = 200;
        const float  percent = float(CRITICAL_ZONE_2::UR.x - CRITICAL_ZONE_2::UL.x) / float(CRITICAL_ZONE_1::DR.x - CRITICAL_ZONE_1::DL.x);

        inline const cv::Point DL(CRITICAL_ZONE_1::DL.x - base, CRITICAL_ZONE_1::DL.y);
        inline const cv::Point DR(CRITICAL_ZONE_1::DR.x + base, CRITICAL_ZONE_1::DR.y);
        inline const cv::Point UR(CRITICAL_ZONE_2::UR.x + int(percent * float(base)), CRITICAL_ZONE_2::UL.y);
        inline const cv::Point UL(CRITICAL_ZONE_2::UL.x - int(percent * float(base)), CRITICAL_ZONE_2::UL.y);
    }
    namespace CRITICAL_ZONE
    {
        //ZONE PRIORITY 1:
        inline const cv::Point DL = CRITICAL_ZONE_1::DL;
        inline const cv::Point DR = CRITICAL_ZONE_1::DR;
        //ZONE PRIORITY 2:
        inline const cv::Point UR = CRITICAL_ZONE_2::UR;
        inline const cv::Point UL = CRITICAL_ZONE_2::UL;
    }
}

// Create the array of pairs consisting of the trapezoid and its warning priority
const std::vector<std::pair<Poligon, ZONE_TYPES>> ZONES_POLIGON = {
    // CRITICAL_ZONE_1 - Critical priority
    { Poligon({TRAPEZOID::CRITICAL_ZONE_1::DL, TRAPEZOID::CRITICAL_ZONE_1::DR, TRAPEZOID::CRITICAL_ZONE_1::UR, TRAPEZOID::CRITICAL_ZONE_1::UL}), ZONE_TYPES::CRITICAL_ZONE_1 },

    // CRITICAL_ZONE_2 - High priority
    { Poligon({TRAPEZOID::CRITICAL_ZONE_2::DL, TRAPEZOID::CRITICAL_ZONE_2::DR, TRAPEZOID::CRITICAL_ZONE_2::UR, TRAPEZOID::CRITICAL_ZONE_2::UL}), ZONE_TYPES::CRITICAL_ZONE_2 },

    // TRACKED_ZONE - Low priority
    { Poligon({TRAPEZOID::TRACKED_ZONE::DL, TRAPEZOID::TRACKED_ZONE::DR, TRAPEZOID::TRACKED_ZONE::UR, TRAPEZOID::TRACKED_ZONE::UL}), ZONE_TYPES::TRACKED_ZONE},

    // CAR_HOOD - Medium priority
    { Poligon({TRAPEZOID::CAR_HOOD::DL, TRAPEZOID::CAR_HOOD::DR, TRAPEZOID::CAR_HOOD::UR, TRAPEZOID::CAR_HOOD::UL}), ZONE_TYPES::CAR_HOOD }

};

//map from pair of zoneType and object category ,to warning priority and description
const std::unordered_map<std::pair<ZONE_TYPES, std::string>, std::pair<WarningPriority, std::string>, pair_hash> ZONE_OBJECT_TO_PRIORITY_DESCRIPTION_MAP = {
    { {ZONE_TYPES::CRITICAL_ZONE_1, OBJECT_CATEGORY::CAR}, {WarningPriority::Critical, "Car in CRITICAL_ZONE_1"} },
    { {ZONE_TYPES::CRITICAL_ZONE_1, OBJECT_CATEGORY::PERSON}, {WarningPriority::Critical, "person in CRITICAL_ZONE_1"} },
    { {ZONE_TYPES::CRITICAL_ZONE_1, OBJECT_CATEGORY::TRUCK}, {WarningPriority::Critical, "truck in CRITICAL_ZONE_1"} },
    { {ZONE_TYPES::CRITICAL_ZONE_1, OBJECT_CATEGORY::BICYCLE}, {WarningPriority::Critical, "bicycle in CRITICAL_ZONE_1"} },
    { {ZONE_TYPES::CRITICAL_ZONE_1, OBJECT_CATEGORY::MOTORBIKE}, {WarningPriority::Critical, "motorbike in CRITICAL_ZONE_1"} },
    { {ZONE_TYPES::CRITICAL_ZONE_1, OBJECT_CATEGORY::BUS}, {WarningPriority::Critical, "bus in CRITICAL_ZONE_1"} },
    { {ZONE_TYPES::CRITICAL_ZONE_1, OBJECT_CATEGORY::TRAIN}, {WarningPriority::Critical, "train in CRITICAL_ZONE_1"} },

    { {ZONE_TYPES::CRITICAL_ZONE_2, OBJECT_CATEGORY::CAR}, {WarningPriority::Safe, "Car in CRITICAL_ZONE_2"} },
    { {ZONE_TYPES::CRITICAL_ZONE_2, OBJECT_CATEGORY::PERSON}, {WarningPriority::Critical, "person in CRITICAL_ZONE_2"} },
    { {ZONE_TYPES::CRITICAL_ZONE_2, OBJECT_CATEGORY::TRUCK}, {WarningPriority::High, "truck in CRITICAL_ZONE_2"} },
    { {ZONE_TYPES::CRITICAL_ZONE_2, OBJECT_CATEGORY::BICYCLE}, {WarningPriority::High, "bicycle in CRITICAL_ZONE_2"} },
    { {ZONE_TYPES::CRITICAL_ZONE_2, OBJECT_CATEGORY::MOTORBIKE}, {WarningPriority::High, "motorbike in CRITICAL_ZONE_2"} },
    { {ZONE_TYPES::CRITICAL_ZONE_2, OBJECT_CATEGORY::BUS}, {WarningPriority::Medium, "bus in CRITICAL_ZONE_2"} },
    { {ZONE_TYPES::CRITICAL_ZONE_2, OBJECT_CATEGORY::TRAIN}, {WarningPriority::Critical, "train in CRITICAL_ZONE_2"} },

    { {ZONE_TYPES::CAR_HOOD, OBJECT_CATEGORY::CAR}, {WarningPriority::Safe, "Car in CAR_HOOD"} },

    { {ZONE_TYPES::TRACKED_ZONE, OBJECT_CATEGORY::CAR}, {WarningPriority::Low, "Car in TRACKED_ZONE"} },
    { {ZONE_TYPES::TRACKED_ZONE, OBJECT_CATEGORY::PERSON}, {WarningPriority::Low, "person in TRACKED_ZONE"} },
    { {ZONE_TYPES::TRACKED_ZONE, OBJECT_CATEGORY::TRUCK}, {WarningPriority::Low, "truck in TRACKED_ZONE"} },
    { {ZONE_TYPES::TRACKED_ZONE, OBJECT_CATEGORY::BICYCLE}, {WarningPriority::Low, "bicycle in TRACKED_ZONE"} },
    { {ZONE_TYPES::TRACKED_ZONE, OBJECT_CATEGORY::MOTORBIKE}, {WarningPriority::Low, "motorbike in TRACKED_ZONE"} },
    { {ZONE_TYPES::TRACKED_ZONE, OBJECT_CATEGORY::BUS}, {WarningPriority::Low, "bus in TRACKED_ZONE"} },
    { {ZONE_TYPES::TRACKED_ZONE, OBJECT_CATEGORY::TRAIN}, {WarningPriority::Critical, "train in TRACKED_ZONE"} },

    { { ZONE_TYPES::GENERAL, OBJECT_CATEGORY::CAR}, {WarningPriority::Safe, ""} },
    { { ZONE_TYPES::GENERAL, OBJECT_CATEGORY::PERSON}, {WarningPriority::Safe, ""} },
    { { ZONE_TYPES::GENERAL, OBJECT_CATEGORY::TRUCK}, {WarningPriority::Safe, ""} },
    { { ZONE_TYPES::GENERAL, OBJECT_CATEGORY::BICYCLE}, {WarningPriority::Safe, ""} },
    { { ZONE_TYPES::GENERAL, OBJECT_CATEGORY::MOTORBIKE}, {WarningPriority::Safe, ""} },
    { { ZONE_TYPES::GENERAL, OBJECT_CATEGORY::BUS}, {WarningPriority::Safe, ""} },
    { { ZONE_TYPES::GENERAL, OBJECT_CATEGORY::TRAIN}, {WarningPriority::Safe, ""} }
};

 

const std::unordered_map<std::tuple<ZONE_TYPES, std::string, bool>, std::pair<WarningPriority, std::string>, tuple_hash> SPEED_BASED_ZONE_PRIORITY_DESCRIPTION_MAP = {
    { {ZONE_TYPES::CRITICAL_ZONE_2, OBJECT_CATEGORY::CAR, true}, {WarningPriority::Safe, "Car moving safely in CRITICAL_ZONE_2"} },
    { {ZONE_TYPES::CRITICAL_ZONE_2, OBJECT_CATEGORY::CAR, false}, {WarningPriority::High, "Car slowing down in CRITICAL_ZONE_2"} },
    { {ZONE_TYPES::CRITICAL_ZONE_2, OBJECT_CATEGORY::TRUCK, true}, {WarningPriority::Safe, "Truck moving safely in CRITICAL_ZONE_2"} },
    { {ZONE_TYPES::CRITICAL_ZONE_2, OBJECT_CATEGORY::TRUCK, false}, {WarningPriority::High, "Truck slowing down in CRITICAL_ZONE_2"} },
    { {ZONE_TYPES::CRITICAL_ZONE_2, OBJECT_CATEGORY::MOTORBIKE, true}, {WarningPriority::Safe, "Motorbike moving safely in CRITICAL_ZONE_2"} },
    { {ZONE_TYPES::CRITICAL_ZONE_2, OBJECT_CATEGORY::MOTORBIKE, false}, {WarningPriority::High, "Motorbike slowing down in CRITICAL_ZONE_2"} },
    { {ZONE_TYPES::TRACKED_ZONE, OBJECT_CATEGORY::CAR, true}, {WarningPriority::Low, "Car moving to CRITICAL_ZONE_2"} },
    { {ZONE_TYPES::TRACKED_ZONE, OBJECT_CATEGORY::CAR, false}, {WarningPriority::Medium, "Car slowing down moving to CRITICAL_ZONE_2"} },
    { {ZONE_TYPES::TRACKED_ZONE, OBJECT_CATEGORY::PERSON, true}, {WarningPriority::High, "Person moving to CRITICAL_ZONE_2"} },
    { {ZONE_TYPES::TRACKED_ZONE, OBJECT_CATEGORY::PERSON, false}, {WarningPriority::Critical, "Person slowing down moving to CRITICAL_ZONE_2"} }
};

const std::unordered_map< std::pair<ZONE_TYPES, std::string>, std::pair<WarningPriority, std::string>, pair_hash> GENERAL_MAP = {
    { {ZONE_TYPES::CRITICAL_ZONE_1, OBJECT_CATEGORY::CAR}, {WarningPriority::Critical, "Car in CRITICAL_ZONE_1"} },
    { {ZONE_TYPES::CRITICAL_ZONE_1, OBJECT_CATEGORY::PERSON}, {WarningPriority::Critical, "person in CRITICAL_ZONE_1"} },
    { {ZONE_TYPES::CRITICAL_ZONE_1, OBJECT_CATEGORY::TRUCK}, {WarningPriority::Critical, "truck in CRITICAL_ZONE_1"} },
    { {ZONE_TYPES::CRITICAL_ZONE_1, OBJECT_CATEGORY::BICYCLE}, {WarningPriority::Critical, "bicycle in CRITICAL_ZONE_1"} },
    { {ZONE_TYPES::CRITICAL_ZONE_1, OBJECT_CATEGORY::MOTORBIKE}, {WarningPriority::Critical, "motorbike in CRITICAL_ZONE_1"} },
    { {ZONE_TYPES::CRITICAL_ZONE_1, OBJECT_CATEGORY::BUS}, {WarningPriority::Critical, "bus in CRITICAL_ZONE_1"} },
    { {ZONE_TYPES::CRITICAL_ZONE_1, OBJECT_CATEGORY::TRAIN}, {WarningPriority::Critical, "train in CRITICAL_ZONE_1"} },

    { {ZONE_TYPES::CRITICAL_ZONE_2, OBJECT_CATEGORY::CAR}, {WarningPriority::High, "Car in CRITICAL_ZONE_2"} },
    { {ZONE_TYPES::CRITICAL_ZONE_2, OBJECT_CATEGORY::PERSON}, {WarningPriority::Critical, "person in CRITICAL_ZONE_2"} },
    { {ZONE_TYPES::CRITICAL_ZONE_2, OBJECT_CATEGORY::TRUCK}, {WarningPriority::High, "truck in CRITICAL_ZONE_2"} },
    { {ZONE_TYPES::CRITICAL_ZONE_2, OBJECT_CATEGORY::BICYCLE}, {WarningPriority::High, "bicycle in CRITICAL_ZONE_2"} },
    { {ZONE_TYPES::CRITICAL_ZONE_2, OBJECT_CATEGORY::MOTORBIKE}, {WarningPriority::High, "motorbike in CRITICAL_ZONE_2"} },
    { {ZONE_TYPES::CRITICAL_ZONE_2, OBJECT_CATEGORY::BUS}, {WarningPriority::High, "bus in CRITICAL_ZONE_2"} },
    { {ZONE_TYPES::CRITICAL_ZONE_2, OBJECT_CATEGORY::TRAIN}, {WarningPriority::Critical, "train in CRITICAL_ZONE_2"} },

    { {ZONE_TYPES::CAR_HOOD, OBJECT_CATEGORY::CAR}, {WarningPriority::Safe, "Car in CAR_HOOD"} },

    { {ZONE_TYPES::TRACKED_ZONE, OBJECT_CATEGORY::CAR}, {WarningPriority::Low, "Car in TRACKED_ZONE"} },
    { {ZONE_TYPES::TRACKED_ZONE, OBJECT_CATEGORY::PERSON}, {WarningPriority::Low, "Person in TRACKED_ZONE"} },
    { {ZONE_TYPES::TRACKED_ZONE, OBJECT_CATEGORY::TRUCK}, {WarningPriority::Low, "Truck in TRACKED_ZONE"} },
    { {ZONE_TYPES::TRACKED_ZONE, OBJECT_CATEGORY::BICYCLE}, {WarningPriority::Low, "Bicycle in TRACKED_ZONE"} },
    { {ZONE_TYPES::TRACKED_ZONE, OBJECT_CATEGORY::MOTORBIKE}, {WarningPriority::Low, "Motorbike in TRACKED_ZONE"} },
    { {ZONE_TYPES::TRACKED_ZONE, OBJECT_CATEGORY::BUS}, {WarningPriority::Low, "Bus in TRACKED_ZONE"} },
    { {ZONE_TYPES::TRACKED_ZONE, OBJECT_CATEGORY::TRAIN}, {WarningPriority::Critical, "Train in TRACKED_ZONE"} },

    { { ZONE_TYPES::GENERAL, OBJECT_CATEGORY::CAR}, {WarningPriority::Safe, ""} },
    { { ZONE_TYPES::GENERAL, OBJECT_CATEGORY::PERSON}, {WarningPriority::Safe, ""} },
    { { ZONE_TYPES::GENERAL, OBJECT_CATEGORY::TRUCK}, {WarningPriority::Safe, ""} },
    { { ZONE_TYPES::GENERAL, OBJECT_CATEGORY::BICYCLE}, {WarningPriority::Safe, ""} },
    { { ZONE_TYPES::GENERAL, OBJECT_CATEGORY::MOTORBIKE}, {WarningPriority::Safe, ""} },
    { { ZONE_TYPES::GENERAL, OBJECT_CATEGORY::BUS}, {WarningPriority::Safe, ""} },
    { { ZONE_TYPES::GENERAL, OBJECT_CATEGORY::TRAIN}, {WarningPriority::Safe, ""} }
};

const std::unordered_map< std::pair<ZONE_TYPES, std::string>, std::pair<WarningPriority, std::string>, pair_hash> IN_CRITICAL_ZONE_2_POSITIVE_SPEED_MAP = {
    { { ZONE_TYPES::CRITICAL_ZONE_2, OBJECT_CATEGORY::CAR}, {WarningPriority::Safe, "Car in CRITICAL_ZONE_2"} },
    { { ZONE_TYPES::CRITICAL_ZONE_2, OBJECT_CATEGORY::PERSON}, {WarningPriority::Critical, "Person in CRITICAL_ZONE_2"} },
    { { ZONE_TYPES::CRITICAL_ZONE_2, OBJECT_CATEGORY::TRUCK}, {WarningPriority::Safe, "Truck in CRITICAL_ZONE_2"} },
    { { ZONE_TYPES::CRITICAL_ZONE_2, OBJECT_CATEGORY::BICYCLE}, {WarningPriority::Safe, "Bicycle in CRITICAL_ZONE_2"} },
    { { ZONE_TYPES::CRITICAL_ZONE_2, OBJECT_CATEGORY::MOTORBIKE}, {WarningPriority::Safe, "Motorbike in CRITICAL_ZONE_2"} },
    { { ZONE_TYPES::CRITICAL_ZONE_2, OBJECT_CATEGORY::BUS}, {WarningPriority::Safe, "Bus in CRITICAL_ZONE_2"} },
    { { ZONE_TYPES::CRITICAL_ZONE_2, OBJECT_CATEGORY::TRAIN}, {WarningPriority::Critical, "Train in CRITICAL_ZONE_2"} }
};

const std::unordered_map< std::pair<ZONE_TYPES, std::string>, std::pair<WarningPriority, std::string>, pair_hash> MOVING_TO_CRITICAL_ZONE_1_MAP = {
    { { ZONE_TYPES::TRACKED_ZONE, OBJECT_CATEGORY::CAR}, {WarningPriority::High, "Car moving to CRITICAL_ZONE_1"} },
    { { ZONE_TYPES::TRACKED_ZONE, OBJECT_CATEGORY::PERSON}, {WarningPriority::High, "Person moving to CRITICAL_ZONE_1"} },
    { { ZONE_TYPES::TRACKED_ZONE, OBJECT_CATEGORY::TRUCK}, {WarningPriority::High, "Truck moving to CRITICAL_ZONE_1"} },
    { { ZONE_TYPES::TRACKED_ZONE, OBJECT_CATEGORY::BICYCLE}, {WarningPriority::High, "Bicycle moving to CRITICAL_ZONE_1"} },
    { { ZONE_TYPES::TRACKED_ZONE, OBJECT_CATEGORY::MOTORBIKE}, {WarningPriority::High, "Motorbike moving to CRITICAL_ZONE_1"} },
    { { ZONE_TYPES::TRACKED_ZONE, OBJECT_CATEGORY::BUS}, {WarningPriority::High, "Bus moving to CRITICAL_ZONE_1"} },
    { { ZONE_TYPES::TRACKED_ZONE, OBJECT_CATEGORY::TRAIN}, {WarningPriority::High, "Train moving to CRITICAL_ZONE_1"} }
};

const std::unordered_map< std::pair<ZONE_TYPES, std::string>, std::pair<WarningPriority, std::string>, pair_hash> MOVING_TO_CRITICAL_ZONE_2_NEGATIVE_SPEED_MAP = {
    { { ZONE_TYPES::TRACKED_ZONE, OBJECT_CATEGORY::CAR}, {WarningPriority::Medium, "Car slowing and moving to CRITICAL_ZONE_2"} },
    { { ZONE_TYPES::TRACKED_ZONE, OBJECT_CATEGORY::PERSON}, {WarningPriority::High, "Person moving to CRITICAL_ZONE_2"} },
    { { ZONE_TYPES::TRACKED_ZONE, OBJECT_CATEGORY::TRUCK}, {WarningPriority::Medium, "Truck slowing and moving to CRITICAL_ZONE_2"} },
    { { ZONE_TYPES::TRACKED_ZONE, OBJECT_CATEGORY::BICYCLE}, {WarningPriority::Medium, "Bicycle slowing and moving to CRITICAL_ZONE_2"} },
    { { ZONE_TYPES::TRACKED_ZONE, OBJECT_CATEGORY::MOTORBIKE}, {WarningPriority::Medium, "Motorbike slowing and moving to CRITICAL_ZONE_2"} },
    { { ZONE_TYPES::TRACKED_ZONE, OBJECT_CATEGORY::BUS}, {WarningPriority::Medium, "Bus slowing and moving to CRITICAL_ZONE_2"} },
    { { ZONE_TYPES::TRACKED_ZONE, OBJECT_CATEGORY::TRAIN}, {WarningPriority::Critical, "Train moving to CRITICAL_ZONE_2"} }
};

const std::unordered_map< std::pair<ZONE_TYPES, std::string>, std::pair<WarningPriority, std::string>, pair_hash> MOVING_TO_CRITICAL_ZONE_2_POSITIVE_SPEED_MAP = {
    { { ZONE_TYPES::TRACKED_ZONE, OBJECT_CATEGORY::CAR}, {WarningPriority::Low, "Car moving to CRITICAL_ZONE_2"} },
    { { ZONE_TYPES::TRACKED_ZONE, OBJECT_CATEGORY::PERSON}, {WarningPriority::High, "Person moving to CRITICAL_ZONE_2"} },
    { { ZONE_TYPES::TRACKED_ZONE, OBJECT_CATEGORY::TRUCK}, {WarningPriority::Low, "Truck moving to CRITICAL_ZONE_2"} },
    { { ZONE_TYPES::TRACKED_ZONE, OBJECT_CATEGORY::BICYCLE}, {WarningPriority::Low, "Bicycle moving to CRITICAL_ZONE_2"} },
    { { ZONE_TYPES::TRACKED_ZONE, OBJECT_CATEGORY::MOTORBIKE}, {WarningPriority::Low, "Motorbike moving to CRITICAL_ZONE_2"} },
    { { ZONE_TYPES::TRACKED_ZONE, OBJECT_CATEGORY::BUS}, {WarningPriority::Low, "Bus moving to CRITICAL_ZONE_2"} },
    { { ZONE_TYPES::TRACKED_ZONE, OBJECT_CATEGORY::TRAIN}, {WarningPriority::Critical, "Train moving to CRITICAL_ZONE_2"} }
};

const int GENERAL = 0;
const int IN_ZONE_2_POSITIVE_SPEED = 1;
const int MOVING_TO_ZONE_1 = 2;
const int MOVING_TO_ZONE_2_NEGATIVE_SPEED = 3;
const int MOVING_TO_ZONE_2_POSITIVE_SPEED = 4;

const std::array<const std::unordered_map<std::pair<ZONE_TYPES, std::string>, std::pair<WarningPriority, std::string>, pair_hash>*, 5> PRIORITY_AND_DESCRIPTION = {
    &GENERAL_MAP,
    &IN_CRITICAL_ZONE_2_POSITIVE_SPEED_MAP,
    &MOVING_TO_CRITICAL_ZONE_1_MAP,
    &MOVING_TO_CRITICAL_ZONE_2_NEGATIVE_SPEED_MAP,
    &MOVING_TO_CRITICAL_ZONE_2_POSITIVE_SPEED_MAP
};

//colors map
namespace COLORS
{
    inline const cv::Scalar RED = cv::Scalar(0, 0, 255);
    inline const cv::Scalar YELLOW = cv::Scalar(0, 255, 255);
    inline const cv::Scalar GREEN = cv::Scalar(0, 255, 0);
    inline const cv::Scalar BLUE = cv::Scalar(255, 0, 0);
    inline const cv::Scalar WHITE = cv::Scalar(255, 255, 255);
    inline const cv::Scalar BLACK = cv::Scalar(0, 0, 0);
    inline const cv::Scalar GRAY = cv::Scalar(128, 128, 128);
    inline const cv::Scalar PURPLE = cv::Scalar(128, 0, 128);
    inline const cv::Scalar ORANGE = cv::Scalar(0, 165, 255);
    inline const cv::Scalar PINK = cv::Scalar(147, 20, 255);
}


//zones color map
const std::unordered_map<ZONE_TYPES, cv::Scalar> zoneTypeToColor = {
    {ZONE_TYPES::CRITICAL_ZONE_1, COLORS::GRAY},
    {ZONE_TYPES::CRITICAL_ZONE_2, COLORS::YELLOW},
    {ZONE_TYPES::CAR_HOOD, COLORS::BLACK},
    {ZONE_TYPES::TRACKED_ZONE, COLORS::PURPLE},
    {ZONE_TYPES::GENERAL, COLORS::BLUE}
};

//warnings color map
const std::unordered_map<WarningPriority, cv::Scalar> warningPriorityToColor = {
	{WarningPriority::Critical, COLORS::RED},
    {WarningPriority::High, COLORS::ORANGE},
    {WarningPriority::Medium, COLORS::PINK},
    {WarningPriority::Low, COLORS::GRAY},
    {WarningPriority::Safe, COLORS::GREEN},
    {WarningPriority::Unknown, COLORS::WHITE}
};




   





  //     // CRITICAL_ZONE - High priority
        ////{{{TRAPEZOID::CRITICAL_ZONE::DL, TRAPEZOID::CRITICAL_ZONE::DR, TRAPEZOID::CRITICAL_ZONE::UR, TRAPEZOID::CRITICAL_ZONE::UL}}, WarningPriority::High},

        
//const std::array<Warning, 6> warningsTypes = std::array<Warning, 6>({
//    {WarningPriority::Critical, cv::Scalar(0, 0, 255), "Critical"},
//    {WarningPriority::High, cv::Scalar(0, 255, 255), "High"},
//    {WarningPriority::Medium, cv::Scalar(0, 255, 0), "Medium"},
//    {WarningPriority::Low, cv::Scalar(255, 255, 0), "Low"},
//    {WarningPriority::Safe, cv::Scalar(255, 0, 0), "Safe"},
//    {WarningPriority::Unknown, cv::Scalar(255, 255, 255), "Unknown"}
//    });
