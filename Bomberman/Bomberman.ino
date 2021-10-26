//***************************************************************************************************************************************
/* Librería para el uso de la pantalla ILI9341 en modo 8 bits
 * Basado en el código de martinayotte - https://www.stm32duino.com/viewtopic.php?t=637
 * Adaptación, migración y creación de nuevas funciones: Pablo Mazariegos y José Morales
 * Con ayuda de: José Guerra
 * Modificaciones y adaptación: Diego Morales
 * IE3027: Electrónica Digital 2 - 2021
 */
//***************************************************************************************************************************************
#include <stdint.h>
#include <stdbool.h>
#include <TM4C123GH6PM.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/rom_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "bitmaps.h"
#include "font.h"
#include "lcd_registers.h"
#include <SPI.h>
#include <SD.h>
#define LCD_RST PD_0
#define LCD_CS PD_1
#define LCD_RS PD_2
#define LCD_WR PD_3
#define LCD_RD PE_1
int DPINS[] = {PB_0, PB_1, PB_2, PB_3, PB_4, PB_5, PB_6, PB_7};  
const int chipSelect = PA_3;
File myFile;
int x = 40;
int y = 20;
int x2 = 265;
int y2 = 205;
float vel = 0;
int x_seg_ciclo;
int posicion_y = 0;
unsigned char char1;
unsigned char char2;
char z;
int prueba;
int UltimaPocicion=0;
int primerCero = 0;
int flag = 0;
int flag1 = 1;
int flag11 = 1;
int flag2 = 1;
int flag22 = 1;


Sd2Card card;
SdVolume volume;
SdFile root;
//***************************************************************************************************************************************
// Functions Prototypes
//***************************************************************************************************************************************
void LCD_Init(void);
void LCD_CMD(uint8_t cmd);
void LCD_DATA(uint8_t data);
void SetWindows(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2);
void LCD_Clear(unsigned int c);
void H_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c);
void V_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c);
void Rect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c);
void FillRect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c);
void LCD_Print(String text, int x, int y, int fontSize, int color, int background);

