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
    move=new Movelist [n];
}

void Movelist::deallocate() {
    delete []move;
}

void Movelist::copy(const Movelist &ml); { // hacer que this-> sea un clon de ml  @warning ??
    move=ml;
}

Movelist:: Movelist(): nMove (0) {
    allocate[5]; //numero inicial?
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
 /**
    * @brief Assign the content of the parameter  object to *this
    * @param orig Right hand side of an assignment
 */
        
void Movelist:: assign (const Movelist& orig){
    
}

    /**
    * @brief Overloads the assignment operator
    * @param orig Right hand side of the assignement
    * @return this Left hand side of the assignement
	 
operator=(orig); */

Move Movelist:: get(int p) {
    assert (p>-1 && p<nMOve);
    return move[p];
}

void Movelist:: set(int p, const Move &m) {
    assert (p>-1 && p<nMove);
    /*
     assert (p>-1 &&p<=nMove);
     * if (p==nMove)
     *  ...
     */
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

	/**
	 * @brief Resets the set and leaves it empty 
	 */
   clear();
	/**
	 * @brief Computes the whole score of the list of movements by adding the individual scores of each movement contained in the set
	 * of movements. It does not need the language to compute the scores because this is done inside each movement. 
	 * See move.h for more details. If at least one movement scores -1, then 
	 * the full list of movements will score -1.
	 * @return The score of the full set of movements
	 */
int Movelist:: getScore() {
    int nscore=0, score=0;
    for (int i=0; i<nMove && nscore!=-1; i++) {
        nscore = move[i]->getScore();
        score += nscore;
    }
}
	/**
	 * @brief Insert the data of the list of movements into an ostream (either cout or file)
	 * @param os The ostream
	 * @param scores If true, it prints the scores of every single movement. False by default
	 * @return true if there was no problen inserting/writing data
	 */
	bool print(std::ostream &os, bool scores=false) const;
	/**
	 * @brief Reads the movement from an istream until the last movement is marked
	 * as a "H 0 0 @" movement, that is, "H 0 0 _" normalized
	 * @param is The istream
	 * @return True if there was no problem reading data, false otherwise.
	 */
	bool read(std::istream &is);
 
 */

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

