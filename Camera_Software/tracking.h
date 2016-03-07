#ifndef TRACKING
#define TRACKING

#include <stdio.h>
#include <stdlib.h>
#include <opencv/highgui.h>
#include <opencv/cv.h>
#include <time.h>

/**

	Structure color_pixel_hsv.
	Cette structure regroupe :
		- les valeurs H, S et V que l'on veut tracker.
		- un pointeur pour l'image original et l'image HSV.

**/
typedef struct {
	uchar h ;
	uchar s ;
	uchar v ;

	IplImage* cam ;
	IplImage* hsv ;
}color_pixel_hsv;

/**

	Cette structure représente des coordonnées.
	Utile pour le centre de l'image, ou encore le barycentre.

**/
typedef struct {
	int x ;
	int y ;
}coordonnees ;

IplImage* hsvConvert (IplImage* original);
void center_calcul(IplImage* image, coordonnees* center);
void mouseEvent(int evt, int x, int y, int flags, void* param);
int ColorTracking (color_pixel_hsv color, int i, uchar pixel_h, uchar pixel_s, uchar pixel_v, int tolerance_h, int tolerance_s);
int ColorTracking (color_pixel_hsv color, int i, uchar pixel_h, uchar pixel_s, uchar pixel_v, int tolerance_h, int tolerance_s);
void showMatImage (IplImage* img);
coordonnees barycenter_calculation (int* barycenter_x, int size_barycenter_x, int* barycenter_y, int size_barycenter_y, int pixel_tracked);
void imageTreatement();

#endif
