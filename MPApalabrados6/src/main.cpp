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
#include <locale> 

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
    
    int random=-1, score=0, p_size, b_size, r=-1, c=-1, total_score=0 ;
    string lang = "", b_secuencia="", p_secuencia="", key="", word = "", pfile_name="", mfile_name ="", ofilename="CONSOLE" ;
    const string FORMAT=".match";
    
    ifstream matchfile, playfile; 
    ofstream ofile;
    istream *input ; 
    ostream *output;
    output=&cout;
    
    
    //Preconditions
    if (nargs<1 || nargs>15 || nargs%2==0) {
        original.~Movelist(); rejected.~Movelist(); accepted.~Movelist(); legal.~Movelist(); tablero.~Tiles(); tablero.~Tiles();      //Destructores
        errorBreak (ERROR_ARGUMENTS, "") ;
    }
    
    
    //Read args
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
                original.~Movelist(); rejected.~Movelist(); accepted.~Movelist(); legal.~Movelist(); tablero.~Tiles();      //Destructores
                cerr << "Please use a matchfile:  -p <*.match>" << endl ;
                errorBreak (ERROR_ARGUMENTS, "") ;
            }
            
            matchfile.open(mfile_name.c_str());
            if (!matchfile) {
                original.~Movelist(); rejected.~Movelist(); accepted.~Movelist(); legal.~Movelist(); tablero.~Tiles();      //Destructores
                errorBreak (ERROR_OPEN, mfile_name) ;
            }
            input=&matchfile;
            
            *input >> key ;
            if (key != PASSWORD) {
                original.~Movelist(); rejected.~Movelist(); accepted.~Movelist(); legal.~Movelist(); tablero.~Tiles();      //Destructores
                errorBreak (ERROR_DATA, mfile_name) ;
            }
                
            *input >> total_score ;  
            *input >> lang ;
            *input >> r >> c ;
            tablero.setSize(r, c);
            
            *input >> tablero ;
            
            *input >> p_size;
            *input >> p_secuencia ;
            player.add(toISO(p_secuencia));
            if (player.size() != p_size) {
                original.~Movelist(); rejected.~Movelist(); accepted.~Movelist(); legal.~Movelist(); tablero.~Tiles();      //Destructores
                errorBreak (ERROR_DATA, mfile_name) ;
            }
               
            
            *input >> b_size;
            *input >> b_secuencia;
            bag.set(toISO(b_secuencia));
            if (bag.size() != b_size) {
                original.~Movelist(); rejected.~Movelist(); accepted.~Movelist(); legal.~Movelist(); tablero.~Tiles();      //Destructores
                errorBreak (ERROR_DATA, mfile_name) ;
            }
            
            input=nullptr;
            matchfile.close();
        } 
        
        else if (s=="-p") {
            pfile_name = args[i++];
            playfile.open(pfile_name.c_str());
            if (!playfile) {
                original.~Movelist(); rejected.~Movelist(); accepted.~Movelist(); legal.~Movelist(); tablero.~Tiles();      //Destructores
                errorBreak (ERROR_OPEN, pfile_name) ;
            }
            
            input=&playfile;
           
            *input >> original; 
            if (original.size()==0) {
                original.~Movelist(); rejected.~Movelist(); accepted.~Movelist(); legal.~Movelist(); tablero.~Tiles();      //Destructores
                errorBreak (ERROR_DATA, pfile_name) ;
            }
            
            input=nullptr; 
            playfile.close();
        }    
        
        else if (s=="-l" && !restored)
            lang=args[i++] ;
        
        else if (s=="-w" && !restored) {
            if (!isdigit(*args[i])) { //? 
                original.~Movelist(); rejected.~Movelist(); accepted.~Movelist(); legal.~Movelist(); tablero.~Tiles();      //Destructores
                errorBreak (ERROR_ARGUMENTS, "") ; 
            }
            c = atoi(args[i++]);
        }
    
        else if (s=="-h" && !restored) {
            if (!isdigit(*args[i])) {
                original.~Movelist(); rejected.~Movelist(); accepted.~Movelist(); legal.~Movelist(); tablero.~Tiles();      //Destructores
                errorBreak (ERROR_ARGUMENTS, "") ;
            }
            r = atoi(args[i++]);
        }
        
        else if (s=="-r" && !restored) {
            if (!isdigit(*args[i])) {
                original.~Movelist(); rejected.~Movelist(); accepted.~Movelist(); legal.~Movelist(); tablero.~Tiles();      //Destructores
                errorBreak (ERROR_ARGUMENTS, "") ; 
            }
            random = atoi(args[i++]);
            bag.setRandom(random) ;
        } 
    
        else if (s=="-save") {
            ofilename = args[i++];
            
            //Comprueba formato
            int len=ofilename.size();
            string file_format=ofilename.substr(len-FORMAT.size(), FORMAT.size());
            if (file_format != FORMAT) {
                original.~Movelist(); rejected.~Movelist(); accepted.~Movelist(); legal.~Movelist(); tablero.~Tiles();      //Destructores
                cerr << " >> Please use a matchfile:  -s <*.match>" << endl ;
                errorBreak (ERROR_ARGUMENTS, "") ;
            }
            
            ofile.open(ofilename.c_str());
            if (!ofile) {
                original.~Movelist(); rejected.~Movelist(); accepted.~Movelist(); legal.~Movelist(); tablero.~Tiles();      //Destructores
                errorBreak (ERROR_OPEN, ofilename) ;
            }
            output=&ofile;
        }
        
        else if (s=="-b") { 
            s=args[i++];
            b_secuencia = s ;
            bag.set(toISO(b_secuencia));
        }
        
        else {
            original.~Movelist(); rejected.~Movelist(); accepted.~Movelist(); legal.~Movelist(); tablero.~Tiles();      //Destructores
            errorBreak (ERROR_ARGUMENTS, "") ;
        }  
    }
    
    
    if (r==-1 && c==-1) {
        original.~Movelist(); rejected.~Movelist(); accepted.~Movelist(); legal.~Movelist(); tablero.~Tiles();      //Destructores
        errorBreak(ERROR_ARGUMENTS, "");
    }
    else if (!restored)
        tablero.setSize(r, c);
    
    if (pfile_name=="") {
        original.~Movelist(); rejected.~Movelist(); accepted.~Movelist(); legal.~Movelist(); tablero.~Tiles();      //Destructores
        errorBreak(ERROR_ARGUMENTS, "");
    }
    
    if (lang == "")  {
        original.~Movelist(); rejected.~Movelist(); accepted.~Movelist(); legal.~Movelist(); tablero.~Tiles();      //Destructores
        errorBreak(ERROR_ARGUMENTS, "");
    }   
    else
        language.setLanguage(lang);
    
    if (b_secuencia == "")
        bag.define(language) ;
    
    if (p_secuencia == "")
        player.add(bag.extract(7-player.size()));
    
    
    //selected output:
    cout << "\nOUTPUT: " << ofilename  << endl ;
    
    
    //assign original to legal, then remove invalid moves
    legal = original;
    legal.zip(language);
    
    
    //read movements
    bool inside;
    for (int i = 0; i < legal.size(); i++){
        
        move = legal.get(i);  
        word = move.getLetters();
        inside = tablero.inside(move);     
        
        cout << "\nPLAYER: " << toUTF(player.to_string()) ;
        cout << "\nMOVE: " << move ;
        
        //if (player.isValid(word) && inside) -->  test!
        if (player.isValid(word)) {
            player.extract(word);
            player.add(bag.extract(7-player.size())) ;
            
            score=move.findScore(language);
            total_score += score;
            move.setScore(score);   
            accepted += move ; 
            
            tablero.add (move);
            
            cout << "\n >> ACCEPTED!" ;
            if (!inside)
                cout << " (out of limits). " ;
            cout << " --> " << move.getScore() << " points" ;
            
            cout << endl << tablero ;
        }
        
        else {
            rejected += move;
            cout << "\n >> REJECTED! " ;
        }
        
        cout << "\n______________________________\n" ;      //separador
    } 
    cout << "\n\n" ;
    
    HallOfFame(language, random, bag, player, tablero, original, legal, accepted, rejected); 
    
    //Final result:
    if (ofilename!="CONSOLE")
        *output << PASSWORD << endl ;
    else
        cout << "\n%%%OUTPUT\n" ;
    *output << total_score << endl ;
    *output << lang << endl;
    *output << tablero ;
    *output << endl << player.size() << " " << toUTF(player.to_string()) << endl;
    *output << bag.size() << " " << toUTF(bag.to_string()) << endl ;
    
    if (output != &cout)
        ofile.close() ;
    
    return 0;
}


