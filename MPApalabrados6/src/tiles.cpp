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
    setSize(r, c);
}

Tiles::Tiles(const Tiles & orig){
    rows = 0;
    columns = 0;
    copy(orig);
}

Tiles::~Tiles(){
    deallocate();
}

void Tiles::setSize(int r, int c){
    assert (r>0 && c>0);
    rows = r;
    columns = c;
    
    //deallocate();
    allocate(r, c);
    
    for (int i=0; i<columns; i++)
        for (int j=0; j<rows; j++)
            set(j, i, '.');
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
    assert(r>=0 && r<rows && c>=0 && c<columns); // assertion if r=0 and c=0 
    
    return cell[r][c];
}

void Tiles::set(int r, int c, char l){
    assert(r>=0 && r<rows && c>=0 && c<columns);
    cell[r][c] = l;
}

Tiles& Tiles:: operator+=(const Move &m) {
    string s = m.getLetters();
    bool h = m.isHorizontal();
    int r = m.getRow(), c = m.getCol();
    
    for (int i=0; i<s.length() && r<=rows && c<=columns; ){
        if (get(r-1,c-1)=='.' || i==0) 
            set(r-1, c-1, s[i++] );
        if (h)
            c++;
        else
            r++;
    }
    return *this;
}

std::ostream& operator<<(std::ostream& os, const Tiles &t) {
    if (t.getHeight()>0 && t.getWidth()>0){     
        
        for (int i=0; i<t.getHeight(); i++){
            for (int j=0; j<t.getWidth(); j++)
                os << toUTF(t.get(i,j)) << " ";
            os << endl ;
        }
    }
    os << endl ;
}

std::istream& operator>>(std::istream& is, Tiles &t) {
    bool valid = true ;
    char n;
    
    for (int i=0; i<t.getHeight() && valid; i++) {
        valid = !is.eof() && !is.bad();
        for (int j=0; j<t.getWidth() && valid ; j++)  {
            is >> n ;
            t.set(i, j, n) ;
        }
    }
}

void Tiles::allocate(int r, int c) {
    cell = new char *[r];
    cell[0] = new char [r*c];
    
    for(int i=1; i < r; i++)
        cell[i] = cell[i-1]+c;
}

void Tiles::deallocate() {
    if (cell != nullptr){
        delete [] cell[0];
        delete [] cell;
        cell = nullptr;
        rows = 0;
        columns = 0;
    }
}

void Tiles::copy (const Tiles &t) {
    bool same = true;
    
    if (t.getHeight() != rows || t.getWidth() != columns)
        same = false;
    
    for (int i=0; i<t.getHeight() && same; i++)
        for (int j=0; j<t.getWidth() && same; j++)
            same = get(j, i) == t.get(j, i) ;
    
    if (!same){
        if (rows != 0 && columns != 0)
            deallocate();
        
        setSize(t.getHeight(), t.getWidth());
    
        for (int i=0; i < t.getHeight(); i++)
            for (int j=0; j < t.getWidth(); j++) {
                set (i, j, t.get(i, j));
            }
    }
}

bool Tiles:: inside (const Move &m) const {
    int r = m.getRow(), c = m.getCol(), word_size = m.getLetters().size(), n=0, p=0 ;
    
    while (r<=rows && c<=columns) {
        if (get(r-1, c-1) != '.') 
            n++;            //letras encontradas
        else 
            p++;            //huecos encontrados (donde estarían las letras del mov)
        
        if (p==word_size)   //si se encuentran tantos huecos como letras tiene el mov, la palabra cabe y se sale del bucle
            break;
        
        if (m.isHorizontal()) 
            c++;
        else 
            r++;
    }
    
    if (m.isHorizontal()) 
            c = m.getCol() + n + word_size-1;    //columna final del mov
    else 
            r = m.getRow() + n + word_size-1;    //fila final del mov
    
    return (r<=rows && c<=columns);
}

void Tiles::moveBack (int &r, int &c, bool hrz)const {
    int row=r-1, col=c-1;
    
    if (hrz && c>0) {
        while (get(row, col-1) != '.' && col>0) 
            col--;
        c=col+1;
    }
    else if (!hrz && row>0) {
        while (get(row-1, col) != '.' && row>0)
            row--;
        r=row+1;
    }
}

void Tiles::moveForward (int r, int c, bool hrz, string &word) const {
    word="";
    bool res = true;
    do {
        if (get(r,c) != '.') 
            word += get(r, c);
        else 
            res=false;
        
        if (hrz)
            c++;
        else
            r++;
    } while (r<rows && c<columns && res);
}

Move Tiles::findMaxWord(int r, int c, bool hrz) const {
    assert (r<=getHeight() && r>=0 && c<=getWidth() && c>=0) ;
    
    Move mov;
    string word;
    char l='v';
    int row=r, col=c ;
    
    if (hrz)
        l='h';
    
    if ( (hrz && col>1) || (!hrz && row>1))
        moveBack(row, col, hrz);     //coordenadas de inicio del posible cruce
    r=row; c=col;   
    if ( (hrz && col<columns) || (!hrz && row<rows) );
        moveForward (row-1, col-1, hrz, word);
    
    mov.set (r, c, l, word) ;
    return mov ;
}

void Tiles:: addCross (Move &m, bool horizontal, const Language &lang, int n, int r, int c) const {
    for (int i=0; i<n ; ) {
        if (get(r-1, c-1) == '.') {
            i++;
            m = findMaxWord (r, c, true);
                
            if (m.getLetters().size()>1) 
                m.findScore(lang);
        }
        if (horizontal)
            r++;
        else
            c++;
    }
}

//m no constante
Movelist Tiles:: findCrosswords(const Move &m, const Language &l) const {
    Tiles aux (*this);
    Movelist crosswords;        
    Move find_mov ;
    int r = m.getRow(), c=m.getCol(), n_h=1, n_v=1 ;    
    bool hor = m.isHorizontal(), in_tiles = inside(m),occupied=false ;
    
    
    //numero posibles cruces verticales y horizontaes
    if (hor) 
        n_v = m.getLetters().size();
    else
        n_h = m.getLetters().size();
    
    
    if (in_tiles) {                 //dentro de tablero
        if (get (r-1, c-1)=='.') {  //pos no ocupada
            aux += m;
            
            //cruces horizontales
            for (int i=0; i<n_h; r++) 
                if (get(r-1, c-1) == '.'){
                    i++;
                    find_mov = aux.findMaxWord (r, c, true);
        
                    if (find_mov.getLetters().size()>1) {
                        find_mov.findScore(l);      
                        crosswords += find_mov ;
                    }
                }
            r=m.getRow();
    
            //cruces verticales
            for (int i=0; i<n_v; c++) 
                if (get(r-1, c-1) == '.'){  
                    i++,
                    find_mov = aux.findMaxWord (r, c, false);
        
                    if (find_mov.getLetters().size()>1) {
                        find_mov.findScore(l);      
                        crosswords += find_mov ;
                    }
                }

        }
        else {      //not free
            find_mov.setScore(NOT_FREE);
            crosswords += find_mov ;
        }
    }
    return crosswords;
}
