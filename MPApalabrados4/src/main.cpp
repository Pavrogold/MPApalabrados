/**
 * @file main.cpp
 * @author Yesenia González Dávila, Pablo Millán Cubero
 * @note To be implemented by students either completely or by giving them
 * key functions prototipes to guide the implementation
 */

//Alumno 1: Pablo Millan Cubero
//Alumno 2: Yesenia Gonzale Davila


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

using namespace std;

#define ERROR_ARGUMENTS 1
#define ERROR_OPEN 2
#define ERROR_DATA 3
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
 * @param original
 * @param legal
 * @param accepted
 * @param rejected
 */
void HallOfFame(const Language &l, int random, const Bag &b, const Player &p, 
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
    Movelist movements,        /// Original list of movements
            legalmovements,    /// Movements with legal words upon the dictionary
            acceptedmovements, /// Movements accepted in the game
            rejectedmovements; /// Movements not accepted in the game
    
    int random=-1, score;
    
    string
        lang = "",
        ofilename="",
        secuencia ="",
        savefile ="",
        word = "" ;
    
    ifstream ifile, playfile; 
    ofstream ofile;
    istream *input; 
    ostream *output;
    
    input=&cin;
    output=&cout;
    
    if (nargs<1 || nargs>9 || nargs%2==0)
        errorBreak (ERROR_ARGUMENTS, "") ;
    
    string s;
    for (int i=1 ; i<nargs;) {
        s=args[i++];
        
        if (s=="-l"){ 
            lang=args[i++] ;
            language.setLanguage(lang);
        }
        else if (s=="-r") {
            if (!isdigit(*args[i])) 
                errorBreak (ERROR_ARGUMENTS, "") ; 
            random = atoi(args[i++]);
            bag.setRandom(random) ;
        }
        
        else if (s == "-p" || s=="-i"){     // test con parametro -i?
            savefile = args[i++];
            playfile.open(savefile.c_str());
            if (!playfile) 
                errorBreak (ERROR_OPEN, savefile) ;
            input=&playfile;
        }
        else if (s=="-b") 
            secuencia=args[i++] ;
        else
            errorBreak (ERROR_ARGUMENTS, "") ; 
    }
    
    if (savefile == "" || lang == "") {
        /*
        if (input != &cin)
            ifile.close() ;
        if (output != &cout)
            ofile.close() ;
        */
        errorBreak(ERROR_ARGUMENTS, "");
    }
        
    
    if (secuencia == "")
        bag.define(language) ;
    else
        bag.set(toISO(secuencia));
    
    player.add(bag.extract(7));

    *output << endl << "ID:" << random ; 
    *output << "\nALLOWED LETTERS: " << toUTF(language.getLetterSet()) << endl;
    *output << "BAG ("<<bag.size()<<"): " << toUTF(bag.to_string()) << endl;
    *output << "PLAYER: " << toUTF(player.to_string()) << endl;
    *output << "READ: \n\n";

    
    //leer el playfile e introducirlo en movements
    if (!movements.read(*input))
        errorBreak(ERROR_DATA, savefile);
    
    //set score for each move
    for (int i=0; i<movements.size(); i++ ) {
        move=movements.get(i);
        score=move.findScore(language);
        move.setScore(score);
        movements.set(i, move);
    }
    
    
    //Copiar original a legal y eliminar los no legales
    legalmovements.assign(movements);
    legalmovements.zip(language);
    
    for (int i = 0; i < legalmovements.size(); i++){
        
        move=legalmovements.get(i);
        word=move.getLetters();
        //word=toISO(move.getLetters());
        *output << "\n\nPLAYER: " << toUTF(player.to_string()) << endl;
        *output << "WORD: " << toUTF(word) ;
        
        if (player.isValid(word)){
            player.extract(word);
            player.add(bag.extract(7-player.size())) ;
            acceptedmovements.add(move);
            
            *output << "\tACCEPTED! " << endl;
        }
        else{
            rejectedmovements.add(move);
            *output << "\tREJECTED! " << endl;
        }
    } 
    
    cout << endl ;
    HallOfFame(language, random, bag, player, 
            movements, legalmovements, acceptedmovements, rejectedmovements); 
    cout << endl;
    
    playfile.close();
    if (input != &cin)
        ifile.close() ;
    if (output != &cout)
        ofile.close() ;
    
    return 0;
}

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
    cerr << endl << "%%%VALGRIND" << endl << "%%%OUTPUT" << endl;
    switch(errorcode) {
        case ERROR_ARGUMENTS:
            cerr<<"Error in call. Please use:\n -l <language> -p <playfile> [-r <randomnumber>]";
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
