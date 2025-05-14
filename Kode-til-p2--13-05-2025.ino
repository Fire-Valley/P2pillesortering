#include <Servo.h> // Include the Servo library to control servo motors

const int NUM_SERVOS = 6;
Servo servos[NUM_SERVOS] = { Servo(), Servo(), Servo(), Servo(), Servo(), Servo() }; // Array to store servo objects
int currentPositions[NUM_SERVOS] = {0, 0, 0, 0, 0, 0}; // Array to track current servo positions

constexpr int NUM_DISPENSERS = 2;  // Number of dispensers
constexpr int NUM_THETAS = 10;  // Number of theta-sets
constexpr int NUM_POSITIONS = 3;   // Number of loading positions (the days for the pill storage)

constexpr int offset[NUM_SERVOS] = {10, 10+90, 63, 0, -10, -20}; // Array to store the servo offsets 
constexpr int Startpos[NUM_SERVOS] = {90, -80, 0, 80, 107, 120}; // array to find the start position 

constexpr int theta[NUM_THETAS][NUM_SERVOS] = { // 2d array to store all of the thetas without the offsets. 
  {113, -80, 0, 80, 107, 120},  //Thetaset_1 (outside dispenser 1)
  {70, 0, 0, 80, 107, 120},  //Thetaset_2 (outside dispenser 2)
  {113, 0, 0, 80, 107, 120},  //Thetaset_3 (dispenser pressed 1)
  {70, 0, 0, 80, 107, 120},  //Thetaset_4 (dispenser pressed 2)
  {25, 32, 0, 105, 20, 20},  //Thetaset_5 (position monday)
  {12, 18, 0, 90, 20, 120},  //Thetaset_6 (position tuesday)
  {5, 5, 0, 80, 20, 120},  //Thetaset_7 (position wednesday)
  {25, 32, 0, 105, 20, 20},  //Thetaset_8 (position monday turned)
  {12, 18, 0, 90, 20, 20},  //Thetaset_9 (position tuesday turned)
  {5, 5, 0, 80, 20, 20}   //Thetaset_10 (position wednseday turned)
};

constexpr int Outside_dispenser[NUM_DISPENSERS][NUM_SERVOS] = { 
  { // Position outside dispenser 1 (Paracetamol) 
    abs(theta[0][0] + offset[0]), 
    abs(theta[0][1] + offset[1]),  
    abs(theta[0][2] + offset[2]), 
    abs(theta[0][3] + offset[3]), 
    abs(theta[0][4] + offset[4]), 
    abs(theta[0][5] + offset[5])
  }, 

  { // Position outside dispenser 2 (Lamotrigin) 
    abs(theta[1][0] + offset[0]), 
    abs(theta[1][1] + offset[1]), 
    abs(theta[1][2] + offset[2]), 
    abs(theta[1][3] + offset[3]),
    abs(theta[1][4] + offset[4]), 
    abs(theta[1][5] + offset[5])
  }
};

constexpr int Pressed_dispenser[NUM_DISPENSERS][NUM_SERVOS] = {
  { // Dispenser 1 pressed (paracetamol) 
    abs(theta[2][0] + offset[0]), 
    abs(theta[2][1] + offset[1]), 
    abs(theta[2][2] + offset[2]), 
    abs(theta[2][3] + offset[3]), 
    abs(theta[2][4] + offset[4]), 
    abs(theta[2][5] + offset[5]), 
  }, 

  { // Dispenser 2 pressed (Lamotrigin)
    abs(theta[3][0] + offset[0]), 
    abs(theta[3][1] + offset[1]), 
    abs(theta[3][2] + offset[2]), 
    abs(theta[3][3] + offset[3]), 
    abs(theta[3][4] + offset[4]), 
    abs(theta[3][5] + offset[5]), 
  }  
};

// Define load positions for Monday, Tuesday, and Wednesday
constexpr int loadPositions[NUM_POSITIONS][NUM_SERVOS] = {
  { // Monday's loading position
    abs(theta[4][0] + offset[0]), 
    abs(theta[4][1] + offset[1]), 
    abs(theta[4][2] + offset[2]), 
    abs(theta[4][3] + offset[3]), 
    abs(theta[4][4] + offset[4]), 
    abs(theta[4][5] + offset[5])
  }, 

  { // Tuesday's loading position
    abs(theta[5][0] + offset[0]), 
    abs(theta[5][1] + offset[1]), 
    abs(theta[5][2] + offset[2]), 
    abs(theta[5][3] + offset[3]), 
    abs(theta[5][4] + offset[4]), 
    abs(theta[5][5] + offset[5])
  }, 

  { // Wednesday's loading position
    abs(theta[6][0] + offset[0]), 
    abs(theta[6][1] + offset[1]), 
    abs(theta[6][2] + offset[2]), 
    abs(theta[6][3] + offset[3]), 
    abs(theta[6][4] + offset[4]), 
    abs(theta[6][5] + offset[5])
  }, 
};

