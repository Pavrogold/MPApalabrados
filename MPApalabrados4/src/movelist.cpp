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
    moves=new Move [n];
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
    allocate(nmov);
}

Movelist::Movelist(const Movelist &orig) {
    copy(orig);
}

Movelist:: ~Movelist() {
    deallocate();
}
        
void Movelist:: assign (const Movelist& orig){  //el objeto apunta al mismo espacio de memoria que orig
    moves=orig.moves; //@warning ??
    nMove=orig.nMove;
}

    /**
    * @brief Overloads the assignment operator
    * @param orig Right hand side of the assignement
    * @return this Left hand side of the assignement
	 
operator=(orig); */

Move Movelist:: get(int p) const {
    assert (p>-1 && p<nMove);
    return moves[p];
}

void Movelist:: set(int p, const Move &m) {
    assert (p>-1 && p<nMove);
    moves[p]=m;
}

int Movelist:: size() const {
    return nMove; 
}

int Movelist::find(const Move &mov) {  // Se ha añadido metodo en move para comparar dos moves (equals)
    bool found=false;
    int pos=-1;
    
    for (int i=0; i<nMove && !found; i++)  {
        if (mov.equals(moves[i])) {
            found=true;
            pos=i;
        }
    }
    return pos;
}



void Movelist:: add(const Move &mov) {
    Move *nuevo = new Move [nMove+1] ;
    for (int i = 0; i < nMove; i++)
        nuevo[i] = moves[i];
    delete[] nuevo;
    moves = nuevo;
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
        moves[i]=moves[i+1];
    nMove--;
}

void Movelist:: zip(const Language &l) {
    string word;
    Move mov;
    for (int i=0; i<nMove; ) {
        mov=moves[i];
        word = mov.getLetters();
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

int Movelist:: getScore() const {
    int nscore=0, score=0;
    bool valid=true;
    Move m;
    for (int i=0; i<nMove && valid; i++) {
        m=moves[i];
        nscore =  m.getScore();
        score += nscore;
        if (nscore==-1) {
            valid=false;
            score=-1;
        }
    }
    return score;
}

bool Movelist:: read(std::istream &is) {
    bool res=true;
    int i=0;
    Move m;
    do {
        moves[i].read(is);
        if (is.bad())
            res=false;
        i++;
    } while ( (normalizeWord(moves[i].getLetters())!="_" || moves[i].getCol()!=0 || moves[i].getRow()!=0 || !moves[i].isHorizontal()) && res ) ;
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

