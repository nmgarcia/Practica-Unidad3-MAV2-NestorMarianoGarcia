#include "Game.h"
#include "Box2DHelper.h"
#include <iostream>

Game::Game(int ancho, int alto, std::string titulo)
{
	wnd = new RenderWindow(VideoMode(ancho, alto), titulo);
	wnd->setVisible(true);
	fps = 60;
	wnd->setFramerateLimit(fps);
	frameTime = 1.0f / fps;
	SetZoom();
	InitPhysics();
}

void Game::Loop()
{
	while(wnd->isOpen())
	{
		wnd->clear(clearColor);
		DoEvents();
		CheckCollitions();
		UpdatePhysics();
		DrawGame();
		wnd->display();
	}
}

void Game::UpdatePhysics()
{
	phyWorld->Step(frameTime, 8, 8);
	phyWorld->ClearForces();
	phyWorld->DebugDraw();
}

void Game::DrawGame()
{ 
	// Dibujamos el suelo
	sf::RectangleShape groundShape(sf::Vector2f(500, 5));
	groundShape.setFillColor(sf::Color::Red);
	groundShape.setPosition(0, 95);
	wnd->draw(groundShape);

	// Dibujamos el techo
	sf::RectangleShape topShape(sf::Vector2f(500, 5));
	topShape.setFillColor(sf::Color::Green);
	topShape.setPosition(0, 0);
	wnd->draw(topShape);

	// Dibujamos las paredes
	sf::RectangleShape leftWallShape(sf::Vector2f(5, alto)); // Alto de la ventana
	leftWallShape.setFillColor(sf::Color::Blue);
	leftWallShape.setPosition(0, 100); // X = 100 para que comience donde termina el suelo
	wnd->draw(leftWallShape);

	sf::RectangleShape rightWallShape(sf::Vector2f(5, alto)); // Alto de la ventana
	rightWallShape.setFillColor(sf::Color::Cyan);
	rightWallShape.setPosition(95, 100);
	wnd->draw(rightWallShape);
	
	headAvatar->Actualizar(); 
	headAvatar->Dibujar(*wnd);
	chestAvatar->Actualizar();
	chestAvatar->Dibujar(*wnd);
	leftArmAvatar->Actualizar();
	leftArmAvatar->Dibujar(*wnd);
	rightArmAvatar->Actualizar();
	rightArmAvatar->Dibujar(*wnd);
	leftLegAvatar->Actualizar();
	leftLegAvatar->Dibujar(*wnd);
	rightLegAvatar->Actualizar();
	rightLegAvatar->Dibujar(*wnd);
}

void Game::DoEvents()
{
	Event evt;
	
	while(wnd->pollEvent(evt))
	{
		switch(evt.type)
		{
			case Event::Closed:
				wnd->close();
				break;
		}
	}

	// Movemos el cuerpo para demostrar el funcionamiento del ragdoll
	headAvatar->SetAwake(true);
	chestAvatar->SetAwake(true);
	leftArmAvatar->SetAwake(true);
	rightArmAvatar->SetAwake(true);
	leftLegAvatar->SetAwake(true);
	rightLegAvatar->SetAwake(true);

	if (Keyboard::isKeyPressed(Keyboard::Up))
		chestBody->ApplyForceToCenter(b2Vec2(0.0f ,-5000.0f),true);
	
}

void Game::CheckCollitions()
{
	// Veremos mas adelante
}

// Definimos el area del mundo que veremos en nuestro juego
// Box2D tiene problemas para simular magnitudes muy grandes
void Game::SetZoom()
{
	View camara;
	// Posicion del view
	camara.setSize(100.0f, 100.0f); //Aqui podemos ver que no es la misma relacion de aspecto por lo que podriamos hacer 100*600/800 para obtener una relacion de aspecto igual a la del window
	camara.setCenter(50.0f, 50.0f);
	wnd->setView(camara); //asignamos la camara
}

