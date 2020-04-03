/**
 * @file move.cpp
 * @author DECSAI
 * @note To be implemented by students
 */
#include <string>
#include <cassert>
#include "move.h"
#include "iostream"

using namespace std;

Move:: Move(){
    row = 0;
    column = 0;
    ishorizontal = false;
    letters = "";
    score = 0;
}

void Move:: set(int &r, int &c, char &h, const std::string  &l){
    
    assert (r>=0 && c>=0);
    setRow(r);
    setCol(c);
    setHorizontal(h) ;
    setLetters(l) ;
}

void Move:: setRow(int &r){
    assert( r >= 0);
    row = r;
}

void Move:: setCol(int &c){
    assert( c >= 0);
    column = c;
}

void Move:: setHorizontal(char &h){
    
    if ( h == 'H' || h == 'h')
        ishorizontal = true;
}

void Move:: setLetters(const std::string &l){
    if (l != "@")
        letters = normalizeWord(l);
    else
        letters = l;
}

int Move:: findScore(const Language &l) const {
    int n = 0;
    
    if (!l.query(letters))
        n = -1;
    
    else{
    
        for (int i = 0; i < letters.length(); i++){
            n += l.getScore(letters[i]);
        }
    }
    
    return n;
}

void Move:: setScore(int &s){
    assert( s >= -2);
    score = s;
}

int Move:: getScore() const {
    return score;
}

int Move:: getRow() const {
    return row;
}

int Move:: getCol() const {
    return column;
}

bool Move:: isHorizontal() const {
    return ishorizontal;
}

std::string Move:: getLetters() const {
    return toISO(letters);
}

void Move:: print(std::ostream &os) const {

    os << "row = " << row << endl;
    os << "column = " << column << endl;
    os << "letters = " << letters << endl;
    
    if (ishorizontal)
        os << "H";
    else
        os << "V";
}

void Move:: read(std::istream &is){
    char h;
    string l;
    int r, c;
    
    is >> h ;
    is >> r ;
    is >> c ;
    is >> l;
    
    set(r, c, h, toUTF(l)) ;
}