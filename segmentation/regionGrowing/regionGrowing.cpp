/*
  Steps:
  - Let f(x, y) be the input image.
  - Let S(x, y) be the seed matrix that assigns the value 1 to the seed positions 
    and 0 to the other positions
  - Let Q be a predicate to be applied to each pixel.
  - Form the image fQ which in point (x, y) contains the value 1 
    if (f(x,y)) is true otherwise it contains the value 0
  - Add to each seed the pixels set to 1 in fQ which are [4, 8] - connected to the seed itself
  - Mark each connected component with a different label

*/

#include <iostream>
#include <stack>
#include <cstdlib>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

void rGrowing(Mat &src, Mat &output, Mat &mask, Point seed, int th);
const int th = 200;
const uchar maxRegionNum = 100;

//A Region, to be considered as such, must have at least 1% of the pixels of the total image
const double minRegionAreaFactor = 0.01;

//Define the 8-neighborhood of the pixel to consider
const Point pointShift2D[8] = {
    Point(1, 0),
    Point(1, -1),
    Point(0, -1),
    Point(-1, -1),
    Point(-1, 0),
    Point(-1, 1),
    Point(0, 1),
    Point(1, 1)
};


int main(int argc, char **argv){
	if(argc < 2){
    cout<<"usage: "<<argv[0]<<" image_name"<<endl;
    exit(0);
  }
  String imageName = argv[1];

	//Read image    
  Mat image;
  image = imread(imageName, IMREAD_COLOR );
  if( image.empty() ){
    cout <<  "Could not open or find the image" << endl ;
    return -1;
  }

  if(image.cols > 500 || image.rows > 500){
    resize(image, image, Size(0,0), 0.5, 0.5);
  }
  namedWindow("src", CV_WINDOW_NORMAL);
  imshow("src", image);

  /*
    Calculate the minimum area that a region must have to be considered 
    as such so that very small regions (2-3 pixels) are not considered, they are not significant
  */
  int minRegionArea = int(minRegionAreaFactor * image.cols * image.rows);
  namedWindow("mask", CV_WINDOW_NORMAL);

  //Padding is a label: is a counter that is incremented every time a new region is found
  uchar padding = 1;
  Mat output = Mat::zeros(image.rows, image.cols, CV_8UC1);
  
  /*
    Mask that allows you identify all the pixels that are located within a specific region 
    in the accretion phase
  */
  Mat mask = Mat::zeros(image.rows, image.cols, CV_8UC1);

  for( int x = 0; x < image.cols; ++x){
    for( int y = 0; y < image.rows; ++y){
      /*
        If det = 0 it means that the pixel in position (x, y) has not yet been considered
        and therefore it is the next seed of the region;
        Vice versa it menas that it is already part of a region and therefore cannot be considered
      */
      if(output.at<uchar>(Point(x, y)) == 0){
        //Starting from pixel (x, y) try to increase the region
        rGrowing(image, output, mask, Point(x, y), th);

        //Sum all pixel with value 1 in the region
        int maskArea = (int)sum(mask).val[0];

        //Verify if region area si greated than min area
        if(maskArea > minRegionArea){
          //Label the pixel in the region
          output = output + mask * padding;
          imshow("mask", mask * 255);
          imshow("output", output * 255);
          waitKey(0);
          //If get more than 100 regions I stop, because over segmenting the images
          if(++padding > maxRegionNum){
            printf("Region number very high!\n");
            return -1;
          }
        }else{
            //Label that identifies all those regions that have an area too small to be a single region
            output = output + mask * 255;
        }
        //Reset mask and move on the next region
        mask = mask - mask;
      }
    }
  }

  return 0;
}

void rGrowing(Mat &src, Mat &output, Mat &mask, Point seed, int th){
  //Use the stak to simulate the depth visit of a graph
  stack<Point> pointStack;
  pointStack.push(seed);
  while(!pointStack.empty()){
    //Check the pixel that is at the top of the stack
    Point center = pointStack.top();
    mask.at<uchar>(center) = 1;
    //Extract the pixel that is at the top of the stack
    pointStack.pop();

    //Analize 8-neighborhood
    for(int i = 0; i < 8; ++i){
      Point estimatingPoint = center + pointShift2D[i];

      /*
        Look for the connected components within the image.
        I check if the point to be considered is external to the image,
        in that case we go the next pixel in the 8-neighborhood
      */
     if(estimatingPoint.x < 0 || estimatingPoint.y < 0
        || estimatingPoint.x > src.cols -1 || estimatingPoint.y > src.rows -1){
          continue;
      }else{
        //If the point to consider is inside the image:

        //Distance
        int delta = int(pow(src.at<Vec3b>(center)[0] - src.at<Vec3b>(estimatingPoint)[0],2) +
          pow(src.at<Vec3b>(center)[1] - src.at<Vec3b>(estimatingPoint)[1],2) +
          pow(src.at<Vec3b>(center)[2] - src.at<Vec3b>(estimatingPoint)[2],2));

        /*
          Verify:
            - EstimatingPoin has not already been taken in dest, 
              otherwise it means that it belongs to another region
            - Which has not already been considered within this region
            - That the distance between the central pixel and the pidxel incresed is less than the threshold
        */
        if(output.at<uchar>(estimatingPoint) == 0 
          && mask.at<uchar>(estimatingPoint) == 0
          && delta < th){

          //Add to region
          mask.at<uchar>(estimatingPoint) = 1;
          //Push the pixel inside the stack
          pointStack.push(estimatingPoint);

        }
      }
    }
  }
}