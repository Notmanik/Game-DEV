#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <sstream>
#include <string>

// Use SFML namespace for convenience
using namespace sf;

// Helper to convert various types to string (for older C++ versions)
namespace
{
    template <typename T>
    std::string to_string(const T& value)
    {
        std::stringstream ss;
        ss << value;
        return ss.str();
    }
} // namespace

// Helper function to center the origin of a text object
void centerOrigin(Text& text)
{
    FloatRect bounds = text.getLocalBounds();
    text.setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);
}

class Game
{
   public:
    Game();
    void run();

   private:
    void processEvents();
    void update(Time elapsedTime);
    void render();
    void resetBall();
    void resetGame();
    void updateScore(bool leftPlayerScored);
    void checkWinCondition();
    void updateScoreDisplay();

   private:
    // Window and game objects
    RenderWindow mWindow;
    RectangleShape mLeftPaddle;
    RectangleShape mRightPaddle;
    CircleShape mBall;

    // UI Elements
    Font mFont;
    Text mLeftScoreText;
    Text mRightScoreText;
    RectangleShape mCenterLine;
    Text mGameOverText;
    Text mInstructionText;
    Text mWinText;

    // Game physics
    float paddleSpeed;
    Vector2f ballVelocity;
    float ballSpeed;

    // Input states
    bool moveLeftUp, moveLeftDown;
    bool moveRightUp, moveRightDown;

    // Game states
    bool ballWaiting;
    Clock waitClock;
    bool gameOver;
    bool showInstructions;

    // Scoring
    int leftScore;
    int rightScore;
    const int winningScore = 5;

    // Constants
    static const float PADDLE_WIDTH;
    static const float PADDLE_HEIGHT;
    static const float BALL_RADIUS;
    static const float WINDOW_WIDTH;
    static const float WINDOW_HEIGHT;
};

// Static constants definition
const float Game::PADDLE_WIDTH = 20.f;
const float Game::PADDLE_HEIGHT = 100.f;
const float Game::BALL_RADIUS = 10.f;
const float Game::WINDOW_WIDTH = 800.f;
const float Game::WINDOW_HEIGHT = 600.f;

Game::Game()
    : mWindow(VideoMode(static_cast<unsigned>(WINDOW_WIDTH), static_cast<unsigned>(WINDOW_HEIGHT)),
              "Enhanced Pong"),
      mLeftPaddle(),
      mRightPaddle(),
      mBall(BALL_RADIUS),
      paddleSpeed(400.f), // Adjusted for frame-rate independence
      ballSpeed(400.f),   // Adjusted for frame-rate independence
      ballVelocity(),
      moveLeftUp(false),
      moveLeftDown(false),
      moveRightUp(false),
      moveRightDown(false),
      ballWaiting(true), // Start in waiting state
      gameOver(false),
      showInstructions(true),
      leftScore(0),
      rightScore(0)
{
    mWindow.setFramerateLimit(60);
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    // Load font
    if (!mFont.loadFromFile("resources/font.ttf"))
    {
        std::cout << "Error: Could not load font from 'resources/font.ttf'." << std::endl;
        // As a fallback, you might want to load a system font if available
        // For example, on Linux: mFont.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf");
    }

    // Setup paddles
    mLeftPaddle.setSize(Vector2f(PADDLE_WIDTH, PADDLE_HEIGHT));
    mLeftPaddle.setPosition(50.f, (WINDOW_HEIGHT - PADDLE_HEIGHT) / 2.f);
    mLeftPaddle.setFillColor(Color::Cyan);
    mLeftPaddle.setOrigin(PADDLE_WIDTH / 2.f, PADDLE_HEIGHT / 2.f);

    mRightPaddle.setSize(Vector2f(PADDLE_WIDTH, PADDLE_HEIGHT));
    mRightPaddle.setPosition(WINDOW_WIDTH - 50.f, (WINDOW_HEIGHT - PADDLE_HEIGHT) / 2.f);
    mRightPaddle.setFillColor(Color::Magenta);
    mRightPaddle.setOrigin(PADDLE_WIDTH / 2.f, PADDLE_HEIGHT / 2.f);

    // Setup ball
    mBall.setOrigin(BALL_RADIUS, BALL_RADIUS);
    mBall.setFillColor(Color::White);

    // Setup UI text
    mLeftScoreText.setFont(mFont);
    mLeftScoreText.setCharacterSize(50);
    mLeftScoreText.setFillColor(Color(200, 200, 200, 128));
    mLeftScoreText.setPosition(WINDOW_WIDTH / 4.f, 50.f);

    mRightScoreText.setFont(mFont);
    mRightScoreText.setCharacterSize(50);
    mRightScoreText.setFillColor(Color(200, 200, 200, 128));
    mRightScoreText.setPosition(3.f * WINDOW_WIDTH / 4.f, 50.f);

    // Center line
    mCenterLine.setSize(Vector2f(2.f, WINDOW_HEIGHT));
    mCenterLine.setPosition(WINDOW_WIDTH / 2.f - 1.f, 0.f);
    mCenterLine.setFillColor(Color(100, 100, 100));

    // Game over text
    mGameOverText.setFont(mFont);
    mGameOverText.setCharacterSize(40);
    mGameOverText.setFillColor(Color::Red);
    mGameOverText.setString("Game Over!\nPress R to restart or ESC to quit");
    centerOrigin(mGameOverText);
    mGameOverText.setPosition(WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f);

    // Win text
    mWinText.setFont(mFont);
    mWinText.setCharacterSize(50);
    mWinText.setFillColor(Color::Green);
    centerOrigin(mWinText);
    mWinText.setPosition(WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 3.f);

    // Instruction text
    mInstructionText.setFont(mFont);
    mInstructionText.setCharacterSize(22);
    mInstructionText.setFillColor(Color::White);
    mInstructionText.setString("Welcome to Pong!\nLeft Player: W/S keys | Right Player: Up/Down arrows\nFirst to " +
                               to_string(winningScore) + " wins!\nPress SPACE to start");
    centerOrigin(mInstructionText);
    mInstructionText.setPosition(WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f);

    // Initialize score display and ball
    updateScoreDisplay();
    resetBall();
}


