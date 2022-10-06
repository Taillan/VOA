#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/xfeatures2d.hpp"

#include <iostream>
#include <iomanip>

#define Nbest 20

using namespace cv;
using namespace std;

Mat src[2];


double moyenne( int argc, char** argv )
{
  if(argc != 3){
    cout << "necessite deux arguments exactement" << endl;
    return 1;
  }
  //char *noms[] = { "source", "cible" };
  vector<KeyPoint> keys[2];
  Mat mat[2];

  for (int i= 0; i <= 1; i++)
    {
      src[i]= imread(argv[1+i]);
      /*if(!i){
        resize(src[i],src[i],Size(),0.25,0.25);
      }*/
      cvtColor(src[i], src[i], COLOR_BGR2GRAY );
      Ptr<Feature2D> feature = SIFT::create();
      feature->detectAndCompute(src[i], Mat(), keys[i], mat[i]);
      drawKeypoints(src[i], keys[i], src[i], Scalar(0, 0, 255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
      //namedWindow(argv[1+i]);
      //imshow(argv[i+1], src[i]);
    }

    BFMatcher matcher(NORM_L2, true);
    vector<DMatch> matches;
    matcher.match(mat[0], mat[1], matches, Mat());
    std::sort(matches.begin(), matches.end());
    
    float moy;

    moy = 0;
    for (int i = 0; i < (int) matches.size(); ++i) {
      if (i < Nbest) {
        moy += matches[i].distance;
      }
    }

    moy /= Nbest;

    return moy;
}

int main( int argc, char** argv )
{
  cout << argv[3] << endl;
  Mat mat = Mat(3,3,CV_32FC1);
  char *file[] = {};
  cout << "  Matrice des distances:" << endl << "--------------------------" << endl;
  file[0] = argv[0];
  file[1] = (char*) malloc(50*sizeof(char));
  file[2] = (char*) malloc(50*sizeof(char));
  for(int i=1; i < argc; i++){
    for(int j=1; j < argc; j++){
      strcpy(file[1],argv[i]);
      strcpy(file[2],argv[j]);
      mat.at<float>(i,j) = moyenne(3, file);
      cout << mat.at<float>(i,j) << "\t";
    }
    cout << endl;
  }
  free(file[1]);
  free(file[2]);
  return 0;
}