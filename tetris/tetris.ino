#include <LedControl.h>

long delays  = 0;
short delay_ = 500;

int gameEnded = 0;

// [16][8] = dimensiunea totala a led-matrix-ului
boolean currentPiece[16][8]; // piesa curenta, pe care trebuie s-o asezam acuma
boolean blocks[16][8]; // toate piesele puse pana acuma
boolean displayedData[16][8]; // datele ce vor fi afisate; toate pozitiile au valoarea 1 (aprins) sau 0 (stins)

int buttonRotate;
int buttonDown;
int buttonRight;
int buttonLeft;

int buttonRotate_old = HIGH;
int buttonDown_old = HIGH;
int buttonRight_old = HIGH;
int buttonLeft_old = HIGH;

int stepsDown = 0;
int stepsRight = 0;
int rotateContor = 0;

// pinii LED matrix-ului
int DIN1 = 12;
int CS1  = 11;
int CLK1 = 10;
LedControl lc2 = LedControl(DIN1,CLK1,CS1,0);

int DIN2 = 16;
int CS2  = 15;
int CLK2 = 14;
LedControl lc1 = LedControl(DIN2,CLK2,CS2,0);

int pieceNumber = 0; // piesele sunt identificate dupa numarul lor
/*  pieceNumber = 0
 *  o o
 *  o o
 *  
 *  pieceNumber = 1
 *  o
 *  o
 *  o
 *  o
 *  
 *  pieceNumber = 2
 *  o
 *  o O O
 *  
 *  pieceNumber = 3
 *      o
 *  o o o 
 *  
 *  pieceNumber = 4
 *  o o
 *    o o
 *    
 *  pieceNumber = 5
 *    o o
 *  o o
 *  
 *  pieceNumber = 6
 *    o 
 *  o o o
 */

void initializeCurrentPiece(){
  
  for(int i = 0; i < 16; i++){
    for(int j = 0; j < 8; j++){
      currentPiece[i][j] = 0;
    }
  }
}

void generateNewPiece(){

  rotateContor = 0;
  stepsDown = 0;
  stepsRight = 0;
  initializeCurrentPiece();
  pieceNumber = random(7);

  switch (pieceNumber){
    case 0: currentPiece[0][3] = 1;
            currentPiece[0][4] = 1;
            currentPiece[1][3] = 1;
            currentPiece[1][4] = 1;
            break;

    case 1: currentPiece[0][3] = 1;
            currentPiece[1][3] = 1;
            currentPiece[2][3] = 1;
            currentPiece[3][3] = 1;
            break;
                                
    case 2: currentPiece[0][3] = 1;
            currentPiece[1][3] = 1;
            currentPiece[1][4] = 1;
            currentPiece[1][5] = 1;
            break;

    case 3: currentPiece[0][5] = 1;
            currentPiece[1][3] = 1;
            currentPiece[1][4] = 1;
            currentPiece[1][5] = 1;
            break; 

    case 4: currentPiece[0][3] = 1;
            currentPiece[0][4] = 1;
            currentPiece[1][4] = 1;
            currentPiece[1][5] = 1;
            break;

    case 5: currentPiece[0][4] = 1;
            currentPiece[0][5] = 1;
            currentPiece[1][3] = 1;
            currentPiece[1][4] = 1;
            break;
            
    case 6: currentPiece[0][4] = 1;
            currentPiece[1][3] = 1;
            currentPiece[1][4] = 1;
            currentPiece[1][5] = 1;
            break;
  }  
}

void changeLEDs(){
  for(int i = 0; i < 16; i++){
    for(int j = 0; j < 8; j++){
      displayedData[i][j] = blocks[i][j] | currentPiece[i][j]; // o pozitie din displayedData va fi aprinsa, doar daca e partea blocului sau daca acolo este piesaa curenta
    }
  }
  printRows1(displayedData);
  printRows2(displayedData);
}


