/**
 * @file main.cpp
 * @author DECSAI
 * @note To be implemented by students either completely or by giving them
 * key functions prototipes to guide the implementation
 */



#include <string>
#include <cassert>
#include <iostream>
#include <random>
#include <fstream>
#include <cmath>
#include "language.h"
#include "bag.h"
#include "player.h"
#include "move.h"
#include "movelist.h"
#include "tiles.h"
#include "window.h"
#include "AnsiTerminal.h"
using namespace std;


#define ERROR_ARGUMENTS 1
#define ERROR_OPEN 2
#define ERROR_DATA 3
#define GENERAL_ERROR 4
#define PASSWORD "MPALABRADOS-V2"

// Alumno1: apellidos nombre
// Alumno2: apellidos nombre

/**
 * @brief Reports an important error and exits the program
 * @param errorcode An integer representing the error detected, which is represented
 * by several symbolic constants: ERROR_ARGUMENTS, ERROR_OPEN y ERROR_DATA
 * @param errorinfo Additional information regarding the error: "" (empty string) for 
 * errors parsing the arguments to main() and, for the case of errors opening or 
 * reading/writing data, the name of the file thas has failed.
 */
void errorBreak(int errorcode, const string & errorinfo);

string errorManagement(int errorcode) ;

/**
 * Evalua si el moviento introducido cumple las siguientes reglas: 
 *   >> Está dentro del tablero (el comienzo y final de la palabra obtenida)
 *   >> Comienza en posicion vacía 
 *   >> La(s) palabra(s) obtenida(s) esta en diccionario 
 */
bool evaluateMove (Game game, int &error_code) ;

/*
 * @brief Main function. 
 * @return 
 */
