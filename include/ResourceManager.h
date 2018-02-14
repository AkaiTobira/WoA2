#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include "SharedElements.h"
#include <string>
#include <unordered_map>
#include "Utilities.h"
#include <iostream>
#include <map>


enum class Direction
{
    None, Up, Down, Left, Right, LeftUp, LeftDown, RightUp, RightDown, ENDMARK

};

class Anim_Base;

template< typename Derived, typename T>
class ResourceManager
{

private:
    std::map<std::string, std::pair<T*, unsigned int> > m_resources;
    std::map<std::string, std::string> m_paths;

public:
    ResourceManager(const std::string& l_pathFile)
    {
        LoadPaths(l_pathFile);
    }
    virtual ~ResourceManager()
    {
        PurgeResources();
    }

    T* GetResource(const std::string& l_id)
    {
        auto res = Find(l_id);
        return ( res ? res->first : nullptr );
    }

    std::string GetPath( const std::string& l_id)
    {
        auto path = m_paths.find(l_id);
        return (path != m_paths.end() ? path->second : "");
    }

    bool RequireResource( const std::string &l_id)
    {
        auto res = Find(l_id);
        if(res)
        {
            ++res->second;

        //    std::cout << res->second << std::endl;
            return true;
        }

        auto path = m_paths.find(l_id);
        if( path == m_paths.end())
        {
            return false;
        }
        T* resource = Load(path->second);
        if( !resource )
        {
            return false;
        }
        m_resources.emplace(l_id, std::make_pair(resource,1));
        return true;

    }

    bool ReleaseResource( const std::string& l_id)
    {
        auto res = Find(l_id);
        if ( !res  )
        {
            return false;
        }

      //  std::cout << " READY TO DELETE " << l_id << " " << res->second <<std::endl;
        --res->second;
        if( res->second <= 0 )
        {

       //     std::cout << "TRYING TO UNLOAD " << std::endl;
            Unload(l_id);
        }
     //   std::cout << "LOOKS FINE  " << std::endl;
        return true;
    }

    void PurgeResources()
    {
        while(m_resources.begin() != m_resources.end())
        {
            delete m_resources.begin()->second.first;
            m_resources.erase(m_resources.begin());
        }
    }

    T* Load(const std::string& l_path)
    {
        return static_cast<Derived*>(this)->Load(l_path);
    }

    std::pair<T*, unsigned int>* Find(const std::string l_id)
    {

    //    for( auto itr  = m_resources.begin(); itr != m_resources.end(); ++itr ){
    //        std::cout << itr->first << " " << &itr->second << std::endl;
    //    }


    //    std::cout << l_id << std::endl;
        auto itr = m_resources.find(l_id);
        return ( itr != m_resources.end() ? &itr->second : nullptr);
    }

    bool Unload(const std::string& l_id)
    {
     //   std::cout << " UNLOAD :: "  << l_id << " START" << std::endl;
        auto itr = m_resources.find(l_id);
        if ( itr == m_resources.end() )
        {


       //     std::cout << " UNLOAD :: "  << l_id << " FAIL" << std::endl;
            return false;
        }
      //  std::cout << " UNLOAD :: "  << l_id << " CONTINUE" << std::endl;
        delete itr->second.first;
        m_resources.erase(itr);

       // std::cout << " UNLOAD :: "  << l_id << " SUCUCEDE" << std::endl;

        return true;
    }

    void LoadPaths(const std::string& l_pathFile)
    {
        std::ifstream paths;
        paths.open(Utils::GetWorkingDirectory() + l_pathFile);
        if(paths.is_open())
        {
            std::string line;
            while(std::getline(paths,line))
            {
                std::stringstream keystream(line);
                std::string pathName;
                std::string path;
                keystream >> pathName;
                keystream >> path;
                m_paths.emplace(pathName,path);
            }
            paths.close();
            return;
        }
        std::cerr << "! Failed loading the path file: " << l_pathFile << std::endl;
    }


};

class TextureManager:
    public ResourceManager<TextureManager, sf::Texture>
{
public:
    TextureManager(std::string  l_path): ResourceManager(l_path) {}

    sf::Texture* Load( const std::string& l_path)
    {
        sf::Texture* texture = new sf::Texture();
        if( !texture->loadFromFile( Utils::GetWorkingDirectory() + l_path))
        {
            delete texture;
            texture = nullptr;
            std::cerr << "! Failed to load texture : " << l_path << std::endl;
        }
        return texture;
    }
};

using Animations = std::unordered_map<std::string, Anim_Base*>;

#include "FontManager.h"

class SpriteSheet
{
public:
    SpriteSheet(TextureManager* l_textMagr);
    ~SpriteSheet();

    void CropSprite( const sf::IntRect& l_rect);

    bool LoadSheet( const std::string&  l_file );
    void ReleaseSheet();

    Anim_Base* GetCurrentAnim();
    bool SetAnimation( const std::string&  l_name, const bool& l_play = false,const bool& l_loop = false );

    void Update( const float& l_dT);
    void Draw ( sf::RenderWindow* l_wind);

    void SetSpriteSize(const sf::Vector2i& l_size);
    void SetSpritePosition( const sf::Vector2f& l_pos);
    void SetDirection(const Direction& l_dir);

    sf::Vector2i GetSpriteSize();
    Direction GetDirection();

private:
    TextureManager* m_textureManager;
    Anim_Base* m_animationCurrent;

    std::string m_texture;
    sf::Sprite m_sprite;
    sf::Vector2i m_spriteSize;
    sf::Vector2f m_spriteScale;

    std::string m_animType;
    Animations m_animations;
    Direction m_direction;
};

using Frame = int;

class Anim_Base
{
    friend class SpriteSheet;

public:
    Anim_Base();
    virtual ~Anim_Base();

    void Play();
    void Pause();
    void Stop();
    void Reset();
    virtual void Update(const float& l_dT);

    friend std::stringstream& operator >> ( std::stringstream& l_stream, Anim_Base& a)
    {
        a.ReadIn(l_stream);
        return l_stream;
    }
    void SetLooping( const bool& l_loop);
    void SetSpriteSheet(SpriteSheet* l_sheet);
    void SetFrame(const Frame& l_frame);
    bool IsInAction();
    void SetName(const std::string& l_name);
    std::string& GetName();

protected:
    virtual void FrameStep() = 0;
    virtual void CropSprite() = 0;
    virtual void ReadIn( std::stringstream& l_stream) = 0;

    Frame m_frameCurrent;
    Frame m_frameStart;
    Frame m_frameEnd;
    Frame m_frameRow;


    float m_frameTime;
    float m_elapsedTime;
    int m_frameActionStart;
    int m_frameActionEnd;
    bool m_loop;
    bool m_playing;
    std::string m_name;
    SpriteSheet* m_spriteSheet;
};

class Anim_Directional : public Anim_Base
{
protected:
    void FrameStep();
    void CropSprite();
    void ReadIn(std::stringstream& l_stream);
};

#endif
