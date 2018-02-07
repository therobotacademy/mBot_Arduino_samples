// include mCore Makeblock library
#include <MeMCore.h>  // mCore Makeblock library

MeLEDMatrix Matrix_1(PORT_1);

char string_data[]="MAKEBLOCK 012345678";
uint8_t Bitmap_Heart[16]={0x00,0x38,0x44,0x42,0x21,0x21,0x42,0x44,0x38,0x44,0x42,0x21,0x21,0x42,0x44,0x38};
int move_times = sizeof(string_data)*6;
void setup()
{
Matrix_1.setBrightness(Brightness_8);//
}
void loop()
{
  Matrix_1.setColorIndex(1);
  Matrix_1.drawBitmap(0, 0, sizeof(Bitmap_Heart), Bitmap_Heart);
   for(uint8_t k=0; k<3; k++)
   {
    for(uint8_t i=0;i<8;i++)
    {
      Matrix_1.setBrightness(i);
      delay(100);
    }
    for(uint8_t i=7;i>0;i--)
    {
      Matrix_1.setBrightness(i);
      delay(100);
    }
   } // End of main 'for'
}