int main(int nargs, char * args[]) {
    Move move;
    Game game;
    Window window;
    int w=-1, h=-1, random=-1, total_score=0, error=0, p_size, b_size=-1 ;
    string lang="",ifilematch="", ofilematch="", word, mfile_name="", key="", 
            b_secuencia="", p_secuencia="", pfile_name="",ofilename="";
    const string FORMAT=".match", END="@";
    ifstream ifile, matchfile, playfile, *input;
    ofstream ofile ;
    ostream *output;
    output=&cout;
    bool end=false, restored=false, valid;
    
    /// Check arguments


    /// Check arguments
    if (nargs<3 || nargs>13 || nargs%2==0) {
        game.~Game();                           
        errorBreak (ERROR_ARGUMENTS, "") ;
    }
    
    string s;
    for (int i=1 ; i<nargs; ) {
        
        s=args[i];
        i++;
        
        if (s=="-open") {
            restored=true;
            ifilematch = args[i++];
            
            //Comprueba formato --- test 
            /*
            int len=ifilematch.size();
            string file_format=ifilematch.substr(len-FORMAT.size(), FORMAT.size());
            if ( file_format != FORMAT ) {
                game.~Game();     
                cerr << "Please use a matchfile:  -p <*.match>" << endl ;
                errorBreak (ERROR_ARGUMENTS, "") ;
            }
            */
        } 
        
        else if (s=="-l" && !restored) {
            lang=args[i] ;
            i++;
        }
        
        else if (s=="-w" && !restored) {
            if (!isdigit(*args[i])) { 
                game.~Game();
                errorBreak (ERROR_ARGUMENTS, "") ; 
            }
            w = atoi(args[i]);
            i++;
        }
    
        else if (s=="-h" && !restored) {
            if (!isdigit(*args[i])) {
                game.~Game();
                errorBreak (ERROR_ARGUMENTS, "") ;
            }
            h = atoi(args[i]);
            i++;
        }
        
        else if (s=="-r" && !restored) {
            if (!isdigit(*args[i])) {
                game.~Game();
                errorBreak (ERROR_ARGUMENTS, "") ; 
            }
            random = atoi(args[i++]);
        } 
    
        else if (s=="-save") {
            ofilename = args[i++];
            
            //Comprueba formato
            int len=ofilename.size();
            string file_format=ofilename.substr(len-FORMAT.size(), FORMAT.size());
            if (file_format != FORMAT) {
                game.~Game();
                cerr << " >> Please use a matchfile: -s <*.match>" << endl ;
                errorBreak (ERROR_ARGUMENTS, "") ;
            }
        }
        
        else if (s=="-b" && !restored)
            b_secuencia = args[i++] ;
        
        else {
            game.~Game();
            errorBreak (ERROR_ARGUMENTS, "") ;
        }  
    }
         
    
    // Process arguments and load data from file, if asked to in arguments
    if (restored) {
        
        if (random != -1 || w != -1 || h != -1) {
            cerr << "error 1" << endl ;
            game.~Game();
            errorBreak (ERROR_ARGUMENTS, "") ;
        }
        
        ifile.open(ifilematch.c_str());
        if (!ifile) {
            game.~Game();
            errorBreak (ERROR_OPEN, ifilematch) ; //ifilematch
        }
        input=&ifile;
        
            
        *input >> key ;
        if (key != PASSWORD && key != "MPALABRADOS-V1") {
            game.~Game();
            window.~Window();
            errorBreak (ERROR_DATA, ifilematch) ;//ifilematch
        }
                
        *input >> total_score ;  
        *input >> lang ;
        *input >> h >> w ;
        game.tiles.setSize(h, w);
        cerr << endl << game.tiles.getHeight() << " " << game.tiles.getWidth() << endl << endl ;
        
        *input >> game.tiles ;
            
        *input >> p_size;
        *input >> p_secuencia ;
        if (!ifile.eof())   //@warning bag
            game.player.add(toISO(p_secuencia));
        if (game.player.size() != p_size) {
            game.~Game();
            window.~Window();
            errorBreak (ERROR_DATA, ifilematch) ;
        }
            
        *input >> b_size;
        *input >> b_secuencia;
        
        
        //*input >> game;
            
        input=nullptr;
        ifile.close();
    } 
    
    cerr << lang << endl ;
    if (lang == "") {
       game.~Game();
       errorBreak (ERROR_ARGUMENTS, "") ;
    }
    else
        game.language.setLanguage(lang);
    
    if ( (h == -1 || w == -1) && !restored ) {
        game.~Game();
        errorBreak (ERROR_ARGUMENTS, "") ;
    }
    else if (!restored) //@warning
        game.tiles.setSize(h, w);
    
    if (b_secuencia=="") {
        if (random != -1)
            game.bag.setRandom(random);
        game.bag.define(lang);
    }
    else {
        game.bag.set(toISO(b_secuencia));
        
        if (b_size != -1)
            if (game.bag.size() != b_size) {
                game.~Game();
                window.~Window();
                errorBreak (ERROR_DATA, ifilematch) ; //ifilematch
            }
    }
    
    if (ofilename != "") {
        ofile.open(ofilename.c_str());
        if (!ofile) {
            game.~Game();
            errorBreak (ERROR_OPEN, ifilematch) ; //
        }
        output=&ofile;
    }

    // Game's main loop 
    // 1) First set the size of the window according to the size (rows & columns) of
    // the new Tiles
            
    game.setWindowSize();
    while (!end)  { // && cc == 'y' || cc == 'Y' 
        
	// 2) Given the inner data members, it pretty-prints the screen
        error=0 ;
        game.player.add(game.bag.extract(7-game.player.size())); 
        game.doPaint();
        
        // 3) Reads the movement from cin
        cin >> move;
        word = move.getLetters();
        move.setScore(move.findScore(game.language));
        
        if (word == END)  //END = "@" ¿?
            end=true;
        
        else {
            game.crosswords.clear();
            
            if (game.player.isValid(word)){ //si move esta en player 
                
                game.crosswords = game.tiles.findCrosswords(move,game.language);
                game.showCrosswords();
                
                valid = evaluateMove(game, error);

                if (valid) //todo crossword está en diccionario, dentro del tablero y en posiciones válidas
                    if (game.doConfirmCrosswords("Confirm movement?")){
                        
                        game.player.extract(word);
                        
                        for (int i=0; i<game.crosswords.size(); i++)
                            game.tiles.add(game.crosswords.get(i));
                        
                        game.score += game.crosswords.getScore();
                        game.acceptedmovements += move ;
                    }
            }
            else    //si move no esta en player
                error = INFEASIBLE_WORD;
            
            //Waits for the next move
        }
        if (error != 0) {
            game.rejectedmovements += move;
            game.doBadCrosswords(errorManagement(error));
        } 
    }
    
    // End of game

    
    // Save file or print screen
    if (output == &cout) 
        *output << endl << "%%%OUTPUT" << endl;
    else
        *output << PASSWORD << endl ;
        
    *output << game ;
        
    return 0;
}

