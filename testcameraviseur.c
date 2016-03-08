#include <stdio.h>
#include <stdlib.h>
#include <opencv/highgui.h>
#include <opencv/cv.h>
#include <time.h>


void imageTreatement() {	
		const char* original = "FAJA" ;
	char key ;
	CvCapture *capture ;
	IplImage* cap ;
	IplImage* cap_resize ;

	capture = cvCreateCameraCapture(CV_CAP_ANY);
	
	if(!capture){
		printf("! Error while capturing from camera !\n");
		exit(1);
	}	
	
	CvFont cross,faja;

cvInitFont(&cross,CV_FONT_HERSHEY_SIMPLEX|CV_FONT_ITALIC, 1,1,0,1,1);
cvInitFont(&faja,CV_FONT_HERSHEY_SIMPLEX|CV_FONT_ITALIC, 1,1,0,2,3);

	cap = cvQueryFrame(capture);
	cap_resize = cvCreateImage(cvSize(1366,768), cap->depth, cap->nChannels);

cvNamedWindow(original, CV_WINDOW_NORMAL);
	cvMoveWindow(original, 0, 0);
	cvSetWindowProperty(original, CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);
//cvShowImage("main_win", cv_img);
	

	while(key != 'q' && key != 'Q') {

		cap = cvQueryFrame(capture);
		cvPutText (cap,"+",cvPoint(cap->width/2,cap->height/2), &cross, cvScalar(0,255,0,0));
		cvPutText (cap,"FAJA - Mode manuel",cvPoint(10,30), &faja, cvScalar(255,0,0,0));
		cvResize(cap, cap_resize, CV_INTER_LINEAR);
	

		cvShowImage(original, cap_resize);
		

		key = cvWaitKey(10);
	}
}

int main (int argc, char* argv[]) {

	imageTreatement();

	return 0 ;
}
