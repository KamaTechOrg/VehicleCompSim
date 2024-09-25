//#include <iostream>
//#include <onnxruntime/core/session/onnxruntime_cxx_api.h>
//#include <opencv2/opencv.hpp>
//
//
//using namespace std;
//using namespace cv;
//
//int main() {
//    // Initialize ONNX Runtime environment
//    Ort::Env env(ORT_LOGGING_LEVEL_WARNING, "ONNXModel");
//
//    // Create an inference session
//    Ort::SessionOptions session_options;
//    Ort::Session session(env, "C:\\Users\\dmaro\\Downloads\\New_folder\\yolov5n.onnx", session_options);
//
//    // Read an image
//    std::string image_path = R"(C:\Users\dmaro\Downloads\Cars3\image_0521.jpg)";
//    Mat img = imread(image_path);
//    if (img.empty()) {
//        cerr << "Image not found!" << endl;
//        return -1;
//    }
//
//    // Prepare the image for the network
//    Mat blob;
//    blobFromImage(img, blob, 1.0 / 255, Size(640, 640), Scalar(), true, false);
//
//    // Create ONNX Runtime tensor from the blob
//    vector<int64_t> input_shape = { 1, 3, 640, 640 };
//    Ort::Value input_tensor = Ort::Value::CreateTensor<float>(env, blob.ptr<float>(), blob.total(), input_shape.data(), input_shape.size());
//
//    // Run inference
//    auto output_tensors = session.Run(Ort::RunOptions{ nullptr }, { "input" }, { input_tensor }, { "output" });
//
//    // Extract results from the output tensor
//    float* floatarr = output_tensors.front().GetTensorMutableData<float>();
//
//    // Process output and draw bounding boxes
//    // Your output processing code here
//
//    // Display the result
//    imshow("Detected Objects", img);
//    waitKey(0);
//
//    return 0;
//}


//
//#include <opencv2/opencv.hpp>
//#include <opencv2/dnn.hpp>
//#include <iostream>
//
//using namespace cv;
//using namespace cv::dnn;
//using namespace std;
//
//int main() {
//    // Load the ONNX model
//    Net net = readNet("C:\\Users\\dmaro\\Downloads\\New_folder\\yolov5n.onnx");
//
//    // Read an image
//    std::string image_path = R"(C:\Users\dmaro\Downloads\Cars3\image_0521.jpg)";
//    Mat img = imread(image_path);
//    if (img.empty()) {
//        cerr << "Image not found!" << endl;
//        return -1;
//    }
//
//    // Prepare the image for the network
//    Mat blob;
//    blobFromImage(img, blob, 1.0 / 255, Size(640, 640), Scalar(), true, false);
//    net.setInput(blob);
//
//    // Perform inference
//    Mat output = net.forward();
//
//    // Process output (example)
//    // You would need to parse the output according to the model's output format
//
//    // Process output
//    Mat detectionMat(output.size[2], output.size[3], CV_32F, output.ptr<float>());
//
//    for (int i = 0; i < detectionMat.rows; i++) {
//        float confidence = detectionMat.at<float>(i, 4);
//
//        if (confidence > 0.5) {  // Confidence threshold
//            int x1 = static_cast<int>(detectionMat.at<float>(i, 0) * img.cols);
//            int y1 = static_cast<int>(detectionMat.at<float>(i, 1) * img.rows);
//            int x2 = static_cast<int>(detectionMat.at<float>(i, 2) * img.cols);
//            int y2 = static_cast<int>(detectionMat.at<float>(i, 3) * img.rows);
//
//            // Draw bounding box
//            rectangle(img, Point(x1, y1), Point(x2, y2), Scalar(0, 255, 0), 2);
//
//            // Label (assuming the model only detects one class)
//            putText(img, "Car", Point(x1, y1 - 10), FONT_HERSHEY_SIMPLEX, 0.9, Scalar(0, 255, 0), 2);
//        }
//    }
//
//    // Display the result
//    imshow("Detected Objects", img);
//    waitKey(0);
//
//    return 0;
//}