void LCD_Bitmap(unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned char bitmap[]);
void LCD_Sprite(int x, int y, int width, int height, unsigned char bitmap[],int columns, int index, char flip, char offset);
char ConversionDecimal(char x, char y);
char ConversionASCII(char x);
void CargarImagen(void);
void inicio(void);
void movimiento(void);
void comparacion1(void);
void comparacion2(void);
void juego (void);
int ord;
int flags;
int inByte;
int inByte2;
unsigned long previousMillis = 0;  
const long interval = 42;
bool rectUp = false;
extern uint8_t MatrizFondo[];
extern uint8_t uvg[];
String text1;
String text2;
//***************************************************************************************************************************************
// Initialization
//***************************************************************************************************************************************
void setup() {
  pinMode(PUSH1, INPUT_PULLUP); 
  pinMode(PUSH2, INPUT_PULLUP); 
  SysCtlClockSet(SYSCTL_SYSDIV_2_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);
  Serial.begin(9600);
  Serial2.begin(9600);
  GPIOPadConfigSet(GPIO_PORTB_BASE, 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD_WPU);
  Serial.println("Start");
  LCD_Init();
  LCD_Clear(0x00);
  
 
  SPI.setModule(0);

  Serial.print("\nInitializing SD card...");
  // On the Ethernet Shield, CS is pin 4. It's set as an output by default.
  // Note that even if it's not used as the CS pin, the hardware SS pin
  // (10 on most Arduino boards, 53 on the Mega) must be left as an output
  // or the SD library functions will not work.
  pinMode(10, OUTPUT);     // change this to 53 on a mega


  // we'll use the initialization code from the utility libraries
  // since we're just testing if the card is working!
  if (!card.init(SPI_HALF_SPEED, chipSelect)) {
    Serial.println("initialization failed. Things to check:");
    Serial.println("* is a card is inserted?");
    Serial.println("* Is your wiring correct?");
    Serial.println("* did you change the chipSelect pin to match your shield or module?");
    return;
  } else {
    Serial.println("Wiring is correct and a card is present.");
  }

  //LCD_Print(String text, int x, int y, int fontSize, int color, int background)
 
    
  //LCD_Bitmap(unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned char bitmap[]);
  FillRect(0, 0, 320, 240, 0xff00);
  String text1 = "BOMBER-TANK";
  String text3 = "by Guillermo Lam & Luis Pedro Molina";
  String text2 = "Press Play";
  LCD_Print(text1, 80, 90, 2, 0x0000, 0xff00);
  LCD_Print(text2, 80, 170, 2, 0x0000, 0xff00);
  LCD_Print(text3, 20, 130, 1, 0x0000, 0xff00);
  //delay(1000);
  //LCD_Print(text2, 80, 170, 2, 0xffff, 0xff00);
  //delay(1000);
  //LCD_Print(text2, 80, 170, 2, 0x0000, 0xff00);
  //delay(1000);
  //LCD_Print(text2, 80, 170, 2, 0xffff, 0xff00);
  //delay(1000);
  //LCD_Print(text2, 80, 170, 2, 0x0000, 0xff00);
  //delay(1000);
  //LCD_Print(text2, 80, 170, 2, 0xffff, 0xff00);
  //delay(1000);
  //LCD_Print(text2, 80, 170, 2, 0x0000, 0xff00);
  

 
  

 
 
}
//***************************************************************************************************************************************
// Loop
//***************************************************************************************************************************************
void loop() {
  if (Serial2.available()) {
    inByte = Serial2.read();}
  if (Serial3.available()) {
    inByte2 = Serial3.read();} 
  vel += 0.00001;    
  inicio();
  juego();
  
  
}
//***************************************************************************************************************************************
// Función para inicializar LCD
//***************************************************************************************************************************************
void inicio(void){
  if ( digitalRead(PUSH1) == LOW && flag == 0){
    flag = 1;
    flag1 = 0;
    flag11 = 0;
    flag2 = 0;
    flag22 = 0;
    inByte = 0;
    LCD_Bitmap(0, 0, 320, 240, uvg);
    
    LCD_Bitmap(38, 74, 18, 18, bowser);
    LCD_Bitmap(76, 37, 18, 18, bowser);
    LCD_Bitmap(76, 18, 18, 18, bowser);
    LCD_Bitmap(95, 18, 18, 18, bowser);
    LCD_Bitmap(114, 18, 18, 18, bowser);
    LCD_Bitmap(133, 18, 18, 18, bowser);
    LCD_Bitmap(152, 18, 18, 18, bowser);
    LCD_Bitmap(171, 18, 18, 18, bowser);
    LCD_Bitmap(190, 18, 18, 18, bowser);
    LCD_Bitmap(209, 18, 18, 18, bowser);
    LCD_Bitmap(228, 18, 18, 18, bowser);
    LCD_Bitmap(247, 18, 18, 18, bowser);
    LCD_Bitmap(266, 18, 18, 18, bowser);
    LCD_Bitmap(38, 56, 18, 18, bowser);
    LCD_Bitmap(57, 56, 18, 18, bowser);
    LCD_Bitmap(76, 56, 18, 18, bowser);
    LCD_Bitmap(95, 56, 18, 18, bowser);
    LCD_Bitmap(114, 56, 18, 18, bowser);
    LCD_Bitmap(133, 56, 18, 18, bowser);
    LCD_Bitmap(152, 56, 18, 18, bowser);
    LCD_Bitmap(171, 56, 18, 18, bowser);
    LCD_Bitmap(190, 56, 18, 18, bowser);
    LCD_Bitmap(209, 56, 18, 18, bowser);
    LCD_Bitmap(228, 56, 18, 18, bowser);
    LCD_Bitmap(247, 56, 18, 18, bowser);
    LCD_Bitmap(266, 56, 18, 18, bowser);
    LCD_Bitmap(38, 93, 18, 18, bowser);
    LCD_Bitmap(57, 93, 18, 18, bowser);
    LCD_Bitmap(76, 93, 18, 18, bowser);
    LCD_Bitmap(95, 93, 18, 18, bowser);
    LCD_Bitmap(114, 93, 18, 18, bowser);
    LCD_Bitmap(133, 93, 18, 18, bowser);
    LCD_Bitmap(152, 93, 18, 18, bowser);
    LCD_Bitmap(171, 93, 18, 18, bowser);
    LCD_Bitmap(190, 93, 18, 18, bowser);
    LCD_Bitmap(209, 93, 18, 18, bowser);
    LCD_Bitmap(228, 93, 18, 18, bowser);
    LCD_Bitmap(247, 93, 18, 18, bowser);
    LCD_Bitmap(266, 93, 18, 18, bowser);
    LCD_Bitmap(38, 130, 18, 18, bowser);
    LCD_Bitmap(57, 130, 18, 18, bowser);
    LCD_Bitmap(76, 130, 18, 18, bowser);
    LCD_Bitmap(95, 130, 18, 18, bowser);
    LCD_Bitmap(114, 130, 18, 18, bowser);
    LCD_Bitmap(133, 130, 18, 18, bowser);
    LCD_Bitmap(152, 130, 18, 18, bowser);
    LCD_Bitmap(171, 130, 18, 18, bowser);
    LCD_Bitmap(190, 130, 18, 18, bowser);
    LCD_Bitmap(209, 130, 18, 18, bowser);
    LCD_Bitmap(228, 130, 18, 18, bowser);
    LCD_Bitmap(247, 130, 18, 18, bowser);
    LCD_Bitmap(266, 130, 18, 18, bowser);
    LCD_Bitmap(38, 166, 18, 18, bowser);
    LCD_Bitmap(57, 166, 18, 18, bowser);
    LCD_Bitmap(76, 166, 18, 18, bowser);
    LCD_Bitmap(95, 166, 18, 18, bowser);
    LCD_Bitmap(114, 166, 18, 18, bowser);
    LCD_Bitmap(133, 166, 18, 18, bowser);
    LCD_Bitmap(152, 166, 18, 18, bowser);
    LCD_Bitmap(171, 166, 18, 18, bowser);
    LCD_Bitmap(190, 166, 18, 18, bowser);
    LCD_Bitmap(209, 166, 18, 18, bowser);
    LCD_Bitmap(228, 166, 18, 18, bowser);
    LCD_Bitmap(247, 166, 18, 18, bowser);
    LCD_Bitmap(266, 166, 18, 18, bowser);
    LCD_Bitmap(38, 204, 18, 18, bowser);
    LCD_Bitmap(57, 204, 18, 18, bowser);
    LCD_Bitmap(76, 204, 18, 18, bowser);
    LCD_Bitmap(95, 204, 18, 18, bowser);
    LCD_Bitmap(114, 204, 18, 18, bowser);
    LCD_Bitmap(133, 204, 18, 18, bowser);
    LCD_Bitmap(152, 204, 18, 18, bowser);
    LCD_Bitmap(171, 204, 18, 18, bowser);
    LCD_Bitmap(190, 204, 18, 18, bowser);
    LCD_Bitmap(209, 204, 18, 18, bowser);
    LCD_Bitmap(228, 204, 18, 18, bowser);
    LCD_Bitmap(228, 185, 18, 18, bowser);
    LCD_Bitmap(266, 148, 18, 18, bowser);
    LCD_Sprite(x, y,17,15,tanque1_2,1, 2,0,0 );
    LCD_Sprite(x2, y2,17,15,tanque2_3,1, 2,0,0 );
    }}
