/**
 * @file main.cpp
 * @author Yesenia González Dávila, Pablo Millán Cubero
 * @note To be implemented by students either completely or by giving them
 * key functions prototipes to guide the implementation
 */

//Alumno 1: Pablo Millan Cubero
//Alumno 2: Yesenia Gonzalez Davila


#include <iostream>
#include <fstream>
#include <string>
#include <cassert>

#include "language.h"
#include "wordlist.h"
#include "bag.h"
#include "player.h"
#include "move.h"
#include "movelist.h"
#include "tiles.h"

using namespace std;

#define ERROR_ARGUMENTS 1
#define ERROR_OPEN 2
#define ERROR_DATA 3
#define PASSWORD "MPALABRADOS-V1"
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
 * @brief Shows final data
 * @param l Language
 * @param random Random seed
 * @param b Final bag
 * @param p Final player
 * @param t Final tiles
 * @param original
 * @param legal
 * @param accepted
 * @param rejected
 */
void HallOfFame(const Language &l, int random, const Bag &b, 
        const Player &p, const Tiles &t,
        const Movelist& original,const Movelist& legal,
        const Movelist& accepted,const Movelist& rejected);


/**
 * @brief Main function. 
 * @return 
 */
int main(int nargs, char * args[]) {
    Bag bag;
    Player player;
    Language language;
    Move move;
    Movelist original,          /// Original list of movements
            legal,              /// Movements with legal words upon the dictionary
            accepted,           /// Movements accepted in the game
            rejected;           /// Movements not accepted in the game
    Tiles tablero;
    
    int random=-1, score=0, p_size, b_size;
    string lang = "", b_sec="", ofilename="", secuencia ="", match_f ="", password="", word = "" ;
    
    ifstream matchfile, playfile; 
    ofstream ofile;
    istream *input; 
    ostream *output;
    
    input=&cin;
    output=&cout;
    
    if (nargs<1 || nargs>15 || nargs%2==0)
        errorBreak (ERROR_ARGUMENTS, "") ;
    
    string s;
    for (int i=1 ; i<nargs;) {
        
        s=args[i++];
        if (s=="-open") {
            match_f = args[i++];
            matchfile.open(match_f.c_str());
            if (!matchfile) 
                errorBreak (ERROR_OPEN, savefile) ;
            input=&matchfile;
            
            *input >> password ;
            if (password != "MPALABRADOS-V1") {
                //ERROR FILE
            }
            *input >> score ;
            
            *input >> lang ;
            language.setLanguage(lang);
            
            *input >> tablero.getWidth();
            *input >> tablero.getHeight();
            
            tablero.read(*input);
            
            *input >> p_size;
            *input >> secuencia ;
            player.add(toISO(secuencia));
            if (player.size() != p_size) {
                //ERROR?
            }
            
            *input >> b_size;
            *input >> b_sec;
            bag.set(toISO(b_sec));
            
            if (bag.size() != b_size) {
                //ERROR?
            }
        } 
        
        else if (s=="-p") {
            *input >> original //@warning
        } 
        
        else if (s=="-l") {
            lang=args[i++] ;
            language.setLanguage(lang);
        }
        
        else if (s=="-w") {
            if (!isdigit(*args[i])) 
                errorBreak (ERROR_ARGUMENTS, "") ; 
            int w = atoi(args[i++]);
        }
        
        else if (s=="-h") {
            if (!isdigit(*args[i])) 
                errorBreak (ERROR_ARGUMENTS, "") ; 
            int h = atoi(args[i++]);
        }
        
        else if (s=="-r") {
            if (!isdigit(*args[i])) 
                errorBreak (ERROR_ARGUMENTS, "") ; 
            random = atoi(args[i++]);
            bag.setRandom(random) ;
        }
         
        else if (s=="-save") {
            ofilename = args[i++];
            if (ofilename) //comprobar extension!
            ofile.open(ofilename.c_str());
            if (!ofile) 
                errorBreak (ERROR_OPEN, ofilename) ;
            output=&ofile;
        }
        
        else
            errorBreak (ERROR_ARGUMENTS, "") ; 
    }
    
    if (lang == "") 
        errorBreak(ERROR_ARGUMENTS, "");
    if (b_sec == "")
        bag.define(language) ;
    
    *output << "\nALLOWED LETTERS: " << toUTF(language.getLetterSet()) << endl;
    *output << "BAG ("<<bag.size()<<"): " << toUTF(bag.to_string()) << endl;
    *output << "PLAYER: " << toUTF(player.to_string()) << endl;
    
    //leer tablero si no -open !
    
    //set score for each move
    for (int i=0; i<movements.size(); i++ ) {
        move=movements.get(i);
        score=move.findScore(language);
        move.setScore(score);
        movements.set(i, move);
    }
    
    //assign original to legal, and remove invalid moves
    legal = original;
    legal.zip(language);
    
    for (int i = 0; i < legal.size(); i++){
        
        move=legal.get(i);  //operadores?
        word=move.getLetters();
        //word=toISO(move.getLetters());
        *output << "\n\nPLAYER: " << toUTF(player.to_string()) << endl;
        *output << "WORD: " << toUTF(word) ;
        
        if (player.isValid(word)){
            player.extract(word);
            player.add(bag.extract(7-player.size())) ;
            
            score=move.findScore(language);
            move.setScore(score);
            accepted.add(move);
            
            *output << "\tACCEPTED! " << endl;
            
            tablero.add (move);
            tablero.print(*output); //comprobar salida
        }
        else{
            rejected.add(move);
            *output << "\tREJECTED! " << endl;
        }
    } 
    
    cout << endl ;
    tablero.print(*output);
    cout << endl ;
    
    HallOfFame(language, random, bag, player, 
            tablero, original, legal, accepted, rejected); 
    cout << endl;
    
    
    //corregir
    playfile.close();
    if (input != &cin)
        ifile.close() ;
    if (output != &cout)
        ofile.close() ;
    
    return 0;
}


