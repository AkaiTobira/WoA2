#ifndef TILEMANAGER_H
#define TILEMANAGER_H

#include "ResourceManager.h"
//#include "State_Manager.h"
#include <string>

//1024 768

const int Sheet_Tile_Size = 32;
const int Sheet_Sheet_Width = 32*9;
const int Sheet_Sheet_Height = 32*9;

const int rows = Sheet_Sheet_Width/Sheet_Tile_Size;
const int colu = Sheet_Sheet_Height/Sheet_Tile_Size;

enum Sheet {Tile_Size = 32, Sheet_Width = 32*9, Sheet_Height = 32*9, Tile_Width = static_cast<int>(Sheet::Sheet_Width/Sheet::Tile_Size), Tile_Height = static_cast<int>(Sheet::Sheet_Height/Sheet::Tile_Size) };

using TileID = unsigned int;

class TileInfo
{
public:

    TileInfo(SharedContext* l_context, const std::string& l_texture = "", TileID l_id = 0)
        : m_context(l_context), m_id(l_id), m_deadly(false)
    {
        TextureManager* tmgr = l_context->m_textureManager;

        if (l_texture == "")
        {
            m_id = l_id;
            return;
        }

    //    std::cout << l_texture << std::endl;


    
        if( !tmgr->RequireResource(l_texture))
        {


            std::cout << " No texture :" << l_texture << std::endl;
            return;
        }
        m_texture = l_texture;
        m_sprite.setTexture(*tmgr->GetResource(m_texture));

        sf::IntRect tileBoundreies(m_id % (Sheet::Sheet_Width/Sheet::Tile_Size)*Sheet::Tile_Size % Sheet::Sheet_Height,m_id /(Sheet::Sheet_Height/Sheet::Tile_Size)*Sheet::Tile_Size,Sheet::Tile_Size,Sheet::Tile_Size);


     //   std::cout << m_id % (Sheet::Sheet_Width/Sheet::Tile_Size)*Sheet::Tile_Size % Sheet::Sheet_Height << m_id /(Sheet::Sheet_Height/Sheet::Tile_Size)*Sheet::Tile_Size << std::endl;
        //    m_coordinates = { m_id % (Sheet::Sheet_Width/Sheet::Tile_Size)*Sheet::Tile_Size % Sheet::Sheet_Height , m_id / (Sheet::Sheet_Height/Sheet::Tile_Size)*Sheet::Tile_Size };

        m_sprite.setTextureRect(tileBoundreies);
    }

    ~TileInfo()
    {
        if( m_texture == "")
        {
            return;
        }
        m_context->m_textureManager->ReleaseResource(m_texture);
    }

    sf::Sprite m_sprite;


    std::string m_name;
    sf::Vector2f m_friction;

    SharedContext* m_context;
    TileID m_id;
    std::string m_texture;
    bool m_deadly;
};

struct Tile
{
    TileInfo* m_properties;
    bool m_warp;

    // Is the tile a warp.
// Other flags unique to each tile.
};


#endif
