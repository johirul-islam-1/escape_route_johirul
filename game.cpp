#include "Game.h"

//Private functions
void Game::initVar()
{
	this->window = nullptr;
	this->playerCar.loadFromFile("CarW-E(G).png");
	this->plr.setTexture(playerCar);
	this->plr.setScale(0.03, 0.03);

	this->dot.loadFromFile("Position.png");
	this->mark.setTexture(this->dot);
	this->mark.setScale(0.025, 0.025);
	/*this->plr.setPosition(100, 100);*/

	//Johirul's code
	//for minimap
	this->minimap_domain_expantion = false;
	this->minimap_border.setFillColor(Color(0, 0, 0, 100));
	this->minimap_border.setOutlineColor(Color::White);
	this->minimap_border.setOutlineThickness(2.f);

	this->left_most_x = numeric_limits<float>::max();
	this->left_most_y = numeric_limits<float>::max();
	this->right_most_x = numeric_limits<float>::min();
	this->right_most_y = numeric_limits<float>::min();

	
}



void Game::initWin()
{
	this->desktopMode = VideoMode::getDesktopMode();
	this->window = new RenderWindow(this->desktopMode, "Escape Route", Style::Close | Style::Titlebar);
	this->window->setFramerateLimit(60);// Johirul's code
	//Code for fetching Coordinate data from a text file
	ifstream file("data.txt");
	int x, y;
	while (file >> x >> y) {
		points.emplace_back(x, y);

		if (x < this->left_most_x) left_most_x = x;
		if (x > this->right_most_x) right_most_x = x;

		if (y < this->left_most_y) left_most_y = y;
		if (y > this->right_most_y) right_most_y = y;


	}
	file.close();
	/*for (auto& p : points) {
		CircleShape x(5.f);
		x.setFillColor(Color::Black);
		x.setPosition(p);
		this->window->draw(x);
	}*/

	this->plr.setPosition(points[0]);//edited by Johirul
	// Johirul's code


	const float roadPointReadious = 30.f;
	const float minimap_roadPointRadious = 30.f;

	for (const auto& p : points)
	{
		CircleShape roadCircle;
		roadCircle.setRadius(roadPointReadious);
		roadCircle.setPosition(p);
		roadCircle.setOrigin(roadPointReadious, roadPointReadious);
		roadCircle.setFillColor(sf::Color(60,60,60));

		CircleShape minimap_roadCircle;
		minimap_roadCircle.setRadius(minimap_roadPointRadious);
		minimap_roadCircle.setPosition(p);
		minimap_roadCircle.setOrigin(minimap_roadPointRadious, minimap_roadPointRadious);
		minimap_roadCircle.setFillColor(sf::Color::Black);


		this->road.push_back(roadCircle);
		this->minimap_road.push_back(minimap_roadCircle);

	}

	this->minimap_view.setSize(this->desktopMode.width, this->desktopMode.height);
	this->minimap_view.zoom(0.5f);

}

//Constructors & Destructors
Game::Game() {
	this->initVar();
	this->initWin();
}

Game::~Game() {
	delete this->window;
	
}

const bool Game::getWinOpen() const
{
	return this->window->isOpen();
}

void Game::pollEvents()
{
	while (this->window->pollEvent(this->ev)) {
		switch (ev.type)
		{
		case Event::Closed:
			this->window->close();
			break;

		case Event::KeyPressed:
			if (this->ev.key.code == Keyboard::Escape)
				this->window->close();
			break;

		//Johirul's code
		case Event::MouseButtonPressed:
			minimap_zoom();
			break;
		}
		

		

	}
}


