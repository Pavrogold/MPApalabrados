/**
 * @file player.cpp
 * @author DECSAI
 * @note To be implemented by students
 */

//Alumno 1: Pablo Millan Cubero
//Alumno 2: Yesenia Gonzalez Davila

#include <cstring>
#include <cassert>
#include "player.h"

///@warning complete missing #includes
/// See http://www.cplusplus.com/reference/cstring/ for details about cstrings

void removeCString(char *cstr, int pos) ;
void sortCString(char *cstr) ;

using namespace std;

Player:: Player() {
    letters[0] = '\0' ;
} 

int Player:: size() const  {
    return strlen (letters) ;
}

std::string Player:: to_string() const {
    string s (letters) ;
    return (s) ;
}

void Player::clear() {
    letters[0] = '\0' ;
}

bool Player:: isValid(const std::string &s) const {
    bool valid = true ;
     Player tmp=*this;
     
     if (!tmp.extract(s))
        valid = false ;
     
    return valid ;
}

Player& Player:: operator -= (const std::string &s) { 
    this->extract(s);
    return *this;
}

bool Player:: extract(const std::string &s) {
    bool valid=true;
    char *ptr ;
    char final[MAXPLAYER+1];
    
    if ( s.length()>strlen(letters) || s.length()<1 )
        valid = false ;
    
    strcpy(final, letters);
    for (int i=0; i<s.size() && valid; i++) {
        ptr=strchr(final,s[i]);
        if (ptr!=nullptr)       //ptr>0
            removeCString(final,ptr-final);
        else
            valid = false;
    }
    if (valid) {
        strcpy(letters, final);
        sortCString(letters);
    }
    return valid;
}

void Player:: add(const string frombag){
    
    int n = size() ;
    
    if ( frombag.length() <= MAXPLAYER-size() ) {     //If the set of additional letters is too large, it does nothing
    
        for(int i=0; i<frombag.length() ; i++) {
            letters[n] = frombag[i];
            n++;
        }
        letters[n] = '\0' ;
        sortCString(letters) ;
    }
}


/**
 * @brief Removes a position from a cstring
 * @param cstr The cstring
 * @param pos The position
 * @return The cstring is modified
 * @warning To be fully implemented
 */

void removeCString(char *cstr, int pos) {
    
    assert ( pos >= 0 && pos<strlen(cstr) ) ;
    
    for ( int i = pos ; cstr[i] != '\0' ; i++ )
        cstr[i] = cstr[i+1] ;
}

/**
 * @brief Sort a cstring from A to Z
 * @param cstr The cstring
 * @return The cstring is modified
 * @warning To be fully implemented
 */
void sortCString(char *cstr) {
    int desplaza, i ;
    
    for (int izq = 1; cstr[izq] != '\0' ; izq++)  {
        desplaza = cstr[izq] ;
        
        for ( i = izq ; i > 0 && desplaza < cstr[i-1] ; i--)
            cstr[i] = cstr[i-1] ;
        
        cstr[i] = desplaza ;
    }
}



