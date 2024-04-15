#include <LiquidCrystal_I2C.h> // Librería LCD mediante I2C
#include <SoftwareSerial.h> // Librería comunicación serial 
#include <MD_MAX72xx.h> // Librería matrices LED MAX7219/7221

#define LCD_ADDRESS 0x27 // Dirección I2C del LCD
#define LCD_COLUMNS 16 // Número de columnas del LCD
#define LCD_ROWS 2 // Número de filas del LCD
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW // Tipo de hardware 
#define NUM_OF_MATRIX 4 // Número de matrices LED utilizadas
#define CLK_PIN   13 // Pin de CLK
#define DATA_PIN  11 // Pin de DATA 
#define CS_PIN    10 // Pin de CS

SoftwareSerial BT1(6, 7); //Comunicación serial
LiquidCrystal_I2C lcd(LCD_ADDRESS, LCD_COLUMNS, LCD_ROWS); // Configura LCD

MD_MAX72XX cartel = MD_MAX72XX(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, NUM_OF_MATRIX); // Configura matrices 
String mensaje = "Shake it off  "; // Mensaje 1
String mensaje2 = "Cruel Summer "; // Mensaje 2
String proximo_mensaje; // Variable para almacenar el próximo mensaje a recibir

void setup() {
  Serial.begin(9600); // Inicia serial a 9600 baudios
  BT1.begin(9600); // Inicia BT1 a 9600 baudios
  lcd.init(); // Inicializa el LCD
  lcd.backlight(); // Enciende la retroiluminación del LCD
  lcd.clear(); // Limpia el contenido del LCD
  lcd.print("Esperando..."); // Muestra un mensaje inicial en el LCD
  cartel.begin(); // Inicializa las matrices LED
  cartel.control(MD_MAX72XX::INTENSITY, 10); // Configura la intensidad de brillo de las matrices LED
  cartel.control( MD_MAX72XX::UPDATE, false ); // Desactiva la actualización automática de las matrices LED
  Serial.println("Listo para recibir mensajes."); // Imprime un mensaje en el puerto serial
}

// Función para actualizar el mensaje
void actualizar_mensaje(){

  while( Serial.available() ){

    char c = Serial.read(); // Lee siguiente carácter disponible en el puerto serial

    if( c == '\n' ){ // Si se encuentra un salto de línea, se ha terminado de recibir el mensaje
      mensaje = proximo_mensaje; // Actualiza el mensaje principal con el próximo mensaje recibido
      proximo_mensaje = "ABC"; // Restablece la variable para almacenar el próximo mensaje
      break; // Sale del bucle
    }
    else
      proximo_mensaje += c; // Agrega el carácter al próximo mensaje
    
  }
}

// Función para desplazar un texto en las matrices LED
void slide_text(int ms_delay){
  int col = 0;
  int last_pos;
  bool completo = false;
  
  cartel.clear(); // Limpia el contenido de las matrices LED

  while( completo == false ){
    last_pos = printText(col, mensaje); // Imprime el texto en las matrices LED desde la posición actual
    delay(ms_delay); // Espera un tiempo determinado
    col++; // Incrementa la columna para desplazar el texto
    if( last_pos > (int)cartel.getColumnCount() ) // Verifica si se ha completado el desplazamiento
      completo = true; // Establece la bandera de completado
  }
}

// Función para desplazar un segundo texto en las matrices LED
void slide_text2(int ms_delay){
  int col = 0;
  int last_pos;
  bool completo = false;
  
  cartel.clear(); // Limpia el contenido de las matrices LED

  while( completo == false ){
    last_pos = printText(col, mensaje2); // Imprime el segundo texto en las matrices LED desde la posición actual
    delay(ms_delay); // Espera un tiempo determinado
    col++; // Incrementa la columna para desplazar el texto
    if( last_pos > (int)cartel.getColumnCount() ) // Verifica si se ha completado el desplazamiento
      completo = true; // Establece la bandera de completado
  }
}

// Función para imprimir un texto en las matrices LED
int printText(int pos, const String text){
  int w;
  
  for( int i = 0; i < text.length(); i++ ){
    // Imprimir letra
    w = cartel.setChar( pos, text[i] ); // Establece el carácter en la posición actual y obtiene su ancho
    // La próxima letra empieza donde termina esta
    pos = pos - w; // Actualiza la posición para el siguiente carácter
    // Se deja una columna entre letras.
    cartel.setColumn(pos, B00000000); // Establece una columna en blanco entre letras
    
    pos = pos - 1; // Actualiza la posición para el siguiente carácter
    
    if( pos < 0 ) // Verifica si se ha alcanzado el límite izquierdo de las matrices LED
      break; // Sale del bucle
      
  }
  cartel.update(); // Actualiza las matrices LED
  
  return pos; // Devuelve la última posición impresa
}

void loop() {
  if (BT1.available()) { // Si hay datos disponibles en el puerto Bluetooth
    char receivedChar = BT1.read(); // Lee el siguiente carácter disponible desde Bluetooth

    if (receivedChar == '1') { // Si se recibe el carácter '1'
      lcd.clear(); // Limpia el contenido del LCD
      lcd.print("Activado"); // Muestra un mensaje en el LCD
      lcd.setCursor(0, 1); // Establece el cursor en la segunda fila del LCD
      lcd.print("Estatus: "); // Muestra un mensaje en la segunda fila del LCD
    }
    else if (receivedChar == '2') { // Si se recibe el carácter '2'
      lcd.clear(); // Limpia el contenido del LCD
      lcd.print("Desactivado"); // Muestra un mensaje en el LCD
    }
    else if (receivedChar == '3') { // Si se recibe el carácter '3'
      lcd.clear(); // Limpia el contenido del LCD
      lcd.print("Estatus: Mens 1"); // Muestra un mensaje en el LCD
      slide_text( 100 ); // Desplaza el primer texto en las matrices LED
      actualizar_mensaje(); // Actualiza el mensaje recibido por Bluetooth
    }
    else if (receivedChar == '4') { // Si se recibe el carácter '4'
      lcd.clear(); // Limpia el contenido del LCD
      lcd.print("Estatus: Mens 2"); // Muestra un mensaje en el LCD
      slide_text2( 100 ); // Desplaza el segundo texto en las matrices LED
      actualizar_mensaje(); // Actualiza el mensaje recibido por Bluetooth
    }
  }
}
