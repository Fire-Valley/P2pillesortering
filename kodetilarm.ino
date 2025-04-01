#include <Servo.h>

Servo myservo13;  
Servo myservo12;
Servo myservo11;  
Servo myservo10;  
Servo myservo9;  
Servo myservo8;  

// Antallet af dispenser positioner og positions er positionerne for mandag-søndag
const int NUM_DISPENSERS = 5;
const int NUM_POSITIONS = 7;

// Dispenserpositioner (de er random, vi skal bruge en funktion for at udregne disse via kinematik)
int dispensers[NUM_DISPENSERS][6] = {
  {50, 100, 90, 80, 70, 90}, // Dispenser 1 (Antibiotika)
  {60, 110, 80, 85, 75, 95}, // Dispenser 2 (Panodiler)
  {70, 120, 85, 90, 80, 100}, // Dispenser 3 (Smertestillende)
  {80, 130, 90, 95, 85, 110}, // Dispenser 4 (Vitamin)
  {90, 140, 95, 100, 90, 120}  // Dispenser 5 (Skibidi)
};

// Aflæsnings postioner (mandag-søndag)
int loadPositions[NUM_POSITIONS][6] = {
  {100, 90, 90, 90, 90, 90}, // Mandag
  {110, 100, 90, 85, 95, 100}, // Tirsdag
  {120, 110, 85, 95, 100, 110}, // Onsdag
  {130, 120, 80, 100, 105, 120}, // Torsdag
  {140, 130, 75, 105, 110, 130}, // Fredag
  {150, 140, 70, 110, 115, 140}, // Lørdag
  {160, 150, 65, 115, 120, 150}  // Søndag
};

// Definere medicintyperne i en enum (enum gør koden mere læsbar, vi kunne have brugt int men så ville de hedde 1-5, med enum får de også et indeks automatisk 0-4)
enum MedicineType {
  Antibiotika,
  Panodil,
  Smertestillende,
  Vitamin,
  Skibidi
};

// Structen gemmer medicinbehovet for en given dag
struct Medicine {
  MedicineType type; // Medicinens navn
  int quantity;      // Antal af piller
  int dispenser;     // Hvilken dispenser medicinen er i (0-4)
  int loadPos;       // Hvilken aflæsnings position der skal bruges (0-6)
};

// Ugeplan for medicin
// Her er alle medicintyper foruddefineret, vi skal kun ændre quantity
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
    {Antibiotika, 0, 0, 1},    // 0 Antibiotika
    {Smertestillende, 4, 2, 1}, // 4 Smertestillende fra dispenser 3 til position 1
    {Vitamin, 0, 3, 1},       
    {Skibidi, 0, 4, 1}           
  },
  { // Onsdag
    {Panodil, 0, 1, 2},        // 0 Panodiler
    {Antibiotika, 3, 0, 2},    // 3 Antibiotika fra dispenser 1 til position 2
    {Smertestillende, 0, 2, 2}, // 0 Smertestillende
    {Vitamin, 2, 3, 2},        // 2 Vitamin fra dispenser 4 til position 2 (onsdag)
    {Skibidi, 0, 4, 2}           
  },
  { // Torsdag
    {Panodil, 0, 1, 3},        // 0 Panodiler
    {Antibiotika, 0, 0, 3},    // 0 Antibiotika
    {Smertestillende, 6, 2, 3}, // 6 Smertestillende fra dispenser 3 til position 3
    {Vitamin, 0, 3, 3},        // 0 Vitamin
    {Skibidi, 0, 4, 3}           
  },
  { // Fredag
    {Panodil, 5, 1, 4},        // 5 Panodiler fra dispenser 2 til position 4
    {Antibiotika, 4, 0, 4},    // 4 Antibiotika fra dispenser 1 til position 4
    {Smertestillende, 0, 2, 4}, // 0 Smertestillende
    {Vitamin, 0, 3, 4},        // 0 Vitamin
    {Skibidi, 0, 4, 4}           
  },
  { // Lørdag
    {Panodil, 0, 1, 5},        // 0 Panodiler
    {Antibiotika, 0, 0, 5},    // 0 Antibiotika
    {Smertestillende, 0, 2, 5}, // 0 Smertestillende
    {Vitamin, 3, 3, 5},        // 3 Vitamin fra dispenser 4 til position 5
    {Skibidi, 0, 4, 5}           
  },
  { // Søndag
    {Panodil, 5, 1, 6},        // 5 Panodiler fra dispenser 2 til position 6
    {Antibiotika, 4, 0, 6},    // 4 Antibiotika fra dispenser 1 til position 6
    {Smertestillende, 4, 2, 6}, // 4 Smertestillende fra dispenser 3 til position 6
    {Vitamin, 0, 3, 6},        // 0 Vitamin
    {Skibidi, 0, 4, 6}           
  }
};

// Attacher motorer til pins
void setup() {
  myservo13.attach(13);
  myservo12.attach(12);
  myservo11.attach(11);
  myservo10.attach(10);
  myservo9.attach(9);
  myservo8.attach(8);
  
  // Startposition
  moveToPosition(dispensers[0]);
}
// Denne funktionen styrer alle servor motorne og styrer dem hen til en given position(de bliver taget fra et af arraysne i starten???)
void moveToPosition(int position[6]) {
  // Bevæger robotten til en specifik position
  myservo13.write(position[0]);
  myservo12.write(position[1]);
  myservo11.write(position[2]);
  myservo10.write(position[3]);
  myservo9.write(position[4]);
  myservo8.write(position[5]);
  delay(1000);  // Giv tid til at bevæge sig
}
// Vi har dispenser array i starten (bevæger motorne)
void moveToDispenser(int dispenserIndex) {
  // Bevæger robotten til en af dispenserne
  moveToPosition(dispensers[dispenserIndex]);
}
// Det samme her
void loadAtPosition(int loadPositionIndex) {
  // Læs af i en af de 7 positioner
  moveToPosition(loadPositions[loadPositionIndex]);
}

void executePlan() {
  for (int day = 0; day < 7; day++) {  // Vi gennemgår alle ugens dage
    
    for (int med = 0; med < 5; med++) { // For hver dag, gennemgå alle medicintyper (Hvilket er 5)
      // Tjek om der er en medicin for den dag (quantity > 0) så den ikke køre forgæves
      if (weekPlan[day][med].quantity > 0) {
        Medicine dailyPlan = weekPlan[day][med];
        
        // Bevæge til den korrekte dispenser
        moveToDispenser(dailyPlan.dispenser);
        
        // Læs af i den korrekte position
        loadAtPosition(dailyPlan.loadPos);
        
        // Vi skal tilføje kode hvis den skal have flere af samme pille
        for (int i = 0; i < dailyPlan.quantity; i++) {
          // Eksempel på at trykke på dispenseren flere gange:
          // trykPåDispenser();  Vi skal definere en funktion sådan vi kan trykke flere gange
          delay(500); // Vente lidt mellem tryk
        }
        
        delay(2000); // Vent et stykke tid før næste dags plan
      }
    }
  }
}

void loop() {
  // Kald på funktionen til at køre planen for hele ugen
  executePlan();
  delay(10000);  // Gentag ikke for hurtigt
}
