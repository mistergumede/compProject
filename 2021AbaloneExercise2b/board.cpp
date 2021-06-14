/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "board.h"
#include <iomanip>
#include <iostream>
string traverse(const cell* start,const int& pd,const int& sd,const int& td){
    string ts;
    const cell* linep = start;
    while(linep!=nullptr/*linep->getAdjacent(pd)!=nullptr || linep->getAdjacent(sd)!=nullptr*/){
        const cell* cellp = linep;
        while(cellp!=nullptr){
            ts+=(cellp->getLocation()+' ');
            cellp=cellp->getAdjacent(td);
        }
        if(linep->getAdjacent(pd)==nullptr)
            linep=linep->getAdjacent(sd);
        else
            linep=linep->getAdjacent(pd);
    }
    /*const cell* cellp = linep;
    while(cellp!=nullptr){
        ts+=(cellp->getLocation()+' ');
        cellp=cellp->getAdjacent(td);
    }*/
    return ts;
}
void setMarbles(cell* row,const int& z, const char& m){
    for(int i=0;i<z;i++){
        row[i].setMarble(m);
    }
    if(z==7){
        char pos[4]={0,1,5,6};
        for(int i=0;i<4;i++){
            row[pos[i]].setMarble('+');
        }
    }
}
void connectHorizontal(cell* row, const int& rz){
    for(int i=0;i<rz-1;i++){
        row[i].setAdjacent(row+i+1,1);
    }
    //row[0].setAdjacent(row+1,1);
    //row[rz-2].setAdjacent(row+rz-1,1);    
}

void connectDiagonal(cell* lrow,cell* srow,const int& f,const int& sz,const bool& top){
    for(int i=f;i<(f+sz);i++){
        if(top){
            if(f==0){//connecting from cell 0 in longer row
                lrow[i].setAdjacent(srow+i,0);
            }
            else if(f==1){//connecting from cell 1 in longer row
                lrow[i].setAdjacent(srow+i-1,5);
            }
            else return;
        }
        else{
            if(f==0){
                lrow[i].setAdjacent(srow+i,2);
            }
            else if(f==1){
                lrow[i].setAdjacent(srow+i-1,3);
            }
            else return;            
        }
    }
}
void mapCells(cell* row,map<string,cell*>& cm,const char& r,const int& z){    
    int f=1;
    if(r>='A'&& r<='E'){
        f=1;
    }else if(r>'E'&& r<='I'){
        f=r-'E'+1;
    }else return;
    for(int i=0;i<z;i++){
        stringstream ss;
        ss<<r;
        ss<<(i+f);
        string lc = ss.str();
        row[i].setLocation(lc);
        cm[lc]=row+i;
    }
}
board::board():woc(0),boc(0){
    char r = 'I';
    for(int i=0;i<9;i++){
        if(i<5){
            rows.push_back(new cell[5+i]);
            connectHorizontal(rows[i],5+i);
            mapCells(rows[i],cells,r--,5+i);
            if(i<3){
                setMarbles(rows[i],5+i,'O');
            }else
                setMarbles(rows[i],5+i,'+');
        }
        else{
            rows.push_back(new cell[5+8-i]);
            connectHorizontal(rows[i],5+8-i);
            mapCells(rows[i],cells,r--,5+8-i);
            if(i>=6){
                setMarbles(rows[i],5+8-i,'@');
            }else
                setMarbles(rows[i],5+8-i,'+');
        }
    }
    for(int i=1;i<9;i++){
        if(i<5){
            connectDiagonal(rows[i],rows[i-1],0,5+i-1,true);
            connectDiagonal(rows[i],rows[i-1],1,5+i-1,true);
        }
        else{
            connectDiagonal(rows[i-1],rows[i],0,5+8-i,false);
            connectDiagonal(rows[i-1],rows[i],1,5+8-i,false);
        }
    }
}

board::board(const board& copy): board(){//constructor delegation - C++11
    woc=copy.woc;
    boc=copy.boc;
    map<string,cell*>::const_iterator it = copy.cells.begin();
    while(it!=copy.cells.end()){
        (cells[it->first])->setMarble((*(it->second)).getMarble());
        it++;
    }
}

board::~board(){
    for(int i=0;i<9;i++){
        delete[] rows[i];
        rows[i]=nullptr;
    }
    cells.clear();
    //cout<<"good bye!";
}

