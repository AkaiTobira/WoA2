#include "Map.h"



Map::Map(SharedContext* l_context, BaseState* l_curentState):m_context(l_context), m_maxMapSize(32,32), m_defaultTile(l_context), m_tileCount(0), m_tileSetCount(0), m_loadNextMap(false),m_currentState(l_curentState){
    
    std::string path = "res/tiles/tileMap.cfg";
    
    #ifdef DEBUGG_RUN
        std::cout << "Map :: START :: " << path << std:: endl;
    #endif
    
    m_context->m_gameMap = this;

    LoadTiles(Utils::GetWorkingDirectory() + path );

    srand( time( NULL ));

    m_terrainsFrictions = new sf::Vector2f[Terrains::MAX];

    m_terrainsFrictions[Terrains::Grass    ] = {         0.80,         0.80 };
    m_terrainsFrictions[Terrains::HolyGrass] = {         0.80,         0.80 };
    m_terrainsFrictions[Terrains::Rock     ] = {         0.90,         0.90 };
    m_terrainsFrictions[Terrains::Plag     ] = {         0.80,         0.80 };
    m_terrainsFrictions[Terrains::Water    ] = {         0.01,         0.01 };
    m_terrainsFrictions[Terrains::Ground   ] = {         0.60,         0.60 };
    m_terrainsFrictions[Terrains::Sand     ] = {         0.40,         0.40 };    
    m_terrainsFrictions[Terrains::Ice      ] = {         0.30,         0.30 };

    #ifdef DEBUGG_RUN
        std::cout << "Map :: LOADED_KINDS_TILE :: " << m_tileSetCount << std:: endl;
    #endif

}

Map::~Map(){

    #ifdef DEBUGG_RUN
         std::cout << "Map :: END" << std:: endl;
    #endif

    PurgeMap();
    PurgeTileSet();
    m_context->m_gameMap = nullptr;


    for ( unsigned int i = 0 ; i < m_maxMapSize.y; i++){
        delete[] m_mapTab[i];
        delete[] m_walkable[i];
    }

    delete[] m_walkable;
    delete[] m_mapTab;
    delete[] m_terrainsFrictions;
}

unsigned int Map::getTileSize() const{
    return 32;
}

TileList Map::GetBorderTile( const unsigned int &l_x,const unsigned int &l_y){
    auto itr = m_tileMapBorders.find(ConvertCoords(l_x,l_y));
    return ( itr != m_tileMapBorders.end() ? itr->second : TileList() );
}

inline Tile* Map::GetTile(const unsigned int &l_x,const unsigned int &l_y){
    auto itr = m_tileMap.find(ConvertCoords(l_x,l_y));
    return ( itr != m_tileMap.end() ? itr->second : nullptr);
}

const sf::Vector2u Map::GetMapSize() const{
    return m_maxMapSize;
}

const sf::Vector2f& Map::GetTerrianFriction(unsigned int l_x, unsigned int l_y){
    return m_terrainsFrictions[m_mapTab[l_x][l_y]];
}

inline unsigned int Map::ConvertCoords(const unsigned int &l_x,const unsigned int &l_y){
    return ( l_x * m_maxMapSize.x) + l_y;
}

void Map::Update(float l_dT __attribute__((unused))){
    if( m_loadNextMap ){
        PurgeMap();
        m_loadNextMap = false;
        if( m_nextMap!= ""){
            LoadBitMap( "data/maps/" + m_nextMap);
        }else{
            m_currentState->GetStateManager()->SwitchTo(StateType::GameOver);
        }
        m_nextMap = "";
    }

   // std::cout << GetTerrianFriction( 6, 12).x << " " << GetTerrianFriction( 6, 12).y << std::endl;

}

inline int Map::ConvertTileSquare( unsigned int l_terr, Squares l_sq ){

       // const int tileWidth = 9;
        int t = ConvertTileId( l_terr );

        if( t == -1 ) return -1;
        return t + l_sq;
}


