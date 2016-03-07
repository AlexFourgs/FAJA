#include "tracking.h"

/**

	@original est l'image de départ à convertir en hsv.
	@return une image en hsv.

	Cette fonction convertit une image BGR en HSV.

**/
IplImage* hsvConvert (IplImage* original) {
	// On déclaire notre nouvelle image.
	IplImage* hsv ;

	// On clone notre image original.
	hsv = cvCloneImage(original);

	// On modifie le ROI de notre image, puis on l'a convertis.
	cvSetImageROI(original, cvRect(0, 0, original->width, original->height));
	cvCvtColor(original, hsv, CV_BGR2HSV);

	// On renvoi notre image en hsv.
	return hsv ;
}

/**

	Cette fonction calcul les coordonnées du centre de l'image et les met dans
	une donnée image_center.

**/
void center_calcul(IplImage* image, coordonnees* center){
	center->x = (image->width)/2 ;
	center->y = (image->height)/2 ;
}

/**

	Fonction qui permet d'obtenir les informations liés au clic.
	X et Y permettent d'obtenir la position du clic.
	On récupère ensuite les informations du pixel sur lequel on a cliqué grâce à
	sa position.
	Les informations récupérés sont les valeurs HSV (Hue, Saturation, Value)
	que l'on met dans une structure de type color_pixel_hsv.

**/
void mouseEvent(int evt, int x, int y, int flags, void* param) {
	color_pixel_hsv* color = (color_pixel_hsv*) param ;

	// On récupére les informations du pixel sur lequel on clique.
	if(evt==CV_EVENT_LBUTTONDOWN) {
		color->v = color->hsv->imageData[((x*3)+(y*color->cam->widthStep))+2] ;
		color->s = color->hsv->imageData[((x*3)+(y*color->cam->widthStep))+1] ;
		color->h = color->hsv->imageData[((x*3)+(y*color->cam->widthStep))] ;

		// Format HSV Gimp : H = 0-360, S = 0-100, V = 0-100
		// Format HSV OpenCV : H = 0-180, S = 0-255, V = 0-255
		//printf("pixel x : %d, pixel y : %d\n", x, y);
		//printf("H = %u, S = %u, V = %u\n", color->h, color->s, color->v);
	}
}

/**

	Fonction de tracking sur UN pixel.
	Lors du parcours de notre image, on passe par chacun des pixels.
	Si le pixel correspond aux valeurs que l'on veut tracké (Plus ou Moins une
	tolérance), alors on le met en blanc et on renvoi 1.
	Sinon on renvoi 0.

**/
int ColorTracking (color_pixel_hsv color, int i, uchar pixel_h, uchar pixel_s, uchar pixel_v, int tolerance_h, int tolerance_s) {
	// Etudier la couleur pour adapter la tolerence en fonction de R, G ou B.
	int toleranceH = tolerance_h ;
	int toleranceS = tolerance_s ;
	int toleranceV = 40 ;

	// Si les pixels sont de la couleur on les met en blanc.
	if ((pixel_h >= (color.h)-toleranceH) && (pixel_h <= (color.h)+toleranceH)
				&& (pixel_s >= (color.s)-toleranceS) && (pixel_s <= (color.s)+toleranceS)
					&& (pixel_v >= (color.v)-toleranceV) && (pixel_v <= (color.v)+toleranceV)) {

		color.cam->imageData[i] = 255 ; // H
		color.cam->imageData[i+1] = 255 ; // S
		color.cam->imageData[i+2] = 255 ; // V

		return 1 ;
	}

	return 0 ;
}

/**

	Fonction qui affiche la matrice de l'image "img" dans le terminal.

**/

void showMatImage (IplImage* img) {
	int i, j = 0 ;

	for (i=0 ; i < (img->width)*3 ; i=i+3){
		for (j=0 ; j < img->height ; j++){
			printf("%3u ",(uchar)img->imageData[(i*3)+(j*img->widthStep)]);
			printf("%u ",(uchar)img->imageData[((i*3)+(j*img->widthStep))+1]);
			printf("%u ",(uchar)img->imageData[((i*3)+(j*img->widthStep))+2]);
		}
		printf("\n");
	}
}

/**

	Fonction qui permet de calculer le barycentre.

**/
coordonnees barycenter_calculation (int* barycenter_x, int size_barycenter_x, int* barycenter_y, int size_barycenter_y, int pixel_tracked){
	coordonnees barycenter ;
	barycenter.x = barycenter.y = 0 ;

	int x_number = 0 ;
	int y_number = 0 ;
	int i = 0 ;

	while((i!=size_barycenter_x) && (x_number <= pixel_tracked/2)){
		x_number = x_number + barycenter_x[i];
		i++;
	}
	barycenter.x = i ;


	i=0;
	while((i!=size_barycenter_y) && (y_number <= pixel_tracked/2)){
		y_number = y_number + barycenter_y[i];
		i++;
	}
	barycenter.y = i ;

	//printf("barycentre = [%d][%d]\n", barycenter.x, barycenter.y);
	return barycenter ;
}

