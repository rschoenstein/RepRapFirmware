/****************************************************************************************************

RepRapFirmware - Move

This is all the code to deal with movement and kinematics.

-----------------------------------------------------------------------------------------------------

Version 0.1

18 November 2012

Adrian Bowyer
RepRap Professional Ltd
http://reprappro.com

Licence: GPL

****************************************************************************************************/

#ifndef MOVE_H
#define MOVE_H

#define DDA_RING_LENGTH 5
#define LOOK_AHEAD_RING_LENGTH 20
#define LOOK_AHEAD 7

enum MovementProfile
{
  moving = 0,  // Ordinary trapezoidal-velocity-profile movement
  noFlat = 1,  // Triangular profile movement
  change = 2   // To make this movement, the initial and final velocities must change
};

enum MovementState
{
  unprocessed = 0,
  vCosineSet = 1,
  upPass = 2,
  complete = 4,
  released = 8
};

enum MovementType
{
  noMove = 0,
  xyMove = 1,
  zMove = 2,
  eMove = 4 
};

class LookAhead
{  
  public:
    LookAhead(Move* m, Platform* p, LookAhead* n);
    void Init(float ep[], float vv, boolean ce);
    LookAhead* Next();
    LookAhead* Previous();
    float* EndPoint();
    float V();
    void SetV(float vv);
    int8_t Processed();
    void SetProcessed(MovementState ms);
    void SetDriveZeroEndSpeed(float a, int8_t drive);
    boolean CheckEndStops();
    void Release();
    
  friend class Move;
    
  private:
    Move* move;
    Platform* platform;
    LookAhead* next;
    LookAhead* previous;
    float endPoint[DRIVES+1];
    float Cosine();
    boolean checkEndStops;
    float cosine;
    float v;
    float instantDv;
    volatile int8_t processed;
};


class DDA
{
  public: 
    DDA(Move* m, Platform* p, DDA* n);
    MovementProfile Init(LookAhead* lookAhead, float& u, float& v);
    void Start(boolean noTest);
    void Step(boolean noTest);
    boolean Active();
    DDA* Next();
    float InstantDv();
    
  friend class Move;

  private:
    Move* move;
    Platform* platform;
    DDA* next;
    LookAhead* myLookAheadEntry;
    long counter[DRIVES];
    long delta[DRIVES];
    boolean directions[DRIVES];
    long totalSteps;
    long stepCount;
    boolean checkEndStops;
    float timeStep;
    float velocity;
    long stopAStep;
    long startDStep;
    float distance;
    float dCross;
    float acceleration;
    float instantDv;
    volatile boolean active;
};




class Move
{   
  public:
  
    Move(Platform* p, GCodes* g);
    void Init();
    void Spin();
    void Exit();
    boolean GetCurrentState(float m[]);
    void Interrupt();
    void InterruptTime();
    boolean AllMovesAreFinished();
    void ResumeMoving();
    void DoLookAhead();
    void HitLowStop(int8_t drive, LookAhead* la);
    void HitHighStop(int8_t drive, LookAhead* la);
    void Diagnostics();
    
  friend class DDA;
    
  private:
  
    boolean DDARingAdd(LookAhead* lookAhead);
    DDA* DDARingGet();
    boolean DDARingEmpty();
    boolean NoLiveMovement();
    boolean DDARingFull();
    boolean GetDDARingLock();
    void ReleaseDDARingLock();
    boolean LookAheadRingEmpty();
    boolean LookAheadRingFull();
    boolean LookAheadRingAdd(float ep[], float vv, boolean ce);
    LookAhead* LookAheadRingGet();
    int8_t GetMovementType(float sp[], float ep[]);

    
    Platform* platform;
    GCodes* gCodes;
    
    DDA* dda;
    DDA* ddaRingAddPointer;
    DDA* ddaRingGetPointer;
    volatile boolean ddaRingLocked;
    
    LookAhead* lookAheadRingAddPointer;
    LookAhead* lookAheadRingGetPointer;
    LookAhead* lastMove;
    DDA* lookAheadDDA;
    int lookAheadRingCount;

    float lastTime;
    boolean addNoMoreMoves;
    boolean active;
    boolean checkEndStopsOnNextMove;
    float currentFeedrate;
    float nextMove[DRIVES + 1];  // Extra is for feedrate
    float stepDistances[(1<<AXES)]; // Index bits: lsb -> dx, dy, dz <- msb
    float extruderStepDistances[(1<<(DRIVES-AXES))]; // NB - limits us to 5 extruders
};

//********************************************************************************************************

inline LookAhead* LookAhead::Next()
{
  return next;
}

inline LookAhead* LookAhead::Previous()
{
  return previous;
}


inline void LookAhead::SetV(float vv)
{
  v = vv;
}

inline float* LookAhead::EndPoint() 
{
  return endPoint;
}


inline float LookAhead::V() 
{
  return v;
}

inline int8_t LookAhead::Processed() 
{
  return processed;
}

inline void LookAhead::SetProcessed(MovementState ms)
{
  if(ms == unprocessed)
    processed = unprocessed;
  else
    processed |= ms;
}

inline void LookAhead::Release()
{
  SetProcessed(released);
}

inline boolean LookAhead::CheckEndStops() 
{
  return checkEndStops;
}

inline void LookAhead::SetDriveZeroEndSpeed(float a, int8_t drive)
{
  endPoint[drive] = a;
  cosine = 2.0;
  v = 0.0; 
}

//******************************************************************************************************

inline boolean DDA::Active()
{
  return active;
}

inline DDA* DDA::Next()
{
  return next;
}

inline float DDA::InstantDv()
{
  return instantDv;
}


//***************************************************************************************

inline boolean Move::DDARingEmpty()
{
  return ddaRingGetPointer == ddaRingAddPointer;
}

inline boolean Move::NoLiveMovement()
{
  if(dda != NULL)
    return false;
  return DDARingEmpty();
}

// Leave a gap of 2 as the last Get result may still be being processed

inline boolean Move::DDARingFull()
{
  return ddaRingAddPointer->Next()->Next() == ddaRingGetPointer;
}

inline boolean Move::LookAheadRingEmpty()
{
  return lookAheadRingCount == 0;
}

// Leave a gap of 2 as the last Get result may still be being processed

inline boolean Move::LookAheadRingFull()
{
  if(!(lookAheadRingAddPointer->Processed() & released))
    return true;
  return lookAheadRingAddPointer->Next()->Next() == lookAheadRingGetPointer;  // probably not needed; just return the boolean in the if above
}

inline boolean Move::GetDDARingLock()
{
  if(ddaRingLocked)
    return false;
  ddaRingLocked = true;
  return true;
}

inline void Move::ReleaseDDARingLock()
{
  ddaRingLocked = false;
}

// To wait until all the current moves in the buffers are
// complete, call this function repeatedly and wait for it to
// return true.  Then do whatever you wanted to do after all
// current moves have finished.  THEN CALL THE ResumeMoving() FUNCTION
// OTHERWISE NOTHING MORE WILL EVER HAPPEN.

inline boolean Move::AllMovesAreFinished()
{
  addNoMoreMoves = true;
  return LookAheadRingEmpty() && NoLiveMovement();
}

inline void Move::ResumeMoving()
{
  addNoMoreMoves = false;
}

inline void Move::HitLowStop(int8_t drive, LookAhead* la)
{
  la->SetDriveZeroEndSpeed(0.0, drive);
}

inline void Move::HitHighStop(int8_t drive, LookAhead* la)
{
  la->SetDriveZeroEndSpeed(platform->AxisLength(drive), drive);
}


#endif