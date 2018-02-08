// include mCore Makeblock library
#include <MeMCore.h>  // mCore Makeblock library

MeLEDMatrix Matrix_1(PORT_4);

char string_data[]="MAKEBLOCK 012345678";
uint8_t Bitmap_Heart[16]={0x00,0x38,0x44,0x42,0x21,0x21,0x42,0x44,0x38,0x44,0x42,0x21,0x21,0x42,0x44,0x38};
int move_times = sizeof(string_data)*6;
void setup()
{
Matrix_1.setBrightness(Brightness_8);//
}
void loop()
{
  for(uint8_t k=0; k<5; k++)
  {
    Matrix_1.showClock(12, 34, PointOn);
    delay(500);
    Matrix_1.showClock(12, 34, PointOff);
    delay(500);
  }
}