inline int Map::ConvertTileId(int l_number){
      //  const int tileWidth = 9;
      //  std::cout << l_number << std::endl;
        switch( l_number) {
            case Terrains::Grass :
                return (( Terrains::Grass * 3 + 1 ) * Sheet::Tile_Width +  1 + (std::rand()%3) * 3 ) ;
            break;

            case Terrains::Ground:
                return (( Terrains::Ground * 3 + 1 ) * Sheet::Tile_Width +  1 + (std::rand()%1) * 3 ) ;
            break;

            case Terrains::HolyGrass:
                return (( Terrains::HolyGrass * 3 + 1 ) * Sheet::Tile_Width +  1 + (std::rand()%1) * 3 ) ;
            break;

            case Terrains::Plag:
                return (( Terrains::Plag * 3 + 1 ) * Sheet::Tile_Width +  1 + (std::rand()%1) * 3 ) ;
            break;

            case Terrains::Sand:
                return (( Terrains::Sand * 3 + 1 ) * Sheet::Tile_Width +  1 + (std::rand()%1) * 3 ) ;
            break;

            case Terrains::Rock:
                return (( Terrains::Rock * 3 + 1 ) * Sheet::Tile_Width +  1 + (std::rand()%1) * 3 ) ;
            break;

            case Terrains::Water :
                return(( Terrains::Water * 3 + 1 ) * Sheet::Tile_Width + 1 + ( std::rand()%1) * 3 ) ;
            break;

            case Terrains::Ice:
                return(( Terrains::Ice * 3 + 1 ) * Sheet::Tile_Width + 1 + ( std::rand()%1) * 3 ) ;
            break;

            default:
                return -1;
            break;
        }
}


void Map::InsertTileBorder( unsigned int l_x, unsigned int l_y, int l_value , Squares l_sq){
    int TileId = -1;
    if( m_mapTab[l_x][l_y] > l_value ){
        TileId =  ConvertTileSquare( l_value, l_sq );
        if (TileId >= 0){
            int mapTileId =  ConvertCoords(l_x,l_y);
            auto itr = m_tileSet.find(TileId);

            if (itr != m_tileSet.end()){
                auto itr2 = m_tileMapBorders.find( mapTileId );
                
                if( itr2 == m_tileMapBorders.end()){
                    m_tileSetBorderCount++;
                    m_tileMapBorders.emplace( mapTileId, TileList() );
                }

                itr2 = m_tileMapBorders.find( mapTileId );
                Tile* tile = new Tile();
                tile->m_properties = itr->second;
                itr2->second.insert( tile );
                TileList list = itr2->second;

            }else{
                std::cout << "TILE NOT FOUND " << TileId << std::endl;
            }
        }
    }
}

void Map::CreateBorders( unsigned int l_x, unsigned int l_y){
    auto value = m_mapTab[l_x][l_y];
 //   int TileId = -1;

    if ( l_y > 0 ){
        if( l_x > 0 and l_x < m_maxMapSize.x - 1 ){
            InsertTileBorder( l_x-1, l_y-1, value, Squares::LU);
            InsertTileBorder( l_x+1, l_y-1, value, Squares::RU );
            InsertTileBorder( l_x  , l_y-1, value, Squares::U );
        }else if( l_x == 0 ){
            InsertTileBorder( l_x+1, l_y-1, value, Squares::RU );
            InsertTileBorder( l_x  , l_y-1, value, Squares::U );
        }else if( l_x == m_maxMapSize.x - 1){
            InsertTileBorder( l_x-1, l_y-1, value, Squares::LU);
            InsertTileBorder( l_x  , l_y-1, value, Squares::U );
        }
    }

    if ( l_y <  m_maxMapSize.y - 1){
       if( l_x > 0 and l_x < m_maxMapSize.x - 1 ){
            InsertTileBorder( l_x-1, l_y+1, value, Squares::LD);
            InsertTileBorder( l_x+1, l_y+1, value, Squares::RD );
            InsertTileBorder( l_x  , l_y+1, value, Squares::D );
        }else if( l_x == 0 ){
            InsertTileBorder( l_x+1, l_y+1, value, Squares::RD );
            InsertTileBorder( l_x  , l_y+1, value, Squares::D );
        }else if( l_x == m_maxMapSize.x - 1){
            InsertTileBorder( l_x-1, l_y+1, value, Squares::LD);
            InsertTileBorder( l_x  , l_y+1, value, Squares::D );
        }
    }

    if( l_x > 0 and l_x < m_maxMapSize.x - 1 ){
        InsertTileBorder( l_x-1  , l_y, value, Squares::L );
        InsertTileBorder( l_x+1  , l_y, value, Squares::R );
    }else if( l_x == 0 ){
        InsertTileBorder( l_x+1  , l_y, value, Squares::R );
    }else if ( l_x == m_maxMapSize.x - 1 ){
        InsertTileBorder( l_x-1  , l_y, value, Squares::L );
    }
}