void HallOfFame(const Language &l, int random, const Bag &b, const Player &p, 
        const Tiles &t, const Movelist& original,const Movelist& legal,
        const Movelist& accepted,const Movelist& rejected) {
    //cout << endl << "%%%OUTPUT" ;
    cout << endl << "LANGUAGE: "<<l.getLanguage()<< " ID: " << random << endl;
    cout << "BAG ("<<b.size()<<"): " << toUTF(b.to_string()) << endl;
    cout << "PLAYER (" <<p.size() << "): " << toUTF(p.to_string());
    cout << endl << endl << "ORIGINAL ("<<original.size()<<"): "<<endl; cout << original ;
    cout << endl << endl << "LEGAL ("<<legal.size()<<"): "<<endl; cout << legal ;
    cout << endl << endl << "ACCEPTED ("<<accepted.size()<<") SCORE "<<accepted.getScore()<< ": "<<endl; cout << accepted ;
    cout << endl << endl << "REJECTED ("<<rejected.size()<<"): "<<endl; cout << rejected ;
    cout << endl;
}

void errorBreak(int errorcode, const string &errordata) {
    cerr << endl << "%%%OUTPUT" << endl;
    switch(errorcode) {
        case ERROR_ARGUMENTS:
            cerr<<"Error in call. Please use either:"<<endl;
            cerr<< "-l <language> -w <width> -h <height> -p <playfile> [-b <bag> -r <randomnumber> -save <matchfile>]"<<endl
                << "-open <matchfile> -p <playfile> [-save <matchfile>]"<<endl;  
            errordata.~basic_string();
            break;
        case ERROR_OPEN:
            cerr<<"Error opening file "<<errordata << endl;
            errordata.~basic_string();
            break;
        case ERROR_DATA:
            cerr<<"Data error in file "<<errordata << endl;
            errordata.~basic_string();
            break;
    }
    std::exit(1);
}