void juego (void){
  unsigned long currentMillis = millis();
  
  // actualización de frame cada 42ms = 24fps
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    movimiento();
    movimiento11();
    movimiento2();
    movimiento22();
    comparacion1();
    comparacion11();
    comparacion2();
    comparacion22();}}

void movimiento (void){
  
  disparo();
  if (inByte == 0x32 && flag1 == 0 ){
    flags = 0;
    vel = 0;
    if( x > 30 & x < 263){
      inByte = 0;
      x +=4;
      LCD_Sprite(x, y,17,15,tanque1_2,1, 2,0,0 );
      FillRect( x-5, y, 5, 17, 0x04E8);}}
  else if (inByte == 0x33 && flag1 == 0 ){
    flags = 0;
    vel = 0;
    if( x > 40 & x < 270){
      inByte = 0;
      x -=4;
      LCD_Sprite(x, y,17,15,tanque1_3,1, 2,0,0 );
      FillRect( x+17, y, 5, 17, 0x04E8);}}}

void movimiento11 (void){
  if (inByte == 0x37 && flag11 == 0 ){
    if( x2 > 30 & x2 < 263){
      inByte = 0;
      x2 +=4;
      LCD_Sprite(x2, y2,17,15,tanque2_2,1, 2,0,0 );
      FillRect( x2-5, y2, 5, 17, 0x04E8);}}
  else if (inByte == 0x38 && flag11 == 0 ){
    if( x2 > 40 & x2 < 270){
      inByte = 0;
      x2 -=4;
      LCD_Sprite(x2, y2,17,15,tanque2_3,1, 2,0,0 );
      FillRect( x2+17, y2, 5, 17, 0x04E8);}}}

