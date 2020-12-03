
// [16][8] = dimensiunea totala a led-matrix-ului
boolean currentPiece[16][8]; // piesa curenta, pe care trebuie s-o asezam acuma
boolean blocks[16][8]; // toate piesele puse pana acuma
boolean displayedData[16][8]; // datele ce vor fi afisate; toate pozitiile au valoarea 1 (aprins) sau 0 (stins)

int buttonUp;
int buttonDown;
int buttonRight;
int buttonLeft;

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

void generateNewPiece(){
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
    for(int j = 0; j < 7; j++){
      displayedData[i][j] = blocks[i][j] | currentPiece[i][j]; // o pozitie din displayedData va fi aprinsa, doar daca e partea blocului sau daca acolo este piesaa curenta
    }
  }
}


void moveLeft(){
  for(int i = 0; i < 16; i++){
    for(int j = 0; j < 7; j++){
      currentPiece[i][j] = currentPiece[i][j+1]; // mutam coloanele cu o pozitie la stanga
    }
  }

  for(int i = 0; i < 16; i++){
    currentPiece[i][7] = 0; // coloana cea mai din dreapta va primi valoarea 0 -> stins
  }

  changeLEDs();
}


void moveRight(){
  for(int i = 0; i < 16; i++){
    for(int j = 7; j > 0; j--){
      currentPiece[i][j] = currentPiece[i][j-1]; // mutam coloanele cu o pozitie la dreapta
    }
  }

  for(int i = 0; i < 16; i++){
    currentPiece[i][0] = 0; // coloana cea mai din stanga va primi valoarea 0 -> stins
  }
  
  changeLEDs();
}


void moveDown(){
  for(int i = 15; i > 0; i--){
    for(int j = 0; j < 8; j++){
      currentPiece[i][j] = currentPiece[i-1][j]; // mutam liniile cu o linie mai in jos
    }
  }

  for(int j = 0; j < 8; j++){
    currentPiece[0][j] = 0; // prima linie va primi valoarea 0 -> stins
  }

  changeLEDs();
}


void rotate(){
  // forma rotata depinde de tipul piesei 
  switch (pieceNumber){
    case 0: break; // piesa 0 arata la fel si cand este rotita
    case 1:      
  }
  
  changeLEDs();
}



void setup() {
  pinMode(2, INPUT_PULLUP); // UP
  pinMode(3, INPUT_PULLUP); // DOWN
  pinMode(4, INPUT_PULLUP); // RIGHT
  pinMode(5, INPUT_PULLUP); // LEFT

  generateNewPiece();
  changeLEDs();
  
}

void loop() {

  // moveDown(); // la fiecare puls de ceas se va apela moveDown()
  
  buttonUp    = digitalRead(2);
  buttonDown  = digitalRead(3);
  buttonRight = digitalRead(4);
  buttonLeft  = digitalRead(5);

  if(buttonUp == 1){
    rotate();
  } else {
    if(buttonRight == 1){
      moveRight();
    } else {
      if(buttonLeft == 1){
        moveLeft();
      } else {
        if(buttonDown == 1){
          moveDown();
        }
      }
    }
  }
  
}
