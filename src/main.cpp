#include "SceneOpenGL.h"


int main(void)
{
	// Creation de la sene

    SceneOpenGL scene("OpenGL", 800, 600);


	// Initialisation de la scene

    if(scene.initialiserFenetre() == false)
	return -1;

    if(scene.initGL() == false)
	return -1;


    // Boucle Principale

    scene.bouclePrincipale();


    // Fin du programme

    return 0;
}