void movimiento2 (void){
  
  if (inByte == 0x30 && flag2 == 0){
    flags = 1;
    if( y > 20 & y < 210){
      inByte = 0;
      y -=4;
      LCD_Sprite(x, y,15,17,tanque1_1,1, 2,0,0 );
      FillRect( x, y+17, 17, 5, 0x04E8);}}
  else if (inByte == 0x31 && flag2 == 0){
    flags = 1;
    if( y > 19 & y < 204){
      inByte = 0;
      y +=4;
      LCD_Sprite(x, y,15,17,tanque1_4,1, 2,0,0 );
      FillRect( x, y-5, 17, 5, 0x04E8);}}}

void movimiento22 (void){
  if (inByte == 0x35 && flag22 == 0){
    if( y2 > 16 & y2 < 210){
      inByte = 0;
      y2 -=4;
      LCD_Sprite(x2, y2,15,17,tanque2_1,1, 2,0,0 );
      FillRect( x2, y2+17, 17, 5, 0x04E8);}}
  else if (inByte == 0x36 && flag22 == 0){
    if( y2 > 16 & y2 < 205){
      inByte = 0;
      y2 +=4;
      LCD_Sprite(x2, y2,15,17,tanque2_4,1, 2,0,0 );
      FillRect( x2, y2-5, 17, 5, 0x04E8);}}}

void comparacion1 (void){
  if (x > 42 && x < 75){
    flag2 = 1;}
  else if(x > 78 && x < 115){
    flag2 = 1;}
  else if(x > 118 && x < 150){
    flag2 = 1;}
  else if(x > 153&& x < 188){
    flag2 = 1;}
  else if(x > 191 && x < 226){
    flag2 = 1;}
  else if(x > 231 && x < 264){
    flag2 = 1;}
  else{
    flag2 = 0;}}

void comparacion11 (void){
  if (y > 21 && y < 55){
    flag1 = 1;}
  else if(y > 58 && y < 92){
    flag1 = 1;}
  else if(y > 95 && y < 127){
    flag1 = 1;}
  else if(y > 130 && y < 165){
    flag1 = 1;}
  else if(y > 168 && y < 203){
    flag1 = 1;}
  else{
    flag1 = 0;}}

