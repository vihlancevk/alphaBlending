#include <iostream>
#include <cstring>
#include <SFML/Graphics.hpp>
#include <immintrin.h>

const int SCREEN_WIDTH  = 800;
const int SCREEN_HEIGHT = 600;

const char* BACK_GROUND_FILE = "../images/Table.bmp";
const char* FORE_GROUND_FILE = "../images/Cat.bmp";

sf::Uint8 * AlphaBlending( sf::Image &backGroundImage, sf::Image &foreGroundImage )
{
    const char I = 255u,
               Z = 0x80u;
           
    const __m128i   _0 =                    _mm_set_epi8 ( 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0 );
    const __m128i _255 = _mm_cvtepu8_epi16( _mm_set_epi8 ( I,I,I,I, I,I,I,I, I,I,I,I, I,I,I,I ) );

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

    for ( int y = 0; y < heightFore; y ++ )
    {
        for ( int x = 0 ; x < widthFore; x += 4 )
        {


            __m128i bk = _mm_lddqu_si128( (__m128i*)&(pixelsBack[y * widthBack * 4 + x * 4]) );
            __m128i fr = _mm_lddqu_si128( (__m128i*)&(pixelsFore[y * widthFore * 4 + x * 4]) );

            __m128i BK = (__m128i)_mm_movehl_ps( (__m128)_0, (__m128)bk );
            __m128i FR = (__m128i)_mm_movehl_ps( (__m128)_0, (__m128)fr );

            bk = _mm_cvtepu8_epi16( bk );
            BK = _mm_cvtepu8_epi16( BK );

            fr = _mm_cvtepu8_epi16( fr );
            FR = _mm_cvtepu8_epi16( FR );

            static const __m128i moveA = _mm_set_epi8( Z, 14, Z, 14, Z, 14, Z, 14,
                                                       Z, 6 , Z, 6 , Z, 6 , Z, 6  );
            __m128i a = _mm_shuffle_epi8( fr, moveA );
            __m128i A = _mm_shuffle_epi8( FR, moveA );

            bk = _mm_mullo_epi16( bk, _mm_sub_epi8( _255, a ) );
            BK = _mm_mullo_epi16( BK, _mm_sub_epi8( _255, A ) );

            fr = _mm_mullo_epi16( fr, a );
            FR = _mm_mullo_epi16( FR, A );

            __m128i sum = _mm_add_epi16( fr, bk );
            __m128i SUM = _mm_add_epi16( FR, BK );

            static const __m128i moveSum = _mm_set_epi8( Z , Z , Z , Z, Z, Z, Z, Z, 
                                                         15, 13, 11, 9, 7, 5, 3, 1 );
            sum = _mm_shuffle_epi8( sum, moveSum );
            SUM = _mm_shuffle_epi8( SUM, moveSum );

            __m128i color = (__m128i)_mm_movelh_ps( (__m128)sum, (__m128)SUM );

           _mm_storeu_si128( (__m128i*)(__m128i*)&(pixelsBackUpdate[y * widthBack * 4 + x * 4]), color );
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
    sf::Uint8 *pixelsBackUpdate = AlphaBlending( backGroundImage, foreGroundImage );
    backGroundTexture.update( pixelsBackUpdate );
 
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
