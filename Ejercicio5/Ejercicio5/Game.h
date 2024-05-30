#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "SFMLRenderer.h"
#include "Avatar.h"

using namespace sf;
class Game
{
private:
	// Propiedades de la ventana
	int alto;
	int ancho;
	RenderWindow* wnd;
	Color clearColor;

	// Objetos de box2d
	b2World *phyWorld;
	SFMLRenderer *debugRender;

	//tiempo de frame
	float frameTime;
	int fps;

	// Cuerpo de box2d	
	b2Body* headBody;	
	Avatar* headAvatar;
	
	b2Body* chestBody;
	Avatar* chestAvatar;

	b2Body* rightArmBody;
	Avatar* rightArmAvatar;

	b2Body* leftArmBody;
	Avatar* leftArmAvatar;

	b2Body* rightLegBody;
	Avatar* rightLegAvatar;

	b2Body* leftLegBody;
	Avatar* leftLegAvatar;
	
	sf::Texture texturaCaja;

public:

	// Constructores, destructores e inicializadores
	Game(int ancho, int alto,std::string titulo);
	void CheckCollitions();
	~Game(void);
	void InitPhysics();

	// Main game loop
	void Loop();
	void DrawGame();
	void UpdatePhysics();
	void DoEvents();
	void SetZoom();
};