void comparacion2 (void){
  if (x2 > 40 && x2 < 75){
    flag22 = 1;}
  else if(x2 > 80 && x2 < 110){
    flag22 = 1;}
  else if(x2 > 115 && x2 < 150){
    flag22 = 1;}
  else if(x2 > 153&& x2 < 188){
    flag22 = 1;}
    else if(x2 > 191 && x2 < 226){
    flag22 = 1;}
    else if(x2 > 231 && x2 < 264){
    flag22 = 1;}
  else{
    flag22 = 0;}}

void comparacion22 (void){
  if (y2 > 21 && y2 < 55){
    flag11 = 1;}
  else if(y2 > 58 && y2 < 92){
    flag11 = 1;}
  else if(y2 > 95 && y2 < 127){
    flag11 = 1;}
  else if(y2 > 130 && y2 < 165){
    flag11 = 1;}
  else if(y2 > 168 && y2 < 205){
    flag11 = 1;}
  else{
    flag11 = 0;}}
    
void disparo(void){
  if (inByte == 0x34 && flags == 0){
      
      FillRect( x+20+vel, y+3, 5, 5, 0x0000);
      FillRect( x+16+vel, y+3, 5, 5, 0x04E8);
      }}
    
char ConversionDecimal(char x, char y) {
int x1;
int y1;
int resultado;
x1 = ConversionASCII(x);
y1 = ConversionASCII(y);
resultado = int(x1)*16 + int(y1);  
return resultado;
}

char ConversionASCII(char x) {
  if (x == '0'){
    return 0;
  }else if (x == '1'){
    return 1;
  }else if (x == '2'){
    return 2;
  }else if (x == '3'){
    return 3;
  }else if (x == '4'){
    return 4;
  }else if (x == '5'){
    return 5;
  }else if (x == '6'){
    return 6;
  }else if (x == '7'){
    return 7;
  }else if (x == '8'){
    return 8;
  }else if (x == '9'){
    return 9;
  }else if (x == 'a'){
    return 10;
  }else if (x == 'b'){
    return 11;
  }else if (x == 'c'){
    return 12;
  }else if (x == 'd'){
    return 13;
  }else if (x == 'e'){
    return 14;
  }else if (x == 'f'){
    return 15;
  }else {
    return 0;
  }
}

void CargarFondo(int height, int width, int x1, int y1, int negro){
  if (negro == 1){
    LCD_Clear(0x00);
  }
  UltimaPocicion = 0;
  posicion_y = y1;
  primerCero = 0;
  char1 = 0;
  char2 = 0;
  x = 0;
  x_seg_ciclo = 0;
   int totalBytes = myFile.size();
    if (myFile) {
      for (x_seg_ciclo = 0; x_seg_ciclo < height; x_seg_ciclo++){
        for (x = 0; x < width*2 ; x++){
          myFile.seek(UltimaPocicion);
          while (myFile.available()){
            char caracter = myFile.read();
            if (caracter == ','){
              UltimaPocicion = myFile.position();
              break;
            }else if (caracter == '0' && primerCero == 0){
              primerCero = 1;
            }else if (caracter == 'x'){

            }else if (caracter == ' '){

            }else if (caracter == 13){

            } else{
              primerCero = 0;
              char1 = caracter;
              char2 = myFile.read();
              MatrizFondo[x] = ConversionDecimal(char1, char2);
            }
          }
        }
        LCD_Bitmap(x1, posicion_y, width, 1, MatrizFondo);
        posicion_y++;
      }
    } else {
    // if the file didn't open, print an error:
    Serial.println("error opening archivo.txt");
  }
}

