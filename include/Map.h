#ifndef TILEMAP_H
#define TILEMAP_H


#include "TileManager.h"

#include <cstdlib>
#include <ctime>
#include <cmath>
#include <sstream>
#include <fstream>
#include <unordered_set>
#include <iostream>
#include "iomanip"
#include "Utilities.h"
#include "StateManager.h"

enum Terrains{
    HolyGrass = 0,
    Plag,
    Ice,
    Grass,
    Sand,
    Ground,
    Water,
    Rock,

    MAX
};

enum Squares{
    //Borders
    LU = - Sheet::Tile_Width -1,
     U = - Sheet::Tile_Width,
    RU = - Sheet::Tile_Width +1,
     L = -1,
     R =  1,
    LD =   Sheet::Tile_Width -1,
     D =   Sheet::Tile_Width,
    RD =   Sheet::Tile_Width +1
};


enum Colors{
    //Colors in BitMap
    PINK = 0,   // 255,125,255
	PURPLE,     // 125,0  ,255
    WHITE,      // 255,255,255
    GREEN,      // 0  ,255,0
    YELLOW,     // 255,255,0
    RED,        // 255,0  ,0
    BLUE,       // 0  ,0  ,255
    ORANGE,     // 255,125,0
    BLACK       // 0  ,0  ,0
};

namespace MapLoader{
using namespace std;
#pragma pack(2)

struct BITMAPFILEHEADER             // File header
{ 
  char bfType[2];                   // File type: should be BM ( 0x42 0x4D ) 
  int bfSize;                       // File size in bytes
  short bfReserved1;                // Reserved - for what i have no idea :P 
  short bfReserved2;                // -||-
  int bfOffBits;                    // Offset, adress of the beginning of the information about image (pixels )
};

struct BITMAPINFOHEADER             // Bitmap header
{
  unsigned int biSize;              // Size of this header
  unsigned int biWidth;             // Width of image ( in pixels)
  unsigned int biHeight;            // Height of this image ( in pixels )
  unsigned short biPlanes;          // Numer of color planes, always 1
  unsigned short biBitCount;        // Number of bytes for pixel. Possibility values :1,4,8,16, 24 and 32
  unsigned int biCompression;       // Used compression (0 -none)
  unsigned int biSizeImage;         // Size of image 
  signed int biXPelsPerMeter;       // Horizontal resolution of the image (pixel per meter)
  signed int biYPelsPerMeter;       // Vertical resolution of the image (pixel per meter)
  unsigned int biClrUsed;           // Number of colors in the color palette, or 0 to default to 2^n ( 0- no palette)
  unsigned int biClrImportant;      // Number of important colors used
};

#pragma pack(push, 1)

const int NUMBER_OF_CHAR_IN_PIXEL = 3;

#pragma pack(pop)


Colors convertForColor( unsigned int B, unsigned int G, unsigned int R); // Gigantic if for changing readed value in Colors::
unsigned int* bitMapSize( string filename ); // returns array with sizes of map
unsigned short int ** getPixelInfo( string filename ); // returns table filled by terrains info

}

using TileList = std::unordered_set< Tile* >;

using TileMap = std::unordered_map<TileID, Tile*>;
using TileMapBoardies = std::unordered_map< TileID, TileList >;
using TileSet = std::unordered_map<TileID, TileInfo*>;


class Map{
    public:
        Map(SharedContext* l_context, BaseState* l_currentState);
        ~Map();

        inline Tile* GetTile(const unsigned int &l_x,const unsigned int &l_y);
        TileList GetBorderTile(const unsigned int& l_x,const unsigned int& l_y);
        TileInfo* GetDefaultTile();
        const sf::Vector2f& GetTerrianFriction(unsigned int l_x, unsigned int l_y);
            
        float GetGravity() const;

        unsigned int getTileSize() const;
        const sf::Vector2u GetMapSize() const;
        const sf::Vector2f&  PlayerStart() const;
        void LoadNext();
        void Update(float l_dT);
        void Draw();
        void LoadBitMap( const std::string & l_path);

        void UnBlockTile( unsigned int i, unsigned int j);
        void BlockTile(unsigned int i, unsigned int j);
        bool IsWalkable(unsigned int i, unsigned int j);
 
    private:

        void InsertTileBorder( unsigned int l_x, unsigned int l_y, int l_value , Squares l_sq);
        inline int ConvertTileSquare( unsigned int l_terr, Squares l_sq );
        inline int ConvertTileId(int l_number);
        inline unsigned int ConvertCoords(const unsigned int& l_x,const unsigned int& l_y);
        void LoadTiles(const std::string& l_path);
        void PurgeMap();
        void PurgeTileSet();
        void CreateBorders( unsigned int l_x, unsigned int l_y);
        void MakeMapTexture();

        SharedContext* m_context;

        sf::Vector2u m_maxMapSize;
        sf::Vector2f m_playerStart;

        TileSet m_tileSet;
        TileMap m_tileMap;
        TileMapBoardies m_tileMapBorders;
        TileInfo m_defaultTile;

        sf::RenderTexture m_fullmap;
        sf::Sprite m_fullMapSprite;

        unsigned int m_tileCount;
        unsigned int m_tileCountBorders;
        unsigned int m_tileSetCount;
        unsigned int m_tileSetBorderCount;

        sf::Vector2u tileBegin;
        sf::Vector2u tileEnd;

        std::string m_nextMap;

        bool m_loadNextMap;

        BaseState* m_currentState;
        
        sf::Vector2f * m_terrainsFrictions;
        unsigned short int ** m_mapTab;
        bool** m_walkable;
};

#endif