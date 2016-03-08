// Progamme de contrôle du servomoteur avec Joystick à l'aide de la librarie SDL 
//
// Inspiré du code de Ingo Runke 2007, sdl-jstest. 
//
// Ce programme nécessite les librairies : SDL 1, ncurses.
//
// Compilation : gcc -Wall -c "%f" -lSDL -lncurses -std=c99 -std=gnu99
// Construction : gcc -Wall -o "%e" "%f" -lSDL -lncurses -std=c99 -std=gnu99 
//
// Remplacer %e et %f par les noms des fichiers que vous souhaitez compiler. Ces variables sont pour l'IDE Geany.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
// 02111-1307, USA.

#include <SDL/SDL.h>
#include <assert.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

//On affiche les informations du Joysticks
void print_joystick_info(int joy_idx, SDL_Joystick* joy)
{
  printf("Nom du Joystick:     '%s'\n", SDL_JoystickName(joy_idx));
  printf("Numéro du Joystick:   %2d\n", joy_idx);
  printf("Nombre d'axes :    %2d\n", SDL_JoystickNumAxes(joy));
  printf("Nombre de bouttons : %2d\n", SDL_JoystickNumButtons(joy));
  printf("Nombre de chapeaux :    %2d\n", SDL_JoystickNumHats(joy));
  printf("Nombre de boules :   %2d\n", SDL_JoystickNumBalls(joy));
  printf("\n");
}

void graphicalJoystick(int joy_idx, SDL_Joystick *joy, FILE* fichierKernel){

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
        
        int num_axes    = SDL_JoystickNumAxes(joy) -1;
        int num_buttons = SDL_JoystickNumButtons(joy);
        int num_hats    = SDL_JoystickNumHats(joy);
FILE* f = NULL;

        f=fopen("/dev/servoblaster", "w");
        if (f == NULL)
          exit(1);
        int quit = 0;
        SDL_Event event;
        while(!quit)
        {
          SDL_Delay(10);

          bool something_new = false;
          while (SDL_PollEvent(&event)) {
			  
			  cap = cvQueryFrame(capture);
		cvPutText (cap,"+",cvPoint(cap->width/2,cap->height/2), &cross, cvScalar(0,255,0,0));
		cvPutText (cap,"FAJA - Mode manuel",cvPoint(10,30), &faja, cvScalar(255,0,0,0));
		cvResize(cap, cap_resize, CV_INTER_LINEAR);
	

		cvShowImage(original, cap_resize);
            something_new = true;
            switch(event.type)
            {
              case SDL_JOYAXISMOTION:
                assert(event.jaxis.axis < num_axes+1);
                axes[event.jaxis.axis] = event.jaxis.value;
                break;

              case SDL_JOYBUTTONDOWN:
              case SDL_JOYBUTTONUP:
                assert(event.jbutton.button < num_buttons);
                buttons[event.jbutton.button] = event.jbutton.state;
                break;

              case SDL_JOYHATMOTION:
                assert(event.jhat.hat < num_hats);
                hats[event.jhat.hat] = event.jhat.value;
                break;

              case SDL_QUIT:
                quit = 1;
                printf("Recieved interrupt, exiting\n");
                break;

              default:
                fprintf(stderr, "Error: Unhandled event type: %d\n", event.type);
                break;
            }
          }
          if (something_new)
          {
            //clear();
            
            res = 50+(((-1 * (axes[0])+32768)*190)/65535);
                fprintf(f,"1=%i\n",(int)res);
                printf("echo 1=%i > /dev/servoblaster\n", (int) res);
                fflush(f);

                res = 50+(((-1 * (axes[1])+32768)*190)/65535);
                fprintf(f,"0=%i\n", (int)res);
                printf("echo 0=%i > /dev/servoblaster\n", (int) res);
                fflush(f);   
     
            }
          }
        } // while
       
      
      

int main()
{
	
int choix=1;


  // FIXME: We don't need video, but without it SDL will fail to work in SDL_WaitEvent()
  if(SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0)
  {
    fprintf(stderr, "Impossible d'initaliser la SDL %s\n", SDL_GetError());
        atexit(SDL_Quit);

    exit(1);
  }
  
  
  FILE *fichierKernel;
    fichierKernel = fopen("/dev/servoblaster", "a");
    if (fichierKernel == NULL) {
        printf("Erreur : Je n'arrive pas à ouvrir le fichierKernel. (/dev/servoblaster)\n");
        exit(0); 
    }   
    
         int num_joysticks = SDL_NumJoysticks();
      if (num_joysticks == 0)
      {
        printf("Aucun Joystick trouvé.\n");
        atexit(SDL_Quit);
        exit(2);
      }
      else
      {
        printf("J'ai trouvé %d joystick(s).\n\n", num_joysticks);
      }
    
      int joy_idx=0;

      SDL_Joystick* joy = SDL_JoystickOpen(joy_idx);
      if (!joy)
      {
        fprintf(stderr, "je ne parviens pas à ouvrir le joystick :  %d\n", joy_idx);
        atexit(SDL_Quit);
        exit(3);
      }
  
  
  graphicalJoystick(joy_idx,joy,fichierKernel);
  
}
