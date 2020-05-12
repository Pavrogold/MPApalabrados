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
    
    int random=-1, score=0, p_size, b_size, r=-1, c=-1;
    string lang = "", b_secuencia="", key="", word = "",
           pfile_name="", mfile_name ="", ofilename="" ;
    const string FORMAT=".match";
    
    ifstream matchfile, playfile; 
    ofstream ofile;
    istream *input ; 
    ostream *output;
    
    input=&cin;
    output=&cout;
    
    if (nargs<1 || nargs>15 || nargs%2==0)
        errorBreak (ERROR_ARGUMENTS, "") ;
    
    string s;
    bool restored=false;
    for (int i=1 ; i<nargs; ) {
        
        s=args[i++];
        if (s=="-open") {
            restored=true;
            
            mfile_name = args[i++];
            
            //comprueba formato
            int len=mfile_name.size();
            string file_format=mfile_name.substr(len-FORMAT.size(), FORMAT.size());
            if ( file_format != FORMAT )
                errorBreak (ERROR_OPEN, mfile_name) ;
            
            matchfile.open(mfile_name.c_str());
            if (!matchfile) 
                errorBreak (ERROR_OPEN, mfile_name) ;
            input=&matchfile;
            
            *input >> key ;
            if (key != PASSWORD) 
                errorBreak (ERROR_DATA, mfile_name) ;
            
            *input >> score ;   //@warning
            *input >> lang ;
            
            *input >> r >> c;
            //tablero.setSize(r, c);
            
            tablero.read(*input);
            
            *input >> p_size;
            string p_secuencia;
            *input >> p_secuencia ;
            player.add(toISO(p_secuencia));
            if (player.size() != p_size)
                errorBreak (ERROR_DATA, mfile_name) ;
            
            *input >> b_size;
            *input >> b_secuencia;
            bag.set(toISO(b_secuencia));
            if (bag.size() != b_size) 
                errorBreak (ERROR_DATA, mfile_name) ;
            
            matchfile.close();
            input=nullptr;
        } 
        
        else if (s=="-p") {
            pfile_name = args[i];
            i++;
            playfile.open(pfile_name.c_str());
            if (!playfile) 
                errorBreak (ERROR_OPEN, pfile_name) ;
            input=&playfile;
           
            *input >> original; 
            
            playfile.close();
            input=nullptr; 
        } 
            
        else if (s=="-l" && !restored) 
            lang=args[i++] ;
        
        else if (s=="-w" && !restored) {
            if (!isdigit(*args[i])) //?
                errorBreak (ERROR_ARGUMENTS, "") ; 
            c = atoi(args[i++]);
        }
    
        else if (s=="-h" && !restored) {
            if (!isdigit(*args[i])) 
                errorBreak (ERROR_ARGUMENTS, "") ; 
            r = atoi(args[i++]);
        }
        
        else if (s=="-r" && !restored) {
            if (!isdigit(*args[i])) 
                errorBreak (ERROR_ARGUMENTS, "") ; 
            random = atoi(args[i++]);
            bag.setRandom(random) ;
        } 
    
        else if (s=="-save") {
            ofilename = args[i++];
            
            //comprueba formato
            int len=ofilename.size();
            string file_format=ofilename.substr(len-FORMAT.size(), FORMAT.size());
            if ( file_format != FORMAT )
                errorBreak (ERROR_OPEN, ofilename) ;
            
            ofile.open(ofilename.c_str());
            if (!ofile) 
                errorBreak (ERROR_OPEN, ofilename) ;
            output=&ofile;
        }     
        
        else
            errorBreak (ERROR_ARGUMENTS, "") ; 
    }
    
    if (r==-1 && c==-1)
        errorBreak(ERROR_ARGUMENTS, "");
    else
        tablero.setSize(r, c);
    
    if (pfile_name=="")
        errorBreak(ERROR_ARGUMENTS, "");
    
    if (lang == "") 
        errorBreak(ERROR_ARGUMENTS, "");
    else
        language.setLanguage(lang);
    
    if (b_secuencia == "")
        bag.define(language) ;
    
    if (ofilename=="") 
        cout << "\nOUTPUT: CONSOLE\n\n";
    
    
    //cout << "\nALLOWED LETTERS: " << toUTF(language.getLetterSet()) << endl;
    //cout << "BAG ("<<bag.size()<<"): " << toUTF(bag.to_string()) << endl;
    //cout << "PLAYER: " << toUTF(player.to_string()) << endl;
    
    //assign original to legal, and remove invalid moves
    legal = original;
    legal.zip(language);
    
    for (int i = 0; i < legal.size(); i++){
        
        move=legal.get(i);  
        word=move.getLetters();
        //word=toISO(move.getLetters());
        
        if (player.isValid(word)) {
            player.extract(word);
            player.add(bag.extract(7-player.size())) ;
            
            score=move.findScore(language);
            move.setScore(score);
            accepted.add(move); //implementar += en movelist
            
            tablero.add (move);
            cout << endl << endl << endl ;
            tablero.print(cout); //para comprobar salida (provisional)
        }
        else {
            rejected.add(move);
            cout << "\n >> REJECTED! " << endl;     //salida solo en consola, no en savefile
        }
    } 
    
    
    //Resultado final:
    cout << endl << endl ;
    *output << PASSWORD << endl ;
    *output << score << endl ;
    *output << lang << endl;
    *output << tablero.getHeight() << " " << tablero.getWidth() << endl ;
    tablero.print(*output);
    *output << endl << player.size() << " " << toUTF(player.to_string()) << endl;
    *output << bag.size() << " " << toUTF(bag.to_string()) << endl;
    
    HallOfFame(language, random, bag, player, tablero, original, legal, accepted, rejected); 
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
