/**
 * @file movelist.cpp
 * @author DECSAI
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
    moves=new Movelist [n];
}

void Movelist::deallocate() {
    delete []moves;
}

void Movelist::copy(const Movelist &ml) { // hacer que this-> sea un clon de ml  (cada elemento es igual, NO apuntan a la misma direccion)
    nMove=ml.size();
    allocate(nMove);
    for (int i=0; i<nMove ; i++ ) 
        moves[i]=ml.get(i);
}

Movelist:: Movelist(): nMove (0) {
    moves=nullptr;
}

Movelist:: Movelist(int nmov): nMove(0) {
    allocate[nmov];
}

Movelist::Movelist(const Movelist &orig) {
    copy(orig);
}

Movelist:: ~Movelist() {
    deallocate();
}
        
void Movelist:: assign (const Movelist& orig){  //el objeto apunta al mismo espacio de memoria que orig
    this=&orig; // @warning ??
}

    /**
    * @brief Overloads the assignment operator
    * @param orig Right hand side of the assignement
    * @return this Left hand side of the assignement
	 
operator=(orig); */

Move Movelist:: get(int p) {
    assert (p>-1 && p<nMove);
    return move[p];
}

void Movelist:: set(int p, const Move &m) {
    assert (p>-1 && p<nMove);
    move[p]=m;
}

inline int Movelist:: size() {
    return nMove; 
}

int Movelist::find(const Move &mov) {
    bool found=false;
    int pos=-1;
    
    for (int i=0; i<nMove && !found; i++) 
        if (move[i]==mov) {
            found=true;
            pos=i;
        }
    return pos;
}

void Movelist:: add(const Move &mov) {
    move[nMove]=mov;
    nMove++;
}

void Movelist:: remove(const Move &mov) {
    int pos = find(mov);
    if (pos!=-1) 
        remove(pos);
}

void Movelist:: remove(int p) {
    assert (p>-1 && p<nMove);
    for (int i=p; i<nMove-1; i++)
        move[i]=move[i+1];
    nMove--;
}

void Movelist:: zip(const Language &l) {
    string word;
    for (int i=0; i<nMove; ) {
        word = move[i]->getLetters();
        if (!l.query(word) || word.size()<2 )
            remove (i);
        else
            i++;
    }
}

void Movelist:: clear() {
    deallocate();
    moves=nullptr;
    nMove=0;
}

int Movelist:: getScore() {
    int nscore=0, score=0;
    bool valid=true;
    for (int i=0; i<nMove && valid; i++) {
        nscore = moves[i]->getScore();
        score += nscore;
        if (nscore==-1) {
            valid=false;
            score=-1;
        }
    }
    return score;
}

bool Movelist:: read(const std::istream &is) {
    bool res=true;
    do {
        moves[i]->read(is);
        if (is.bad())
            res=false;
    } while ( (normalizeWord(moves[i]->getLetters())!="_" || moves[i]->getCol()!=0 &|| moves[i]->getRow()!=0 || !moves[i]->isHorizontal()) && res ) ;
    return res;
}
 

bool Movelist::print(const std::ostream &os, bool scores) const {
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

