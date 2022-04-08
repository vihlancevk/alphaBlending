#include <iostream>
#include <cstring>
#include <chrono>
#include <SFML/Graphics.hpp>

const int SCREEN_WIDTH  = 800;
const int SCREEN_HEIGHT = 600;

const char* BACK_GROUND_FILE = "../images/Table.bmp";
const char* FORE_GROUND_FILE = "../images/Cat.bmp";

sf::Uint8 * AlphaBlending( sf::Image backGroundImage, sf::Image foreGroundImage, int xForeGroundImage, int yForeGroundImage )
{
    int widthBack  = backGroundImage.getSize().x;
    int heightBack = backGroundImage.getSize().y;

    int widthFore  = foreGroundImage.getSize().x;
    int heightFore = foreGroundImage.getSize().y;

    sf::Uint8 const *pixelsBack = new sf::Uint8[widthBack*heightBack*4];
    pixelsBack = backGroundImage.getPixelsPtr();

    sf::Uint8 *pixelsBackUpdate = new sf::Uint8[widthBack*heightBack*4];
    std::memcpy( pixelsBackUpdate, pixelsBack, widthBack*heightBack*4 );
    
    sf::Uint8 const *pixelsFore = new sf::Uint8[widthFore*heightFore*4];
    pixelsFore = foreGroundImage.getPixelsPtr();

    for ( int n = 0; n < 1000; n ++)
    {
        for ( int y = 0; y < heightFore; y ++ )
        {
            for ( int x = 0 ; x < widthFore; x++ )
            {
                int positionPixelsBack = ( y + yForeGroundImage ) * widthBack * 4 + ( x + xForeGroundImage ) * 4;
                int positionPixelsFore = y * widthFore * 4 + x * 4;

                for (int i = 0; i < 3; i++)
                {
                    pixelsBackUpdate[positionPixelsBack + i] = ( pixelsFore[positionPixelsFore + i] * pixelsFore[positionPixelsFore + 3]
                                                               + pixelsBack[positionPixelsBack + i] * ( 255 - pixelsFore[positionPixelsFore + 3] ) ) >> 8;
                }
            }
        }
    }

    return pixelsBackUpdate;
}

int main()
{
    sf::String title = "alpha blending without optimizations";
    sf::RenderWindow window( sf::VideoMode( SCREEN_WIDTH, SCREEN_HEIGHT ), title );

    sf::Image backGroundImage;
    backGroundImage.loadFromFile( BACK_GROUND_FILE );

    sf::Image foreGroundImage;
    foreGroundImage.loadFromFile( FORE_GROUND_FILE );

    sf::Texture backGroundTexture;
	backGroundTexture.loadFromImage( backGroundImage );
 
	sf::Sprite backGroundSprite;
	backGroundSprite.setTexture( backGroundTexture );
	backGroundSprite.setPosition( 0, 0 );

    while ( window.isOpen() )
    {
        auto start = std::chrono::steady_clock::now();

        sf::Event Event;
        while ( window.pollEvent( Event ) )
        {
            if ( Event.type == sf::Event::Closed )
                window.close();
        }

        const int xForeGroundImage = 200;
        const int yForeGroundImage = 200;
        sf::Uint8 *pixelsBackUpdate = AlphaBlending( backGroundImage, foreGroundImage, xForeGroundImage, yForeGroundImage );
        backGroundTexture.update( pixelsBackUpdate );

        window.clear();
        window.draw( backGroundSprite );
        window.display();

        auto end     = std::chrono::steady_clock::now();

        auto elapsed = std::chrono::duration_cast< std::chrono::milliseconds >( end - start );
        int fps = 1000 / (int)(elapsed.count());
        window.setTitle( "alpha blending without optimizations ( fps - " + std::to_string( fps ) + " )" );
    }

    return 0;
}
