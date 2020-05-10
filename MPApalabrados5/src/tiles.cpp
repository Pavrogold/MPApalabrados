/**
 * @file tiles.cpp
 * @author DECSAI
 * @note To be implemented by students
 */
#include <string>
#include <iostream>
#include <cassert>
#include <stdio.h>

#include "tiles.h"


using namespace std;

Tiles::Tiles(){
    cell = nullptr;
    columns = 0;
    rows = 0;
}

Tiles::Tiles(int r, int c){
    assert(r > 0 && c > 0);
    setSize(r, c);
    
    for (int i=0; i<columns; i++){
            for (int j=0; j<rows; j++){
                
                set(j, i, '.');
            }
        }
}

Tiles::Tiles(const Tiles & orig){
    copy(orig);
}

Tiles::~Tiles(){
    deallocate();
}

void Tiles::setSize(int r, int c){
    assert(r < rows && c < columns);
    
    rows = r;
    columns = c;
    
    allocate(r, c);
    
    for (int i=0; i<columns; i++){
            for (int j=0; j<rows; j++){
                
                set(j, i, '.');
            }
        }
}

Tiles& Tiles::operator=(const Tiles& orig){
    
    copy(orig);
    
    return *this;
}

int Tiles::getWidth() const {
    
    return columns;
}

int Tiles::getHeight() const {
    
    return rows;
}

char Tiles::get(int r, int c) const {
    assert(r < rows && c < columns);
    
    return cell[r][c];
    
}

void Tiles::set(int r, int c, char l){
    assert(r < rows && c < columns);
    
    cell[r][c] = l;
    
}

void Tiles::add(const Move&m){
    string s;
    bool h = m.isHorizontal()
    int r = m.getRow();
    int c = m.getCol();
    
    for (int i=0; i < s.length(); i++){
        set(r, c, s[i]);
        
        if (h)
            r++;
        else
            c++;
    }
}

void Tiles::print(std::ostream &os) const {
    
    *os << "Rows: " << rows << endl;
    *os << "Columns: " << columns << endl;
    if (rows > 0 && columns > 0){
        *os << "Tiles:" << endl;
        
        for (int i=0; i<columns; i++){
            for (int j=0; j<rows; j++){
                
                *os << get(j, i) << " ";
            }
            *os << endl;
        }
    }
    
}

bool Tiles::read(std::istream &is) {
    
    
    
}

void Tiles::allocate(int r, int c) {
    cell = new char *[r];
    cell[0] = new char [r*c];
    
    for(int i=1; i < r; i++)
        char[i] = char[i-1]+c;
}

void Tiles::deallocate() {
    if (cell != nullptr){
        delete [] cell[0];
        delete [] cell;
        cell = nullptr;
    }
}

void Tiles::copy( Tiles  &t) {
    
    deallocate();
    setSize(t.getHeight(), t.getWidth());
    
    for (int i=0; i < t.getHeight(); i++){
        for (int j=0; j < t.getWidth(); j++){
            
            cell[i][j] = t.get(i, j);
        }
    }
}