constexpr int loadPosTurned[NUM_POSITIONS][NUM_SERVOS] = {
  { // Monday's loading position turned
    abs(theta[7][0] + offset[0]), 
    abs(theta[7][1] + offset[1]), 
    abs(theta[7][2] + offset[2]), 
    abs(theta[7][3] + offset[3]), 
    abs(theta[7][4] + offset[4]), 
    abs(theta[7][5] + offset[5])
  }, 

  { // Tuesday's loading position turned
    abs(theta[8][0] + offset[0]), 
    abs(theta[8][1] + offset[1]), 
    abs(theta[8][2] + offset[2]), 
    abs(theta[8][3] + offset[3]), 
    abs(theta[8][4] + offset[4]), 
    abs(theta[8][5] + offset[5])
  }, 

  { // Wednesday's loading position turned
    abs(theta[9][0] + offset[0]), 
    abs(theta[9][1] + offset[1]), 
    abs(theta[9][2] + offset[2]), 
    abs(theta[9][3] + offset[3]), 
    abs(theta[9][4] + offset[4]), 
    abs(theta[9][5] + offset[5])
  }, 
};

// Enum defining medicine types
enum MedicineType { Panodil, Lamotrigin };

// Struct to store medicine details (type, quantity, dispenser used, and loading position)
struct Medicine {
  MedicineType type;  // Medicine type
  int quantity;       // Number of pills to dispense
  int dispenser;      // Which dispenser to use
  int loadPos;        // Loading position after dispensing
};

// Weekly medicine plan: Defines medicine distribution for three days
constexpr Medicine weekPlan[3][2] = {
  { {Panodil, 1, 0, 0}, {Lamotrigin, 2, 1, 0} }, // Monday
  { {Panodil, 0, 0, 1}, {Lamotrigin, 1, 1, 1} }, // Tuesday
  { {Panodil, 2, 0, 2}, {Lamotrigin, 0, 1, 2} }  // Wednesday
};

// Setup function (runs once at startup)
void setup() {
  for (int i = 0; i < NUM_SERVOS; i++) {
    servos[i].attach(i + 8); // Attach servos to pins 8-13
  }
  moveToPosition(Startpos);
}

// Function to gradually move servos to a target position
void moveToPosition(const int target[NUM_SERVOS]) {
  bool reached = false;
  
  while (!reached) {
    reached = true;

    for (int i = 0; i < NUM_SERVOS; i++) {
      if (currentPositions[i] < target[i]) {
        currentPositions[i]++;
        reached = false;
      } else if (currentPositions[i] > target[i]) {
        currentPositions[i]--;
        reached = false;
      }
      servos[i].write(currentPositions[i]); // Move servo incrementally
    }

    delay(50); // Small delay for smoother movement
  }
}

// Function to move servos to a dispenser position
void Presseddispenser(int dispenserIndex) {
  moveToPosition(Pressed_dispenser[dispenserIndex]);
}

// Function to move servos to a loading position
void loadAtPosition(int loadPositionIndex) {
  moveToPosition(loadPositions[loadPositionIndex]);
}

// Function to move servos to a dispensing position
void Outsidedispenser(int dispenserIndex) {
  moveToPosition(Outside_dispenser[dispenserIndex]);
}

// Function to move servos to a loading position
void loadAtdifferentPosition(int loadPositionIndex) {
  moveToPosition(loadPosTurned[loadPositionIndex]);
}

// Function to press the dispenser multiple times
void Dispensera(int dispenserIndex, int times) {
  for (int i = 0; i < times; i++) {
    Outsidedispenser(dispenserIndex); // Move to correct dispensing position
    Presseddispenser(dispenserIndex); // Activate dispenser
    delay(500); // Short delay to ensure proper activation
  }
}

// Function to execute the weekly medicine plan
void executePlan() {
  for (int day = 0; day < 3; day++) {  // Iterate through Monday-Wednesday
    for (int med = 0; med < 2; med++) {  // Iterate through two medicines per day
      if (weekPlan[day][med].quantity > 0) { // Only dispense if there's medicine planned
        Medicine dailyPlan = weekPlan[day][med]; // Get today's medicine plan
        
        Dispensera(dailyPlan.dispenser, dailyPlan.quantity); // Dispense medicine
        delay(1000); // Simulate processing time
        
        loadAtPosition(dailyPlan.loadPos); // Move servos to the loading position
        loadAtdifferentPosition(dailyPlan.loadPos);

        delay(2000); // Wait before processing the next medicine
      }
    }
  }
}

// Loop function (runs continuously)
void loop() {
  executePlan(); // Execute medicine dispensing plan
  delay(100000); // Wait before running the plan again
}