bool evaluateMove (Game game, int &error_code)  {
    bool valid=true ;
    
    if (game.crosswords.size()==0) {
        valid=false;
        error_code = BOARD_OVERFLOW;
    }
    else if (game.acceptedmovements.size()>0 && game.crosswords.size()<1 && valid) {    //valid=false --> no hay cruces 
        valid = false;
        error_code = MISSING_CROSSWORDS ;
    }
    else
        for (int i = 0; i<game.crosswords.size() && valid; i++) {
            valid = game.crosswords.get(i).findScore(game.language) != NONEXISTENT_WORD ;       //valid=false --> error (score)
            if (!valid)
                error_code = NONEXISTENT_WORD;
        } 
    return valid ;
}

ostream & operator<<(ostream & os, const Game &game)  {
    os << game.score << endl 
       << game.language.getLanguage() << endl 
       << game.tiles.getHeight() << " " << game.tiles.getWidth() << endl 
       << game.tiles 
       << game.player.size() << " " << game.player.to_string() << endl
       << game.bag.size() << " " << game.bag.to_string() << endl ;
    os << game.acceptedmovements.size() << endl << game.acceptedmovements << endl ;
    os << game.rejectedmovements.size() << endl << game.rejectedmovements << endl ;
}

istream & operator>>(istream & is, Game &game) {
    string s;
    int r, c;
    
    is >> s;
    if (s == PASSWORD){
        is >> game.score
           >> s;
        game.language.setLanguage(s);
        game.bag.define(game.language);
        is >> r >> c;
        game.tiles.setSize(r, c);
        is >> game.tiles;
        is >> r >> s;
        game.player.add(s);
        is >> r >> s;
        game.bag.set(s);
    }
}

void errorBreak(int errorcode, const string &errordata) {
    cerr << endl << "%%%OUTPUT" << endl;
    switch(errorcode) {
        case ERROR_ARGUMENTS:
            cerr<<"Error in call. Please use either:"<<endl;
            cerr<< "-l <language> -w <width> -h <height> [-b <bag> -r <randomnumber> -save <matchfile>]"<<endl;
            cerr<< "-open <matchfile> [-save <matchfile>]"<<endl << endl ;            
            break;
        case ERROR_OPEN:
            cerr<<"Error opening file "<<errordata << endl;
            break;
        case ERROR_DATA:
            cerr<<"Data error in file "<<errordata << endl;
            break;
        case GENERAL_ERROR:
            cerr<<"Error: "<<errordata << endl;
            break;
    }
    std::exit(1);
}

string errorManagement(int errorcode) {
    string s;
    switch(errorcode) {
        case UNKNOW:
            s = "ERROR UNKOWN";
            break;
        case BOARD_OVERFLOW:
            s = "ERROR. BOARD OVERFLOW";
            break;
        case NONEXISTENT_WORD:
            s = "ERROR. NONEXISTENT WORD";
            break;
        case INFEASIBLE_WORD:
            s = "ERROR. INEFASIBLE WORD";
            break;
        case NOT_FREE:
            s = "ERROR. THE TILE ISN'T FREE";
            break;
        case MISSING_CROSSWORDS:
            s = "ERROR. NO CROSSWORD";
            break;
    }
    return s;
}
