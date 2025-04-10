#include <SFML/Graphics.hpp>

class Game
{
   public:
    Game();
    void run();

   private:
    void processEvents();
    void update(sf::Time time);
    void render();
    void handlePlayerInput(sf::Keyboard::Key key, bool ispressed);

   private:
    sf::RenderWindow mWindow;
    sf::CircleShape mPlayer;
    bool mIsMovingUp, mIsMovingDown, mIsMovingRight, mIsMovingLeft;
};
// here all the functions defeinition as Game::run(){}
Game::Game()
    : mWindow(sf::VideoMode(640, 480), "SFML Application")  // Create the main window
      ,
      mPlayer()  // Default-constructed player
{
    // mWindow.setFramerateLimit(60);          // sets the frames cap to 60 so that it wont run without bounds
    mPlayer.setRadius(40.f);                // Set player size
    mPlayer.setPosition(100.f, 100.f);      // Set player position
    mPlayer.setFillColor(sf::Color::Cyan);  // Set player color
}

void Game::run()
{
    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;
    sf::Time TimePerFrame = sf::seconds(1.f/60.f);
    while (mWindow.isOpen())
    {

        timeSinceLastUpdate += clock.restart();
        while (timeSinceLastUpdate > TimePerFrame)
        {
            timeSinceLastUpdate -= TimePerFrame;
            processEvents();
            update(TimePerFrame);
        }
        
        render();
    }
}
void Game::handlePlayerInput(sf::Keyboard::Key key, bool ispressed)
{
    if (key == sf::Keyboard::W)
    {
        mIsMovingUp = ispressed;
    }
    if (key == sf::Keyboard::A)
    {
        mIsMovingLeft = ispressed;
    }
    if (key == sf::Keyboard::S)
    {
        mIsMovingDown = ispressed;
    }
    if (key == sf::Keyboard::D)
    {
        mIsMovingRight = ispressed;
    }
}
void Game::processEvents()
{
    sf::Event event;
    while (mWindow.pollEvent(event))
    {
        switch (event.type)
        {
            case sf::Event::KeyPressed:
                handlePlayerInput(event.key.code, true);
                break;
            case sf::Event::KeyReleased:
                handlePlayerInput(event.key.code, false);
                break;
            case sf::Event::Closed:
                mWindow.close();
                break;
        }
    }
}
void Game::update(sf::Time time)
{
    sf::Vector2f movement(0.f, 0.f);
    if (mIsMovingUp)
        movement.y -= 1.f;
    if (mIsMovingDown)
        movement.y += 1.f;
    if (mIsMovingLeft)
        movement.x -= 1.f;
    if (mIsMovingRight)
        movement.x += 1.f;
    mPlayer.move(movement * time.asSeconds());
}
void Game::render()
{
    mWindow.clear();
    mWindow.draw(mPlayer);
    mWindow.display();
}
int main()
{
    Game game;
    game.run();
}