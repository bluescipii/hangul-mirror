//
//  main.cpp
//  opencv test
//
//  Created by 노현수 on 2017. 1. 28..
//  Copyright © 2017년 노현수. All rights reserved.
//

#include <iostream>
#include <opencv2/opencv.hpp>



using namespace cv;
using namespace std;

int threshold1 = 50;
int threshold2 = 80;
int threshold3 = 140;
int matrixMean;
int squareValue = 15;

Mat rawFrame;
Mat resizedFrame;
Mat grayscaleFrame;
Mat houseFrame;
Mat firstImage;
Mat whiteBackground;



int main(int argc, char* argv[])

{
    Mat Image1 = imread("/Users/hivcd1/Desktop/1.png",IMREAD_GRAYSCALE);
    Mat Image2 = imread("/Users/hivcd1/Desktop/2.png",IMREAD_GRAYSCALE);
    Mat Image4 = imread("/Users/hivcd1/Desktop/4.png",IMREAD_GRAYSCALE);
    Mat Image5 = imread("/Users/hivcd1/Desktop/5.png",IMREAD_GRAYSCALE);
    whiteBackground = imread("/Users/hivcd1/Desktop/white2.png", IMREAD_GRAYSCALE);

    if (!Image1.data) cout << "이미지 1 불러오기 불가능" << endl;
    if (!Image2.data) cout << "이미지 2 불러오기 불가능" << endl;
    if (!Image4.data) cout << "이미지 4 불러오기 불가능" << endl;
    if (!Image5.data) cout << "이미지 5 불러오기 불가능" << endl;
    
    resize(Image1, Image1, cv::Size(squareValue,squareValue));
    resize(Image2, Image2, cv::Size(squareValue,squareValue));
    resize(Image4, Image4, cv::Size(squareValue,squareValue));
    resize(Image5, Image5, cv::Size(squareValue,squareValue));
  
    VideoCapture cap = VideoCapture(0);

    if (!cap.isOpened())
        {
            cout << " Cannot open the video file" << endl;
            return -1;
        }

    while(1){
        
        bool bSuccess = cap.read(rawFrame);
        
        if (!bSuccess)
            {
                cout << " Cannot read the frame from Video File" << endl;
                break;

                return 2;
            }

         // 프레임 체크
     
        double fps = cap.get(CV_CAP_PROP_FPS);
        cout << "Frame per seconds : " << fps << endl;
      
        // 기존 house frame 사이즈 설정
        cap >> rawFrame;
        resize(rawFrame, resizedFrame, cv::Size(1920,1080));
        cvtColor(resizedFrame, grayscaleFrame, CV_RGB2GRAY);

        // 모니터 해상도에 따른 크기 설정
        Rect finalCrop(200, 0, 1680, 1050);
        Mat croppedFrame = grayscaleFrame(finalCrop);
        flip(croppedFrame, houseFrame, 1);
       
        if (!houseFrame.data) {
            cout << "cannot build houseFrame" << endl;
            return 3;
        }
     
        // house information
        cout << "Number of houseFrame Channel : " << houseFrame.channels() << endl;
        cout << "Pixel Size of houseFrame : " << houseFrame.elemSize() << endl;
      
         // make whiteBackground

        for ( int j = 0; j <1040 ; j=j+squareValue)
            {
                for (int i = 0; i < 1680; i=i+squareValue)
                    {
                        Rect ROI(i, j, squareValue, squareValue);
                        Mat readROI = houseFrame(ROI);
                        Mat writeROI = whiteBackground(ROI);
                        Scalar tempVal = mean(readROI);
                        int matrixMean = tempVal.val[0];
                        cout << "평균값은 " <<matrixMean << endl;
                    
                        if ( threshold3 <= matrixMean && matrixMean < 256)
                            {
                                Image1.copyTo(writeROI);
                            }
                        
                        else if( threshold2 <= matrixMean && matrixMean < threshold3 )
                            {
                                Image2.copyTo(writeROI);
                            }
                   
                        else if ( threshold1 <= matrixMean && matrixMean < threshold2 )
                            {
                                Image4.copyTo(writeROI);
                            }
                   
                        else if( matrixMean < threshold1 )
                            {
                                Image5.copyTo(writeROI);
                            }
                        else
                            {
                                cout << "로드할 수 없습니다." << endl;
                                break;
                                return 99;
                             }
                    }
                }
    
            // 결과창에 대하여
            namedWindow("Window_houseFrame", WINDOW_NORMAL);
            imshow("Window_houseFrame", houseFrame);
  
            namedWindow("Window_WhiteBackground", WINDOW_NORMAL);
            imshow("Window_WhiteBackground", whiteBackground);
     
            cout << "--------------------------------------------------------" << endl;
  
        // esc 누르면 꺼짐
        if(waitKey(30) == 1)
            {
                cout << "esc key is pressed by user" << endl;
                break;
            } 
    }
}



