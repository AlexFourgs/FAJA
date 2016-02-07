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
#include <curses.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>


// Fonction d'affichage dynamique sur terminal
void print_bar(int pos, int len)
{
  addch('[');
  for(int i = 0; i < len; ++i)
  {
    if (i == pos)
      addch('#');
    else
      addch(' ');
  }
  addch(']');
}
// Fonction d'affichage dynamique sur terminal

int str2int(const char* str, int* val)
{
  char* endptr;
  errno = 0;    /* Pour distringuer les reussites/echec après l'appel */

  *val = strtol(str, &endptr, 10);

  /* On check les erreurs éventuelles*/
  if ((errno == ERANGE && (*val == LONG_MAX || *val == LONG_MIN))
      || (errno != 0 && *val == 0)) {
    return 0;
  }

  if (endptr == str) {
    return 0;
  }

  return 1;
}

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
initscr();

        //cbreak();
        //noecho();
        //nonl();
        curs_set(0);
        
        int num_axes    = SDL_JoystickNumAxes(joy) -1;
        int num_buttons = SDL_JoystickNumButtons(joy);
        int num_hats    = SDL_JoystickNumHats(joy);

        Sint16* axes    = calloc(num_axes,    sizeof(Sint16));
        Uint8*  buttons = calloc(num_buttons, sizeof(Uint8));
        Uint8*  hats    = calloc(num_hats,    sizeof(Uint8));
FILE* f = NULL;

        f=fopen("/dev/servoblaster", "w");
        if (f == NULL)
          exit(1);
