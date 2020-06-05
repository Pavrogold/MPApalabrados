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
#define PASSWORD1 "MPALABRADOS-V1"
#define PASSWORD2 "MPALABRADOS-V2"
#define FORMAT ".match"

// Alumno1: Gonzalez Davila, Yesenia
// Alumno2: Millan Cubero, Pablo

/**
 * @brief Reports an important error and exits the program
 * @param errorcode An integer representing the error detected, which is represented
 * by several symbolic constants: ERROR_ARGUMENTS, ERROR_OPEN y ERROR_DATA
 * @param errorinfo Additional information regarding the error: "" (empty string) for 
 * errors parsing the arguments to main() and, for the case of errors opening or 
 * reading/writing data, the name of the file thas has failed.
 */
void errorBreak(int errorcode, const string & errorinfo);


/**
 * @brief Lee argumentos dados. Llama a errorBreak en caso de que el formato introducido no coincida con el requerido.
 */
void checkArguments (int nargs, char *arguments[], Game &game, string &ifilename, string &ofilename, string &lang, string &bag, 
                     int &h, int &w, int &random, bool &res, bool &save) ;


/**
 * @brief Configura game a partir de los argumentos dados por el usuario.
 */
void setGame (Game &game, string &lang, string &bag, int h, int w, int random);


/**
 * @brief Evalua si el moviento introducido cumple las siguientes reglas: 
 *   >> Está dentro del tablero (tanto el comienzo como el final de la palabra obtenida)
 *   >> Comienza en una posicion vacía 
 *   >> La(s) palabra(s) obtenida(s) esta(n) en diccionario 
 */
void evaluateMove (Game &game, Move &m, Player &player, int &error_code) ;

string errorManagement(int errorcode) ;

/*
 * @brief Main function. 
 * @return 
 */
int main(int nargs, char * args[]) {
    
    Move move;
    Game game;
    Window window;
    
    int w=-1, h=-1, random=-1, error=0 ;
    bool end=false, load=false, save=false ;
    string lang="", ifilematch="", ofilematch="", word, key="", bag="", player="" ;
    const string END="@";       //Final movement to end the game: @
    
    ifstream ifile ;
    istream *input;
    ofstream ofile ;
    ostream *output = &cout;
    
    /// Check arguments
    checkArguments (nargs, args, game, ifilematch, ofilematch, lang, bag, h, w, random, load, save) ;
    
    
    // Load data from file, if asked to in arguments
    if (load) {
        ifile.open(ifilematch.c_str());
        if (!ifile) {
            game.~Game();
            errorBreak (ERROR_OPEN, ifilematch) ; 
        }
        input=&ifile;
            
        *input >> game;
        if (game.score == ERROR_DATA) {
            game.~Game();
            errorBreak (ERROR_DATA, ifilematch) ;
        }
        
        input=nullptr;
        ifile.close();
    } 
    else    // Set game from arguments
        setGame (game, lang, bag, h, w, random);
    
    
    // Set output file (save game), if asked to in arguments
    if (save) {
        ofile.open(ofilematch.c_str());
        if (!ofile) {
            game.~Game();
            errorBreak (ERROR_OPEN, ifilematch) ; //
        }
        output=&ofile;
    }
    

    // Game's main loop.
    
    // 1) Set the size of the window according to the size (rows & columns) of the new Tiles
    game.setWindowSize();
    
    while (!end)  {
        
	// 2) Given the inner data members, it pretty-prints the screen
        error=0 ;
        game.player.add(game.bag.extract(7-game.player.size())); 
        game.doPaint();
        
        // 3) Reads the movement from cin
        cin >> move;
        word = move.getLetters();
        move.setScore(move.findScore(game.language));
        
        if (word == END)  
            end=true;
        
        else {   
            evaluateMove(game, move, game.player, error);

            if (error!=0) {                       //move en plater, todo crossword está en diccionario, dentro del tablero y en posiciones válidas
                game.rejectedmovements += move;
                game.doBadCrosswords(errorManagement(error));
            }
            
            else if (game.doConfirmCrosswords("Confirm movement?")) {
                game.player -= word;
                        
                for (int i=0; i<game.crosswords.size(); i++)
                    game.tiles += game.crosswords.get(i) ;
                        
                game.score += game.crosswords.getScore();
                game.acceptedmovements += move ;
            }    
        }  
    }
    
    // End of game
    // Save file or print screen
    if (output == &cout) 
        *output << endl << "%%%OUTPUT" << endl;     //test
    else
        *output << PASSWORD2 << endl ;
        
    *output << game ;
        
    return 0;
}