/**

	Fonction qui récupère l'image de la webcam et effectue le traitement en
	faisant appel aux autres fonctions.

**/
void imageTreatement() {
	/*
		Déclaration des variables
	*/
	int i, j ;
	char key ;
	color_pixel_hsv color ;
	CvCapture *capture ;
	//IplImage* cap = cvLoadImage("imagetest.jpg", CV_LOAD_IMAGE_UNCHANGED);
	IplImage* cap ;
	IplImage* cap_resize ;
	IplImage* hsv ;

	coordonnees center ;
	coordonnees barycenter ;
	center.x = center.y = barycenter.x = barycenter.y = 0 ;
	int *barycenter_x ;
	int *barycenter_y ;
	int pixel_tracked_number = 0 ;

	uchar pixel_h ;
	uchar pixel_s ;
	uchar pixel_v ;

	int tolerance_h = 20 ;
	int tolerance_s = 20 ;

	/*

		On capture à partir de la webcam, si la capture echoue un message s'affiche.

	*/
	capture = cvCreateCameraCapture(CV_CAP_ANY);

	if(!capture){
		printf("! Error while capturing from camera !\n");
		exit(1);
	}

	/*
		On capture l'image une seule fois pour connaitre les informations de
		l'image et pouvoir définir la nouvelle image avec résolution plus faible.
	*/
	cap = cvQueryFrame(capture);
	cap_resize = cvCreateImage(cvSize(320, 240), cap->depth, cap->nChannels);

	center_calcul(cap_resize, &center);

	//printf("Width image = %d, Height image = %d, Centre théorique = [%d][%d], Centre calculé = [%d][%d]\n", cap_resize->width, cap_resize->height, (cap_resize->width)/2, (cap_resize->height)/2, center.x, center.y);
	//printf("Width = %d, Height = %d\n", cap_resize->width ,cap_resize->height);*/

	/*
		conversion de l'image original (BVR) en image HSV.
	*/
	//hsv = hsvConvert(cap);

	/*
		Déclaration et création des fenêtres.
	*/
	const char* original = "Image Originale" ;
	const char* hsv_window = "Image HSV" ;

	cvNamedWindow(original, CV_WINDOW_NORMAL);
	cvMoveWindow(original, 0, 0);
	cvSetWindowProperty(original, CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);
	cvNamedWindow(hsv_window, CV_WINDOW_AUTOSIZE);

	cvCreateTrackbar("Hue Tolerance", "Image Originale", &tolerance_h, 125, NULL);
	cvCreateTrackbar("Saturation Tolerance", "Image Originale", &tolerance_s, 125, NULL);

	// Enlever le commentaire pour afficher la matrice de l'image dans le terminal.
	//showMatImage(hsv);

	/*
		Traitement des images.
	*/
	while(key != 'q' && key != 'Q') {

		pixel_tracked_number = 0 ;

		// On alloue la mémoire pour les tableaux des barycentres.
		barycenter_x = (int*)calloc(cap->width, sizeof(int));
		barycenter_y = (int*)calloc(cap->height, sizeof(int));

		// On met à jour l'image et on la convertis.
		cap = cvQueryFrame(capture);
		cvResize(cap, cap_resize, CV_INTER_LINEAR);
		hsv = hsvConvert(cap_resize);

		// On met les pointeurs sur les images dans la structure "color".
		color.cam = cap_resize ;
		color.hsv = hsv ;

		// Fonction du clic de la souris.
		cvSetMouseCallback(original, mouseEvent, &color);

		// On parcours toute l'image, pixel par pixel.
		for (i = 0 ; i < (hsv->width)*3 ; i=i+3) {
			for (j = 0 ; j < hsv->height ; j++) {

				// Pixels H,S,V en fonction de la position.
				pixel_h = (uchar)(hsv->imageData[((i*3)+(j*hsv->widthStep))]) ;
				pixel_s = (uchar)(hsv->imageData[((i*3)+(j*hsv->widthStep))+1]) ;
				pixel_v = (uchar)(hsv->imageData[((i*3)+(j*hsv->widthStep))+2]) ;

				// Tracking des pixels en fonction de la couleur.
				if(ColorTracking(color, ((i*3)+(j*cap_resize->widthStep)), pixel_h, pixel_s, pixel_v, tolerance_h, tolerance_s)){
					//printf("y = %d, x = %d\n", i/((cap->width)*3), i%((cap->width)*3));

					barycenter_y[j]++ ;

					barycenter_x[i/3]++ ;

					pixel_tracked_number++ ;
				}
			}
		}

		// Calcul des barycentres.
		barycenter = barycenter_calculation(barycenter_x, cap->width, barycenter_y, cap->height, pixel_tracked_number);

		CvPoint p ;
		p.x = barycenter.x ;
		p.y = barycenter.y ;

		cvCircle(cap_resize, p, 5, CV_RGB(0,0,255), -1, 8, 0);

		// Calculé le barycentre et y mettre un point.
		// On met les images dans les fenêtres.
		cvShowImage(original, cap_resize);
		cvShowImage(hsv_window, hsv);

		// Libération de la mémoire des tableaux de barycentres.
		free(barycenter_x);
		free(barycenter_y);

		key = cvWaitKey(10);
	}
}