board::operator std::string() const{
    stringstream ss;
    char r ='I';
    int d=9;
    for(int i=0;i<9;i++){
        if(i<5){
            ss<<setw(5-i)<<r<<" ";
            for(int k=0;k<5+i;k++){
                ss<<rows[i][k].getMarble()<<" ";
            }
            ss<<endl;
            r--;
        }
        else{
            ss<<setw(i-3)<<r<<" ";
            for(int k=0;k<5+8-i;k++){
                ss<<rows[i][k].getMarble()<<" ";
            }
            ss<<d<<endl;
            r--;
            d--;
        }
    }
    d=1;
    ss<<setw(8)<<d++<<" ";
    for(;d<6;d++)
        ss<<d<<" ";
    ss<<endl;
    for(int i=0;i<woc;i++)
        ss<<'O'<<" ";
    ss<<endl;
    for(int i=0;i<boc;i++)
        ss<<'@'<<" ";
    ss<<endl;
    return ss.str();
}

string board::traverseDiagonal() const{
    //backward diagonal - nw to se
    /*
    map<string,cell*>::const_iterator it = cells.find(string("A1"));
    cell* start = it->second;
    return traverse(start,1,0,5); 
    */ 
    //forward diagonal - sw to ne
    /**/
    map<string,cell*>::const_iterator it = cells.find(string("E1"));
    cell* start = it->second;
    return traverse(start,0,1,2); 
    /**/
}
string board::traverseHorizontal() const{
    map<string,cell*>::const_iterator it = cells.find(string("A1"));
    cell* start = it->second;
    return traverse(start,5,0,1);
}

bool board::validateMove(const string& l,const int& n, const int& fd, const int& md, int& mtype, bool& scoreMove, int& pf) const{
    if(!cell::validateLocation(l)){//confirm existence of location
        pf = 1;
        return false;
    }else{        
        cell* cp = cells.at(l);//initialize to leading cell
        const char marble = cp->getMarble();
        if(marble=='+'){//trying to move a space
            pf = 2;
            return false;
        }
        for(int i=1;i<n;i++){
            cp=cp->getAdjacent(fd);
            if(cp==nullptr || marble!=cp->getMarble()){//confirm availability and type of claimed resources
                pf=3;
                return false;
            }
        }
        if(fd==getAdjacentIndex(md)){//in-line move
            cp = cells.at(l);//initialize to leading cell
            mtype=0;//how many opponent marbles are in front of the leading marble i.e in the move direction
            for(int i=0;i<n;i++){
                cp=cp->getAdjacent(md);
                if(cp!=nullptr){//confirm availability of required resources
                    if(marble==cp->getMarble()){//met own marble instead of space
                        pf= 4;
                        return false;
                    }
                    if('+'!=cp->getMarble())//if it belongs to opponent
                        mtype++;//count opponent marbles
                    else
                        break;
                }
                else{
                    if(mtype>0)//opponent's marble will be pushed off the board
                        scoreMove=true;
                    else{//if trying to push your own marble off the board
                        pf =8;
                        return false;
                }break;
                }
            }
            if(mtype>=n){//should only push fewer than n of the opponent's marbles
                pf =5;
                return false;
            }
        }
        else{//broad-side move
            mtype=-1;
            if(n<=1){//broad-side move must use at least 2 marbles
                pf =6;
                return false;
            }
            cp = cells.at(l);
            for(int i=0;i<n;i++){
                cell* dp=cp->getAdjacent(md);//destination pointer
                if(dp!=nullptr && '+' == dp->getMarble())//is there an empty space to move to?
                    cp=cp->getAdjacent(fd);
                else{
                    pf = 7;
                    return false;
                }      
            }
        }
        return true;
    }
}

bool board::executeMove(const string& l,const int& n, const int& fd, const int& md, int& pf){
    int moveType = 0;//default to in-line with no following marbles
    
    bool scoreMove = false;
    bool valid = validateMove(l,n,fd,md,moveType,scoreMove,pf);
    if(valid){
        if(moveType==-1){//broad-side
            cell* cp = cells.at(l);
            for(int i=0;i<n;i++){
                cell* dp=cp->getAdjacent(md);//destination pointer
                dp->setMarble(cp->getMarble());
                cp->setMarble('+');
                cp=cp->getAdjacent(fd);
            }
        }
        else{//in-line
            cell* cp = cells.at(l), *rearp=cp;
            const char marble = cp->getMarble();
            cell* frontp=cp->getAdjacent(md);
            for(int i=1;i<n;i++){
                rearp=rearp->getAdjacent(fd);
            }
            if(scoreMove){                
                frontp->setMarble(rearp->getMarble());
                rearp->setMarble('+');
                if(marble=='O')
                    boc++;
                if(marble=='@')
                    woc++;
            }
            else{
                cell* tipp = frontp;
                for(int i=0;i<moveType;i++){
                    tipp=tipp->getAdjacent(md);
                }
                tipp->setMarble(frontp->getMarble());
                frontp->setMarble(rearp->getMarble());
                rearp->setMarble('+');
            }
        }
    }
    return valid;
}