/**
 * @file bag.cpp
 * @author Pablo Millan Cubero, Yesenia Gonzalez Davila
 * @note To be implemented by students except function shuffle()
 */

#include <cassert>
#include <random>
#include <algorithm>
#include <cassert>
#include <cstring>
#include <string>
#include <fstream>

#include "mi_random.h"
#include "bag.h"
#include "language.h"

using namespace std;

char Bag::get( int p) const {
    assert (p < nletters && p >= 0 ) ;
    return letters[p] ;
}

void Bag::set(int p, char c)  {   
    assert (p < nletters && p >= 0 ) ;
    letters[p] = c ;
}

void Bag::set(std::string s){
    assert (s.size() <= MAXBAG) ;
    nletters = s.size() ;
    
    
    for ( int i = 0 ; s[i] != '\0' ; i++ ) {
        set(i,s[i]);
    }
}

void Bag::define(const Language &l)
{
    string letterSet = l.getLetterSet() ;
    int n = 0 ;
    nletters = 0 ;
    
    for (int i = 0; i < letterSet.size(); i++)
        nletters += l.getFrequency(letterSet[i]); 
    
    assert (nletters <= MAXBAG) ;
    
    for (int i = 0 ; i < letterSet.size() ; i++ ) {
        for ( int j = 0 ; j < l.getFrequency( letterSet[i] ) ; j++ ) {
            set(n, letterSet[i]);
            n ++ ;
        }
    }
    shuffleVector (letters, nletters, id) ;
}

int Bag::size() const {
    return nletters ;
}

void Bag::setRandom(int id) {
    this -> id=id ;
}


void Bag::removeVector(char *s, int &n, int p)
{
    assert (p<n && p>=0) ;
    
    for ( int i = p ; i < n-1 ; i++ )
        s[i] = s[i+1] ;
    
    n-- ;
}

std::string Bag::extract(int n)
{    
    string s = "" ;
    
    assert ( n >= 0 ) ;
    
    if (n > nletters)
        n = nletters;
    
    for (int i = 0 ; i < n ; i++) {
        s += letters[0] ;
        removeVector(letters, nletters, 0);
    }
    
    return s;
}

string Bag::to_string() const {
    
    string s="";
    
    for ( int i=0 ; i < nletters ; i++ )
        s += letters[i] ;
    
    return s;
}

// Main methods
Bag::Bag() {
    nletters = 0;
    id = time(NULL);
}

// Auxiliary functions 


void Bag:: shuffleVector(char *s, int n, int id) {
    vector <char> aux(s, s+n);
    //std::shuffle(aux.begin(), aux.end(), std::default_random_engine(id));
    shuffle(aux.begin(), aux.end(), mi_random(id));
    for (int i=0; i<n; i++)
        s[i] = aux[i];    
} 




