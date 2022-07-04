#include "../otsu.h"

vector <int> otsu2Th(vector<double> histogram);

int main(int argc, char** argv){

    if(argc < 2){
        cout<<"usage: "<<argv[0]<<" image_name"<<endl;
        exit(0);
    }

    String imageName = argv[1];

    //Read image   
    Mat image;
    image = imread(imageName, IMREAD_GRAYSCALE);

    if (image.empty())
    {
        cout << "Could not open or find the image" << endl;
        return -1;
    }

    imshow("Original image", image);
    Mat blur;
    GaussianBlur(image, blur, Size(3, 3), 0);

    vector <double> hist = normalizedHistogram(blur);

    Mat output;

    vector<int> thresholds = otsu2Th(hist);
    multipleThrehosld(blur, output, thresholds);
    imshow("Otsu image", output);

    waitKey(0);
    destroyAllWindows();

    return 0;
}

vector<int> otsu2Th(vector<double> his){
    double avgCum = 0.0f;
    for(int i = 0; i < 256; i++){
        avgCum += i * his[i];
    }
    vector <double> prob(3, 0.0f);
    vector <double> currAvgCum (3, 0.0f);
    double currVariance = 0.0f;
    double maxVariance = 0.0f;
    vector <int> threshold(2, 0);

    for(int i = 0; i < 256 - 2; i++){
        prob[0] += his[i];
        currAvgCum[0] += i * his[i];
        for(int j = i+1; j < 256-1; j++){
            prob[1] += his[j];
            currAvgCum[1] += j * his[j];
            for(int k = j+1; k < 256; k++){
                prob[2] += his[k];
                currAvgCum[2] += k * his[k];
                currVariance = 0.0f;
                for(int w = 0; w < 3; w ++){
                    currVariance += prob[w]* pow(currAvgCum[w] / prob[w] - avgCum, 2);
                }
                if(currVariance > maxVariance){
                    maxVariance = currVariance;
                    threshold[0] = i;
                    threshold[1] = j;
                }
            }
            prob[2] = currAvgCum[2] = 0.0f;
        }
        prob[1] = currAvgCum[1] = 0.0f;
    }

    return threshold;

}