 // STATES -----------------------------------------------------------
typedef enum {
  _OFF,   // stop Robot
  _ON,    // start Robot with control loop of state _BASIC
  _BASIC, // Basic loop control: turn RIGHT on left side, turn LEFT on righ side, move FORWARD on line
  _PI     // Pseudo-PI control (proportional-integral)
} 
States;

States nextState = _OFF; // Robot stopped when starting

// -----------------------
int moveSpeed = 255*0.5; // Velocidad de referencia //230 in ref Program
int moveSpeedTurn = 255; // Velocidad de referencia //230 in ref Program
float SIG1 = -1.0; //Left motor scale factor, negative for going forward
float SIG2 = 1.0;  //Right motor scale factor
// ----------------------------------------------------------------------------
