#include "IO.h"
#include "control.h"

static double P = 80;
static double I = 20;
static double D = 40;


void setup()
{
  startup();
}

void loop()
{
  
  IO();


  if (mode == 0)
  {
    disable();
  }



  if (mode == 1)
  {
    OffenerRegelkreis();
  }



  if (mode == 2)
  {
    OnOff();
  }



  if (mode == 3)
  {
    tempControl.SetTunings(P, 0, 0); //P
    tempControl.Compute();
  }



  if (mode == 4)
  {
    tempControl.SetTunings(0, I, 0); //I
    tempControl.Compute();
  }


  
  if (mode == 5)
  {
    tempControl.SetTunings(P, I, 0); //PI
    tempControl.Compute();
  }


  
  if (mode == 6)
  {
    tempControl.SetTunings(P, 0, D); //PD
    tempControl.Compute();
  }



  if (mode == 7)
  {
    tempControl.SetTunings(P, I, D); //PID
    tempControl.Compute();
  }

}