void LCD_Init(void) {
  pinMode(LCD_RST, OUTPUT);
  pinMode(LCD_CS, OUTPUT);
  pinMode(LCD_RS, OUTPUT);
  pinMode(LCD_WR, OUTPUT);
  pinMode(LCD_RD, OUTPUT);
  for (uint8_t i = 0; i < 8; i++){
    pinMode(DPINS[i], OUTPUT);
  }
  //****************************************
  // Secuencia de Inicialización
  //****************************************
  digitalWrite(LCD_CS, HIGH);
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_WR, HIGH);
  digitalWrite(LCD_RD, HIGH);
  digitalWrite(LCD_RST, HIGH);
  delay(5);
  digitalWrite(LCD_RST, LOW);
  delay(20);
  digitalWrite(LCD_RST, HIGH);
  delay(150);
  digitalWrite(LCD_CS, LOW);
  //****************************************
  LCD_CMD(0xE9);  // SETPANELRELATED
  LCD_DATA(0x20);
  //****************************************
  LCD_CMD(0x11); // Exit Sleep SLEEP OUT (SLPOUT)
  delay(100);
  //****************************************
  LCD_CMD(0xD1);    // (SETVCOM)
  LCD_DATA(0x00);
  LCD_DATA(0x71);
  LCD_DATA(0x19);
  //****************************************
  LCD_CMD(0xD0);   // (SETPOWER) 
  LCD_DATA(0x07);
  LCD_DATA(0x01);
  LCD_DATA(0x08);
  //****************************************
  LCD_CMD(0x36);  // (MEMORYACCESS)
  LCD_DATA(0x40|0x80|0x20|0x08); // LCD_DATA(0x19);
  //****************************************
  LCD_CMD(0x3A); // Set_pixel_format (PIXELFORMAT)
  LCD_DATA(0x05); // color setings, 05h - 16bit pixel, 11h - 3bit pixel
  //****************************************
  LCD_CMD(0xC1);    // (POWERCONTROL2)
  LCD_DATA(0x10);
  LCD_DATA(0x10);
  LCD_DATA(0x02);
  LCD_DATA(0x02);
  //****************************************
  LCD_CMD(0xC0); // Set Default Gamma (POWERCONTROL1)
  LCD_DATA(0x00);
  LCD_DATA(0x35);
  LCD_DATA(0x00);
  LCD_DATA(0x00);
  LCD_DATA(0x01);
  LCD_DATA(0x02);
  //****************************************
  LCD_CMD(0xC5); // Set Frame Rate (VCOMCONTROL1)
  LCD_DATA(0x04); // 72Hz
  //****************************************
  LCD_CMD(0xD2); // Power Settings  (SETPWRNORMAL)
  LCD_DATA(0x01);
  LCD_DATA(0x44);
  //****************************************
  LCD_CMD(0xC8); //Set Gamma  (GAMMASET)
  LCD_DATA(0x04);
  LCD_DATA(0x67);
  LCD_DATA(0x35);
  LCD_DATA(0x04);
  LCD_DATA(0x08);
  LCD_DATA(0x06);
  LCD_DATA(0x24);
  LCD_DATA(0x01);
  LCD_DATA(0x37);
  LCD_DATA(0x40);
  LCD_DATA(0x03);
  LCD_DATA(0x10);
  LCD_DATA(0x08);
  LCD_DATA(0x80);
  LCD_DATA(0x00);
  //****************************************
  LCD_CMD(0x2A); // Set_column_address 320px (CASET)
  LCD_DATA(0x00);
  LCD_DATA(0x00);
  LCD_DATA(0x01);
  LCD_DATA(0x3F);
  //****************************************
  LCD_CMD(0x2B); // Set_page_address 480px (PASET)
  LCD_DATA(0x00);
  LCD_DATA(0x00);
  LCD_DATA(0x01);
  LCD_DATA(0xE0);