void moveLeft(){
  int ok = 1;
  
  for(int i = 0; i < 16; i++){
    if(currentPiece[i][0] == 1){ // daca este in coloana cea mai din stanga, nu o lasam sa mearga mai la stanga
        ok = 0;
      }
  }

  for(int i = 0; i < 16; i++){
      for(int j = 1; j < 8; j++){
        if(currentPiece[i][j] == 1 & blocks[i][j-1] == 1){ // daca piesa ar intra in bloc, , nu o lasam sa mearga mai la stanga
          ok = 0;
        }
      }
    }

  if(ok == 1){
    stepsRight--;
    for(int i = 0; i < 16; i++){
      for(int j = 0; j < 7; j++){
        currentPiece[i][j] = currentPiece[i][j+1]; // mutam coloanele cu o pozitie la stanga
      }
    }
    
    for(int i = 0; i < 16; i++){
      currentPiece[i][7] = 0; // coloana cea mai din dreapta va primi valoarea 0 -> stins
    }
  }

  changeLEDs();
}


void moveRight(){
  int ok = 1;

  for(int i = 0; i < 16; i++){
    if(currentPiece[i][7] == 1){ // daca este in coloana cea mai din dreapta, nu o lasam sa mearga mai la dreapta
        ok = 0;
      }
  }

  for(int i = 0; i < 16; i++){
      for(int j = 0; j < 7; j++){
        if(currentPiece[i][j] == 1 & blocks[i][j+1] == 1){ // daca piesa ar intra in bloc, , nu o lasam sa mearga mai la dreapta
          ok = 0;
        }
      }
    }

  if(ok == 1){
    stepsRight++;
    for(int i = 0; i < 16; i++){
      for(int j = 7; j > 0; j--){
        currentPiece[i][j] = currentPiece[i][j-1]; // mutam liniile cu o linie mai la dreapta
      }
    }
    
    for(int i = 0; i < 16; i++){
      currentPiece[i][0] = 0; // prima coloana va primi valoarea 0 -> stins
    }
  }

  changeLEDs();
}

boolean isSpaceBelow(){
  for(int i = 15; i >= 0; i--){
      for(int j = 0; j < 8; j++){
          if(currentPiece[i][j] == 1){
            if(i == 15){ // daca vreo parte a piesei curente a ajuns in ultimul rand
              return false; // atunci nu mai este loc in jos
              }
            
            if(blocks[i+1][j] == 1){ // daca blocul incepe deja in urmatorul rand
              return false; // atunci nu mai este loc in jos
            }
         }
      }
  }
  return true; // daca nu s-au indeplinit criteriile de mai sus, atunci piesa se poate deplasa in jos
  
}

void gameOver(){
  
  for(int i = 0; i < 16; i++){
    for(int j = 0; j < 8; j++){ 
      displayedData[i][j] = false;
    }
  }

  printRows1(displayedData);
  printRows2(displayedData);
  delay(1500);

  for(int i = 0; i < 16; i++){
    for(int j = 0; j < 8; j++){ 
      if(i%2 == 0){
        displayedData[i][j] = true;
      }
    }
  }

  printRows1(displayedData);
  printRows2(displayedData);
  delay(1500);

  for(int i = 0; i < 16; i++){
    for(int j = 0; j < 8; j++){ 
      displayedData[i][j] = false;
    }
  }

  printRows1(displayedData);
  printRows2(displayedData);
  delay(1500);
  setup();
}

boolean isTopReached(){
  for(int i = 0; i < 4; i++){
    for(int j = 3; j < 6; j++){
      if(blocks[i][j] == 1 && currentPiece[i][j] == 1){
        return true;
      }
    }
  }

  return false;
}

void moveDownOnFullRow(int row){
  
  for(int i = row; i > 0; i--){
      for(int j = 0; j < 8; j++){
        blocks[i][j] = blocks[i-1][j]; // mutam liniile cu o pozitie la dreapta, incepand de la linia ca este plina
      }
    }

    for(int i = 0; i < 8; i++){
      blocks[0][i] = 0; // prima linie va primi valoarea 0 -> stins
    }
}

