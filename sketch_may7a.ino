#include <LiquidCrystal_I2C.h> // Incluir la libreria LiquidCrystal_I2C
#include <Keypad.h>            // Incluir la libreria Keypad
#include <Servo.h>             // Incluir la libreria Servo
#include <RTClib.h>            //Libreria del reloj
#include <Wire.h>
RTC_DS3231 rtc;//se le da nombre al rtc
int estado=0;                  // 0=cerrado 1=abierto
Servo TOAD;                 // Crea el objeto TOAD con las caracteristicas de Servo
const byte FILAS = 4;          // define numero de filas
const byte COLUMNAS = 4;       // define numero de columnas
char keys[FILAS][COLUMNAS] = {    // define la distribucion de teclas
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte pinesFilas[FILAS] = {9,8,7,6};         // pines correspondientes a las filas
byte pinesColumnas[COLUMNAS] = {5,4,3,2};  // pines correspondientes a las columnas
Keypad teclado = Keypad(makeKeymap(keys), pinesFilas, pinesColumnas, FILAS, COLUMNAS);  // crea objeto teclado
char TECLA;                        // almacena la tecla presionada
char CLAVE[5];                     // almacena en un array 4 digitos ingresados
char CLAVE_GOD[5] = "0000";    // almacena en un array la contraseña inicial
byte INDICE = 0;                   // indice del array
LiquidCrystal_I2C lcd(0x27,16,2);  // dependiendo del fabricante del I2C el codigo 0x27 cambiar a
                                   // 0x3F , 0x20 , 0x38 ,   
void setup()
{
 lcd.init();                          // inicializa el LCD
 lcd.backlight();
 TOAD.attach(11,660,1400);        // Asocia el servo1 al pin 11, define el min y max del ancho del pulso 
 TOAD.write(150);                // Gira el servo a 150 grados Cierra la puerta
 limpia();
 Wire.begin();
 rtc.begin();
 rtc.adjust(DateTime(__DATE__,__TIME__));//Se inicia la fecha y el tiempo
}

void loop(){ 
  lcd.setCursor(11,0);//se dise que en la columna 0 se imprima lo siguiente y que se coloque en la fila 0
  DateTime fecha = rtc.now();//funcion que devuelve fecha y horario en formato
  lcd.print(   fecha.hour ());//funcion que obtiene la hora de la fecha completa
  lcd.print(":");//caracter dos puntos como separador
  lcd.print( fecha.minute ());//funcion que obtiene los minutos de la fecha completa;
  TECLA = teclado.getKey();   // obtiene tecla presionada y asigna a variable
  if (TECLA)              // comprueba que se haya presionado una tecla
  {
    CLAVE[INDICE] = TECLA;    // almacena en array la tecla presionada
    INDICE++;                // incrementa indice en uno
    lcd.print(TECLA);         // envia al LCD la tecla presionada
  }
  if(INDICE == 4)             // si ya se almacenaron los 4 digitos 
  {
    if(!strcmp(CLAVE,CLAVE_GOD))    // compara clave ingresada con clave GOD
        abierto();              
    else 
        error();
    INDICE = 0;
  }
  if(estado==1 && (analogRead(A3)==0))     // si esta abierta y se pulsa boton de Nueva Clave
         nueva_clave();
}  
void error(){   
      lcd.setCursor(0,1);
      lcd.print("ERROR DE ACCESO    "); 
      limpia();
}
void abierto(){  
   if(estado==0){ 
      estado=1;
      lcd.setCursor(0,1);
      lcd.print("RANURA ABIERTA            ");  // imprime en el LCD que esta abierta
      TOAD.write(30);                 // Gira el servo a 30 grados  abre la puerta
   }
   else{
    estado=0;
    lcd.setCursor(0,1);
    lcd.print("BLOQUEADO              ");  // imprime en el LCD que esta cerrada
    TOAD.write(150);                // Gira el servo a 150 grados  cierra la puerta
   } 
 limpia();
}
void nueva_clave(){  
  lcd.setCursor(0,0);
  lcd.print("NUEVA CLAVE:        ");
  lcd.setCursor(12,0);
  INDICE=0;
  while (INDICE<=3){
   TECLA = teclado.getKey();   // obtiene tecla presionada y asigna a variable TECLA
   if (TECLA)                 // comprueba que se haya presionado una tecla
    {
      CLAVE_GOD[INDICE] = TECLA;    // almacena en array la tecla presionada
      CLAVE[INDICE] = TECLA;
      INDICE++;                 // incrementa indice en uno
      lcd.print(TECLA);         // envia a monitor serial la tecla presionada
    }  
  }
 estado=0;
  lcd.setCursor(0,1);
  lcd.print("CLAVE CAMBIADA");
  delay(1000);
  limpia();
}
void limpia(){  
 lcd.setCursor(0,0);
 lcd.print("CLAVE:          ");
 lcd.setCursor(6,0);
}