void Game::update()
{
	this->pollEvents();
	const double pi = 3.14159265358979323846;
	/*ang = this->plr.getRotation();
	dx = speed * cosf(ang * pi / 180.f);
	dy = speed * sinf(ang * pi / 180.f);
	plr.move(dx, dy);*/
	if (Keyboard::isKeyPressed(Keyboard::Down)) {
		(this->speed > 0) ? this->speed -= 0.5 : this->speed = 0;
	}
	if (Keyboard::isKeyPressed(Keyboard::Up)) {
		if (speed < 3)
		this->speed += 0.2;
	}
	if (Keyboard::isKeyPressed(Keyboard::Right)) {
		plr.rotate(9);
	}
	if (Keyboard::isKeyPressed(Keyboard::Left)) {
		plr.rotate(-9);
	}

	// Johirul's edit
	Making_circle_shaped_boundary_for_car_that_it_wont_go_out_of_road();


	view.setCenter(plr.getPosition());//Setting the view around the moving Car
	view.setSize(160, 90);
	view.zoom(2.f);

	if (!minimap_domain_expantion) {
		this->minimap_view.setCenter(plr.getPosition());
	}

	//A red dot marking the position of the Player in the MiniMap
	mark.setPosition(plr.getPosition().x + (plr.getGlobalBounds().width / 2) * sinf(plr.getRotation() * \
		pi / 180.f), plr.getPosition().y - (plr.getGlobalBounds().height / 2) * cosf(plr.getRotation() * pi / 180.f));

	float mark_x_pos = plr.getPosition().x;

	/*mark.setPosition(plr.getPosition().x , plr.getPosition().y ); 

	mark.setRotation(plr.getRotation());*/

	

}

void Game::render()
{
	this->window->clear(Color::Green);
	/*this->window->setView(view);*/


	// Draw road points (black circles)
	/*CircleShape dot(20.f);
	dot.setFillColor(Color::Black);
	for (auto& p : points) {
		dot.setPosition(p);
		this->window->draw(dot);
	}

	this->window->draw(plr); */  // Car
	

	//Johirul's edit
	//if click the minimap then the screen gets half.
	// left side will contunue the game and right side will show the expanded minimap
	if (this->minimap_domain_expantion) {
		this->view.setViewport(FloatRect(0.f, 0.f, 0.5f, 1.f));
		this->window->setView(view);

		for (const auto& circle : this->road) {
			this->window->draw(circle);
		}

		this->window->draw(plr);
		/*this->window->draw(mark);  */// Red pointer

		/*this->window->display();*/

		//expended minimap

		this->minimap_view.setViewport(FloatRect(0.5f, 0.f, 0.5f, 1.f));

		this->window->setView(minimap_view);

		for (const auto& circle : this->minimap_road) {
			this->window->draw(circle);
		}

		/*this->window->draw(plr);*/
		this->window->draw(mark);  // Red pointer

		/*this->window->display();*/

		//expended minimap border

		this->minimap_border.setPosition(this->window->getSize().x / 2.f, 0.f);
		this->minimap_border.setSize(Vector2f(this->window->getSize().x / 2.f, this->window->getSize().y));

		//this->window->draw(this->minimap_border);
	}
	else {
		this->view.setViewport(FloatRect(0.f, 0.f, 1.f, 1.f));
		this->window->setView(this->view);

		for (const auto& circle : this->road) {
			this->window->draw(circle);
		}

		this->window->draw(plr);
		/*this->window->draw(mark);*/  // Red pointer

		/*this->window->display();*/

		this->minimap_view.setViewport(FloatRect(0.75f, 0.f, 0.25f, 0.25f));
		
		this->window->setView(this->minimap_view);

		for (const auto& circle : this->minimap_road) {
			this->window->draw(circle);
		}

		/*this->window->draw(plr);*/
		this->window->draw(mark);  // Red pointer

		/*this->window->display();*/

		this->minimap_border.setPosition(this->window->getSize().x * 0.75f, 0.f);
		this->minimap_border.setSize(Vector2f(this->window->getSize().x * 0.25, this->window->getSize().y * 0.25f));

	/*	this->window->display();*/
	}

	this->window->setView(this->window->getDefaultView());
	this->window->draw(this->minimap_border);
	this->window->display();
}

