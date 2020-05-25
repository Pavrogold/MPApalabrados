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

void Tiles::add(const Move& m){
    string s = m.getLetters();
    bool h = m.isHorizontal();
    int r = m.getRow();
    int c = m.getCol();
    
    //si no cabe -> se añade el string cortado 
    for (int i=0; i<s.length() && r<=rows && c<=columns ; i++){
        set(r-1, c-1, s[i]);
        if (h)
            c++;
        else
            r++;
    }
}

std::ostream& operator<<(std::ostream& os, const Tiles &t) {
    os << t.getHeight() << " " << t.getWidth() << endl;
    
    if (t.getHeight()>0 && t.getWidth()>0){     
        
        for (int i=0; i<t.getHeight(); i++){
            for (int j=0; j<t.getWidth(); j++)
                os << t.get(i,j) << " ";
            os << endl ;
        }
    }
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
            //if (get(j, i) != t.get(j, i))
            //    same = false;    
    
    if (!same){
        if (rows != 0 && columns != 0)
            deallocate();
        
        setSize(t.getHeight(), t.getWidth());
    
        for (int i=0; i < t.getHeight(); i++)
            for (int j=0; j < t.getWidth(); j++)
                set (i, j, t.get(i, j));
                //cell[i][j] = t.get(i, j);
    }
}

bool Tiles:: inside (const Move &m) {
    //return (m.getRow()<=rows && m.getCol()<=columns);
    int r = m.getRow(), c = m.getCol(), size = m.getLetters().size()-1;
    return (r+size<=rows && c+size<=columns);
}


//Read-print methods
void Tiles::print(std::ostream &os) const {
    os << rows << " " << columns << endl;
    
    if (rows > 0 && columns > 0)
        
        for (int i=0; i<rows; i++){
            for (int j=0; j<columns; j++)
                os << get(i,j) << " ";
            os << endl ;
        }
}

 
bool Tiles::read(std::istream &is) {
    bool valid=true; 
    
    for (int i=0; i<rows && valid; i++) {
        valid = !is.eof() && !is.bad();
        for (int j=0; j<columns && valid ; j++)  {
            is >> cell[i][j] ;
        }
            
    }
    return valid;
}

Move Tiles::findMaxWord(int r, int c, bool hrz) const {
    assert (r<getHeight() && r>=0 && c<getWeight() && c >=0) ;
    
    Move mov;
    string word;
    char l='v';
    int row=r, col=c;
    
    if (hrz)
        l='h';
    
    //retrocedemos hasta el limite del tablero o hasta que deje de haber letras
    while (get(row,col) != '.' && row>0 && col>0) {
        if (hrz)
            col--;
        else
            row--;
    }
    
    if (hrz && (col != 0 || get (row, 0) == '.')) 
        c = col += 1;
    else (!hrz && (row !=0 || get (0, col) == '.') ) 
        r = row +=1 ;
    
    //formamos la palabra a partir de las coordenadas obtenidas, y avanzamos hasta el limite del tablero o hasta que deje de haber letras
    //while (get(row,col) != '.' && row<getHeight() && col<getWeight()) {
    while (get(row,col) != '.' && row<getHeight() && col<getWeight()) {
        word += get (r, c);
        if (hrz)
            col++;
        else
            row++;
    }
    
    mov.set (r, c, l, word) ;
    return mov ;
}

//m no constante
Movelist Tiles:: findCrosswords(Move &m, const Language &l) const {
    
    Movelist crosswords;        //@warning crossword vacio = moves=nullptr (??)
    Move mov;
    int r = m.getRow()-1, c=m.getCol()-1, n_h, n_v, tam = m.getLetters().size();    
    bool hor = m.isHorizontal(),
         in_tiles = inside(m), r_word;  //repeat word
    
    //mirar score del move antes (en main)
    //devolver crossword vacio (??  -  board overflow)
    //asumo que al tablero aun no se ha añadido la palabra
    
    if (in_tiles && get(r, c) == '.' ) {
        crosswords += m;
        
        int n_h = 1, n_v = 1 ;
        if (hor) 
            n_v = m.getLetters().size();
        else
            n_h = m.getLetters().size();
        
        
        //cruces horizontales
        for (int i=0; i<n_h; i++) {
            mov = findMaxWord (r, c, true);
            r_word = crosswords.repeat_word(mov);       //segun ejemplo guion: si una palabra se repite en los cruces, no se tienen en cuenta estas repeticiones
                
            if (mov.getLetters().size()>1 && mov.getLetters().size() != tam && !r_word) {
                mov.findScore(l);                       //si no esta en diccionario: score = -2
                crosswords += mov ;
            }
            r++;
        }
        r=m.getRow()-1;
        
        //cruces verticales
        for (int i=0; i<n_v; i++) {
            mov = findMaxWord (r, c, false);
            r_word = crosswords.repeat_word(mov);
            
            if (mov.getLetters().size() > 1 && mov.getLetters().size() != tam && !r_word) {
                    mov.findScore(l);
                    crosswords += mov ;
            }
            c++;
        }
    }
    
    
    /*
    else if (!in_tiles) {           //no esta dentro del tablero --> board_overflow
        m.setScore(-2);
        crossword += m ;
    }
    
    else if (get(r, c) != '.') {    //empieza en posicion ocupada --> not_free
        m.setScore(-5);
        crosswords += m ;
    }
    
    if (crosswords.size()==1) {     //==0  -- no se cruza con ninguna otra --> missing crosswords
        m.setScore(-6);
        crosswords.clear();
        crosswords += m;
    } 
    */
        
    return crosswords;
}

