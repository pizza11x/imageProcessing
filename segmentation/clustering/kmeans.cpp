/*
    STEPS:
    - Initialize the cluster ccenters.
    - Assign each pixel to the nearest center:
      For each pixel Pj calculate the distance from the k centers Ci 
      and assign Pj to the cluster with the closest Ci center.
    - Repeat points 2 and 3 until the center (avg) of each cluster is no longer modified
      (i.e the groups are not modified)
*/

#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

double distance(Scalar p1, Scalar p2);
void kMeans(Mat &src, Mat &output, int nClusters);

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

  imshow("Original image", image);
  int nClusters = 0;
  cout << "Choose the number of clusters: ";
  cin >> nClusters;

  Mat output(image.rows, image.cols, image.type());
  kMeans(image, output, nClusters);

  imshow("K-Means", output);
  waitKey(0);
  return 0;
}

double distance(Scalar p1, Scalar p2){
  double diffBlue = p1.val[0] - p2[0];
  double diffGreen = p1.val[1] - p2[1];
  double diffRed = p1.val[2] - p2[2];

  return sqrt(pow(diffBlue, 2) + pow(diffGreen, 2) + pow(diffRed, 2));
}

void kMeans(Mat &src, Mat &output, int nClusters){
  //Vector containing the colors of the centers
  vector <Scalar> centersColors;
  //Vector that contains the pixels that belong to the cluser
  vector <vector<Point>> clusters;

  RNG random(getTickCount());

  //Calculate the centers
  for(int k = 0; k < nClusters; k++){
    Point center;
    center.x = random.uniform(0, src.cols);
    center.y = random.uniform(0, src.rows);

    //Scalar that contains the colors of the center
    Scalar cColors(src.at<Vec3b>(center)[0], src.at<Vec3b>(center)[1], src.at<Vec3b>(center)[2]);
    //Add the scalar of the calculater center to the vector containing the colors centers
    centersColors.push_back(cColors);

    //Vector that will contain the pixels that will be assigned to cluster k
    vector <Point> t;
    //Add a vector of point to the vector of clusters
    clusters.push_back(t);
  }

  //Assign the pixels to the clusters, recalculate the centers using the means, until the difference > 0.1
  double oldCenterSum = 0;
  double difference = INFINITY;

  //Cycle until the difference between the old avereges and the new averages is greater than a threshold (0.1)
  while(difference > 0.1){
    //Clean up the point vectors present in clusters
    for(int k = 0; k < nClusters; k++){
      clusters[k].clear();
    }

    //Update the pixels in the clusters
    for(int x = 0; x < src.rows; x++){
      for(int y = 0; y < src.cols; y++){
        //Calculate the distances from each cluster center and place the pixel in the closest cluster
        double minDistance = INFINITY;
        int clusterIndex = 0;

        //Cuttent point of the image
        Scalar point = src.at<Vec3b>(x,y);
        //For each cluster extract the center and calculate the distance
        for(int k = 0; k < nClusters; k++){
          //Extract the scalar of the center from the vector containing the colors
          Scalar center = centersColors[k];

          //Calculate the distance between the center and the current point of the image
          double dist = distance(point, center);

          //If the distance is minimal, update it
          if(dist < minDistance){
            minDistance = dist;
            //Save the pixel in the cluster of the index k
            clusterIndex = k;
          }
        }
        //Add the pixel in the vector k of the cluster
        clusters[clusterIndex].push_back(Point(y, x));
      }
    }
    //Recalculate the average of each cluster
    double newCenterSum = 0;
    for(int k = 0; k < nClusters; k++){
      //Define a vector that will contain all the points present in cluster k
      vector <Point> clusterPoints = clusters[k];
      double blue = 0, green = 0, red = 0;

      /*
      Scoll through all the points of the vector and add the components of the colors 
      in the appropriate variables
      */
      for(int i = 0; i < clusterPoints.size(); i++){
        Point pixel = clusterPoints[i];
        blue += src.at<Vec3b>(pixel)[0];
        green += src.at<Vec3b>(pixel)[1];
        red += src.at<Vec3b>(pixel)[2];
      }
      /*
      Calculate the average of the colors of the new center by dividing
      the sum calculated earlier by the number of points
      */
      blue /= clusterPoints.size();
      green /= clusterPoints.size();
      red /= clusterPoints.size();

      //Extract the old center from the vector containing the scalars of the centers
      Scalar center = centersColors[k];

      //Define the new center with the calculated means
      Scalar newCenter(blue, green, red);

      //Calculated the distance of the new center to the old one and add it to newCenterSum
      newCenterSum += distance(newCenter, center);

      //Update the new center
      centersColors[k] = newCenter;


    }
    //Calculate the average by dividing the sum by the number of clusters
    newCenterSum /= nClusters;

    //Calculate the difference between the old sym and the one just calculated
    difference = abs(oldCenterSum - newCenterSum);
    oldCenterSum = newCenterSum;
    

  }
  //Draw the output image
  for(int k = 0; k < nClusters; k++){
    vector <Point> clusterPoints = clusters[k];
    Scalar center = centersColors[k];
    for(int i = 0; i < clusterPoints.size(); i++){
      /*Assign the center value to the other pixels in the cluster.
        All pixels in a given cluster will receive the average (center) value of that cluster
      */
      Point pixel = clusterPoints[i];
      output.at<Vec3b>(pixel)[0] = center[0];
      output.at<Vec3b>(pixel)[1] = center[1];
      output.at<Vec3b>(pixel)[2] = center[2];

    }
  }


}