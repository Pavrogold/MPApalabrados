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
string errorManagement(int errorcode);


/**
 * @brief Main function. 
 * @return 
 */
int main(int nargs, char * args[]) {
    Move move;
    Game game;
    Window window;
    int w=-1, h=-1, wait=0, r, c, total_score=0, p_size, b_size, random, error;
    string lang="",ifilematch="", ofilematch="", word, mfile_name, key="", 
            b_secuencia="", p_secuencia="", pfile_name="",ofilename="";
    const string FORMAT=".match";
    ifstream ifile, matchfile, playfile, *input;
    ofstream ofile, *output;
    //output=&cout;
    
    bool end=false, valid;
    char cc;
    
    /// Check arguments
    
    cout << nargs;
    if (nargs<1 || nargs>15 || nargs%2==0) {
        //original.~Movelist(); rejected.~Movelist(); accepted.~Movelist(); legal.~Movelist(); tablero.~Tiles(); tablero.~Tiles();      //Destructores
        errorBreak (ERROR_ARGUMENTS, "") ;
    }

    // Process arguments
    string s;
    bool restored=false;
    for (int i=1 ; i<nargs; ) {
        
        s=args[i];
        i++;
        
        if (s=="-open") {
            restored=true;
            mfile_name = args[i++];
            
            //Comprueba formato
            int len=mfile_name.size();
            string file_format=mfile_name.substr(len-FORMAT.size(), FORMAT.size());
            if ( file_format != FORMAT ) {
                //original.~Movelist(); rejected.~Movelist(); accepted.~Movelist(); legal.~Movelist(); tablero.~Tiles();      //Destructores
                cerr << "Please use a matchfile:  -p <*.match>" << endl ;
                errorBreak (ERROR_ARGUMENTS, "") ;
            }
            
            matchfile.open(mfile_name.c_str());
            if (!matchfile) {
                //original.~Movelist(); rejected.~Movelist(); accepted.~Movelist(); legal.~Movelist(); tablero.~Tiles();      //Destructores
                errorBreak (ERROR_OPEN, mfile_name) ;
            }
            input=&matchfile;
            
            *input >> key ;
            if (key != PASSWORD) {
                //original.~Movelist(); rejected.~Movelist(); accepted.~Movelist(); legal.~Movelist(); tablero.~Tiles();      //Destructores
                errorBreak (ERROR_DATA, mfile_name) ;
            }
                
            *input >> total_score ;  
            *input >> lang ;
            *input >> r >> c ;
            game.tiles.setSize(r, c);
            
            *input >> game.tiles;
            
            *input >> p_size;
            *input >> p_secuencia ;
            game.player.add(toISO(p_secuencia));
            if (game.player.size() != p_size) {
                //original.~Movelist(); rejected.~Movelist(); accepted.~Movelist(); legal.~Movelist(); tablero.~Tiles();      //Destructores
                errorBreak (ERROR_DATA, mfile_name) ;
            }
               
            
            *input >> b_size;
            *input >> b_secuencia;
            game.bag.set(toISO(b_secuencia));
            if (game.bag.size() != b_size) {
                //original.~Movelist(); rejected.~Movelist(); accepted.~Movelist(); legal.~Movelist(); tablero.~Tiles();      //Destructores
                errorBreak (ERROR_DATA, mfile_name) ;
            }
            
            input=nullptr;
            matchfile.close();
        }
        
        /*
         else if (s=="-p") {  comento el p porque no es necesario en esta práctica
            pfile_name = args[i++];
            playfile.open(pfile_name.c_str());
            if (!playfile) {
                //original.~Movelist(); rejected.~Movelist(); accepted.~Movelist(); legal.~Movelist(); tablero.~Tiles();      //Destructores
                errorBreak (ERROR_OPEN, pfile_name) ;
            }
            
            input=&playfile;
        
            *input >> original;o
            if (original.size()==0) {
                //original.~Movelist(); rejected.~Movelist(); accepted.~Movelist(); legal.~Movelist(); tablero.~Tiles();      //Destructores
                errorBreak (ERROR_DATA, pfile_name) ;
            }
        
            
            
            input=nullptr; 
            playfile.close();
        }   
        */  
        
        else if (s=="-l" && !restored)
            lang=args[i++] ;
        
        else if (s=="-w" && !restored) {
            if (!isdigit(*args[i])) { //? 
                //original.~Movelist(); rejected.~Movelist(); accepted.~Movelist(); legal.~Movelist(); tablero.~Tiles();      //Destructores
                cout << 9;
                errorBreak (ERROR_ARGUMENTS, "") ; 
            }
            c = atoi(args[i++]);
        }
    
        else if (s=="-h" && !restored) {
            if (!isdigit(*args[i])) {
                //original.~Movelist(); rejected.~Movelist(); accepted.~Movelist(); legal.~Movelist(); tablero.~Tiles();      //Destructores
                errorBreak (ERROR_ARGUMENTS, "") ;
            }
            r = atoi(args[i++]);
        }
        
        else if (s=="-r" && !restored) {
            if (!isdigit(*args[i])) {
                //original.~Movelist(); rejected.~Movelist(); accepted.~Movelist(); legal.~Movelist(); tablero.~Tiles();      //Destructores
                errorBreak (ERROR_ARGUMENTS, "") ; 
            }

            random = atoi(args[i++]);
            game.bag.setRandom(random) ;

        } 
    
        else if (s=="-save") {
            ofilename = args[i++];
            
            //Comprueba formato
            int len=ofilename.size();
            string file_format=ofilename.substr(len-FORMAT.size(), FORMAT.size());
            if (file_format != FORMAT) {
                //original.~Movelist(); rejected.~Movelist(); accepted.~Movelist(); legal.~Movelist(); tablero.~Tiles();      //Destructores
                cerr << " >> Please use a matchfile:  -s <*.match>" << endl ;
                errorBreak (ERROR_ARGUMENTS, "") ;
            }
            
            ofile.open(ofilename.c_str());
            if (!ofile) {
                //original.~Movelist(); rejected.~Movelist(); accepted.~Movelist(); legal.~Movelist(); tablero.~Tiles();      //Destructores
                errorBreak (ERROR_OPEN, ofilename) ;
            }
            output=&ofile;
        }
        
        else if (s=="-b") { 
            s=args[i++];
            b_secuencia = s ;
            game.bag.set(toISO(b_secuencia));
        }
        
        else {
            //original.~Movelist(); rejected.~Movelist(); accepted.~Movelist(); legal.~Movelist(); tablero.~Tiles();      //Destructores
            errorBreak (ERROR_ARGUMENTS, "") ;
        }  
    }

    if (r==-1 && c==-1) {
        //original.~Movelist(); rejected.~Movelist(); accepted.~Movelist(); legal.~Movelist(); tablero.~Tiles();      //Destructores
        errorBreak(ERROR_ARGUMENTS, "");
    }
    else if (!restored)
        game.tiles.setSize(r, c);
    
    /*if (pfile_name=="") {
        //original.~Movelist(); rejected.~Movelist(); accepted.~Movelist(); legal.~Movelist(); tablero.~Tiles();      //Destructores
        errorBreak(ERROR_ARGUMENTS, "");
    }*/
    
    if (lang == "")  {
        //original.~Movelist(); rejected.~Movelist(); accepted.~Movelist(); legal.~Movelist(); tablero.~Tiles();      //Destructores
        errorBreak(ERROR_ARGUMENTS, "");
    }   
    else
        game.language.setLanguage(lang);
    
    if (b_secuencia == "")
        game.bag.define(game.language) ;
    
    if (p_secuencia == "")
        game.player.add(game.bag.extract(7-game.player.size()));

    // Game's main loop 
    // 1) First set the size of the window according to the size (rows & columns) of
    // the new Tiles
    
    game.setWindowSize();


    while (!end)  { // && cc == 'y' || cc == 'Y'
        error = 0;
        system("clear");
        game.player.add(game.bag.extract(7-game.player.size()));
        game.doPaint();

        
        // 3) Reads the movement from cin
        cin >> move;
        word = move.getLetters();
        move.setScore(move.findScore(game.language));
        if (word=="_") {
            end=true;
        }
        else {

            if (game.player.isValid(word)){ //si move esta en player
                
                game.player.extract(word);
                move.setScore(move.findScore(game.language));
                game.crosswords = game.tiles.findCrosswords(move,game.language);
                game.showCrosswords();
                
                valid = true;
                for (int i = 0; i < game.crosswords.size() && valid; i++){
                    if(game.crosswords.get(i).findScore(game.language) < 0)
                        valid = false;
                }
                
                
                
                if (game.acceptedmovements.size() > 0 && game.crosswords.size() <= 1){
                    valid = false;
                    error = MISSING_CROSSWORDS;
                }

                
                if (valid){ // si cumple: la palabra esta en el diccionario, se sale,
                            // hace más de un cruce si no es la primera y todo crosswords está en el diccionario

                    if (game.doConfirmCrosswords("Confirm movement?")){ //si confirma movimiento
                        
                        game.tiles.add(move);
                        
                        game.score += game.crosswords.getScore();
                        game.acceptedmovements += move;

                    }
                }
                else{  //si no es valido move
                    error = move.getScore();
                    game.rejectedmovements += move;
                }
            }
            else //si move no esta en player 
                error = INFEASIBLE_WORD;
            
        }
        if (error != 0)
            game.doBadCrosswords(errorManagement(error));
        
    }
    // End of game
    // Save file or print screen
 
    return 0;
}

ostream & operator<<(ostream & os, const Game & game)  {
    // To implement
}

istream & operator>>(istream & is, Game &game) {
    // To implement
}

void errorBreak(int errorcode, const string &errordata) {
    cerr << endl << "%%%OUTPUT" << endl;
    switch(errorcode) {
        case ERROR_ARGUMENTS:
            cerr<<"Error in call. Please use either:"<<endl;
            cerr<< "-l <language> -w <width> -h <height> [-r <randomnumber> -save <matchfile>]"<<endl;
            cerr<< "-open <matchfile> [-save <matchfile>]"<<endl;            
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