//cambiar 
void HallOfFame(const Language &l, int random, const Bag &b, const Player &p, 
        const Movelist& original,const Movelist& legal,
        const Movelist& accepted,const Movelist& rejected) {
    cout << endl << "%%%OUTPUT" << endl << "LANGUAGE: "<<l.getLanguage()<< " ID: " << random << endl;
    cout << "BAG ("<<b.size()<<"): " << toUTF(b.to_string()) << endl;
    cout << "PLAYER (" <<p.size() << "): " << toUTF(p.to_string());
    cout << endl << endl << "ORIGINAL ("<<original.size()<<"): "<<endl; original.print(cout);
    cout << endl << endl << "LEGAL ("<<legal.size()<<"): "<<endl; legal.print(cout);
    cout << endl << endl << "ACCEPTED ("<<accepted.size()<<") SCORE "<<accepted.getScore()<< ": "<<endl; accepted.print(cout);
    cout << endl << endl << "REJECTED ("<<rejected.size()<<"): "<<endl; rejected.print(cout);
    cout << endl;
}

void errorBreak(int errorcode, const string &errordata) {
    cerr << endl << "%%%OUTPUT" << endl;
    switch(errorcode) {
        case ERROR_ARGUMENTS:
            cerr<<"Error in call. Please use either:"<<endl;
            cerr<< "-l <language> -w <width> -h <height> -p <playfile> [-b <bag> -r <randomnumber> -save <matchfile>]"<<endl;
            cerr<< "-open <matchfile> -p <playfile> [-save <matchfile>]"<<endl;            
            break;
        case ERROR_OPEN:
            cerr<<"Error opening file "<<errordata << endl;
            break;
        case ERROR_DATA:
            cerr<<"Data error in file "<<errordata << endl;
            break;
    }
    std::exit(1);
}