void isRowFull(){
  int contor = 0;

  for(int i = 0; i < 16; i++){
    for(int j = 0; j < 8; j++){
       if(blocks[i][j] == 1){
          contor++;
       }
    }
    if(contor == 8){ // daca linia este plina
      moveDownOnFullRow(i);
    }
    contor = 0;
  }
}


void moveDown(){
  if(isSpaceBelow()){
    stepsDown++;
    for(int i = 15; i > 0; i--){
      for(int j = 0; j < 8; j++){
        currentPiece[i][j] = currentPiece[i-1][j]; // mutam liniile cu o pozitie in jos
      }
    }

    for(int i = 0; i < 8; i++){
      currentPiece[0][i] = 0; // prima linie va primi valoarea 0 -> stins
    }
  } else {
    for(int i = 15; i > 0; i--){
      for(int j = 0; j < 8; j++){
        if(currentPiece[i][j] == 1){
          blocks[i][j] = 1;
          currentPiece[i][j] = 0;
        }
      }
    }
    isRowFull();
    generateNewPiece();  
  }
  
  changeLEDs();
  if(isTopReached()){
      gameEnded = 1; 
  }
}

bool isBlock(int i, int j){
  if(blocks[i][j] == 1){
    return true;
  } else {
    return false;
  }
}