void Game::updateScoreDisplay()
{
    mLeftScoreText.setString(std::to_string(leftScore));
    mRightScoreText.setString(std::to_string(rightScore));
}

void Game::resetBall()
{
    // Reset ball position and velocity
    mBall.setPosition(WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f);

    // Randomize ball direction
    float angle;
    do
    {
        angle = (std::rand() % 360) * 2 * 3.14159f / 360.f;
    } while (std::abs(std::cos(angle)) < 0.7f); // Ensure the ball doesn't start too vertically

    ballVelocity.x = ballSpeed * std::cos(angle);
    ballVelocity.y = ballSpeed * std::sin(angle);

    // Add a brief waiting period after scoring
    ballWaiting = true;
    waitClock.restart();
}

void Game::resetGame()
{
    leftScore = 0;
    rightScore = 0;
    gameOver = false;
    showInstructions = true; // Show instructions on new game

    // Reset paddle positions
    mLeftPaddle.setPosition(50.f, WINDOW_HEIGHT / 2.f);
    mRightPaddle.setPosition(WINDOW_WIDTH - 50.f, WINDOW_HEIGHT / 2.f);

    // Update score display and reset ball
    updateScoreDisplay();
    resetBall();
}

void Game::updateScore(bool leftPlayerScored)
{
    if (leftPlayerScored)
    {
        leftScore++;
    }
    else
    {
        rightScore++;
    }

    updateScoreDisplay();
    checkWinCondition();
}

void Game::checkWinCondition()
{
    if (leftScore >= winningScore || rightScore >= winningScore)
    {
        gameOver = true;

        if (leftScore >= winningScore)
        {
            mWinText.setString("Left Player Wins!");
            mWinText.setFillColor(Color::Cyan);
        }
        else
        {
            mWinText.setString("Right Player Wins!");
            mWinText.setFillColor(Color::Magenta);
        }
        centerOrigin(mWinText);
    }
}

void Game::run()
{
    Clock clock;
    Time timeSinceLastUpdate = Time::Zero;
    const Time TimePerFrame = seconds(1.f / 60.f);

    while (mWindow.isOpen())
    {
        timeSinceLastUpdate += clock.restart();
        while (timeSinceLastUpdate > TimePerFrame)
        {
            timeSinceLastUpdate -= TimePerFrame;
            processEvents();
            if (!gameOver)
            {
                update(TimePerFrame);
            }
        }
        render();
    }
}

