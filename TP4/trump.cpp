#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace cv;
using namespace std;


void mouse_callback(int event, int x, int y, int flags, void* unused)
{
if (event == EVENT_LBUTTONDOWN)
	cout << "Vous avez cliqué sur le point (" << x << ", " << y << ")\n";
}



int main( int, char** argv )
{
  const char* win_name = "Mon image";
  
  // Charger les images
  Mat panneau = imread("panneau.jpg", 1 );
  Mat panneau_org = imread("panneau.jpg", 1 );
  Mat trump = imread("trump.jpg", 1);

  
  // Les afficher
  namedWindow( win_name, cv::WINDOW_AUTOSIZE );
  imshow( win_name, panneau );
  vector<Point2f> psrc, pdst;
  pdst.push_back(Point2f(500.0f,100.0f));
  pdst.push_back(Point2f(727.0f,76.0f));
  pdst.push_back(Point2f(761.0f,465.0f));
  pdst.push_back(Point2f(472.0f,435.0f));

  psrc.push_back(Point2f(0.0f,0.0f));
  psrc.push_back(Point2f(729.0f,0.0f));
  psrc.push_back(Point2f(729.0f,985.0f));
  psrc.push_back(Point2f(0.0f,985.0f));
 
  Mat H = findHomography(psrc, pdst);
  // A compléter
  warpPerspective(trump, panneau, H, panneau.size());

  cv::namedWindow("resultat", WINDOW_AUTOSIZE );
  panneau = panneau_org | panneau;
  imshow("resultat", panneau );

  imwrite("resultat.jpg", panneau);
  // Attendre une touche 
  setMouseCallback("Mon image", mouse_callback, NULL);
  waitKey(0);

  
  return(0);
}