void generateRotatedPiece(){
  switch (pieceNumber){  //forma rotata depinde de tipul piesei 
    case 0: if(!(isBlock(0+stepsDown, 3+stepsRight) | isBlock(0+stepsDown, 4+stepsRight) | isBlock(1+stepsDown, 3+stepsRight) | isBlock(1+stepsDown, 4+stepsRight)) & 
              ((3+stepsRight)>=0 & (3+stepsRight)<8) & ((4+stepsRight)>=0 & (4+stepsRight)<8)){
              initializeCurrentPiece();
              currentPiece[0+stepsDown][3+stepsRight] = 1;
              currentPiece[0+stepsDown][4+stepsRight] = 1;
              currentPiece[1+stepsDown][3+stepsRight] = 1;
              currentPiece[1+stepsDown][4+stepsRight] = 1;
            }  
            break;
    
    case 1: if(rotateContor % 2 == 0){ // arata ca in forma initiala
              if(!(isBlock(0+stepsDown, 3+stepsRight) | isBlock(1+stepsDown, 3+stepsRight) | isBlock(2+stepsDown, 3+stepsRight) | isBlock(3+stepsDown, 3+stepsRight))
              & ((3+stepsRight)>=0 & (3+stepsRight)<8)){
                initializeCurrentPiece();
                currentPiece[0+stepsDown][3+stepsRight] = 1;
                currentPiece[1+stepsDown][3+stepsRight] = 1;
                currentPiece[2+stepsDown][3+stepsRight] = 1;
                currentPiece[3+stepsDown][3+stepsRight] = 1;
              }
            } else{
              if(!(isBlock(1+stepsDown, 2+stepsRight) | isBlock(1+stepsDown, 3+stepsRight) | isBlock(1+stepsDown, 4+stepsRight) | isBlock(1+stepsDown, 5+stepsRight)) & 
              ((2+stepsRight)>=0 & (2+stepsRight)<8) & ((3+stepsRight)>=0 & (3+stepsRight)<8) & ((4+stepsRight)>=0 & (4+stepsRight)<8) & ((5+stepsRight)>=0 & (5+stepsRight)<8)){
                initializeCurrentPiece();
                currentPiece[1+stepsDown][2+stepsRight] = 1;
                currentPiece[1+stepsDown][3+stepsRight] = 1;
                currentPiece[1+stepsDown][4+stepsRight] = 1;
                currentPiece[1+stepsDown][5+stepsRight] = 1;
              }
            }
            
            break;
    
    case 2: switch(rotateContor % 4){
              case 0: if(!(isBlock(0+stepsDown, 3+stepsRight) | isBlock(1+stepsDown, 3+stepsRight) | isBlock(1+stepsDown, 4+stepsRight) | isBlock(1+stepsDown, 5+stepsRight)) & 
                      ((3+stepsRight)>=0 & (3+stepsRight)<8) & ((4+stepsRight)>=0 & (4+stepsRight)<8) & ((5+stepsRight)>=0 & (5+stepsRight)<8)){
                        initializeCurrentPiece();
                        currentPiece[0+stepsDown][3+stepsRight] = 1;
                        currentPiece[1+stepsDown][3+stepsRight] = 1;
                        currentPiece[1+stepsDown][4+stepsRight] = 1;
                        currentPiece[1+stepsDown][5+stepsRight] = 1;
                      }
                      break;
                      
              case 1: if(!(isBlock(0+stepsDown, 3+stepsRight) | isBlock(0+stepsDown, 4+stepsRight) | isBlock(1+stepsDown, 3+stepsRight) | isBlock(2+stepsDown, 3+stepsRight)) & 
                      ((3+stepsRight)>=0 & (3+stepsRight)<8) & ((4+stepsRight)>=0 & (4+stepsRight)<8)){
                        initializeCurrentPiece();
                        currentPiece[0+stepsDown][3+stepsRight] = 1;
                        currentPiece[0+stepsDown][4+stepsRight] = 1;
                        currentPiece[1+stepsDown][3+stepsRight] = 1;
                        currentPiece[2+stepsDown][3+stepsRight] = 1;
                      }
                      break;
              
              case 2: if(!(isBlock(1+stepsDown, 3+stepsRight) | isBlock(1+stepsDown, 4+stepsRight) | isBlock(1+stepsDown, 5+stepsRight) | isBlock(2+stepsDown, 5+stepsRight)) & 
                      ((3+stepsRight)>=0 & (3+stepsRight)<8) & ((4+stepsRight)>=0 & (4+stepsRight)<8) & ((5+stepsRight)>=0 & (5+stepsRight)<8)){
                        initializeCurrentPiece();
                        currentPiece[1+stepsDown][3+stepsRight] = 1;
                        currentPiece[1+stepsDown][4+stepsRight] = 1;
                        currentPiece[1+stepsDown][5+stepsRight] = 1;
                        currentPiece[2+stepsDown][5+stepsRight] = 1;
                      }
                      break;
                      
              case 3: if(!(isBlock(0+stepsDown, 3+stepsRight) | isBlock(1+stepsDown, 3+stepsRight) | isBlock(2+stepsDown, 3+stepsRight) | isBlock(2+stepsDown, 2+stepsRight)) & 
                      ((2+stepsRight)>=0 & (2+stepsRight)<8) & ((3+stepsRight)>=0 & (3+stepsRight)<8)){
                        initializeCurrentPiece();
                        currentPiece[0+stepsDown][3+stepsRight] = 1;
                        currentPiece[1+stepsDown][3+stepsRight] = 1;
                        currentPiece[2+stepsDown][3+stepsRight] = 1;
                        currentPiece[2+stepsDown][2+stepsRight] = 1;
                      }
                      break; 
            }
            break;
     
    case 3: switch(rotateContor % 4){
              case 0: if(!(isBlock(0+stepsDown, 5+stepsRight) | isBlock(1+stepsDown, 3+stepsRight) | isBlock(1+stepsDown, 4+stepsRight) | isBlock(1+stepsDown, 5+stepsRight)) & 
                      ((3+stepsRight)>=0 & (3+stepsRight)<8) & ((4+stepsRight)>=0 & (4+stepsRight)<8) & ((5+stepsRight)>=0 & (5+stepsRight)<8)){
                        initializeCurrentPiece();
                        currentPiece[0+stepsDown][5+stepsRight] = 1;
                        currentPiece[1+stepsDown][3+stepsRight] = 1;
                        currentPiece[1+stepsDown][4+stepsRight] = 1;
                        currentPiece[1+stepsDown][5+stepsRight] = 1;
                      }
                      break;
                      
              case 1: if(!(isBlock(0+stepsDown, 4+stepsRight) | isBlock(1+stepsDown, 4+stepsRight) | isBlock(2+stepsDown, 4+stepsRight) | isBlock(2+stepsDown, 5+stepsRight)) & 
                      ((4+stepsRight)>=0 & (4+stepsRight)<8) & ((5+stepsRight)>=0 & (5+stepsRight)<8)){
                        initializeCurrentPiece();
                        currentPiece[0+stepsDown][4+stepsRight] = 1;
                        currentPiece[1+stepsDown][4+stepsRight] = 1;
                        currentPiece[2+stepsDown][4+stepsRight] = 1;
                        currentPiece[2+stepsDown][5+stepsRight] = 1;
                      }
                      break;
              
              case 2: if(!(isBlock(1+stepsDown, 3+stepsRight) | isBlock(1+stepsDown, 4+stepsRight) | isBlock(1+stepsDown, 5+stepsRight) | isBlock(2+stepsDown, 3+stepsRight)) & 
                      ((3+stepsRight)>=0 & (3+stepsRight)<8) & ((4+stepsRight)>=0 & (4+stepsRight)<8) & ((5+stepsRight)>=0 & (5+stepsRight)<8)){
                        initializeCurrentPiece();
                        currentPiece[1+stepsDown][3+stepsRight] = 1;
                        currentPiece[1+stepsDown][4+stepsRight] = 1;
                        currentPiece[1+stepsDown][5+stepsRight] = 1;
                        currentPiece[2+stepsDown][3+stepsRight] = 1;
                      }
                      break;
                      
              case 3: if(!(isBlock(0+stepsDown, 3+stepsRight) | isBlock(0+stepsDown, 4+stepsRight) | isBlock(1+stepsDown, 4+stepsRight) | isBlock(2+stepsDown, 4+stepsRight)) & 
                      ((3+stepsRight)>=0 & (3+stepsRight)<8) & ((4+stepsRight)>=0 & (4+stepsRight)<8)){
                        initializeCurrentPiece();
                        currentPiece[0+stepsDown][3+stepsRight] = 1;
                        currentPiece[0+stepsDown][4+stepsRight] = 1;
                        currentPiece[1+stepsDown][4+stepsRight] = 1;
                        currentPiece[2+stepsDown][4+stepsRight] = 1;
                      }
                      break; 
            }
            break;
            
    case 4: switch(rotateContor % 2){
              case 0: if(!(isBlock(0+stepsDown, 3+stepsRight) | isBlock(0+stepsDown, 4+stepsRight) | isBlock(1+stepsDown, 4+stepsRight) | isBlock(1+stepsDown, 5+stepsRight)) & 
                      ((3+stepsRight)>=0 & (3+stepsRight)<8) & ((4+stepsRight)>=0 & (4+stepsRight)<8) & ((5+stepsRight)>=0 & (5+stepsRight)<8)){
                        initializeCurrentPiece();
                        currentPiece[0+stepsDown][3+stepsRight] = 1;
                        currentPiece[0+stepsDown][4+stepsRight] = 1;
                        currentPiece[1+stepsDown][4+stepsRight] = 1;
                        currentPiece[1+stepsDown][5+stepsRight] = 1;
                      }
                      break;
                      
              case 1: if(!(isBlock(0+stepsDown, 5+stepsRight) | isBlock(1+stepsDown, 4+stepsRight) | isBlock(1+stepsDown, 5+stepsRight) | isBlock(2+stepsDown, 4+stepsRight)) & 
                      ((4+stepsRight)>=0 & (4+stepsRight)<8) & ((5+stepsRight)>=0 & (5+stepsRight)<8)){
                        initializeCurrentPiece();
                        currentPiece[0+stepsDown][5+stepsRight] = 1;
                        currentPiece[1+stepsDown][4+stepsRight] = 1;
                        currentPiece[1+stepsDown][5+stepsRight] = 1;
                        currentPiece[2+stepsDown][4+stepsRight] = 1;
                      }
                      break;
            }
            break;
            
    case 5: switch(rotateContor % 2){
              case 0: if(!(isBlock(0+stepsDown, 4+stepsRight) | isBlock(0+stepsDown, 5+stepsRight) | isBlock(1+stepsDown, 3+stepsRight) | isBlock(1+stepsDown, 4+stepsRight)) & 
                      ((3+stepsRight)>=0 & (3+stepsRight)<8) & ((4+stepsRight)>=0 & (4+stepsRight)<8) & ((5+stepsRight)>=0 & (5+stepsRight)<8)){
                        initializeCurrentPiece();
                        currentPiece[0+stepsDown][4+stepsRight] = 1;
                        currentPiece[0+stepsDown][5+stepsRight] = 1;
                        currentPiece[1+stepsDown][3+stepsRight] = 1;
                        currentPiece[1+stepsDown][4+stepsRight] = 1;
                      }
                      break;
                      
              case 1: if(!(isBlock(0+stepsDown, 4+stepsRight) | isBlock(1+stepsDown, 4+stepsRight) | isBlock(1+stepsDown, 5+stepsRight) | isBlock(2+stepsDown, 5+stepsRight)) & 
                      ((4+stepsRight)>=0 & (4+stepsRight)<8) & ((5+stepsRight)>=0 & (5+stepsRight)<8)){
                        initializeCurrentPiece();
                        currentPiece[0+stepsDown][4+stepsRight] = 1;
                        currentPiece[1+stepsDown][4+stepsRight] = 1;
                        currentPiece[1+stepsDown][5+stepsRight] = 1;
                        currentPiece[2+stepsDown][5+stepsRight] = 1;
                      }
                      break;
            }
            break;
            
    case 6: switch(rotateContor % 4){
              case 0: if(!(isBlock(0+stepsDown, 4+stepsRight) | isBlock(1+stepsDown, 3+stepsRight) | isBlock(1+stepsDown, 4+stepsRight) | isBlock(1+stepsDown, 5+stepsRight)) & 
                      ((3+stepsRight)>=0 & (3+stepsRight)<8) & ((4+stepsRight)>=0 & (4+stepsRight)<8) & ((5+stepsRight)>=0 & (5+stepsRight)<8)){
                        initializeCurrentPiece();
                        currentPiece[0+stepsDown][4+stepsRight] = 1;
                        currentPiece[1+stepsDown][3+stepsRight] = 1;
                        currentPiece[1+stepsDown][4+stepsRight] = 1;
                        currentPiece[1+stepsDown][5+stepsRight] = 1;
                      }
                      break;
                      
              case 1: if(!(isBlock(0+stepsDown, 4+stepsRight) | isBlock(1+stepsDown, 4+stepsRight) | isBlock(1+stepsDown, 5+stepsRight) | isBlock(2+stepsDown, 4+stepsRight)) & 
                      ((4+stepsRight)>=0 & (4+stepsRight)<8) & ((5+stepsRight)>=0 & (5+stepsRight)<8)){
                        initializeCurrentPiece();
                        currentPiece[0+stepsDown][4+stepsRight] = 1;
                        currentPiece[1+stepsDown][4+stepsRight] = 1;
                        currentPiece[1+stepsDown][5+stepsRight] = 1;
                        currentPiece[2+stepsDown][4+stepsRight] = 1;
                      }
                      break;
              
              case 2: if(!(isBlock(2+stepsDown, 4+stepsRight) | isBlock(1+stepsDown, 3+stepsRight) | isBlock(1+stepsDown, 4+stepsRight) | isBlock(1+stepsDown, 5+stepsRight)) & 
                      ((3+stepsRight)>=0 & (3+stepsRight)<8) & ((4+stepsRight)>=0 & (4+stepsRight)<8) & ((5+stepsRight)>=0 & (5+stepsRight)<8)){
                        initializeCurrentPiece();
                        currentPiece[2+stepsDown][4+stepsRight] = 1;
                        currentPiece[1+stepsDown][3+stepsRight] = 1;
                        currentPiece[1+stepsDown][4+stepsRight] = 1;
                        currentPiece[1+stepsDown][5+stepsRight] = 1;
                      }
                      break;
                      
              case 3: if(!(isBlock(0+stepsDown, 4+stepsRight) | isBlock(1+stepsDown, 4+stepsRight) | isBlock(1+stepsDown, 3+stepsRight) | isBlock(2+stepsDown, 4+stepsRight)) & 
                      ((3+stepsRight)>=0 & (3+stepsRight)<8) & ((4+stepsRight)>=0 & (4+stepsRight)<8)){
                        initializeCurrentPiece();
                        currentPiece[0+stepsDown][4+stepsRight] = 1;
                        currentPiece[1+stepsDown][4+stepsRight] = 1;
                        currentPiece[1+stepsDown][3+stepsRight] = 1;
                        currentPiece[2+stepsDown][4+stepsRight] = 1;
                      }
                      break; 
            }
            break;
    }
}



