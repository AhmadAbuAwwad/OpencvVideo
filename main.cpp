//Uncomment the following line if you are compiling this code in Visual Studio

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <thread>        
#include <unistd.h>

using namespace cv;
using namespace std;

mutex mtx1;    
mutex mtx2;      
int imageCounter = 0;
void readImage(shared_ptr<Mat> matPtr) 
{
//Open the default video camera
    VideoCapture cap(0);
 // if not success, exit program
     if (cap.isOpened() == false)  
    {
        cout << "Cannot open the video camera" << endl;
    } 
    else{
        cout << "start" << endl;
        while (true)
        {
            mtx1.lock();     
	    cap.read(*matPtr); 
            cout << "1- read" << endl;
            imageCounter++;
            mtx2.unlock();
        }
    }
}

void showImage(shared_ptr<Mat> matPtr)
{
    namedWindow("My Camera Feed"); //create a window called "My Camera Feed"

    while (true)
    
    {
        mtx2.lock();
        if(matPtr->empty()){
            cout << "Empty image" << endl;
            return;
        }
        cout << "2- write" << endl;
        putText(*matPtr , to_string(imageCounter),Point(10,40),FONT_HERSHEY_DUPLEX,1,Scalar(0,255,0),2,false);
        imshow("My Camera Feed", *matPtr);
        // cv::waitKey(10);

        cout << "2- write done" << endl;

        // sleep(1);
        if (cv::waitKey(10) == 27){
            cout << "Esc key is pressed by user. Stoppig the video" << std::endl;
            break;
        }  
        cout << imageCounter << endl;
     
        mtx1.unlock();
    }
}

int main(int argc, char* argv[]){
    Mat frames;
    shared_ptr<Mat> matPtr = make_shared<Mat>();
    *matPtr = frames;

    mtx2.lock();
    thread t1 (readImage,matPtr);    
    thread t2 (showImage,matPtr);

    t1.join(); 
    t2.join(); 

    
 return 0;

}
