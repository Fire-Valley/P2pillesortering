#include <Servo.h>

Servo myservo13;  
Servo myservo12;
Servo myservo11;  
Servo myservo10;  
Servo myservo9;  
Servo myservo8;  

// Global array storing the current positions for each servo.
// Order: {myservo13, myservo12, myservo11, myservo10, myservo9, myservo8}
int currentPositions[6] = {0, 0, 0, 0, 0, 0};

// Antallet af dispenser positioner og positions (mandag-søndag)
const int NUM_DISPENSERS = 5;
const int NUM_POSITIONS = 7;

// Dispenserpositioner (disse værdier skal i praksis beregnes med kinematik)
int dispensers[NUM_DISPENSERS][6] = {
  {150, 150, 150, 150, 150, 150}, // Dispenser 1 (Antibiotika)
  {100, 100, 100, 100, 100, 100}, // Dispenser 2 (Panodiler)
  {70, 70, 70, 70, 70, 70},       // Dispenser 3 (Smertestillende)
  {180, 180, 180, 180, 180, 180}, // Dispenser 4 (Vitamin)
  {90, 90, 90, 90, 90, 90}        // Dispenser 5 (Skibidi)
};

// Aflæsningspositioner (mandag-søndag)
int loadPositions[NUM_POSITIONS][6] = {
  {100, 100, 100, 100, 100, 100}, // Mandag
  {100, 100, 100, 100, 100, 100}, // Tirsdag
  {100, 100, 100, 100, 100, 100}, // Onsdag
  {100, 100, 100, 100, 100, 100}, // Torsdag
  {100, 100, 100, 100, 100, 100}, // Fredag
  {100, 100, 100, 100, 100, 100}, // Lørdag
  {100, 100, 100, 100, 100, 100}  // Søndag
};

// Brug af enum for at gøre medicinnavne mere læselige
enum MedicineType {
  Antibiotika,
  Panodil,
  Smertestillende,
  Vitamin,
  Skibidi
};

// Struktur til at holde medicinbehov for den enkelte dag
struct Medicine {
  MedicineType type; // Medicinens type
  int quantity;      // Antal piller
  int dispenser;     // Hvilken dispenser (index 0-4)
  int loadPos;       // Hvilken aflæsningsposition (index 0-6)
};

// Ugeplan for medicin – her justeres blot antallet af piller (quantity)
Medicine weekPlan[7][5] = {
  { // Mandag
    {Panodil, 0, 1, 0},        
    {Antibiotika, 0, 0, 0},    
    {Smertestillende, 0, 2, 0}, 
    {Vitamin, 0, 3, 0},        
    {Skibidi, 0, 4, 0}           
  },
  { // Tirsdag
    {Panodil, 7, 1, 1},        // 7 Panodiler fra dispenser 2 til position 1
    {Antibiotika, 0, 0, 1},    
    {Smertestillende, 4, 2, 1}, // 4 Smertestillende fra dispenser 3 til position 1
    {Vitamin, 0, 3, 1},       
    {Skibidi, 0, 4, 1}           
  },
  { // Onsdag
    {Panodil, 0, 1, 2},        
    {Antibiotika, 3, 0, 2},    // 3 Antibiotika fra dispenser 1 til position 2
    {Smertestillende, 0, 2, 2}, 
    {Vitamin, 2, 3, 2},        // 2 Vitamin fra dispenser 4 til position 2
    {Skibidi, 0, 4, 2}           
  },
  { // Torsdag
    {Panodil, 0, 1, 3},        
    {Antibiotika, 0, 0, 3},    
    {Smertestillende, 6, 2, 3}, // 6 Smertestillende fra dispenser 3 til position 3
    {Vitamin, 0, 3, 3},        
    {Skibidi, 0, 4, 3}           
  },
  { // Fredag
    {Panodil, 5, 1, 4},        // 5 Panodiler fra dispenser 2 til position 4
    {Antibiotika, 4, 0, 4},    // 4 Antibiotika fra dispenser 1 til position 4
    {Smertestillende, 0, 2, 4}, 
    {Vitamin, 0, 3, 4},        
    {Skibidi, 0, 4, 4}           
  },
  { // Lørdag
    {Panodil, 0, 1, 5},        
    {Antibiotika, 0, 0, 5},    
    {Smertestillende, 0, 2, 5}, 
    {Vitamin, 3, 3, 5},        // 3 Vitamin fra dispenser 4 til position 5
    {Skibidi, 0, 4, 5}           
  },
  { // Søndag
    {Panodil, 5, 1, 6},        // 5 Panodiler fra dispenser 2 til position 6
    {Antibiotika, 4, 0, 6},    // 4 Antibiotika fra dispenser 1 til position 6
    {Smertestillende, 4, 2, 6}, // 4 Smertestillende fra dispenser 3 til position 6
    {Vitamin, 0, 3, 6},        
    {Skibidi, 0, 4, 6}           
  }
};

