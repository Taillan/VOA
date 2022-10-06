#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>

using namespace cv;
using namespace std;

Mat src, gradX, gradY;

double harris(Mat& Gx, Mat& Gy, int x, int y, int w, Mat& H)
{
  
  H = Mat(2, 2, CV_64FC1);
  double k= 0.2;

  double Ix = 0;
  double Iy = 0;
  double Ixy = 0;
  
  for (int i = 0; i < H.rows; ++i) {
    for (int j = 0; j < H.cols; ++j) {
      H.at<double>(i, j) = 0;
    }
  }
  
  for (int j = y - w; j < y + w; ++j) {
    for (int i = x - w; i < x + w; ++i) {
      Ix += Gx.at<double>(i,j);
      Iy += Gy.at<double>(i,j);
      Ixy += Gx.at<double>(i,j) * Gy.at<double>(i,j);
          
    }
  }
    
  cout << "Ix: " <<endl<< Ix << endl;
  cout << "Iy: " <<endl<< Iy << endl;
  Ix = Ix * Ix;
  Iy = Iy * Iy;

  for (int j = y - w; j < y + w; ++j) {
    for (int i = x - w; i < x + w; ++i) {
      Mat u = Mat(2, 1, CV_64FC1);
      u.at<double>(0) = Gx.at<double>(j, i);
      u.at<double>(1) = Gy.at<double>(j, i);

      H += u * u.t();
    }
  }

  return determinant(H) - k * trace(H)[0] * trace(H)[0];  // doit en fait retourner det(H)-0.15 tr^2(H)
}


void mouse_callback(int event, int x, int y, int flags, void* unused)
{
if (event == EVENT_LBUTTONDOWN)
  {
  Mat H = Mat(2, 2, CV_32FC1);
  Mat values;
  Mat vectors;

  double score = harris(gradX, gradY,  x,  y, 2, H);
  eigen(H,values,vectors);

	cout << "Vous avez cliqué sur le point (" << x << ", " << y << ")\n";
  cout << "score : " << score<< endl;

  cout << "H : " <<endl<< H << endl;
  cout << "valeur propre : " << endl<< values << endl;
  cout << "vectors : " << endl<< vectors<< endl<< endl<< endl;
  }
}


void getDoGX(Mat& K, int w, double sigma)
{
  K= Mat(2*w+1,2*w+1, CV_64FC1);

  double alpha= 1/(2*M_PI*pow(sigma,4));
  double beta= -1/(2*sigma*sigma);
  
  for (int i=-w; i <= w; i++)
    for (int j=-w; j<= w; j++)
      K.at<double>(j+w,i+w)= i*alpha*exp(beta*(i*i+j*j));
}

string type2str(int type) {
  string r;

  uchar depth = type & CV_MAT_DEPTH_MASK;
  uchar chans = 1 + (type >> CV_CN_SHIFT);

  switch ( depth ) {
    case CV_8U:  r = "8U"; break;
    case CV_8S:  r = "8S"; break;
    case CV_16U: r = "16U"; break;
    case CV_16S: r = "16S"; break;
    case CV_32S: r = "32S"; break;
    case CV_32F: r = "32F"; break;
    case CV_64F: r = "64F"; break;
    default:     r = "User"; break;
  }

  r += "C";
  r += (chans+'0');

  return r;
}

int main( int argc, char** argv )
{
  Mat tmp = imread(argv[1]);
  Mat kernel_x;
  Mat kernel_y;

  if (tmp.channels() > 1)
    {
      cvtColor( tmp, src, COLOR_BGR2GRAY );
      tmp= src;
    }

  tmp.convertTo(src, CV_64FC1);

  getDoGX(kernel_x, 2.0, 2.0/3.0);

  kernel_y = kernel_x.t();

  filter2D(src, gradX, -1, kernel_x);
  filter2D(src, gradY, -1, kernel_y);

  cout<< type2str(gradX.type())<<endl;
  

  namedWindow("src");
  imshow("src", tmp);

  namedWindow("gradX");
  imshow("gradX", gradX);

  namedWindow("gradY");
  imshow("gradY", gradY);

  // Routine de traitement du clic souris
  setMouseCallback("src", mouse_callback, NULL);

  // A vous de compléter le code...
  
  waitKey();
  return 0;
}


