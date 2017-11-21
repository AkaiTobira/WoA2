#include <fstream>
#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <vector>
#include <string>





int main(){
    
    std::ofstream mfle ( "dane.txt");

    std::vector< std::string > t1;
    std::vector< std::string > t2;
    std::vector< int > t3;
    
    t3.push_back(1);
    t3.push_back(1);
    t3.push_back(1);
    t3.push_back(3);
    t3.push_back(1);
    t3.push_back(1);
    t3.push_back(3);
    t3.push_back(1);


    t1.push_back("LU");
    t1.push_back("U");
    t1.push_back("RU");
    t1.push_back("L");
    t1.push_back("R");
    t1.push_back("LD");
    t1.push_back("D");
    t1.push_back("RD");
    
    t2.push_back("HG");
    t2.push_back("P");
    t2.push_back("I");
    t2.push_back("G");
    t2.push_back("S");
    t2.push_back("Gr");
    t2.push_back("W");
    t2.push_back("R");

char t;
int nmb = 0;
int nmb2 = 0;
const int longest = 9;
int numberoft3 = 0;

for( std::string t : t2 ){

for( int i = 0; i < t3[numberoft3]*3; i++){
    if( i%3 == 0 ) {nmb2++; mfle << nmb << " " << t << t1[0] << nmb2 << " 0.8 0 0" << std::endl;}
    if( i%3 == 1 ) mfle << nmb << " " << t << t1[1] << nmb2 << " 0.8 0 0" << std::endl;
    if( i%3 == 2 ) mfle << nmb << " " << t << t1[2] << nmb2 << " 0.8 0 0" << std::endl;    
    nmb++;
}
nmb+= ( longest - t3[numberoft3]*3);

nmb2 = 0;
for( int i = 0; i < t3[numberoft3]*3; i++){
    if( i%3 == 0 ) {nmb2++; mfle << nmb << " " << t << t1[3] << nmb2 << " 0.8 0 0" << std::endl;}
    if( i%3 == 1 ) mfle << nmb << " " << t <<  nmb2 << " 0.8 0 0" << std::endl;
    if( i%3 == 2 ) mfle << nmb << " " << t << t1[4] << nmb2 << " 0.8 0 0" << std::endl;    
    nmb++;
}
nmb+= ( longest - t3[numberoft3]*3);
nmb2 = 0;
for( int i = 0; i < t3[numberoft3]*3; i++){
    if( i%3 == 0 ) {nmb2++; mfle << nmb << " " << t << t1[5] << nmb2 << " 0.8 0 0" << std::endl;}
    if( i%3 == 1 ) mfle << nmb << " " << t << t1[6] << nmb2 << " 0.8 0 0" << std::endl;
    if( i%3 == 2 ) mfle << nmb << " " << t << t1[7] << nmb2 << " 0.8 0 0" << std::endl;    
    nmb++;
}
nmb+= ( longest - t3[numberoft3]*3);
nmb2 = 0;

numberoft3++;
}

    mfle.close();
    return 0;
    
}
