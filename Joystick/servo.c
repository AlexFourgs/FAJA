//  sdl-jstest - Joystick Test Program for SDL
//  Copyright (C) 2007 Ingo Ruhnke <grumbel@gmx.de>
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 2
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
//  02111-1307, USA.

#include <SDL/SDL.h>
#include <assert.h>
#include <curses.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int str2int(const char* str, int* val)
{
  char* endptr;
  errno = 0;    /* To distinguish success/failure after call */

  *val = strtol(str, &endptr, 10);

  /* Check for various possible errors */
  if ((errno == ERANGE && (*val == LONG_MAX || *val == LONG_MIN))
      || (errno != 0 && *val == 0)) {
    return 0;
  }

  if (endptr == str) {
    return 0;
  }

  return 1;
}

void print_joystick_info(int joy_idx, SDL_Joystick* joy)
{
  printf("Joystick Name:     '%s'\n", SDL_JoystickName(joy_idx));
  printf("Joystick Number:   %2d\n", joy_idx);
  printf("Number of Axes:    %2d\n", SDL_JoystickNumAxes(joy));
  printf("Number of Buttons: %2d\n", SDL_JoystickNumButtons(joy));
  printf("Number of Hats:    %2d\n", SDL_JoystickNumHats(joy));
  printf("Number of Balls:   %2d\n", SDL_JoystickNumBalls(joy));
  printf("\n");
}

void print_help(const char* prg)
{
  printf("Usage: %s [OPTION]\n", prg);
  printf("List available joysticks or test a  joystick.\n");
  printf("This programm uses SDL for doing its test instead of using the raw\n"
         "/dev/input/jsX interface\n");
  printf("\n");
  printf("Options:\n");
  printf("  --help             Print this help\n");
  printf("  --version          Print version number and exit\n");
  printf("  --list             Search for available joysticks and list their properties\n");
  printf("  --test  JOYNUM     Display a graphical representation of the current joystick state\n");
  printf("  --event JOYNUM     Display the events that are recieved from the joystick\n");
  printf("\n");
  printf("Examples:\n");
  printf("  %s --list\n", prg);
  printf("  %s --test 1\n", prg);
}

