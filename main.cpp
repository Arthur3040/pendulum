
#include <iostream>
#include <sstream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
#include <fstream>
#include <cstdio> // delete the .txt to "reset data"
#include <string>



using namespace cv;
using namespace std;

const char* params
    = "{ help h         |           | Print usage }"
      "{ input          | ./videos/laranja3.mp4 | Path to a video or a sequence of image }"
      "{ algo           | KNN      | Background subtraction method (KNN, MOG2) }";

int main(int argc, char* argv[])
{
    remove("data/pendulumdata.txt"); //if you don't want to delete previous data, comment or erase this line;
    fstream fout; //fstream is going to be used to make a .csv file with the position, time, (and frame indirectly)
    fout.open("data/pendulumdata.txt", ios::out | ios::app);
    //fout << "time" << ", "<< "x" << "\n"; create head collums of the .csv, THINKING TO CHANGE THIS TO DATATRAINING.CPP

    CommandLineParser parser(argc, argv, params);
    if (parser.has("help"))
    {
        //print help information
        parser.printMessage();
    }

    //create Background Subtractor objects
    Ptr<BackgroundSubtractor> pBackSub;
    if (parser.get<String>("algo") == "MOG2")
        pBackSub = createBackgroundSubtractorMOG2();
    else
        pBackSub = createBackgroundSubtractorKNN();

    VideoCapture capture( samples::findFile( parser.get<String>("input") ) );
    if (!capture.isOpened()){
        //error in opening the video input
        cerr << "Unable to open: " << parser.get<String>("input") << endl;
        return 0;
    }

    Mat frame, fgMask;
    double x, y, center_x, center_y; 
    double t=0;
    string str_center_x, str_center_y;
    while (true) {
        capture >> frame;
        if (frame.empty())
            break;

        //update the background model
        pBackSub->apply(frame, fgMask);
        //Not satisfied with common threshold, i changed some parameters
        threshold(fgMask, fgMask, 220, 255, THRESH_BINARY);
        

        //get the frame number and write it on the current frame; (that part is from opencv mask tutorial docs)
        rectangle(frame, cv::Point(10, 2), cv::Point(100,20),cv::Scalar(255,255,255), -1);
        stringstream ss;
        ss << capture.get(CAP_PROP_POS_FRAMES);
        string frameNumberString = ss.str();
        putText(frame, frameNumberString.c_str(), cv::Point(15, 15),
                FONT_HERSHEY_SIMPLEX, 1 , cv::Scalar(0,0,0));

        //FIND POINTERS
        vector<vector<Point>> contours, area;
        vector<Vec4i> hierarchy;
        findContours(fgMask, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);
        
        //draw contours
        for (unsigned int i = 0;  i < contours.size();  i++){   
            
            
            std::cout << " Area: " << contourArea(contours[i]) << std::endl;
            //draw box of the object and send data PARTE IMPORTANTISSIMA;
            if (1001 < contourArea(contours[i])){
                rectangle(frame,boundingRect(contours[i]),Scalar(0,255,0),2);
                //botar texto e passar posição e tempo pra csv
                x = boundingRect(contours[i]).x;
                y = boundingRect(contours[i]).y;
                center_x = (boundingRect(contours[i]).width/2) + x;
                center_y = (boundingRect(contours[i]).height/2) + y;
                str_center_x = to_string(center_x);
                str_center_y = to_string(center_y);
                putText(frame, (str_center_x + " | " + str_center_y) , Point(x, y),
                        FONT_HERSHEY_SIMPLEX, 0.5 , Scalar(250,0,0), 2);

                fout << t << " "<< center_x << "\n"; //write the dates in pendulumdata.csv
                break;
            }
        }
        t += (1/capture.get(CAP_PROP_FPS));
        
        //show the current frame and the fg masks
        imshow("Frame", frame);
        imshow("FG Mask", fgMask);

        //get the input from the keyboard
        int keyboard = waitKey(30);
        if (keyboard == 'q' || keyboard == 27)
            break;
    }

    fout.close();
    return 0;
}