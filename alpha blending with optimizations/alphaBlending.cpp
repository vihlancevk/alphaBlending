#include <iostream>
#include <stdio.h>
#include <SFML/Graphics.hpp>

const int SCREEN_WIDTH  = 800;
const int SCREEN_HEIGHT = 600;

const char* BACK_GROUND_FILE = "../images/Table.bmp";
const char* FORE_GROUND_FILE = "../images/Cat.bmp";

const int X_FORE_GROUND_IMAGE = 500;
const int Y_FORE_GROUND_IMAGE = 225;

void AlphaBlending( int X, int Y, sf::Image &backGroundImage, sf::Image foreGroundImage )
{
    int redBack   = 0;
    int greenBack = 0;
    int blueBack  = 0;
    int alphaBack = 0;

    int widthFront  = foreGroundImage.getSize().x;
    int heightFront = foreGroundImage.getSize().y;

    for ( int y = 0; y < heightFront; y++ )
    {
        for ( int x = 0; x < widthFront; x += 4 )
        {
            sf::Color colorBack[4]  = { backGroundImage.getPixel( X + x + 0, Y + y + 0 ),
                                        backGroundImage.getPixel( X + x + 1, Y + y + 1 ),
                                        backGroundImage.getPixel( X + x + 2, Y + y + 2 ),
                                        backGroundImage.getPixel( X + x + 3, Y + y + 3 ) };
            sf::Color colorFront[4] = { foreGroundImage.getPixel( x + 0, y ),
                                        foreGroundImage.getPixel( x + 1, y ),
                                        foreGroundImage.getPixel( x + 2, y ),
                                        foreGroundImage.getPixel( x + 3, y ), };
            for ( int i = 0; i < 4; i++ )
            {
                if ( colorFront[i].a >= 0 )
                {
                    redBack   = ( colorFront[i].r * colorFront[i].a + colorBack[i].r * ( 255 - colorFront[i].a ) ) >> 8;
                    greenBack = ( colorFront[i].g * colorFront[i].a + colorBack[i].g * ( 255 - colorFront[i].a ) ) >> 8;
                    blueBack  = ( colorFront[i].b * colorFront[i].a + colorBack[i].b * ( 255 - colorFront[i].a ) ) >> 8;
                    sf::Color color ( redBack, greenBack, blueBack );
                    backGroundImage.setPixel( X + x + i, Y + y + i, color );
                }
            }
       }
    }

    return;
}

int main()
{
    sf::String title = "alpha blending without optimizations";
    sf::RenderWindow window( sf::VideoMode( SCREEN_WIDTH, SCREEN_HEIGHT ), title );

    sf::Image backGroundImage;
    backGroundImage.loadFromFile( BACK_GROUND_FILE );

    sf::Image foreGroundImage;
    foreGroundImage.loadFromFile( FORE_GROUND_FILE );

    AlphaBlending( Y_FORE_GROUND_IMAGE, Y_FORE_GROUND_IMAGE, backGroundImage, foreGroundImage );

     sf::Texture backGroundTexture;
	backGroundTexture.loadFromImage( backGroundImage );
 
	sf::Sprite backGroundSprite;
	backGroundSprite.setTexture( backGroundTexture );
	backGroundSprite.setPosition( 0, 0 );

    while ( window.isOpen() )
    {
        sf::Event Event;
        while ( window.pollEvent( Event ) )
        {
            if ( Event.type == sf::Event::Closed )
                window.close();
        }
        window.clear();
        window.draw( backGroundSprite );
        window.display();
    }

    return 0;
}
 
