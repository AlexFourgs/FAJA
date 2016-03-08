#include <stdio.h>
#include <stdlib.h>
#include <highgui.h>
#include <cv.h>
#include <time.h>

// Ligne de compilation : gcc -o cam testCam.c 


typedef struct {
	int x ;
	int y ;
} barycenter ;

// Structure contenant les valeurs de la couleur que l'on veut tracker et l'image.
typedef struct {
	uchar r;
	uchar g;
	uchar b;
	IplImage* cam ;
}color_pixel;

// Fonction lié au clic. Renvoi la position du pixel et sa couleur.
void mouseEvent(int evt, int x, int y, int flags, void* param) {
	color_pixel* color = (color_pixel*) param ;
	int i = 0 ;

	// On récupére la couleur du pixel sur lequel on clique.
	if(evt==CV_EVENT_LBUTTONDOWN) {
		color->r = color->cam->imageData[((x*3)+(y*color->cam->widthStep))+2] ;
		color->g = color->cam->imageData[((x*3)+(y*color->cam->widthStep))+1] ;
		color->b = color->cam->imageData[((x*3)+(y*color->cam->widthStep))] ;

		printf("pixel_colors x = %d, pixel_colors y = %d\nR = %u, G = %u, B = %u\n", x, y, color->r, color->g, color->b);
	}
}

int ColorTracking (IplImage* cap, color_pixel color, int i, uchar pixel_blue, uchar pixel_green, uchar pixel_red) {
	// Etudier la couleur pour adapter la tolerence en fonction de R, G ou B.
	int toleranceR = 15 ;
	int toleranceG = 26 ;
	int toleranceB = 40 ;
	/*int toleranceR = 5 ;
	int toleranceG = 5 ;
	int toleranceB = 5 ;*/

	// Si les pixels sont de la couleur on les met en rouge.
	if ((pixel_blue >= (color.b)-toleranceB) && (pixel_blue <= (color.b)+toleranceB) && (pixel_green >= (color.g)-toleranceG) && (pixel_green <= (color.g)+toleranceG) && (pixel_red >= (color.r)-toleranceR) && (pixel_red <= (color.r)+toleranceR)) {
		cap->imageData[i] = 0 ;
		cap->imageData[i+1] = 0 ;
		cap->imageData[i+2] = 255 ;

		return 1 ;
	}

	return 0 ;
}

barycenter BarycenterCalculation (int *barycenter_x, int *barycenter_y, int size_x, int size_y) {
	barycenter coordonnees ;
	coordonnees.x = coordonnees.y = 0 ;
	//int value_max = 0 ;
	int i, j = 0 ;
	int nombre_x = 0 ;
	int nombre_y = 0 ;
	int compte_x = 0 ;
	int compte_y = 0 ;

	for (i = 0 ; i < size_x ; i++){
		nombre_x = nombre_x + (i*barycenter_x[i]);
		compte_x = compte_x + barycenter_x[i];
	}
	if (compte_x != 0){
		coordonnees.x = nombre_x/compte_x;
	}

	for (i = 0 ; i < size_y ; i++){
		nombre_y = nombre_y + (i*barycenter_y[i]);
		compte_y = compte_y + barycenter_y[i];
	}
	if (compte_x != 0){
		coordonnees.y = nombre_y/compte_y ;
	}
	/*
	for(i = 0 ; i < size_x ; i++){
		for(j = 0 ; j < size_y ; j++){
			if ((barycenter_x[i]+barycenter_y[j]) > value_max) {
				value_max = barycenter_x[i]+barycenter_y[j];
				coordonnees.x = i ;
				coordonnees.y = j ;
			}
		}
	}*/

	return coordonnees ;
}