void Map::MakeMapTexture( ){

    m_fullmap.clear( sf::Color::Black );

    for(unsigned  int x = 0; x < m_maxMapSize.x; x++){
        for(unsigned  int  y = 0; y < m_maxMapSize.y; y++){


            Tile* tile = GetTile(x,y);

            if( !tile ) { std::cout << "Tile not exist" << std::endl; continue; }

            sf::Sprite& sprite = tile->m_properties->m_sprite;
            sprite.setPosition(x * Sheet::Tile_Size, y*Sheet::Tile_Size);
            m_fullmap.draw(sprite);

        }
    }

    for( unsigned int x = 0 ; x < m_maxMapSize.x ; x++){
        for(unsigned  int y = 0 ; y < m_maxMapSize.y; y++){


            TileList TileSett = GetBorderTile(x,y);

            if( TileSett.empty() ){ continue; }

            for( auto itr = TileSett.begin(); itr != TileSett.end(); ++itr){

                Tile* tile = *itr;
                sf::Sprite& sprite = tile->m_properties->m_sprite;
                sprite.setPosition(x * Sheet::Tile_Size, y*Sheet::Tile_Size);
                m_fullmap.draw(sprite);
                m_tileCountBorders++;
            }
        }
    }


    m_fullmap.display();
    m_fullMapSprite.setTexture(m_fullmap.getTexture());
}

/*

    Ice / Grass / Plag / Rock / Water / Ground / HolyGrass / Sand


    Plag / HolyGrass

    Grass / Ice

    Sand

    Ground

    Water

    Rock

*/


void Map::Draw(){
    sf::RenderWindow* l_wind = m_context->m_wind->GetRenderWindow();

    sf::FloatRect viewSpace = m_context->m_wind->GetViewSpace();
    
    int tempCoord1 = floor(viewSpace.left / Sheet::Tile_Size);
    int tempCoord2 = floor(viewSpace.top  / Sheet::Tile_Size);

    tileBegin =
    {
        tempCoord1 > 0 ? static_cast<unsigned int>(tempCoord1) : 0 ,
        tempCoord2 > 0 ? static_cast<unsigned int>(tempCoord2) : 0
    };

    tempCoord1 = ceil((viewSpace.left + viewSpace.width) / Sheet::Tile_Size);
    tempCoord2 = ceil((viewSpace.top + viewSpace.height) / Sheet::Tile_Size);
    
    tileEnd = 
    {
        static_cast<unsigned int>(tempCoord1) > m_maxMapSize.x ? static_cast<unsigned int>(tempCoord1) : m_maxMapSize.x,
        static_cast<unsigned int>(tempCoord2) > m_maxMapSize.y ? static_cast<unsigned int>(tempCoord2) : m_maxMapSize.y        
    };

      //  std::cout <<tileBegin.x*32 << " " <<tileBegin.y*32 << " " << tileEnd.x*32 << " " << tileEnd.x*32 << std::endl;

    m_fullMapSprite.setTextureRect(sf::IntRect(tileBegin.x*32 ,tileBegin.y*32,tileEnd.x*32,tileEnd.x*32));
    m_fullMapSprite.setPosition(tileBegin.x*32,tileBegin.y*32);

    l_wind->draw(m_fullMapSprite);
}

