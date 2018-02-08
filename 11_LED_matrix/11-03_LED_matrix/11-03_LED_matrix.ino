// include mCore Makeblock library
#include <MeMCore.h>  // mCore Makeblock library

MeLEDMatrix Matrix_1(PORT_4);

char string_data[]="SOFIA, I LOVE YOU";
uint8_t Bitmap_Heart[16]={0x00,0x38,0x44,0x42,0x21,0x21,0x42,0x44,0x38,0x44,0x42,0x21,0x21,0x42,0x44,0x38};
int move_times = sizeof(string_data)*6;
void setup()
{
Matrix_1.setBrightness(Brightness_8);//
}
void loop()
{
  for(int16_t i=0; i<move_times; i++)
  {
    if(i >move_times)i=0;
      Matrix_1.drawStr(15-i,7,string_data);
      delay(100);
  }
}