void checkArguments (int nargs, char *arguments[], Game &game, string &ifilename, string &ofilename, string &lang, string &bag, 
                     int &h, int &w, int &random, bool &res, bool &save) {
    
    string s;
    
    if (nargs<3 || nargs>13 || nargs%2==0) {
        cerr << endl << "numer" << endl ;
        game.~Game();                           
        errorBreak (ERROR_ARGUMENTS, "") ;
    }
    
    for (int i=1 ; i<nargs; ) {
        s=arguments[i];
        i++;
        
        if (s=="-open") {
            
            if ( h != -1 || w != -1 || random != -1 || lang != "" || bag != "") {   // se ha leido -r, -h, -w (...) antes --> no compatible con el formato requerido de los argumentos
                cerr << endl << "ERROR 1" << endl ;
                game.~Game();
                errorBreak (ERROR_ARGUMENTS, "") ;
            }
            
            res=true;
            ifilename = arguments[i++];
            
            //int len=ifilename.size();
            //string file_format=ifilename.substr(len-FORMAT.size(), FORMAT.size());
            //if ( file_format != FORMAT ) {
            //    game.~Game();     
            //    cerr << "Please use a matchfile:  -p <*.match>" << endl ;
            //    errorBreak (ERROR_ARGUMENTS, "") ;
            //}
        } 
        
        else if (s=="-l" && !res) 
            lang=arguments[i++] ;
        
        else if (s=="-w" && !res) {
            if (!isdigit(*arguments[i])) { 
                cerr << endl << "dig w" << endl ;
                game.~Game();
                errorBreak (ERROR_ARGUMENTS, "") ; 
            }
            w = atoi(arguments[i++]);
        }
    
        else if (s=="-h" && !res) {
            if (!isdigit(*arguments[i])) {
                cerr << endl << "dig h" << endl ;
                game.~Game();
                errorBreak (ERROR_ARGUMENTS, "") ;
            }
            h = atoi(arguments[i++]);
        }
        
        else if (s=="-r" && !res) {
            if (!isdigit(*arguments[i])) {
                game.~Game();
                errorBreak (ERROR_ARGUMENTS, "") ; 
            }
            random = atoi(arguments[i++]);
        } 
    
        else if (s=="-save") {
            ofilename = arguments[i++];
            
            //Comprueba formato
            //int len=ofilename.size();
            //string file_format=ofilename.substr(len-FORMAT.size(), FORMAT.size());
            //if (file_format != FORMAT) {
            //    game.~Game();
            //    cerr << " >> Please use a matchfile: -s <*.match>" << endl ;
            //    errorBreak (ERROR_ARGUMENTS, "") ;
            //}
        }
        
        else if (s=="-b" && !res)
            bag = arguments[i++] ;
        
        else {
            cerr << endl << "error else" << endl ;
            game.~Game();
            errorBreak (ERROR_ARGUMENTS, "") ;
        }  
    } 
    
    if (ofilename!="")
        save=true;
}


void setGame (Game &game, string &lang, string &bag, int h, int w, int random) {
    
    if (lang == "") {
        game.~Game();
        errorBreak (ERROR_ARGUMENTS, "") ;
    }
    else
        game.language.setLanguage(lang);
    
    if (h == -1 || w == -1) {
        game.~Game();
        errorBreak (ERROR_ARGUMENTS, "") ;
    }
    else 
        game.tiles.setSize(h, w);
    
    if (bag=="") {
        if (random != -1)
            game.bag.setRandom(random);
        game.bag.define(lang);
    }
    else
        game.bag.set(toISO(bag));
}



void evaluateMove (Game &game, Move &m, Player &player, int &error_code)  {
    bool valid=true ;
    string word=m.getLetters();
    
    if (player.isValid(word)) {
        game.crosswords = game.tiles.findCrosswords(m,game.language);    
        game.showCrosswords();
    }        
    else 
        error_code=INFEASIBLE_WORD;
        
    if (game.crosswords.size()==0) {                                                        //valid=false --> fuera de tablero
        valid=false;
        error_code = BOARD_OVERFLOW;
    }
    else if (game.acceptedmovements.size()>0 && game.crosswords.size()<1 && valid) {        //valid=false --> no hay cruces 
        valid = false;
        error_code = MISSING_CROSSWORDS ;
    }
    else
        for (int i = 0; i<game.crosswords.size() && valid; i++) {
            valid = game.crosswords.get(i).findScore(game.language) != NONEXISTENT_WORD ;   //valid=false --> no esta en diccionario
            if (!valid)
                error_code = NONEXISTENT_WORD;
        } 
}


ostream & operator<<(ostream & os, const Game &game)  {
    os << game.score << endl ;
    os << game.language.getLanguage() << endl ;
    os << game.tiles.getHeight() << " " << game.tiles.getWidth() << endl << game.tiles ;
    os << game.player.size() << " " << toUTF(game.player.to_string()) << endl ;
    os << game.bag.size() << " " << toUTF(game.bag.to_string()) << endl ;
    os << game.acceptedmovements.size() << endl << game.acceptedmovements << endl ;
    os << game.rejectedmovements.size() << endl << game.rejectedmovements << endl ;
}

//si cuando salidos de >>, game está vacio --> error data !!
istream & operator>>(istream &is, Game &game) {
    string key, player, bag, l;
    int score, h, w, size=-1 ;
    Move move;
    
    is >> key ;         //key
    if (key != PASSWORD2 && key != PASSWORD1) 
        game.score = ERROR_DATA;
                
    is >> score ;  
    game.score = score;
    
    is >> l ;
    game.language.setLanguage(l) ;
    
    is >> h >> w ;
    game.tiles.setSize(h, w);
       
    is >> game.tiles ;
         
    is >> size;         //player size
    is >> player ;
    player = toISO(player);
    if (is.eof() || player.size() != size )     
        game.score = ERROR_DATA;
    else
        game.player.add(player);
            
    is >> size;         // bag size
    is >> bag;
    bag = toISO(bag);
    if (is.eof() || bag.size() != size ) {
        game.score = ERROR_DATA;
    }
        
    else
        game.bag.set(bag);
        
    size=-1;
    if (!is.eof())
        is >> size;     //accepted size
    for (int i=0; i < size; i++){
        is >> move;
        game.acceptedmovements += move;
    }
    
    size=-1;
    if (!is.eof())
        is >> size;     //rejected size
    for (int i=0; i < size; i++){
        is >> move;
        game.rejectedmovements += move;
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
