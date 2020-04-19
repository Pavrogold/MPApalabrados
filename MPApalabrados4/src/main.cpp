/**
 * @file main.cpp
 * @author DECSAI
 * @note To be implemented by students either completely or by giving them
 * key functions prototipes to guide the implementation
 */


#include <iostream>
#include <fstream>

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
    
    int 
        random=-1;
    
    string
        lang = "",
        ifilename = "",
        ofilename="",
        secuencia ="",
        savefile ="",
        word = "",
        result;   
    
    ifstream ifile; 
    ofstream ofile;
    istream *input; 
    ostream *output;
    
    ifstream playfile;
    
    input=&cin;
    output=&cout;
    
    if (nargs<1 || nargs>9 || nargs%2==0)
        errorBreak (ERROR_ARGUMENTS, "") ;
    
    for (int i = 1; i < nargs; i++){
        
        if (arg[i] == "-r"){
            if (!isdigit(args[i])) 
                errorBreak (ERROR_ARGUMENTS, "") ; 
            random = atoi(arg[i++]);
            bag.setRandom(random);
        }
        
        if (arg[i] == "-l"){
            lang = arg[i++];
            language.setLanguage(lang);
        }
            
        if (arg[i] == "-p"){
            savefile = arg[i++];
            playfile.open(savefile.c_str());
            if (!playfile) 
                errorBreak (ERROR_OPEN, ifilename) ;
        }
        
        if (arg[i] == "-b"){
            secuencia = toISO(arg[i++]);
        }
    }
    
    if (savefile == "" || lang == "")
        errorBreak(ERROR_ARGUMENTS, "");
    
    cout << "\nConjunto de caracteres: " << language.getLetterSet() << endl;
    
    bag.define(language);
    player.add(bag.extract(7));
    
    *output << endl << "ID:" << random ; 
    *output << "\nALLOWED LETTERS: " << toUTF(language.getLetterSet()) << endl;
    *output << "BAG ("<<bag.size()<<"): " << toUTF(bag.to_string()) << endl;
    *output << "PLAYER: " << toUTF(player.to_string()) << endl;
    
    //leer el playfile e introducirlo en original
    move.read(playfile);
    word = move.getLetters();
    
    while (!playfile.eof()){
        movements.add(move);
        move.read(playfile);
        word = move.getLetters();
    }
    
    playfile.close();
    word = "";
    
    //copiar original a legal y eliminar los no legales
    legalmovements.assign(movements);
    legalmovements.zip(language);
    
    for (int i = 0; i < legalmovements.size(); i++){
        
        move = legalmovements.get(i);
        word = move.getLetters();
        
        if (player.isValid(word)){
            
            player.extract(word);
            acceptedmovements.add(move);
            
            *output << "ACCEPTED: " << word << endl;
        }
        
        else{
            
            rejectedmovements.add(move);
            
            *output << "REJECTED: " << word << endl;
        }
    }

    HallOfFame(language, random, bag, player, 
            movements, legalmovements, acceptedmovements, rejectedmovements);
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
    cerr << endl << "%%%OUTPUT" << endl;
    switch(errorcode) {
        case ERROR_ARGUMENTS:
            cerr<<"Error in call. Please use:\n -l <language> -p <playfile> [-r <randomnumber>]"<<endl;
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