/*State-Diagram: SWITCHING BETWEEN CONTROL ALGORITHMS
    start
      |                   +-------\
      V                   V       |    
  --------              ------    |
 |  OFF   |<--click-+->|  ON  |   |    Bright WHITE 
  --------          |   ------    |    
 Light WHITE        |     |       |
 on the first start |double-click |
                    |     |       |
        |     V       |
                    |   -------   |
                    +- | BASIC |  |    RED & GREEN
                    |   -------   |
                    |     |       |
                    |double-click |        - RED on the side  is OUT
                    |     |       |        - GREEN on the side is IN
                    |     V       |
                    |   ------    |
                    +--|  PI  |---/    ORANGE & BLUE
                        ------ 
 */
// TRANSITION FUNCTIONS ----------------------------------------------
// On LONG PRESS: empty function, not used
void myPressFunction() {
  // Nothing to do  
}

// On SINGLE CLICK: this function will be called when the button was pressed 1 time and them some time has passed.
void myClickFunction() {
  if (nextState == _OFF) {
    nextState = _ON;
  }
  else
    nextState = _OFF;
  }

// On DOUBLE CLICK: this function will be called when the button was pressed 2 times in a short timeframe.
void myDoubleClickFunction() {
  
  if (nextState == _ON) {
    nextState = _BASIC;

  } else if (nextState == _BASIC) {
    nextState = _PI;

  } else if (nextState == _PI) {
    nextState = _ON;
  } // if
}

