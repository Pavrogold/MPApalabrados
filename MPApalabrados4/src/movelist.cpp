/**
 * @file movelist.cpp
 * @author Yesenia González Dávila, Pablo Millán Cubero
 * @note To be implemented by students
 * @warning Complete the code
 *  */
#include <iostream>
#include <cassert>

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
    deallocate();
    nMove=0;
}
        
void Movelist:: assign (const Movelist& orig){
    if (nMove>0)               //si ya tiene espacio reservado se borra y nMove=0
        clear();
    
    if (orig.size()!=0)       //si orig está vacio, se mantiene vacio y no se entra en copy
        copy(orig);
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

void Movelist:: add(const Move &mov) {
    Move *aux= new Move[nMove+1];
    
    for(int i=0; i<nMove; i++)
        aux[i]= moves[i];
    
    deallocate();
    moves= aux;
    moves[nMove]= mov;
    nMove++;
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
    
    Move *aux= new Move[nMove-1];
    for(int i=0; i<nMove-1; i++)
        aux[i]= moves[i];
    
    deallocate();
    moves= aux;
    nMove--;
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
    deallocate();
    nMove=0;
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

bool Movelist:: read(std::istream &is) {
    bool res=true;
    int i=0;
    Move m;
    res=m.read(is);
    while ( (m.getLetters()!="@" || m.getCol()!=0 || m.getRow()!=0 || !m.isHorizontal())  && res ) {
        add(m);
        res=m.read(is);
    } 
    return res;
}
 

bool Movelist::print(std::ostream &os, bool scores) const {
    bool res=true;
    for (int i=0; i<size() && res; i++) {
        get(i).print(os);
        if (scores)
            os << " ("<<get(i).getScore()<<") - ";
        else
            os << " - ";
        if (os.bad())
            res=false;
    }
    return res;
}