void Game::processEvents()
{
    Event event;
    while (mWindow.pollEvent(event))
    {
        if (event.type == Event::Closed || (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape))
        {
            mWindow.close();
        }

        if (event.type == Event::KeyPressed)
        {
            if (event.key.code == Keyboard::R)
            {
                resetGame();
            }
            if (event.key.code == Keyboard::Space && showInstructions)
            {
                showInstructions = false;
                ballWaiting = false; // Start the ball movement
                waitClock.restart();
            }
        }

        // Handle real-time input for paddle movement
        moveLeftUp = Keyboard::isKeyPressed(Keyboard::W);
        moveLeftDown = Keyboard::isKeyPressed(Keyboard::S);
        moveRightUp = Keyboard::isKeyPressed(Keyboard::Up);
        moveRightDown = Keyboard::isKeyPressed(Keyboard::Down);
    }
}

void Game::update(Time elapsedTime)
{
    if (showInstructions || gameOver)
        return;

    // Handle paddle movement
    float paddleMove = paddleSpeed * elapsedTime.asSeconds();
    if (moveLeftUp && mLeftPaddle.getPosition().y - PADDLE_HEIGHT / 2 > 0)
    {
        mLeftPaddle.move(0.f, -paddleMove);
    }
    if (moveLeftDown && mLeftPaddle.getPosition().y + PADDLE_HEIGHT / 2 < WINDOW_HEIGHT)
    {
        mLeftPaddle.move(0.f, paddleMove);
    }
    if (moveRightUp && mRightPaddle.getPosition().y - PADDLE_HEIGHT / 2 > 0)
    {
        mRightPaddle.move(0.f, -paddleMove);
    }
    if (moveRightDown && mRightPaddle.getPosition().y + PADDLE_HEIGHT / 2 < WINDOW_HEIGHT)
    {
        mRightPaddle.move(0.f, paddleMove);
    }

    // Update ball position if not waiting
    if (ballWaiting)
    {
        if (waitClock.getElapsedTime().asSeconds() > 1.0f)
        {
            ballWaiting = false;
        }
        return; // Don't move the ball while waiting
    }

    mBall.move(ballVelocity * elapsedTime.asSeconds());

    // Check for wall collisions
    Vector2f ballPos = mBall.getPosition();
    if (ballPos.y - BALL_RADIUS < 0)
    {
        ballVelocity.y = -ballVelocity.y;
        mBall.setPosition(ballPos.x, BALL_RADIUS);
    }
    else if (ballPos.y + BALL_RADIUS > WINDOW_HEIGHT)
    {
        ballVelocity.y = -ballVelocity.y;
        mBall.setPosition(ballPos.x, WINDOW_HEIGHT - BALL_RADIUS);
    }

    // Check for paddle collisions
    if (ballVelocity.x < 0 && mBall.getGlobalBounds().intersects(mLeftPaddle.getGlobalBounds()))
    {
        ballVelocity.x = -ballVelocity.x;
        mBall.setPosition(mLeftPaddle.getPosition().x + PADDLE_WIDTH / 2.f + BALL_RADIUS, ballPos.y);
    }
    if (ballVelocity.x > 0 && mBall.getGlobalBounds().intersects(mRightPaddle.getGlobalBounds()))
    {
        ballVelocity.x = -ballVelocity.x;
        mBall.setPosition(mRightPaddle.getPosition().x - PADDLE_WIDTH / 2.f - BALL_RADIUS, ballPos.y);
    }

    // Check for scoring
    if (ballPos.x + BALL_RADIUS < 0)
    {
        updateScore(false); // Right player scores
        resetBall();
    }
    else if (ballPos.x - BALL_RADIUS > WINDOW_WIDTH)
    {
        updateScore(true); // Left player scores
        resetBall();
    }
}

void Game::render()
{
    mWindow.clear(sf::Color::Black);

    // Draw center line
    mWindow.draw(mCenterLine);

    // Draw paddles and ball
    mWindow.draw(mLeftPaddle);
    mWindow.draw(mRightPaddle);
    mWindow.draw(mBall);

    // Draw scores
    mWindow.draw(mLeftScoreText);
    mWindow.draw(mRightScoreText);

    // Draw UI text based on game state
    if (showInstructions)
    {
        mWindow.draw(mInstructionText);
    }

    if (gameOver)
    {
        mWindow.draw(mWinText);
        mWindow.draw(mGameOverText);
    }

    mWindow.display();
}

int main()
{
    Game game;
    game.run();
    return 0;
}