void Game::InitPhysics()
{
	// Inicializamos el mundo con la gravedad por defecto
	phyWorld = new b2World(b2Vec2(0.0f, 9.8f));

	//MyContactListener* l= new MyContactListener();
	//phyWorld->SetContactListener(l);
	// Creamos el renderer de debug y le seteamos las banderas para que dibuje TODO
	debugRender = new SFMLRenderer(wnd);
	//debugRender->SetFlags(UINT_MAX);
	phyWorld->SetDebugDraw(debugRender);

	// Creamos un piso, techo y paredes
	b2Body* topBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 100, 10);
	topBody->SetTransform(b2Vec2(50.0f, 0.0f), 0.0f);

	b2Body* groundBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 100, 10);
	groundBody->SetTransform(b2Vec2(50.0f, 100.0f), 0.0f);
	groundBody->GetFixtureList()->SetFriction(0.2f);

	b2Body* leftWallBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 10, 100);
	leftWallBody->SetTransform(b2Vec2(0.0f, 50.0f), 0.0f);

	b2Body* rightWallBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 10, 100);
	rightWallBody->SetTransform(b2Vec2(100.0f, 50.0f), 0.0f);

		
	
	headBody = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 4.0f, 4.0f, 0.8f, 0.5f, 0.5f);
	headBody->SetTransform(b2Vec2(50, 20), 0.0f);

	chestBody = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 6.0f, 10.0f, 3.0f, 0.5f, 0.5f);
	chestBody->SetTransform(b2Vec2(50, 28), 0.0f);

	leftArmBody = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 2.0f, 8.0f, 0.2f, 0.5f, 0.1f);
	leftArmBody->SetTransform(b2Vec2(46, 27), 0.0f);

	rightArmBody = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 2.0f, 8.0f, 0.2f, 0.5f, 0.1f);
	rightArmBody->SetTransform(b2Vec2(54, 27), 0.0f);

	leftLegBody = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 2.0f, 10.0f, 0.4f, 0.5f, 0.1f);
	leftLegBody->SetTransform(b2Vec2(48, 39), 0.0f);

	rightLegBody = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 2.0f, 10.0f, 0.4f, 0.5f, 0.1f);
	rightLegBody->SetTransform(b2Vec2(52, 39), 0.0f);

	// Atamos las partes mediante resortes
	//NOTA: Al utilizar el getworldpoint las coordenadas pasan a ser desde el origen de la pieza
	Box2DHelper::CreateDistanceJoint(phyWorld, headBody, headBody->GetWorldPoint(b2Vec2(0, 2)),
		chestBody, chestBody->GetWorldPoint(b2Vec2(0, -5)), 0.25f, 2.0f, 1.0f);
	Box2DHelper::CreateDistanceJoint(phyWorld, leftArmBody, leftArmBody->GetWorldPoint(b2Vec2(1, -4)),
		chestBody, chestBody->GetWorldPoint(b2Vec2(-3.0f, -4.5)), 0.25f, 2.0f, 1.0f);
	Box2DHelper::CreateDistanceJoint(phyWorld, rightArmBody, rightArmBody->GetWorldPoint(b2Vec2(-1, -4)),
		chestBody, chestBody->GetWorldPoint(b2Vec2(3.0f, -4.5)), 0.25f, 2.0f, 1.0f);
	Box2DHelper::CreateDistanceJoint(phyWorld, leftLegBody, leftLegBody->GetWorldPoint(b2Vec2(0.0, -5)),
		chestBody, chestBody->GetWorldPoint(b2Vec2(-1, 5.0)), 0.25f, 10.0f, 1.0f);
	Box2DHelper::CreateDistanceJoint(phyWorld, rightLegBody, rightLegBody->GetWorldPoint(b2Vec2(0.0, -5)),
		chestBody, chestBody->GetWorldPoint(b2Vec2(1, 5.0)), 0.25f, 10.0f, 1.0f);

	// Carga la textura de la pelota para el avatar, decidi dejarla para ver como funcionaba con sprites

	texturaCaja.loadFromFile("Box.png");

	// Inicializa el avatar del jugador con el cuerpo físico creado y la textura de la pelota
	headAvatar = new Avatar(headBody, new sf::Sprite(texturaCaja));
	chestAvatar = new Avatar(chestBody, new sf::Sprite(texturaCaja));
	leftArmAvatar = new Avatar(leftArmBody, new sf::Sprite(texturaCaja));
	rightArmAvatar = new Avatar(rightArmBody, new sf::Sprite(texturaCaja));
	leftLegAvatar = new Avatar(leftLegBody, new sf::Sprite(texturaCaja));
	rightLegAvatar = new Avatar(rightLegBody, new sf::Sprite(texturaCaja));
}

Game::~Game(void)
{ }