float angleY,angleX = 50;
                fprintf(f,"1=%i\n",(int)angleY);
                fprintf(f,"2=%i\n",(int)angleX);
                fflush(f);	
        int quit = 0;
        SDL_Event event;
        while(!quit)
        {
          SDL_Delay(10);

          bool something_new = false;
          while (SDL_PollEvent(&event)) {
            something_new = true;
            switch(event.type)
            {
              case SDL_JOYAXISMOTION:
                assert(event.jaxis.axis < num_axes+1);
                axes[event.jaxis.axis] = event.jaxis.value;
           
                      //printf("SDL_JOYAXISMOTION: joystick: %d axis: %d value: %d\n",
                    // event.jaxis.which, event.jaxis.axis, event.jaxis.value);
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
     		if(angleY>240){
				angleY=240;
			}
			else if(angleY<50){
				angleY=50;
			}
			if(angleX>240){
				angleX=240;
			}
			else if(angleX<50){
				angleX=50;
			}
			  
			 if(axes[0]<0){
				 
				 if(axes[0]<0 && axes[0] > -10000){
				 angleY--;
				 }
				else if (axes[0]< -10000 && axes[0]>-20000){
				angleY=angleY-2;
				}
				else if(axes[0] < -20000 && axes[0] > -40000){
				angleY=angleY-3;
				} 
                fprintf(f,"1=%i\n",(int)angleY);
                //printf("echo 1=%i > /dev/servoblaster\n", (int) angleY);
                fflush(f);
                		}
			else if(axes[0]>0){
				
				if(axes[0]>0 && axes[0]<10000){
								angleY++;
				}
				else if (axes[0]>10000 && axes[0]<20000){
				angleY=angleY+2;
				}
				else if (axes[0]>20000 && axes[0]<40000){
				angleY=angleY+3;
				}
			    fprintf(f,"1=%i\n",(int)angleY);
			                    fflush(f);
			    }
			    
			    
			     if(axes[1]<0){
				 
				 if(axes[1]<0 && axes[1] > -10000){
				 angleX--;
				 }
				else if (axes[1]< -10000 && axes[1]>-20000){
				angleX=angleX-2;
				}
				else if(axes[1] < -20000 && axes[1] > -40000){
				angleX=angleX-3;
				} 
                fprintf(f,"2=%i\n",(int)angleX);
                fflush(f);
                		}
			else if(axes[1]>0){
				
				if(axes[1]>0 && axes[1]<10000){
								angleX++;
				}
				else if (axes[1]>10000 && axes[1]<20000){
				angleX=angleX+2;
				}
				else if (axes[1]>20000 && axes[1]<40000){
				angleX=angleX+3;
				}
			    fprintf(f,"2=%i\n",(int)angleX);
			                    fflush(f);
			    }
          if (something_new)
          {
            //clear();
            move(0,0);

            printw("Joystick Name:   '%s'\n", SDL_JoystickName(joy_idx));
            printw("Joystick Number: %d\n", joy_idx);
			printw("Nombre d'axes :    %2d\n", SDL_JoystickNumAxes(joy));
			printw("Nombre de boutons : %2d\n", SDL_JoystickNumButtons(joy));
			printw("Nombre de chapeaux :    %2d\n", SDL_JoystickNumHats(joy));
            printw("\n");

            printw("Axes %2d:\n", num_axes);
            for(int i = 0; i < num_axes; ++i)
            {
              int len = COLS - 20;
              printw("  %2d: %6d  ", i, axes[i]);
              print_bar((axes[i] + 32767) * (len-1) / 65534, len);
              addch('\n');
            }
            printw("\n");

            printw("Boutons %2d:\n", num_buttons);
            for(int i = 0; i < num_buttons; ++i)
            {
              printw("  %2d: %d  %s\n", i, buttons[i], buttons[i] ? "[#]":"[ ]");
            }
            printw("\n");

            printw("Hats %2d:\n", num_hats);
            for(int i = 0; i < num_hats; ++i)
            {
              printw("  %2d: valeurs: %d\n", i, hats[i]);
              printw("  +-----+  up:    %c\n"
                     "  |%c %c %c|  down:  %c\n"
                     "  |%c %c %c|  left:  %c\n"
                     "  |%c %c %c|  right: %c\n"
                     "  +-----+\n",

                     (hats[i] & SDL_HAT_UP)?'1':'0',

                     ((hats[i] & SDL_HAT_UP) && (hats[i] & SDL_HAT_LEFT)) ? 'O' : ' ',
                     ((hats[i] & SDL_HAT_UP) && !(hats[i] & (SDL_HAT_LEFT | SDL_HAT_RIGHT))) ? 'O' : ' ',
                     ((hats[i] & SDL_HAT_UP) && (hats[i] & SDL_HAT_RIGHT)) ? 'O' : ' ',

                     (hats[i] & SDL_HAT_DOWN)?'1':'0',

                     (!(hats[i] & (SDL_HAT_UP | SDL_HAT_DOWN)) && (hats[i] & SDL_HAT_LEFT)) ? 'O' : ' ',
                     (!(hats[i] & (SDL_HAT_UP | SDL_HAT_DOWN)) && !(hats[i] & (SDL_HAT_LEFT | SDL_HAT_RIGHT))) ? 'O' : ' ',
                     (!(hats[i] & (SDL_HAT_UP | SDL_HAT_DOWN)) && (hats[i] & SDL_HAT_RIGHT)) ? 'O' : ' ',

                     (hats[i] & SDL_HAT_LEFT)?'1':'0',

                     ((hats[i] & SDL_HAT_DOWN) && (hats[i] & SDL_HAT_LEFT)) ? 'O' : ' ',
                     ((hats[i] & SDL_HAT_DOWN) && !(hats[i] & (SDL_HAT_LEFT | SDL_HAT_RIGHT))) ? 'O' : ' ',
                     ((hats[i] & SDL_HAT_DOWN) && (hats[i] & SDL_HAT_RIGHT)) ? 'O' : ' ',

                     (hats[i] & SDL_HAT_RIGHT)?'1':'0');
            }
            printw("\n");
            refresh();
          }
        } // while
        free(hats);
        free(buttons);
        free(axes);

        endwin();
      }
      
void listJoystick(int joy_idx, FILE* fichierKernel){
	
      SDL_Joystick* joy = SDL_JoystickOpen(joy_idx);
      if (!joy)
      {
        fprintf(stderr, "Impossible d'ouvrir le Joystick numéro :  %d\n", joy_idx);
      }
      else
      {
        print_joystick_info(joy_idx, joy);

        printf("Appuyer sur Ctrl + C pour quitter\n");
        int quit = 0;
        SDL_Event event;

        while(!quit && SDL_WaitEvent(&event))
        {
          switch(event.type)
          {
            case SDL_JOYAXISMOTION:
              printf("SDL_JOYAXISMOTION: joystick: %d axis: %d value: %d\n",
                     event.jaxis.which, event.jaxis.axis, event.jaxis.value);
              break;

            case SDL_JOYBUTTONDOWN:
              printf("SDL_JOYBUTTONDOWN: joystick: %d button: %d state: %d\n",
                     event.jbutton.which, event.jbutton.button, event.jbutton.state);
              break;
            case SDL_JOYBUTTONUP:
              printf("SDL_JOYBUTTONUP: joystick: %d button: %d state: %d\n",
                     event.jbutton.which, event.jbutton.button, event.jbutton.state);
              break;

            case SDL_JOYHATMOTION:
              printf("SDL_JOYHATMOTION: joystick: %d hat: %d value: %d\n",
                     event.jhat.which, event.jhat.hat, event.jhat.value);
              break;

            case SDL_JOYBALLMOTION:
              printf("SDL_JOYBALLMOTION: joystick: %d ball: %d x: %d y: %d\n",
                     event.jball.which, event.jball.ball, event.jball.xrel, event.jball.yrel);
              break;

            case SDL_QUIT:
              quit = 1;
              printf("Signal d'intéruption reçu, je me ferme.\n");
              break;

            default:
              fprintf(stderr, "Erreur : je ne connais pas l'évènements : %d\n", event.type);
              break;
          }
        }
        SDL_JoystickClose(joy);

      }
      fprintf(stderr, "Impossible d'initaliser la SDL : %s\n", SDL_GetError());
    }



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
  
  
  if(choix==1) graphicalJoystick(joy_idx,joy,fichierKernel);
  else if (choix == 3 ) listJoystick(joy_idx,fichierKernel);
}

/* EOF */