void setup() {
  myservo13.attach(13);
  myservo12.attach(12);
  myservo11.attach(11);
  myservo10.attach(10);
  myservo9.attach(9);
  myservo8.attach(8);
  
  // Start i en basisposition
  // Use the slow movement function to update our currentPositions gradually.
  moveToPosition(dispensers[0]);
}

// Updated moveToPosition function for slow, gradual movement.
//
// This function interpolates from the current positions to the target positions
// by incrementing or decrementing one unit at a time. The delay in the loop (15 ms)
// controls the overall speed.
void moveToPosition(int target[6]) {
  bool reached = false;
  while (!reached) {
    reached = true;
    for (int i = 0; i < 6; i++) {
      // Move up or down one step toward the target for servo i.
      if (currentPositions[i] < target[i]) {
        currentPositions[i]++;
        reached = false;
      } else if (currentPositions[i] > target[i]) {
        currentPositions[i]--;
        reached = false;
      }
    }
    // Update all servo positions
    myservo13.write(currentPositions[0]);
    myservo12.write(currentPositions[1]);
    myservo11.write(currentPositions[2]);
    myservo10.write(currentPositions[3]);
    myservo9.write(currentPositions[4]);
    myservo8.write(currentPositions[5]);
    delay(50);  // Adjust delay to control the speed of movement
  }
}

// Flytter robotten til en bestemt dispenserposition
void moveToDispenser(int dispenserIndex) {
  moveToPosition(dispensers[dispenserIndex]);
}

// Flytter robotten til en bestemt aflastningsposition (f.eks. en dag)
void loadAtPosition(int loadPositionIndex) {
  moveToPosition(loadPositions[loadPositionIndex]);
}

// Denne funktion flytter dispenseren bagud med en offset på 5 cm pr. pille.
// Her antages det, at position[0] styrer bevægelsen fremad/bagud, og at 1 enhed svarer til ca. 1 cm.
void moveToDispenserWithOffset(int dispenserIndex, int pillNumber) {
  int positionWithOffset[6];
  // Kopiér basispositionen for denne dispenser
  for (int i = 0; i < 6; i++) {
    positionWithOffset[i] = dispensers[dispenserIndex][i];
  }
  // For hver pille, flyt dispenseren 5 enheder (ca. 5 cm) bagud.
  positionWithOffset[0] -= 5 * pillNumber;
  moveToPosition(positionWithOffset);
}

void executePlan() {
  for (int day = 0; day < 7; day++) {  // Loop over ugens dage
    for (int med = 0; med < 5; med++) {  // For hver medicintype
      if (weekPlan[day][med].quantity > 0) {
        Medicine dailyPlan = weekPlan[day][med];
        
        // Bevæg til den grundlæggende dispenserposition
        moveToDispenser(dailyPlan.dispenser);
        
        // Bevæg til den foruddefinerede aflæsningsposition for dagen
        loadAtPosition(dailyPlan.loadPos);
        
        // For hver pille skal dispenseren flyttes bagud med en yderligere 5 cm
        for (int i = 0; i < dailyPlan.quantity; i++) {
          moveToDispenserWithOffset(dailyPlan.dispenser, i + 1);
          delay(500);  // Kort pause mellem udløsning af hver pille
        }
        
        delay(2000);  // Pause før næste dags plan
      }
    }
  }
}

void loop() {
  // Kør den planlagte uges medicinuddeling
  executePlan();
  delay(100000);  // Undgå for hyppig gentagelse
}