/**
 * @file movelist.cpp
 * @author Yesenia González Dávila, Pablo Millán Cubero
 * @note To be implemented by students
 * @warning Complete the code
 *  */

//Alumno 1: Pablo Millan Cubero
//Alumno 2: Yesenia Gonzale Davila

#include <iostream>
#include <cassert>
#include <fstream>
#include <string>

#include "movelist.h"
#include "move.h"
#include "language.h"

using namespace std;

    
void Movelist::allocate(int n) {
    moves=new Move [n];
}

void Movelist::deallocate() {
    delete []moves;
    moves=nullptr;
}

void Movelist::copy(const Movelist &ml) { 
    nMove=ml.size();
    allocate(nMove);
    for (int i=0; i<ml.size(); i++)
        moves[i]=ml.get(i);
}

Movelist:: Movelist(): nMove (0), moves(nullptr) { }

Movelist:: Movelist(int nmov): nMove(0) {
    allocate(nmov);
}

Movelist::Movelist(const Movelist &orig) {
     if (orig.size()==0) {
       nMove=0;
       moves=nullptr;
       }
     else
       copy(orig);
}

Movelist:: ~Movelist() {
    if (nMove!=0) {
        deallocate();
        nMove=0;
    }
}

Move Movelist:: get(int p) const {
    assert (p>-1 && p<nMove);
    return moves[p];
}

void Movelist:: set(int p, const Move &m) {
    assert (p>-1 && p<nMove);
    moves[p]=m;
}

int Movelist::find(const Move &mov) const {  // Se ha añadido metodo en "move" para comparar los dos movimientos (equals)
    bool found=false;
    int pos=-1;
    for (int i=0; i<nMove && !found; i++)  
        if (mov.equals(moves[i])) {
            found=true;
            pos=i;
        }
    return pos;
}

void Movelist:: remove(const Move &mov) {
    //int pos = find(mov);
    //if (pos!=-1) 
    //    remove(pos);
    for (int i=0; i<nMove; i++) 
        if (mov.equals(moves[i]))
            remove(i);
}

void Movelist:: remove(int p) {
    assert (p>-1 && p<nMove);
    for (int i=p; i<nMove-1; i++)
        moves[i]=moves[i+1];
    
    //si nMove=0 -> assert
    if (nMove!=1) {
        Move *aux= new Move[nMove-1];
        for(int i=0; i<nMove-1; i++)
            aux[i]= moves[i];
        deallocate();
        moves= aux;
        nMove--;
    } //nMove=1, p=0 -> borrar la unica posicion que queda
    else      
        clear();
}

void Movelist:: zip(const Language &l) {
    string word;
    for (int i=0; i<nMove; ) {
        word = moves[i].getLetters();
        if (!l.query(word) || word.size()<2 )
            remove (i);
        else
            i++;
    }
}

void Movelist:: clear() {
    if (nMove!=0) {
        deallocate();
        nMove=0;
    }
}

int Movelist:: getScore() const {
    int nscore=0, score=0;
    for (int i=0; i<nMove && score!=-1; i++) {
        nscore =  moves[i].getScore();
        if (nscore==-1) 
            score=-1;
        else
            score+=nscore;
    }
    return score;
}

Movelist& Movelist:: operator=(const Movelist &orig) {
    bool equals = nMove==orig.size() ;
    
    for (int i=0; i<nMove && equals; i++ )
            equals = moves[i].equals(orig.get(i));
    
    if (!equals) {
        clear();                    //si ya tiene espacio reservado se borra y nMove=0  (=obj)
    
        if (orig.size()!=0)         //si orig está vacio, se mantiene vacio y no se entra en copy
            copy(orig);
    }
    return *this;
}

Movelist& Movelist:: operator+= (const Move &mov) {
    Move *aux= new Move[nMove+1];
    
    for(int i=0; i<nMove; i++)
        aux[i]= moves[i];
    
    if (nMove!=0)
        deallocate();
    
    moves = aux;
    moves[nMove]= mov;
    nMove++;
    
    return *this;
}

bool Movelist:: repeat_word (const Move &m) {
    bool same = false ;
    for (int i=0; i<size() && !same; i++) 
        same = m.getLetters() == get(i).getLetters() ;
    
    return same ;
}
 



std::istream &operator>>(std:: istream &is, Movelist &i) { 
    Move move ;
    bool end = false ;
    
    i.clear();
    
    //while ( (move.getLetters()!="@" || move.getCol()!=0 || move.getRow()!=0 || !move.isHorizontal()) && !end )
    while (move.getLetters()!="@" && !end) {
        is >> move ; 
        if (is.eof() || is.bad()) {         //si se ha sobrepasado el final de fichero --> no finaliza correctamente -->
            end=true ;                      //se vacia movelist --> en main movelist.size()==0 --> data_error
            i.clear();
        } else i += move;
    } 
    
    return is;
}

std::ostream &operator<<(std::ostream & os, const Movelist &i) {
    Move m;
    assert (!os.bad());
    
    for (int j=0; j<i.size(); j++) {
        m=i.get(j) ;
        os << m << " " ;
    }
    
    return os;
}

