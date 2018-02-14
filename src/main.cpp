#include <memory.h>
#include "Game.h"


int main(int argc __attribute__((unused)), char * argv[] __attribute__((unused)))
{
    #ifdef DEBUGG_RUN
    std::cout << "MAIN :: START" << std::endl;
    #endif

    Game game;
    while(!game.GetWindow() -> IsDone() )
    {

        clock_t fpsMeansure1 = clock();
        game.Update();
        game.Render();
        game.LateUpdate();
        clock_t fpsMeansure2 = clock();

        while( 1.0f / static_cast<float>( static_cast<float>(fpsMeansure2-fpsMeansure1) /CLOCKS_PER_SEC ) < 1.0f/60.0f ){
            fpsMeansure2 = clock();
        }


    }

    #ifdef DEBUGG_RUN
    std::cout << "MAIN :: END " << std::endl;
    #endif

    return 0;
}