void Map::PurgeMap(){
    m_tileCount = 0;
    m_tileCountBorders = 0;
    m_tileSetBorderCount=0;

    for( auto &itr : m_tileMap){
        delete itr.second;
    }
    m_tileMap.clear();

    for( auto &itr : m_tileMapBorders){
        for( auto &itr2 : itr.second){
            delete itr2;
        }
    }
    m_tileMapBorders.clear();

}

void Map::PurgeTileSet(){
    for ( auto &itr : m_tileSet ){
        delete itr.second;
    }
    m_tileSet.clear();
    m_tileSetCount = 0;
}

void Map::LoadTiles(const std::string& l_path){
    std::ifstream file;

    file.open(l_path);

    if ( !file.is_open()){
        std::cout << "! FAILED LOAD TILE SET FILE: " << l_path << std::endl;
        return;
    }
    std:: string line;

    while ( std::getline(file,line)){

        if( line[0] == '|' ) { continue; }

        std::stringstream keystream(line);
        int tileId;
        keystream >> tileId;

        if ( tileId < 0 ){ continue; }
        TileInfo* tile = new TileInfo( m_context, "MapTile", tileId);
        m_tileSetCount++;

        keystream >> tile->m_name >> tile->m_friction.x >> tile->m_friction.y >> tile->m_deadly >> tile->m_walkable;
        if( !m_tileSet.emplace(tileId,tile).second){
            std::cout << "!DUPLICATE TILE TYPE: " << tile->m_name << std::endl;
            delete tile;
        }
    }
    file.close();

}

void Map::LoadNext(){
    m_loadNextMap = true;
}



void Map::UnBlockTile( unsigned int i, unsigned int j){
    m_walkable[i][j] = true;
}

void Map::BlockTile(unsigned int i, unsigned int j){
    m_walkable[i][j] = false;
}

bool Map::IsWalkable(unsigned int i, unsigned int j){
    return m_walkable[i][j];
}


