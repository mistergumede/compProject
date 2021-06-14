/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: MSCS
 *
 * Created on 12 March 2020, 8:38 AM
 */

#include <cstdlib>
#include <iostream>
#include <fstream>
#include "board.h"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
    if(argc==2){
        board abalone;
        string ibs(abalone);
        cout<<"Initial Abalone board state:"<<endl;
        cout<<ibs<<endl;        
        ifstream log(argv[1]);//sequence of game moves
        string l;
        int n, fd, md, pf;
        int count =0;
        while(log>>l>>n>>fd>>md){
            cout<<"Move: "<<l<<" "<<n<<" "<<fd<<" "<<md;
            pf =0;
            count++;
            bool valid = abalone.executeMove(l,n,fd,md,pf);
            
            if(!valid){
               if( pf ==1)
                   cout<<" - Invalid move - Invalid location!"<<endl;
               else if( pf ==2)
                   cout<<" - Invalid move - You Can't Move A Space!"<<endl;
               else if( pf ==3)
                   cout<<" - Invalid move - There is no adjacent Marble at the Specified Direction!"<<endl;
               else if( pf ==4)
                   cout<<" - Invalid move - met own marble instead of space!"<<endl;
               else if( pf ==5)
                   cout<<" - Invalid move - Your number of marbles must be greater than opponent's!"<<endl;
               else if( pf ==6)
                   cout<<" - Invalid move - Broad-side Move Require at least 2 marbles!"<<endl;
               else if( pf ==7)
                   cout<<" - Invalid move - Trying to Move to a nonempty space"<<endl;
               else if( pf ==8)
                   cout<<" - Invalid move - You're Trying to push your own marble off the board"<<endl;
                
                
            }
            else{
                cout<<" - Executed"<<endl;
                string nbs(abalone);
                cout<<"Next Abalone board state:"<<endl;
                cout<<nbs<<endl;
                
                
            }
        } cout<< "count = " << count <<endl;   
        return 0;
    }
    return -1;
}

