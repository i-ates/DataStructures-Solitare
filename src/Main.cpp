#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class Card {
    string name; //d01,h13..
    string category; //diamond, heart, spade,club
    int greatness; //1-13 range
    int color; //red=1, black=0
    bool is_open= false; //false=kapalı

public:
    void setName(const std::string &name) {
        Card::name = name;
    }
    const std::string &getName() const {
        return name;
    }

    const std::string &getCategory() const {
        return category;
    }

    void setCategory(const std::string &category) {
        Card::category = category;
    }

    int getGreatness() const {
        return greatness;
    }

    void setGreatness(int greatness) {
        Card::greatness = greatness;
    }

    int getColor() const {
        return color;
    }

    void setColor(int color) {
        Card::color = color;
    }
    bool isOpen() const {
        return is_open;
    }

    void setIsOpen(bool isOpen) {
        is_open = isOpen;
    }
};

Card deck[52];
Card pile[7][19];
Card foundation[4][13]; //0=Heart, 1=Diamond, 2=Spade, 3=Club
Card waste[24];
int pile_indexes[7]={0,1,2,3,4,5,6};
int foundation_indexes[4]={0,0,0,0};
int deck_index=0;
int waste_index=0;
int waste_counter=0;



void dealCards(){
    int i=7;
    int j;
    pile[0][0]=deck[deck_index];
    deck_index-=1;
    while (i>0){
        for (int k = 7-i; k <7 ; ++k) {
            pile[k][7-i]=deck[deck_index];
            deck_index-=1;
        }
        i-=1;
    }
    for (int k = 0; k < 7; ++k) {
        pile[k][k].setIsOpen(true);
    }

};

void readDeckFiles(const string& deck_file){

    string line;
    ifstream my_file(deck_file);

    if (my_file.is_open()){
        while (getline(my_file,line)){
            deck[deck_index].setName(line.substr(0,3));
            deck[deck_index].setCategory(line.substr(0,1));
            deck[deck_index].setGreatness(stoi(line.substr(1,2)));
            if (line.substr(0,1).compare("S")==0 || line.substr(0,1).compare("C")==0){
                deck[deck_index].setColor(0);
            }else{
                deck[deck_index].setColor(1);
            }
            deck_index+=1;
        }
    }
}

