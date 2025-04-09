#include <SFML/Graphics.hpp>

class Game{
    public:
        Game();
        void run();
    private:
        void processEvents();
        void update();
        void render();
    private:
        sf::RenderWindow mWindow;
        sf::CircleShape mPlayer;
};
// here all the functions defeinition as Game::run(){} 
Game::Game()
: mWindow(sf::VideoMode(640, 480), "SFML Application") // Create the main window
, mPlayer() // Default-constructed player
{
    mWindow.setFramerateLimit(60);
    mPlayer.setRadius(40.f);  // Set player size
    mPlayer.setPosition(100.f, 100.f);  // Set player position
    mPlayer.setFillColor(sf::Color::Cyan);  // Set player color
}
void Game::run(){
    while(mWindow.isOpen()){
        processEvents();
        update();
        render();
        
    }
}
void Game::processEvents()
{
sf::Event event;
while (mWindow.pollEvent(event))
{
if (event.type == sf::Event::Closed)
mWindow.close();
}
}
void Game::update(){

}
void Game::render(){
    mWindow.clear();
    mWindow.draw(mPlayer);
    mWindow.display();
}
int main(){
    Game game;
    game.run();
}