/**
 * @file move.cpp
 * @author DECSAI
 * @note To be implemented by students
 */
#include <string>
#include <cstring>
#include <iostream>
#include <cassert>
#include <stdio.h>

#include "move.h"


using namespace std;

Move:: Move() : row(0), column(0), ishorizontal(false), letters(""), score(0) {}

void Move:: set(int &r, int &c, char &h, const std::string  &l){
    setRow(r);
    setCol(c);
    setHorizontal(h) ;
    setLetters(l) ;
}

void Move:: setRow(int &r){
    assert(r >= 0);
    row = r;
}

void Move:: setCol(int &c){
    assert(c >= 0);
    column = c;
}

void Move:: setHorizontal(char &h){
    if ( h == 'H' || h == 'h')
        ishorizontal = true;
    else
        ishorizontal = false ;
}

void Move:: setLetters(const std::string &l){
    if (l != "@")
        letters = normalizeWord(l);
    else
        letters = l;
}

int Move:: findScore(const Language &l) {
    int n = 0;
    
    if (!l.query(letters))
        n = NONEXISTENT_WORD ;
    
    else
        for (int i = 0; i < letters.length(); i++)
            n += l.getScore(letters[i]);
    
    setScore(n);
    
    return n;
}

void Move:: setScore(int s){
    //assert(s > -2);
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
    if (ishorizontal)
        os << "H";
    else
        os << "V";
    
    os << " " << row << " " << column << " " << toUTF(letters) ;
}

bool Move:: read(std::istream &is){
    char h;
    string l;
    int r, c;
    bool res=true;

    is >> h ;
    is >> r ;
    is >> c ;
    is >> l;
    
    if (is.eof() || is.bad()) {
        res=false;
    } else set(r, c, h, l) ;
        
    return res;
}

bool Move:: equals (const Move &mov) const {    //@warning cambiar por operador
    bool equals = (mov.getLetters()==letters && mov.getCol()==column && mov.getRow()==row && mov.isHorizontal()==ishorizontal);
    return equals;
}


std::ostream& operator<<(std::ostream& os, const Move &m) {
    if (m.isHorizontal())
        os << "H";
    else
        os << "V";
    
    os << " " << m.getRow() << " " << m.getCol() << " " << toUTF(m.getLetters()) ;
    
    return os;
}

std::istream& operator>>(std::istream& is, Move &m) {
    char h;
    string l;
    int r, c;

    is >> h >> r >> c >> l ;
    
    if (!is.eof() && !is.bad())
        m.set(r, c, h, l) ;
    else {
        r = c = 0;
        h='h';
        l="@";
        m.set(r, c, h, l) ;
    }
    
    return is;
}