int maxElement(){
    int max_element = pile_indexes[0];

    for (int i = 1; i < 7; i++)
        if (pile_indexes[i] > max_element)
            max_element = pile_indexes[i];
    return max_element;
}
void printBoard(ofstream& output) {
    if (deck_index!=-1){
        output<<"@@@ ";

        for (int i = waste_index-waste_counter; i <waste_index ; ++i) {
            output<<waste[i].getName()<<" ";
        }
        for (int i = 0; i <3-waste_counter; ++i) {
            output<<"___ ";
        }
        output<<"        ";
        if (foundation[0][foundation_indexes[0]-1].getName().empty()){
            output<<"___ ";
        }else{
            output<<foundation[0][foundation_indexes[0]-1].getName()<<" ";
        }
        if (foundation[1][foundation_indexes[1]-1].getName().empty()){
            output<<"___ ";
        }else{
            output<<foundation[1][foundation_indexes[1]-1].getName()<<" ";
        }
        if (foundation[2][foundation_indexes[2]-1].getName().empty()){
            output<<"___ ";
        }else{
            output<<foundation[2][foundation_indexes[2]-1].getName()<<" ";
        }
        if (foundation[3][foundation_indexes[3]-1].getName().empty()){
            output<<"___ "<<"\n";
        }else{
            output<<foundation[3][foundation_indexes[3]-1].getName()<<" "<<"\n";
        }
        output<<"\n";

    }else{
        output<<"___ ";

        for (int i = waste_index-waste_counter; i <waste_index ; ++i) {
            output<<waste[i].getName()<<" ";
        }
        for (int i = 0; i <3-waste_counter; ++i) {
            output<<"___ ";
        }
        output<<"        ";

        if (foundation[0][foundation_indexes[0]-1].getName().empty()){
            output<<"___ ";
        }else{
            output<<foundation[0][foundation_indexes[0]-1].getName()<<" ";
        }
        if (foundation[1][foundation_indexes[1]-1].getName().empty()){
            output<<"___ ";
        }else{
            output<<foundation[1][foundation_indexes[1]-1].getName()<<" ";
        }
        if (foundation[2][foundation_indexes[2]-1].getName().empty()){
            output<<"___ ";
        }else{
            output<<foundation[2][foundation_indexes[2]-1].getName()<<" ";
        }
        if (foundation[3][foundation_indexes[3]-1].getName().empty()){
            output<<"___ "<<"\n";
        }else{
            output<<foundation[3][foundation_indexes[3]-1].getName()<<" "<<"\n";
        }
        output<<"\n";
    }

    int x=maxElement();
    for (int i = 0; i <x+1; ++i) {
        for (int j = 0; j <7 ; ++j) {
            if (!pile[j][i].getName().empty()){
                if (pile[j][i].isOpen()){
                    if (i<=pile_indexes[j]){
                        output <<pile[j][i].getName()<<"   ";
                    }
                    else{
                        output<<"      ";
                    }
                }
                else{
                    output<<"@@@   ";
                }
            }
            else{
                output<<"      ";
            }
        }
        output<< "\n";
    }
    output<<"\n";

}
void openPile(int index,ofstream& output){
    if (pile[index][pile_indexes[index]].isOpen()==0){
        pile[index][pile_indexes[index]].setIsOpen(1);
    }
    else{
        output<<"Invalid Move!"<<"\n";
        output<<"\n";
    }
}
void movePile(int sourcePileNum, int sourcePileCardIndex, int destinationPileNum, ofstream& output){
    bool check=true;
    int temp=pile_indexes[sourcePileNum];

    if (sourcePileCardIndex<pile_indexes[sourcePileNum]+1){
        for (int i = 0; i <sourcePileCardIndex ; ++i) {
            if (!pile[sourcePileNum][temp].isOpen()==1){
                check= false;
            }
            temp-=1;
        }
        if ((pile_indexes[destinationPileNum]==-1 & pile[sourcePileNum][temp].getGreatness()!=13)==1){
            check=false;
        }
        if (pile_indexes[sourcePileNum]==-1){
            check=false;
        }
        if (check){
            if (pile[sourcePileNum][temp].getColor()!=pile[destinationPileNum][pile_indexes[destinationPileNum]].getColor()){
                if (pile[sourcePileNum][temp].getGreatness()+1==pile[destinationPileNum][pile_indexes[destinationPileNum]].getGreatness()){
                    for (int i = 0; i <=sourcePileCardIndex ; ++i) {
                        pile[destinationPileNum][pile_indexes[destinationPileNum]+1]=pile[sourcePileNum][temp];
                        temp+=1;
                        pile_indexes[destinationPileNum]+=1;
                        pile_indexes[sourcePileNum]-=1;
                    }
                }else if(pile_indexes[destinationPileNum]==-1){
                        for (int i = 0; i <=sourcePileCardIndex ; ++i) {
                            pile[destinationPileNum][pile_indexes[destinationPileNum]+1]=pile[sourcePileNum][temp];
                            temp+=1;
                            pile_indexes[destinationPileNum]+=1;
                            pile_indexes[sourcePileNum]-=1;
                        }
                }
                else{
                    output<<"Invalid Move!"<<"\n";
                    output<<"\n";
                }


            } else{
                output<<"Invalid Move!"<<"\n";
                output<<"\n";
            }
        }else{
            output<<"Invalid Move!"<<"\n";
            output<<"\n";
        }

    }else{
        output<<"Invalid Move!"<<"\n";
        output<<"\n";
    }

}
void moveFoundationPile(int destinationPileNum, ofstream& output){
    int tempFoundationIndex=0;

    if (pile[destinationPileNum][pile_indexes[destinationPileNum]].getCategory().compare("H")==0){
        tempFoundationIndex=0;
    }
    else if (pile[destinationPileNum][pile_indexes[destinationPileNum]].getCategory().compare("D")==0){
        tempFoundationIndex=1;
    }
    else if (pile[destinationPileNum][pile_indexes[destinationPileNum]].getCategory().compare("S")==0){
        tempFoundationIndex=2;
    }
    else if (pile[destinationPileNum][pile_indexes[destinationPileNum]].getCategory().compare("C")==0){
        tempFoundationIndex=3;
    }

    if (foundation_indexes[tempFoundationIndex]==0){
        if (pile[destinationPileNum][pile_indexes[destinationPileNum]].getGreatness()==1){
            foundation[tempFoundationIndex][0]=pile[destinationPileNum][pile_indexes[destinationPileNum]];
            foundation_indexes[tempFoundationIndex]+=1;
            pile_indexes[destinationPileNum]-=1;

        }else{
            output<<"Invalid Move!"<<"\n";
            output<<"\n";
        }
    }else{
        if ((pile[destinationPileNum][pile_indexes[destinationPileNum]].getCategory())==foundation[tempFoundationIndex][foundation_indexes[tempFoundationIndex]-1].getCategory()){
            if (foundation[tempFoundationIndex][foundation_indexes[tempFoundationIndex]-1].getGreatness()+1==pile[destinationPileNum][pile_indexes[destinationPileNum]].getGreatness()){
                foundation[tempFoundationIndex][foundation_indexes[tempFoundationIndex]]=pile[destinationPileNum][pile_indexes[destinationPileNum]];
                foundation_indexes[tempFoundationIndex]+=1;
                pile_indexes[destinationPileNum]-=1;
            }
            else{
                output<<"Invalid Move!"<<"\n";
                output<<"\n";
            }

        }else{
            output<<"Invalid Move!"<<"\n";
            output<<"\n";
        }

    }

}
void openFromStock(){
    if (deck_index>2){
        for (int i = 0; i <3 ; ++i) {
            waste[waste_index]=deck[deck_index];
            waste[waste_index].setIsOpen(1);
            deck_index-=1;
            waste_index+=1;
        }
        waste_counter=3;
    }else{
        if (deck_index==-1){
            for (int i = 0; i <waste_index; ++i) {
                deck[i]=waste[waste_index-i-1];
            }
            deck_index=waste_index-1;
            waste_index=0;
            waste_counter=0;


        }else{
            int temp;
            temp=deck_index+1;
            while (deck_index>-1){
                waste[waste_index]=deck[deck_index];
                waste[waste_index].setIsOpen(1);
                deck_index-=1;
                waste_index+=1;
            }
            waste_counter=temp;
        }
    }
}
void moveWaste(int destination_pile, ofstream& output){
    bool check=false;
    if (pile_indexes[destination_pile]==-1 & waste[waste_index-1].getGreatness()==13){
        check= true;
    }
    if (pile[destination_pile][pile_indexes[destination_pile]].isOpen() | check){
        if ((pile[destination_pile][pile_indexes[destination_pile]].getColor()!=waste[waste_index-1].getColor()) | check){
            if ((pile[destination_pile][pile_indexes[destination_pile]].getGreatness()==waste[waste_index-1].getGreatness()+1) | check){
                pile[destination_pile][pile_indexes[destination_pile]+1]=waste[waste_index-1];
                waste_index-=1;
                pile_indexes[destination_pile]+=1;
                waste_counter-=1;
                if(waste_index!=0 & waste_counter==0){

                    waste_counter+=1;
                }
            }else{
                output<<"Invalid Move!"<<"\n";
                output<<"\n";
            }

        }else{
            output<<"Invalid Move!"<<"\n";
            output<<"\n";
        }
    }else{
        output<<"Invalid Move!"<<"\n";
        output<<"\n";
    }

}
void moveFoundationWaste(ofstream& output){
    int tempFoundationIndex=0;

    if (waste[waste_index-1].getCategory().compare("H")==0){
        tempFoundationIndex=0;
    }
    else if (waste[waste_index-1].getCategory().compare("D")==0){
        tempFoundationIndex=1;
    }
    else if (waste[waste_index-1].getCategory().compare("S")==0){
        tempFoundationIndex=2;
    }
    else if (waste[waste_index-1].getCategory().compare("C")==0){
        tempFoundationIndex=3;
    }

    if (foundation_indexes[tempFoundationIndex]==0){
        if (waste[waste_index-1].getGreatness()==1){
            foundation[tempFoundationIndex][0]=waste[waste_index-1];
            foundation_indexes[tempFoundationIndex]+=1;
            waste_index-=1;
            waste_counter-=1;

        }else{
            output<<"Invalid Move!"<<"\n";
            output<<"\n";
        }
    }else{
        if ((waste[waste_index-1].getCategory())==foundation[tempFoundationIndex][foundation_indexes[tempFoundationIndex]-1].getCategory()){
            if (foundation[tempFoundationIndex][foundation_indexes[tempFoundationIndex]-1].getGreatness()+1==waste[waste_index-1].getGreatness()){
                foundation[tempFoundationIndex][foundation_indexes[tempFoundationIndex]]=waste[waste_index-1];
                foundation_indexes[tempFoundationIndex]+=1;
                waste_index-=1;
                waste_counter-=1;
                if(waste_index!=0 & waste_counter==0){

                    waste_counter+=1;
                }
            }
            else{
                output<<"Invalid Move!"<<"\n";
                output<<"\n";
            }

        }else{
            output<<"Invalid Move!"<<"\n";
            output<<"\n";
        }

    }

}
void readCommandsFile(const string &command_file, ofstream& output) {

    string line;
    ifstream my_file1(command_file);
    bool check=false;

    if (my_file1.is_open()){
        while (getline(my_file1,line)){
            if ((foundation_indexes[0]==13) && (foundation_indexes[1]==13) && (foundation_indexes[2]==13) && (foundation_indexes[3]==13)){
                check= true;
            }
            if (check){
                output<<"****************************************"<<"\n";
                output<<"\n";
                output<<" You Win!"<<"\n";
                output<<"\n";
                output<<"Game Over!"<<"\n";
                break;
            }
            //move to foundation pile
            else if (line.length()==26){
                output<<line<<"\n";
                output<<"\n";
                moveFoundationPile(stoi(line.substr(24,1)),output);
                output<<"****************************************"<<"\n";
                output<<"\n";
                printBoard(output);
            }
            //move to foundation waste
            else if (line.length()==25){
                output<<line<<"\n";
                output<<"\n";
                moveFoundationWaste(output);
                output<<"****************************************"<<"\n";
                output<<"\n";
                printBoard(output);
            }
            else if(line.length()==17){
                output<<line<<"\n";
                output<<"\n";
                movePile(stoi(line.substr(10,1)),stoi(line.substr(12,2)),
                         stoi(line.substr(15,1)),output);
                output<<"****************************************"<<"\n";
                output<<"\n";
                printBoard(output);
            }
            else if (line.length()==16){
                //move pile
                if ((line.substr(5,4))=="pile"){
                    output<<line<<"\n";
                    output<<"\n";
                    movePile(stoi(line.substr(10,1)),stoi(line.substr(12,1)),
                             stoi(line.substr(14,1)),output);
                    output<<"****************************************"<<"\n";
                    output<<"\n";
                    printBoard(output);

                }//open from stock
                else{
                    output<<line<<"\n";
                    output<<"\n";
                    openFromStock();
                    output<<"****************************************"<<"\n";
                    output<<"\n";
                    printBoard(output);
                }
            }//move wastee
            else if (line.length()==13){
                output<<line<<"\n";
                output<<"\n";
                moveWaste(stoi(line.substr(11,1)),output);
                output<<"****************************************"<<"\n";
                output<<"\n";
                printBoard(output);
            }//open pile
            else if(line.length()==7){
                output<<line<<"\n";
                output<<"\n";
                openPile(stoi(line.substr(5,1)),output);
                output<<"****************************************"<<"\n";
                output<<"\n";
                printBoard(output);
            }
            else if(line.length()==5){
                output<<line<<"\n";
                output<<"\n";
                output<<"****************************************"<<"\n";
                output<<"\n";
                output<<"Game Over!"<<"\n";
                break;
            }
        }
        if ((foundation_indexes[0]==13) && (foundation_indexes[1]==13) && (foundation_indexes[2]==13) && (foundation_indexes[3]==13)){
            output<<"****************************************"<<"\n";
            output<<"\n";
            output<<"You Win!"<<"\n";
            output<<"\n";
            output<<"Game Over!"<<"\n";
        }
    }
    else{
        output<<"file can not opened!"<<"\n";
    }
}

int main(int argc, char** argv) {
    //argv[1]= deck_file, argv[2]=commands_file, argv[3]=output_file
    ofstream output_file(argv[3]);
    readDeckFiles(argv[1]);
    dealCards();
    output_file<<"\n";
    printBoard(output_file);
    readCommandsFile(argv[2], output_file);

    return 0;
}
