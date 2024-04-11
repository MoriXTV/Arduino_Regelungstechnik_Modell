
void disable()
{
  Power = 0;
}



void OffenerRegelkreis()
{
  Power = Potentiometer;
}





void OnOff()
{
  if (setTemperature > Temperature)
  {
    Power = 255;
  }
  else
  {
    Power = 0;
  }
}