int main(int argc, char** argv)
{
	
	
		int i, j ;
	char key ;
	color_pixel_hsv color ;
	CvCapture *capture ;
	//IplImage* cap = cvLoadImage("imagetest.jpg", CV_LOAD_IMAGE_UNCHANGED);
	IplImage* cap ;
	IplImage* cap_resize ;
	IplImage* hsv ;

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
		On capture l'image une seule fois pour connaitre les informations de l'image et pouvoir définir
		la nouvelle image avec résolution plus faible.
	*/
	cap = cvQueryFrame(capture);
	cap_resize = cvCreateImage(cvSize(320, 240), cap->depth, cap->nChannels);
	
  if (argc == 1)
  {
    print_help(argv[0]);
    exit(1);
  }

  // FIXME: We don't need video, but without it SDL will fail to work in SDL_WaitEvent()
  if(SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0)
  {
    fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
    exit(1);
  }
  else
  {
    atexit(SDL_Quit);
    if (argc == 2 && (strcmp(argv[1], "--help") == 0 ||
                      strcmp(argv[1], "-h") == 0))
    {
      print_help(argv[0]);
    }
    if (argc == 2 && (strcmp(argv[1], "--version") == 0))
    {
      printf("sdl-jstest 0.1.0\n");
      exit(EXIT_SUCCESS);
    }
    else if (argc == 2 && (strcmp(argv[1], "--list") == 0 ||
                           (strcmp(argv[1], "-l") == 0)))
    {
      int num_joysticks = SDL_NumJoysticks();
      if (num_joysticks == 0)
      {
        printf("No joysticks were found\n");
      }
      else
      {
        printf("Found %d joystick(s)\n\n", num_joysticks);
        for(int joy_idx = 0; joy_idx < num_joysticks; ++joy_idx)
        {
          SDL_Joystick* joy = SDL_JoystickOpen(joy_idx);
          if (!joy)
          {
            fprintf(stderr, "Unable to open joystick %d\n", joy_idx);
          }
          else
          {
            print_joystick_info(joy_idx, joy);
            SDL_JoystickClose(joy);
          }
        }
      }
    }
    else if (argc == 3 && (strcmp(argv[1], "--test") == 0 ||
                           strcmp(argv[1], "-t") == 0))
    {
      int joy_idx;
      if (!str2int(argv[2], &joy_idx))
      {
        fprintf(stderr, "Error: JOYSTICKNUM argument must be a number, but was '%s'\n", argv[2]);
        exit(1);
      }

      SDL_Joystick* joy = SDL_JoystickOpen(joy_idx);
      if (!joy)
      {
        fprintf(stderr, "Unable to open joystick %d\n", joy_idx);
      }
      else
      {
        int num_axes    = SDL_JoystickNumAxes(joy);
        int num_buttons = SDL_JoystickNumButtons(joy);
        int num_hats    = SDL_JoystickNumHats(joy);
        int num_balls   = SDL_JoystickNumBalls(joy);

        Sint16* axes    = calloc(num_axes,    sizeof(Sint16));
        Uint8*  buttons = calloc(num_buttons, sizeof(Uint8));
        Uint8*  hats    = calloc(num_hats,    sizeof(Uint8));
        Uint8*  balls   = calloc(num_balls,   2*sizeof(Sint16));

        int quit = 0;
        FILE* f = NULL;

        SDL_Event event;

        f=fopen("/dev/servoblaster", "w");
        if (f == NULL)
          exit(1);

        while(!quit)
        {
          SDL_Delay(10);

          bool something_new = false;
          float res = 50;

          while (SDL_PollEvent(&event)) {
            something_new = true;
            switch(event.type)
            {
              case SDL_JOYAXISMOTION:
                assert(event.jaxis.axis < num_axes);
                axes[event.jaxis.axis] = event.jaxis.value;
                break;

              case SDL_JOYBUTTONDOWN:
              case SDL_JOYBUTTONUP:
                //                assert(event.jbutton.button < num_buttons);
                //                buttons[event.jbutton.button] = event.jbutton.state;
                break;

              case SDL_JOYHATMOTION:
                //                assert(event.jhat.hat < num_hats);
                //                hats[event.jhat.hat] = event.jhat.value;
                break;
            case SDL_JOYBALLMOTION:
              //                assert(event.jball.ball < num_balls);
              //                balls[2*event.jball.ball + 0] = event.jball.xrel;
              //                balls[2*event.jball.ball + 1] = event.jball.yrel;
                break;

              case SDL_QUIT:
                quit = 1;
                printf("Recieved interrupt, exiting\n");
                break;

              default:
                fprintf(stderr, "Error: Unhandled event type: %d\n", event.type);
            }
          }


          if (something_new)
          {
                res = 50+(((-1 * (axes[0])+32768)*190)/65535);
                fprintf(f,"1=%i\n",(int)res);
                printf("echo 1=%i > /dev/servoblaster\n", (int) res);
                fflush(f);

                res = 50+(((-1 * (axes[1])+32768)*190)/65535);
                fprintf(f,"0=%i\n", (int)res);
                printf("echo 0=%i > /dev/servoblaster\n", (int) res);
                fflush(f);
          }
              refresh();

        } // while

        free(balls);
        free(hats);
        free(buttons);
        free(axes);
        fclose(f);
        endwin();
      }
    }
    else if (argc == 3 && (strcmp(argv[1], "--event") == 0 ||
                           strcmp(argv[1], "-e") == 0))
    {
      int joy_idx;
      if (!str2int(argv[2], &joy_idx))
      {
        fprintf(stderr, "Error: JOYSTICKNUM argument must be a number, but was '%s'\n", argv[2]);
        exit(1);
      }

      SDL_Joystick* joy = SDL_JoystickOpen(joy_idx);
      if (!joy)
      {
        fprintf(stderr, "Unable to open joystick %d\n", joy_idx);
      }
      else
      {
        print_joystick_info(joy_idx, joy);

        printf("Entering joystick test loop, press Ctrl-c to exit\n");
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
              printf("SDL_JOYBUTTONUP: joystick: %d button: %d state: %d\n",
                     event.jbutton.which, event.jbutton.button, event.jbutton.state);
              break;
            case SDL_JOYBUTTONUP:
              printf("SDL_JOYBUTTONDOWN: joystick: %d button: %d state: %d\n",
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
              printf("Recieved interrupt, exiting\n");
              break;

            default:
              fprintf(stderr, "Error: Unhandled event type: %d\n", event.type);
          }
        }
        SDL_JoystickClose(joy);

      }
      fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
    }
    else
    {
      fprintf(stderr, "%s: unknown arguments\n", argv[0]);
      fprintf(stderr, "Try '%s --help' for more informations\n", argv[0]);
    }
  }
}

/* EOF */