void Map::LoadBitMap( const std::string & l_path){
    #ifdef DEBUGG_RUN
    std::cout << "Map :: Loaded ::  " << l_path << std::endl;
#endif

    


        m_mapTab = MapLoader::getPixelInfo( Utils::GetWorkingDirectory() + l_path);
        
        if( m_mapTab == nullptr ){
            return;
        }

        unsigned int * m_mapSize = MapLoader::bitMapSize(Utils::GetWorkingDirectory() + l_path );

        m_maxMapSize.x = (float)m_mapSize[0];
        m_maxMapSize.y = (float)m_mapSize[1];
        int TileId = 0;

        m_walkable = new bool*[m_maxMapSize.x];


        for(unsigned  int i = 0; i <m_maxMapSize.x; i++){
            m_walkable[i] = new bool[m_maxMapSize.y];
        }


        for(unsigned  int i = 0; i <m_maxMapSize.x; i++){
            for(unsigned  int j = 0; j <m_maxMapSize.y; j++ ){
                //ty++;
                TileId = ConvertTileId( m_mapTab[i][j] );

                if (TileId < 0){
                     std::cout << "! Bad tile id: " << TileId << std::endl;
                     continue;
                }
            //    std::cout << TileId << std::endl;

                auto itr = m_tileSet.find(TileId);

                if (itr == m_tileSet.end()){
                     std::cout << "! Tile id(" << TileId << ") was not found in tileset." << std::endl;
                     continue;
                }

                m_walkable[i][j] = itr->second->m_walkable;

                Tile* tile = new Tile();
                tile->m_properties = itr->second;
                if(!m_tileMap.emplace(ConvertCoords(i,j),tile).second){
                // Duplicate tile detected!
                    std::cout << "! Duplicate tile! : " << i << "" << j << std::endl;
                    delete tile;
                    tile = nullptr;
                    continue;
                }
                m_tileCount++;
                CreateBorders( i, j );

            tile->m_warp = false;

        }
            m_defaultTile.m_friction.x = 0.8;
            m_defaultTile.m_friction.y = 0;
        }

      m_fullmap.create( m_maxMapSize.x*32, m_maxMapSize.y*32);
      MakeMapTexture();

        #ifdef DEBUGG_RUN 
            std::cout << "Map :: TILES_IN_MAP " << m_tileCount << std::endl; 
            std::cout << "Map :: BORDER_TILES " << m_tileCountBorders << " :: BORDER_TILES_SETS " << m_tileSetBorderCount << std::endl;

        #endif

    }

    Colors MapLoader::convertForColor( unsigned int B, unsigned int G, unsigned int R){
        Colors color;
    
                switch( R ){
                case 0 :
    
                        switch( G ){
                        case 0 :
                                    switch( B ){
                                    case 0 :
                                        color = Colors::BLACK;
                                    //	cout << "GET BLACK" << endl;
                                    break;
    
                                    case 125 :
                                        //NONE
                                    break;
    
                                    case 255 :
                                        color = Colors::BLUE;
                                    //	cout << "GET BLUE" << endl;
                                    break;
    
                                    default:
                                        break;
                                }
                        break;
    
                        case 125 :
                                switch( B ){
                                    case 0 :
                                            //NONE
                                    break;
    
                                    case 125 :
                                            //NONE
                                    break;
    
                                    case 255 :
                                            //NONE
                                    break;
    
                                    default:
                                        break;
                                }
                        break;
    
                        case 255 :
                            switch( B ){
                                    case 0 :
                                        color = Colors::GREEN;
                                    break;
    
                                    case 125 :
                                            //NONE
                                    break;
    
                                    case 255 :
                                            //NONE
                                    break;
    
                                    default:
                                        break;
                                }
                        break;
    
                        default:
                            break;
                    }
    
                break;
    
                case 125 :
                    switch( G ){
                        case 0 :
                                    switch( B ){
                                    case 0 :
                                            //NONE
                                    break;
    
                                    case 125 :
                                            //NONE
                                    break;
    
                                    case 255 :
                                            color = Colors::PURPLE;
                                    break;
    
                                    default:
                                        break;
                                }
                        break;
    
                        case 125 :
                                switch( B ){
                                    case 0 :
                                            //NONE
                                    break;
    
                                    case 125 :
                                            //NONE
                                    break;
    
                                    case 255 :
                                            //NONE
                                    break;
    
                                    default:
                                        break;
                                }
                        break;
    
                        case 255 :
                            switch( B ){
                                    case 0 :
                                            //NONE
                                    break;
    
                                    case 125 :
                                            //NONE
                                    break;
    
                                    case 255 :
                                            //NONE
                                    break;
    
                                    default:
                                        break;
                                }
                        break;
    
                        default:
                            break;
                    }
                break;
    
                case 255 :
                    switch( G ){
                        case 0 :
                                    switch( B ){
                                    case 0 :
                                        color = Colors::RED;
                                    break;
    
                                    case 125 :
                                            //NONE
                                    break;
    
                                    case 255 :
                                            //NONE
                                    break;
    
                                    default:
                                        break;
                                }
                        break;
    
                        case 125 :
                                switch( B ){
                                    case 0 :
                                        color = Colors::ORANGE;
                                    break;
    
                                    case 125 :
                                            //NONE
                                    break;
    
                                    case 255 :
                                        color = Colors::PINK;
                                    break;
    
                                    default:
                                        break;
                                }
                        break;
    
                        case 255 :
                            switch( B ){
                                    case 0 :
                                        color = Colors::YELLOW;
                                    break;
    
                                    case 125 :
                                            //NONE
                                    break;
    
                                    case 255 :
                                        color = Colors::WHITE;
                                    break;
    
                                    default:
                                        break;
                                }
                        break;
    
                        default:
                            break;
                    }
                break;
    
                default:
                    break;
            }
    
        return color;
    }
    
    unsigned int* MapLoader::bitMapSize( string filename )	{
    
        ifstream ifs(filename, ios::binary);
    
        if(!ifs){
             cout << "! FAILED LOAD TILE SET FILE: " << filename << endl;
            return nullptr;
        }
    
        // Reading information about BITMAPFILEHEADER
        char* temp = new char[sizeof(BITMAPFILEHEADER)];
        ifs.read(temp, sizeof(BITMAPFILEHEADER));
        BITMAPFILEHEADER* bfh = (BITMAPFILEHEADER*)(temp);
    
        temp = new char[sizeof(BITMAPINFOHEADER)];
        ifs.read(temp, sizeof(BITMAPINFOHEADER));
        BITMAPINFOHEADER* bih = (BITMAPINFOHEADER*)(temp);
    
        unsigned int * end = new unsigned int[2];
        end[0] = bih->biWidth;
        end[1] = bih->biHeight;
    
        delete bih;
        delete bfh;
    
        return end;
    }
    
    unsigned short int ** MapLoader::getPixelInfo( string filename ){
    
        ifstream ifs(filename, ios::binary);
    
        if(!ifs){
             cout << "! FAILED LOAD TILE SET FILE: " << filename << endl;
           return nullptr;
        }
    
        // Reading information about BITMAPFILEHEADER
        char* temp = new char[sizeof(BITMAPFILEHEADER)];
        ifs.read(temp, sizeof(BITMAPFILEHEADER));
        BITMAPFILEHEADER* bfh = (BITMAPFILEHEADER*)(temp);
    
        // Reading information about BITMAPINFOHEADER
        temp = new char[sizeof(BITMAPINFOHEADER)];
        ifs.read(temp, sizeof(BITMAPINFOHEADER));
        BITMAPINFOHEADER* bih = (BITMAPINFOHEADER*)(temp);
    
        ifs.seekg(bfh->bfOffBits, ios::beg); // bfOffBits points for beginning of the image information
        int charTableSize =  bih->biWidth * NUMBER_OF_CHAR_IN_PIXEL;
    
        while( charTableSize % 4 != 0 ){ // Adding missing to  X%4 = 0
            charTableSize++;
        }
    
        ifs.seekg(bfh->bfOffBits, ios::beg);
        temp = new char[charTableSize ];
    
        using Usi = unsigned short int;
    
        Usi ** Values = new Usi*[bih->biHeight];
         for ( unsigned int i = 0; i < bih->biHeight ; ++i)
                Values[i] = new Usi[bih->biWidth];
    
        for(unsigned int  i = 0; i < bih->biHeight; i++){ // Converting PIXEL info from BMP
            ifs.read(temp, charTableSize);
    
            for(unsigned int j = 0; j < bih->biWidth; j++){
                Values[i][j] = convertForColor((unsigned int)(unsigned char)temp[j*3], (unsigned int)(unsigned char)temp[j*3+1], (unsigned int)(unsigned char)temp[j*3+2]);
            }
        }
    
    
    
        //Reversing values:
        for( unsigned int i = 0; i < bih->biHeight/2; i++)
            for( unsigned int j = 0; j < bih->biWidth; j++){
                auto temp = Values[i][j];
                Values[i][j] = Values[bih->biHeight-i-1][j];
                Values[bih->biHeight-i-1][j] = temp;
            }
    
        for(unsigned int i= 0; i < bih->biHeight; i++){
            for (unsigned int j = i; j < bih->biHeight; j++){
                    auto temp = Values[i][j];
                    Values[i][j] = Values[j][i];
                    Values[j][i] = temp;
            }
        }
    
        delete bfh;
        delete bih;
        return Values;
    }
    