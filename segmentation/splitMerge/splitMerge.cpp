#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>

#define MIN_DEV 10.7
#define MAX_AREA 64

using namespace std;
using namespace cv;

//Behavior region
struct Region{
    vector<Region> adj;
    bool valid;
    Scalar label;
    Rect area;
};

bool predicate(Mat src);
void mergeRegion(Mat src, Region &r1, Region &r2);
void merge(Mat src, Region &r);
void display(Mat &out, Region R);
Region split(Mat src, Rect area);

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

    //Divide image. The rectangle is the whole image
    Region r = split(image, Rect(0,0, image.cols, image.rows));
    merge(image, r);

    Mat output = image.clone();
    display(output, r);

    Mat output2 = Mat::zeros(image.size(), image.type());
    display(output2, r);

    imshow("Output 1", output);
    imshow("Output 2", output2);

    waitKey(0);
    return 0;
}

Region split(Mat src, Rect area){
    Region R;
    R.valid = true;
    R.area = area;

    //The region cannot be divided
    if(predicate(src)){
        Scalar mean;
        //Calculate the mean
        meanStdDev(src, mean, Scalar());
        //Assign mean to the label of the region
        R.label = mean;
    }else{
        //Divide the size of the region in two
        int w = src.cols / 2; //100
        int h = src.rows / 2; //80

        //Divide the regions into 4 sub-regions by calling the split funcition
        Region r1 = split(src(Rect(0,0, w, h)), Rect(area.x, area.y, w, h));
        Region r2 = split(src(Rect(w,0, w, h)), Rect(area.x + w, area.y, w, h));
        Region r3 = split(src(Rect(0,h, w, h)), Rect(area.x, area.y + h, w, h));
        Region r4 = split(src(Rect(w,h, w, h)), Rect(area.x + w, area.y + h, w, h));

        //Add the newly calculated regions to the vector of adjacent regions
        R.adj.push_back(r1);
        R.adj.push_back(r2);
        R.adj.push_back(r3);
        R.adj.push_back(r4);
    }
    return R;
}

void display(Mat &output, Region R){
    //If the master region ha no sub-regions and the master region is valid draw a rectangle
    if(R.adj.size() == 0 && R.valid){
        //Area = area of interest; Label = Color; Filled = Type of rectangle
        rectangle(output, R.area, R.label, FILLED);
    }
    //Recursively display the sub-regions of the master region
    for(int i = 0; i < R.adj.size(); i++)
        display(output, R.adj.at(i));
}

void merge(Mat src, Region &r){
    //If region has adjacent regions
    if(r.adj.size() > 0){

        //Check if possible merge the regions
        mergeRegion(src, r.adj.at(0), r.adj.at(1));
        mergeRegion(src, r.adj.at(2), r.adj.at(3));
        mergeRegion(src, r.adj.at(0), r.adj.at(2));
        mergeRegion(src, r.adj.at(1), r.adj.at(3));

        //Call the function for all adjacent regions
        for(int i = 0; i < r.adj.size(); i++){
            merge(src, r.adj.at(i));
        }
    }
}

bool predicate(Mat src){
    Scalar dev;
    //Calculate the mean and the deviation of the matrix
    meanStdDev(src, Scalar(), dev);
    return (dev[0] < MIN_DEV || src.cols * src.rows < MAX_AREA);
}

void mergeRegion(Mat src, Region &r1, Region &r2){
    //If 2 regions have no adjacent regions
    if(r1.adj.size() == 0 && r2.adj.size() == 0){
        Rect r12 = r1.area | r2.area;
        if(predicate(src(r12))){
            //Merge in r1
            r1.area = r12;
            r1.label = (r1.label + r2.label) / 2;
            //Invalid r2 because its area is merged into r1
            r2.valid = false;
        }
    }
}