void CameraTraitement() {
	int nbImage = 0 ;
	float exe_time = 0 ;
	clock_t t1, t2, time_test ;
	t1 = clock() ;


	color_pixel color ;
	color.r = 0 ;
	color.g = 255 ;
	color.b = 0 ;

	barycenter barycentre_coordonnees ;
	barycentre_coordonnees.x = barycentre_coordonnees.y = 0 ;

	int *barycenter_x ;
	int *barycenter_y ;

	// Déclaration d'un caractère pour la fermeture des fenêtres
	char key;
	int i, j = 0 ;

	// Déclaration des images
	IplImage* cap ;

	// Déclaration de l'élément de capture à partir de la webcam
	CvCapture *capture ;

	// Déclaration des différents noms de fenêtres
	const char* window_title = "Original Camera" ;
	const char* window_hsv = "Hsv Camera" ;

	// On choisis la source pour l'image (webcam).
	capture = cvCreateCameraCapture (CV_CAP_ANY);

	// Si la capture d'image échoue, on arrête le programme avec un message.
	if (!capture) {
		printf("Ouverture du flux video impossible !\n") ;
		exit(1);
	}

	// Création de la fenêtre.
	cvNamedWindow (window_title, CV_WINDOW_AUTOSIZE);

	printf("\n") ;

	uchar pixel_blue ;
	uchar pixel_green ;
	uchar pixel_red ;

	// On alloue la mémoire pour nos tableaux de calcul du barycentre
	cap = cvQueryFrame(capture);

	/*barycenter_x = calloc(cap->width, sizeof(int));
	barycenter_y = calloc(cap->height, sizeof(int));*/


	// Tant qu'on appuie pas sur q le on continu la boucle.
	while (key != 'q' && key != 'Q') {
		barycenter_x = calloc(cap->width, sizeof(int));
		barycenter_y = calloc(cap->height, sizeof(int));


		// On met la capture de la webcam dans l'attribut cap.
		cap = cvQueryFrame(capture);

		// Si on traite en BGR
		color.cam = cap ;


		// On parcours notre image (les pixels).
		for (i = 0 ; i < (cap->width) ; i++) {
			for (j = 0 ; j < cap->height ; j++) {

				// Pixels B,G,R en fonction de la position.
				pixel_blue = (uchar)(cap->imageData[((i*3)+(j*cap->widthStep))]) ;
				pixel_green = (uchar)(cap->imageData[((i*3)+(j*cap->widthStep))+1]) ;
				pixel_red = (uchar)(cap->imageData[((i*3)+(j*cap->widthStep))+2]) ;

				// Tracking des pixels en fonction de la couleur.
				if(ColorTracking(cap, color, ((i*3)+(j*cap->widthStep)), pixel_blue, pixel_green, pixel_red)){
				//printf("y = %d, x = %d\n", i/((cap->width)*3), i%((cap->width)*3));
					//printf("x = %d, y = %d\n", i/3, j);
					barycenter_y[j]++ ;

					barycenter_x[i]++ ;
				}
			}
		}

		// On calcul le barycentre.
		barycentre_coordonnees = BarycenterCalculation (barycenter_x, barycenter_y, cap->width, cap->height) ;

		//printf("barycentre x main = %d, barycentre y main = %d\n", barycentre_coordonnees.x, barycentre_coordonnees.y);
		CvPoint p ;
		p.x = barycentre_coordonnees.x ;
		p.y = barycentre_coordonnees.y ;

		cvCircle(cap, p, 5, CV_RGB(0,0,255), -1, 8, 0);

		// On affiche la webcam normalement.
		cvShowImage(window_title, cap);
		cvSetMouseCallback(window_title, mouseEvent, &color);

		// On réinitialise les barycentres
		/*memset(barycenter_y, 0, cap->height);
		memset(barycenter_x, 0, cap->width);*/
		free(barycenter_x);
		free(barycenter_y);
		key = cvWaitKey(10);
		//nbImage++ ;
		//printf("bary x = %d, bary y = %d\n", barycentre_coordonnees.x, barycentre_coordonnees.y);
	}



	cvReleaseCapture(&capture);
	cvDestroyWindow(window_title);

	t2 = clock() ;
	exe_time = (float)t2-t1/CLOCKS_PER_SEC;

	//printf("Execution time for %d seconds = %f\n", time_test, exe_time);
}

int main (int argc, char* argv[]){
	CameraTraitement();

	return 0 ;
}