void rotate(){
  generateRotatedPiece();
  changeLEDs();
}


void printRows1(boolean displayedData[][8]) // afisare pe primul LED matrix
{
  byte row = 0;
  
  for(int i=0; i<8; i++)
  {
    for(int j=0; j<8; j++){
      if(displayedData[i][j]){
        row |= (1 << j);
      }
    }
    lc1.setRow(0,i,row);
    row = 0;
  }
}

void printRows2(boolean displayedData[][8]) // afisare pe al doilea LED matrix 
{
  byte row = 0;
  
  for(int i=8; i<16; i++)
  {
    for(int j=0; j<8; j++){
      if(displayedData[i][j]){
        row |= (1 << j);
      }
    }
    lc2.setRow(0,i-8,row);
    row = 0;
  }
}

void initialize(){
  for(int i = 0; i < 16; i++){
    for(int j = 0; j < 8; j++){ 
      blocks[i][j] = 0;
      displayedData[i][j] = false;
    }
  }
}

void setup() {
  lc1.shutdown(0,false);       //The LED matrix is in power-saving mode on startup
  lc1.setIntensity(0,15);      // set the brightness to maximum value
  lc1.clearDisplay(0);         // clear the display

  lc2.shutdown(0,false);       //The LED matrix is in power-saving mode on startup
  lc2.setIntensity(0,15);      // set the brightness to maximum value
  lc2.clearDisplay(0);         // clear the display
  
  pinMode(4, INPUT_PULLUP); // ROTATE
  pinMode(5, INPUT_PULLUP); // DOWN
  pinMode(6, INPUT_PULLUP); // RIGHT
  pinMode(7, INPUT_PULLUP); // LEFT

  gameEnded = 0;
  initialize();
  
  generateNewPiece(); // mai apelam cand un block ajunge la "blocks"
  changeLEDs();
}

void loop() {

  if(gameEnded == 0){
    if (delays < millis())
    {
       delays = millis() + delay_;
       moveDown();
    }
   

    buttonRotate = digitalRead(4);
    buttonDown   = digitalRead(5);
    buttonRight  = digitalRead(7);
    buttonLeft   = digitalRead(6);

    if((buttonRotate == LOW) && (buttonRotate_old == HIGH)){
      rotateContor++;
      rotate();
      buttonRotate_old = buttonRotate;
    }

    if((buttonDown == LOW) && (buttonDown_old == HIGH)){
      moveDown();
      buttonDown_old = buttonDown;
    }
  
    if((buttonRight == LOW) && (buttonRight_old == HIGH)){
      moveRight();
      buttonRight_old = buttonRight;
    }

    if((buttonLeft == LOW) && (buttonLeft_old == HIGH)){
      moveLeft();
      buttonLeft_old = buttonLeft;
    }

    buttonRotate_old = buttonRotate;
    buttonDown_old   = buttonDown;
    buttonRight_old  = buttonRight;
    buttonLeft_old   = buttonLeft;
    
    delay(100);
    } else {
      gameOver();
    }
}