//  LCD_DATA(0x8F);
  LCD_CMD(0x29); //display on 
  LCD_CMD(0x2C); //display on

  LCD_CMD(ILI9341_INVOFF); //Invert Off
  delay(120);
  LCD_CMD(ILI9341_SLPOUT);    //Exit Sleep
  delay(120);
  LCD_CMD(ILI9341_DISPON);    //Display on
  digitalWrite(LCD_CS, HIGH);
}
//***************************************************************************************************************************************
// Función para enviar comandos a la LCD - parámetro (comando)
//***************************************************************************************************************************************
void LCD_CMD(uint8_t cmd) {
  digitalWrite(LCD_RS, LOW);
  digitalWrite(LCD_WR, LOW);
  GPIO_PORTB_DATA_R = cmd;
  digitalWrite(LCD_WR, HIGH);
}
//***************************************************************************************************************************************
// Función para enviar datos a la LCD - parámetro (dato)
//***************************************************************************************************************************************
void LCD_DATA(uint8_t data) {
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_WR, LOW);
  GPIO_PORTB_DATA_R = data;
  digitalWrite(LCD_WR, HIGH);
}
//***************************************************************************************************************************************
// Función para definir rango de direcciones de memoria con las cuales se trabajara (se define una ventana)
//***************************************************************************************************************************************
void SetWindows(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2) {
  LCD_CMD(0x2a); // Set_column_address 4 parameters
  LCD_DATA(x1 >> 8);
  LCD_DATA(x1);   
  LCD_DATA(x2 >> 8);
  LCD_DATA(x2);   
  LCD_CMD(0x2b); // Set_page_address 4 parameters
  LCD_DATA(y1 >> 8);
  LCD_DATA(y1);   
  LCD_DATA(y2 >> 8);
  LCD_DATA(y2);   
  LCD_CMD(0x2c); // Write_memory_start
}
//***************************************************************************************************************************************
// Función para borrar la pantalla - parámetros (color)
//***************************************************************************************************************************************
void LCD_Clear(unsigned int c){  
  unsigned int x, y;
  LCD_CMD(0x02c); // write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW);   
  SetWindows(0, 0, 319, 239); // 479, 319);
  for (x = 0; x < 320; x++)
    for (y = 0; y < 240; y++) {
      LCD_DATA(c >> 8); 
      LCD_DATA(c); 
    }
  digitalWrite(LCD_CS, HIGH);
} 
//***************************************************************************************************************************************
// Función para dibujar una línea horizontal - parámetros ( coordenada x, cordenada y, longitud, color)
//*************************************************************************************************************************************** 
void H_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c) {  
  unsigned int i, j;
  LCD_CMD(0x02c); //write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW);
  l = l + x;
  SetWindows(x, y, l, y);
  j = l;// * 2;
  for (i = 0; i < l; i++) {
      LCD_DATA(c >> 8); 
      LCD_DATA(c); 
  }
  digitalWrite(LCD_CS, HIGH);
}
//***************************************************************************************************************************************
// Función para dibujar una línea vertical - parámetros ( coordenada x, cordenada y, longitud, color)
//*************************************************************************************************************************************** 
void V_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c) {  
  unsigned int i,j;
  LCD_CMD(0x02c); //write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW);
  l = l + y;
  SetWindows(x, y, x, l);
  j = l; //* 2;
  for (i = 1; i <= j; i++) {
    LCD_DATA(c >> 8); 
    LCD_DATA(c);
  }
  digitalWrite(LCD_CS, HIGH);  
}
//***************************************************************************************************************************************
// Función para dibujar un rectángulo - parámetros ( coordenada x, cordenada y, ancho, alto, color)
//***************************************************************************************************************************************
void Rect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c) {
  H_line(x  , y  , w, c);
  H_line(x  , y+h, w, c);
  V_line(x  , y  , h, c);
  V_line(x+w, y  , h, c);
}
//***************************************************************************************************************************************
// Función para dibujar un rectángulo relleno - parámetros ( coordenada x, cordenada y, ancho, alto, color)
//***************************************************************************************************************************************
void FillRect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c) {
  LCD_CMD(0x02c); // write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW); 
  
  unsigned int x2, y2;
  x2 = x+w;
  y2 = y+h;
  SetWindows(x, y, x2-1, y2-1);
  unsigned int k = w*h*2-1;
  unsigned int i, j;
  for (int i = 0; i < w; i++) {
    for (int j = 0; j < h; j++) {
      LCD_DATA(c >> 8);
      LCD_DATA(c); 
      k = k - 2;
     } 
  }
  digitalWrite(LCD_CS, HIGH);
}
//***************************************************************************************************************************************
// Función para dibujar texto - parámetros ( texto, coordenada x, cordenada y, color, background) 
//***************************************************************************************************************************************
void LCD_Print(String text, int x, int y, int fontSize, int color, int background) {
  int fontXSize ;
  int fontYSize ;
  
  if(fontSize == 1){
    fontXSize = fontXSizeSmal ;
    fontYSize = fontYSizeSmal ;
  }
  if(fontSize == 2){
    fontXSize = fontXSizeBig ;
    fontYSize = fontYSizeBig ;
  }
  
  char charInput ;
  int cLength = text.length();
  Serial.println(cLength,DEC);
  int charDec ;
  int c ;
  int charHex ;
  char char_array[cLength+1];
  text.toCharArray(char_array, cLength+1) ;
  for (int i = 0; i < cLength ; i++) {
    charInput = char_array[i];
    Serial.println(char_array[i]);
    charDec = int(charInput);
    digitalWrite(LCD_CS, LOW);
    SetWindows(x + (i * fontXSize), y, x + (i * fontXSize) + fontXSize - 1, y + fontYSize );
    long charHex1 ;
    for ( int n = 0 ; n < fontYSize ; n++ ) {
      if (fontSize == 1){
        charHex1 = pgm_read_word_near(smallFont + ((charDec - 32) * fontYSize) + n);
      }
      if (fontSize == 2){
        charHex1 = pgm_read_word_near(bigFont + ((charDec - 32) * fontYSize) + n);
      }
      for (int t = 1; t < fontXSize + 1 ; t++) {
        if (( charHex1 & (1 << (fontXSize - t))) > 0 ) {
          c = color ;
        } else {
          c = background ;
        }
        LCD_DATA(c >> 8);
        LCD_DATA(c);
      }
    }
    digitalWrite(LCD_CS, HIGH);
  }
}
//***************************************************************************************************************************************
// Función para dibujar una imagen a partir de un arreglo de colores (Bitmap) Formato (Color 16bit R 5bits G 6bits B 5bits)
//***************************************************************************************************************************************
void LCD_Bitmap(unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned char bitmap[]){  
  LCD_CMD(0x02c); // write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW); 
  
  unsigned int x2, y2;
  x2 = x+width;
  y2 = y+height;
  SetWindows(x, y, x2-1, y2-1);
  unsigned int k = 0;
  unsigned int i, j;

  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
      LCD_DATA(bitmap[k]);
      LCD_DATA(bitmap[k+1]);
      //LCD_DATA(bitmap[k]);    
      k = k + 2;
     } 
  }
  digitalWrite(LCD_CS, HIGH);
}
//***************************************************************************************************************************************
// Función para dibujar una imagen sprite - los parámetros columns = número de imagenes en el sprite, index = cual desplegar, flip = darle vuelta
//***************************************************************************************************************************************
void LCD_Sprite(int x, int y, int width, int height, unsigned char bitmap[],int columns, int index, char flip, char offset){
  LCD_CMD(0x02c); // write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW); 

  unsigned int x2, y2;
  x2 =   x+width;
  y2=    y+height;
  SetWindows(x, y, x2-1, y2-1);
  int k = 0;
  int ancho = ((width*columns));
  if(flip){
    for (int j = 0; j < height; j++){
        k = (j*(ancho) + index*width -1 - offset)*2;
        k = k+width*2;
       for (int i = 0; i < width; i++){
        LCD_DATA(bitmap[k]);
        LCD_DATA(bitmap[k+1]);
        k = k - 2;
       } 
    }
  }
  else{
     for (int j = 0; j < height; j++){
      k = (j*(ancho) + index*width + 1 + offset)*2;
     for (int i = 0; i < width; i++){
      LCD_DATA(bitmap[k]);
      LCD_DATA(bitmap[k+1]);
      k = k + 2;
     } 
    }
  }
  digitalWrite(LCD_CS, HIGH);
}
