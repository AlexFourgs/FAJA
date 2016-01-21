#include <iostream>
#include <SDL/SDL.h>

using namespace std;


int main(){
	
	int i=0;
	
printf("Programme de configuration du Joystick\n\n *******************\n\n\n"); 
    if(SDL_Init(SDL_INIT_JOYSTICK) < 0) // initialise juste le joystick (pas besoin d'une fenêtre pour nos tests)
        return EXIT_FAILURE;


        printf("Il y a %d joysticks. \n",SDL_NumJoysticks()); // on écrit combien il y a de joysticks
       
       for(i=0;i<SDL_NumJoysticks();i++)    {                                    // tant qu'il y a un joystick non-traité
            printf("Nom du joystick numero %d : %s",i,SDL_JoystickName(i)); // on écrit les noms des joysticks
    }
    
	if(i>1 || i<0){
printf("Merci de brancher que un Joystick.\n");
return 1;

}

    SDL_Joystick *joystick; // attention, c'est bien un pointeur !
	joystick = SDL_JoystickOpen(0); // prend en paramètre le numéro du joystick, ici 0
    printf("Le Joystick possède :\nBoutons : %d \nAxes : %d\n Chapeaux :%d \nTrackballs %d\n",SDL_JoystickNumButtons(joystick),SDL_JoystickNumAxes(joystick),SDL_JoystickNumHats(joystick),SDL_JoystickNumBalls(joystick));

	
	
	switch(evenements.type)
{
    case SDL_JOYHATMOTION:
        switch(evenements.jhat.which)
        {
            case 0:
                switch(evenements.jhat.hat)
                {
                    case 0:
                        switch(evenements.jhat.value)
                        {
                            case SDL_HAT_UP:
                                avancerPerso(&perso1,HAUT);
                                break;
                            case SDL_HAT_DOWN:
                                avancerPerso(&perso1,BAS);
                                break;
                            case SDL_HAT_RIGHT:
                                avancerPerso(&perso1,DROITE);
                                break;
                            case SDL_HAT_LEFT:
                                avancerPerso(&perso1,GAUCHE);
                                break;
                        }
                    break;    
                }
                break;
  
            case 1:
                switch(evenements.jhat.hat)
                {
                    case 0:
                        switch(evenements.jhat.value)
                        {
                            case SDL_HAT_UP:
                                avancerPerso(&perso2,HAUT);
                                break;
                            case SDL_HAT_DOWN:
                                avancerPerso(&perso2,BAS);
                                break;
                            case SDL_HAT_RIGHT:
                                avancerPerso(&perso2,DROITE);
                                break;
                            case SDL_HAT_LEFT:
                                avancerPerso(&perso2,GAUCHE);
                                break;
                        }
                        break;    
                }
                break;
            }
            break;  
        }

	
    SDL_Quit();
 
  return 0;
}
