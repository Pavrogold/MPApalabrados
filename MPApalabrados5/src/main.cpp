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
    
    int random=-1, score=0, p_size, b_size, w, h;
    string lang = "", b_secuencia="",
           pfile_name="", mfile_name ="", ofilename="",  key="", word = "" ;
    
    ifstream matchfile, playfile; 
    ofstream ofile;
    istream *input, *i_playfile; 
    ostream *output;
    
    input=&cin;
    output = &cout;
    
    if (nargs<1 || nargs>15 || nargs%2==0)
        errorBreak (ERROR_ARGUMENTS, "") ;
    
    string s;
    bool restored=false;
    for (int i=1 ; i<nargs; ) {
        
        s=args[i];
        i++;
        if (s=="-open") {
            restored=true;
            
            mfile_name = args[i];
            i++;
            matchfile.open(mfile_name.c_str());
            if (!matchfile) //@warning
                errorBreak (ERROR_OPEN, mfile_name) ;
            input=&matchfile;
            
            *input >> key ;
            if (key != PASSWORD) 
                errorBreak (ERROR_DATA, mfile_name) ;
            
            *input >> score ;
            *input >> lang ;
            
            int r, c;
            *input >> r >> c;
            tablero.setSize(r, c);
            
            tablero.read(*input);
            
            *input >> p_size;
            string p_secuencia;
            *input >> p_secuencia ;
            player.add(toISO(p_secuencia));
            if (player.size() != p_size) {
                errorBreak (ERROR_DATA, mfile_name) ;
            }
            
            *input >> b_size;
            *input >> b_secuencia;
            bag.set(toISO(b_secuencia));
            if (bag.size() != b_size) {
                errorBreak (ERROR_DATA, mfile_name) ;
            }
            
            matchfile.close();
            input=nullptr; //?
        } 
        
        else if (s=="-p") {
            pfile_name = args[i];
            i++;
            playfile.open(pfile_name.c_str());
            if (!playfile) //@warning
                errorBreak (ERROR_OPEN, pfile_name) ;
            input=&playfile;
           
            //cin.operator<<(original);   //???
            *input >> original; //@warning
            
            playfile.close();
            input=nullptr; //?
        } 
        
            
        else    if (s=="-l" && !restored) {
                lang=args[i] ;
                i++;
            }
            else if (s=="-w" && !restored) {
                if (!isdigit(*args[i])) //?
                    errorBreak (ERROR_ARGUMENTS, "") ; 
                w = atoi(args[i]);
                i++;
            }
            else if (s=="-h" && !restored) {
                if (!isdigit(*args[i])) 
                    errorBreak (ERROR_ARGUMENTS, "") ; 
                h = atoi(args[i]);
                i++;
            }
            else if (s=="-r" && !restored) {
                if (!isdigit(*args[i])) 
                    errorBreak (ERROR_ARGUMENTS, "") ; 
                random = atoi(args[i]);
                i++;
                bag.setRandom(random) ;
            }
        
        else if (s=="-save") {
            ofilename = args[i];
            i++;
            //comprobar extension ?
            ofile.open(ofilename.c_str());
            if (!ofile) 
                errorBreak (ERROR_OPEN, ofilename) ;
            output=&ofile;
        }     
        
        else
            errorBreak (ERROR_ARGUMENTS, "") ; 
    }
    
    if (!restored)
        tablero.setSize(w, h);
    
    if (pfile_name=="")
        errorBreak(ERROR_ARGUMENTS, "");
    
    if (lang == "") 
        errorBreak(ERROR_ARGUMENTS, "");
    else
        language.setLanguage(lang);
    
    if (b_secuencia == "")
        bag.define(language) ;
    
    if (ofilename=="") {
        cout << "\nOUTPUT: CONSOLE\n\n";
    }
    
    
    *output << "\nALLOWED LETTERS: " << toUTF(language.getLetterSet()) << endl;
    *output << "BAG ("<<bag.size()<<"): " << toUTF(bag.to_string()) << endl;
    *output << "PLAYER: " << toUTF(player.to_string()) << endl;
    
    
    //assign original to legal, and remove invalid moves
    legal = original;
    legal.zip(language);
    
    for (int i = 0; i < legal.size(); i++){
        
        move=legal.get(i);  
        word=move.getLetters();
        //word=toISO(move.getLetters());
        //*output << "\n\nPLAYER: " << toUTF(player.to_string()) << endl;
        //*output << "WORD: " << toUTF(word) ;
        bool probando = true;
        if (player.isValid(word) || probando){
            player.extract(word);
            player.add(bag.extract(7-player.size())) ;
            
            score=move.findScore(language);
            move.setScore(score);
            accepted.add(move);
            
            tablero.add (move);
            cout << endl << endl << endl ;
            tablero.print(*output); //para comprobar salida (provisional)
        }
        else {
            rejected.add(move);
            *output << "\n >> REJECTED! " << endl;
        }
    } 
    
    
    //Resultado final:
    *output << PASSWORD << endl ;
    *output << score ;
    *output << toUTF(language.getLetterSet()) << endl;
    *output << tablero.getHeight() << " " << tablero.getWidth() << endl ;
    tablero.print(*output);
    *output << endl ;
    *output << player.size() << " " << toUTF(player.to_string()) << endl;
    *output << bag.size() << " " << toUTF(bag.to_string()) << endl;
    
    HallOfFame(language, random, bag, player, 
            tablero, original, legal, accepted, rejected); 
    cout << endl;
    
    if (output != &cout)
        ofile.close() ;
    
    return 0;
}


void HallOfFame(const Language &l, int random, const Bag &b, const Player &p, 
        const Tiles &t, const Movelist& original,const Movelist& legal,
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
