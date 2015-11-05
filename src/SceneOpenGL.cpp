#include "SceneOpenGL.h"
#include "Shader.h"
#include "polygon.h"

// Constructeur de Destucteur

SceneOpenGL::SceneOpenGL(std::string titreFenetre, int largeurFenetre, int hauteurFenetre) :
	m_titreFenetre(titreFenetre), m_largeurFenetre(largeurFenetre),
	m_hauteurFenetre(hauteurFenetre), m_fenetre(0), m_contexteOpenGL(0)
{

}


SceneOpenGL::~SceneOpenGL()
{
	SDL_GL_DeleteContext(m_contexteOpenGL);
	SDL_DestroyWindow(m_fenetre);
	SDL_Quit();
}


// Methodes

bool SceneOpenGL::initialiserFenetre()
{
	// Initialisation de la SDL

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "Erreur lors de l'initialisation de la SDL : " << SDL_GetError() << std::endl;
		SDL_Quit();

		return false;
	}


	// Version d'OpenGL

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	// Double Buffer

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);


	// Creation de la fenetre

	m_fenetre = SDL_CreateWindow(m_titreFenetre.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_largeurFenetre, m_hauteurFenetre, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

	if(m_fenetre == 0)
	{
		std::cout << "Erreur lors de la creation de la fenetre : " << SDL_GetError() << std::endl;
		SDL_Quit();

		return false;
	}


	// Creation du contexte OpenGL

	m_contexteOpenGL = SDL_GL_CreateContext(m_fenetre);

	if(m_contexteOpenGL == 0)
	{
		std::cout << SDL_GetError() << std::endl;
		SDL_DestroyWindow(m_fenetre);
		SDL_Quit();

		return false;
	}

	return true;
}


bool SceneOpenGL::initGL()
{
#ifdef WIN32

	// On initialise GLEW

	GLenum initialisationGLEW( glewInit() );


	// Si l'initialisation a echoue :

	if(initialisationGLEW != GLEW_OK)
	{
		// On affiche l'erreur grace a la fonction : glewGetErrorString(GLenum code)

		std::cout << "Erreur d'initialisation de GLEW : " << glewGetErrorString(initialisationGLEW) << std::endl;


		// On quitte la SDL

		SDL_GL_DeleteContext(m_contexteOpenGL);
		SDL_DestroyWindow(m_fenetre);
		SDL_Quit();

		return false;
	}

#endif

	glEnable(GL_DEPTH_TEST);
	return true;
}

void SceneOpenGL::bouclePrincipale()
{
	// Vertices

	Shader	shader("Shaders/couleur2D.vert", "Shaders/couleur2D.frag");

	shader.charger();
	float vertices[] =	{-0.5, -0.5, 0.0,    0.5, -0.5, 0.0,   0.5, 0.5, 0.0,    // Triangle 1
						 -0.5, -0.5, 0.0,   -0.5,  0.5, 0.0,   0.5, 0.5, 0.0};   // Triangle 2

	float color[] =		{1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0,
						 1.0, 1.0, 1.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0};


	// Variables relatives au framerate

	unsigned int frameRate (1000 / 60);
	Uint32 debutBoucle(0), finBoucle(0), tempsEcoule(0);
	int		state = 1;
	SDL_Event	e;
	// Boucle principale
	glm::mat4	proj;
	glm::mat4	view;
	glm::vec3	tmp(0.0, 0.0, 0.0);

	Polygon	square(vertices, 18, color);
	square.load();

	proj = glm::perspective(70.0, (double) m_largeurFenetre / m_hauteurFenetre, 0.1, 100.0);
	view = glm::lookAt(glm::vec3(-1.0, 0.0, 1.0), glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 1.0));

	while(state)
	{
		// On définit le temps de début de boucle

		debutBoucle = SDL_GetTicks();


		// Gestion des évènements
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
				state = 0;
		}

		tmp.z += 0.01;
		square.setPos(tmp);

		// Nettoyage de l'écran

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Verrouillage du VAO


		glUseProgram(shader.getProgramID());

		square.draw(shader.getProgramID(), proj * view);

		glUseProgram(0);


		// Actualisation de la fenêtre

		SDL_GL_SwapWindow(m_fenetre);


		// Calcul du temps écoulé

		finBoucle = SDL_GetTicks();
		tempsEcoule = finBoucle - debutBoucle;


		// Si nécessaire, on met en pause le programme

		if(tempsEcoule < frameRate)
			SDL_Delay(frameRate - tempsEcoule);
	}
}

