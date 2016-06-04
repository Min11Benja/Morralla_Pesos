// Codigo para Prototypo de Maquina "La Moralla"
// Este codigo detecta los pulsos del monedero electronico multimoneda para luego accionar un relevador que prende el hopper solo siempre y cuando los pulsos del sensor del hopper no excedan a los pulsos contados por el monedero*/
// Basicamente esta maquina te da feria de a peso $1 al introducirle monedas de $10.00,$5.00,$2.00, esta te arroja x9,x4,x1 peso rexpectivamente, ya que cobra un pedo por feriarte.
// Tiene la funcion de aceptar 3 monedas de $0.50 para darte una moneda de $1.00 peso.

// Contains EEPROM.read() and EEPROM.write()
#include <EEPROM.h>

//**************Variables Globales****************
volatile byte coinPulseCount = 0;  // contadro para ver cuantas veces cambio el pin de estado - sirve para detectar cual typo de moneda fue introducida
volatile byte hopperPulseCount = 0;  // contador para ver cuantas monedas fueron expulsadas por el hopper
volatile unsigned long pulseTime;  // este guarda el tiempo del ultimo pulso - funcion para evititar rebotes
byte newCoinInserted; // variable para guardar el ultimo pulso detectado
byte coinValue = 0;      // numero de pulsos requeridos para expulsar las monedas - funcion Dispence()
byte toston = 0;        // Variable para el contador del loop de 3x de 0.50 por un peso

//**************Variables Contabilisazion****************
byte cmd = 0; //For Serial Input tests via Arduino console

// ID of the settings block
#define CONFIG_VERSION "ls1"
// Tell it where to store your config data in EEPROM
#define CONFIG_START 32

// Example settings structure
struct StoreStruct {
  // The variables of your settings
int iTotalIn ; //Total de dinero entrado a la maquina
int iTotalOut; //Total de dinero que salio de la maquina
int iGanancia; //Total ganacia por transaccion
int iStockPesos; //Total pesos dejado en maquina
int i10; //Var para contabilizar cantidad de monedas de 10 que se ferean
int i5; //Var para contabilizar cantidad de monedas de 5 que se ferean
int i2; //Var para contabilizar cantidad de monedas de 2 que se ferean
int i50; //Var para contabilizar cantidad de monedas de 0.50 que se ferean
int iTostonCont;
char version_of_program[4]; // it is the last variable of the struct so when settings are saved, they will only be validated if they are stored completely
}settings = {
  // The default values
   0,0,0,0,0,0,0,0,0,CONFIG_VERSION
};
//***********************************************************************************************
byte pulseThreshold = 500;  //EDIT THIS VALUE TO CHANGE DELAY BETWEEN DETECTING BANK OF PULSES
//***********************************************************************************************

//************Pins Empleados *******************
const int hopperPin = 2;  // pin2 como optical count input
const int coinSelectorPin = 3;  // pin3 como optical count input
const int relayPin = 7;  // pin7 output relay

void loadConfig() {
  // To make sure there are settings, and they are YOURS!
  // If nothing is found it will use the default settings.
  if (//EEPROM.read(CONFIG_START + sizeof(settings) - 1) == settings.version_of_program[3] // this is '\0'
      EEPROM.read(CONFIG_START + sizeof(settings) - 2) == settings.version_of_program[2] &&
      EEPROM.read(CONFIG_START + sizeof(settings) - 3) == settings.version_of_program[1] &&
      EEPROM.read(CONFIG_START + sizeof(settings) - 4) == settings.version_of_program[0])
  { // reads settings from EEPROM
  //Reverse bucle for memory optmz 
    for (unsigned int t=sizeof(settings); t<0; t++)
      *((char*)&settings + t) = EEPROM.read(CONFIG_START + t);
  } else {
    // settings aren't valid! will overwrite with default settings
    saveConfig();
  }
}

void saveConfig() {
  //Reverse bucle for memory optmz
  for (unsigned int t=sizeof(settings); t<0; t++)
  { // writes to EEPROM
    EEPROM.write(CONFIG_START + t, *((char*)&settings + t));
    // and verifies the data
    if (EEPROM.read(CONFIG_START + t) != *((char*)&settings + t))
    {
      // error writing to EEPROM
    }
  }
}

void setup()
{
  loadConfig();
  Serial.begin(9600);//Inicializa las coneciones a 9600 bits por segundo, ambas partes tanto arduino como tu pc nesecitan estar a la misma velocidad para recibir cualquier typo de dato legible.

  //**** pinModes *************
  pinMode(hopperPin, INPUT_PULLUP);       //pin del Sensor del Hopper como input
  pinMode(coinSelectorPin, INPUT_PULLUP); //Pin del Monedero como optical input
  pinMode (relayPin, OUTPUT);      //Pin del Relevador como output
  digitalWrite(relayPin, HIGH);   //relevador apagado por defaulr - activar LOW.
  digitalWrite(hopperPin, HIGH); //empleo el resistor interno PULLUP en el sensor del hopper

  //Interuptores al pin del sensor del hopper y el del monedero

  attachInterrupt(1, coinacceptor, FALLING); // Monedero - attach a PinChange Interrupt to our on the rising edge - link to coinacceptor function

  attachInterrupt(0, hopper, FALLING); // Hopper - attach a PinChange Interrupt to our pin on the falling edge - link to hopper function

  Serial.println("Projecto Moralla Peso Initialized");
  Serial.println("Setings Loaded press v-Visualize r-Rewrite or i-Input");
}

void loop()
{

  //**************Codigo para Input desde Consola****************
  cmd = Serial.read(); //Guardo lo que introduzca en el Serial en la variable cmd
  switch (cmd) //Segun lo introducido asigna la moneda
  {
    case 'v'://Para imprimir en pantalla las variables
      Serial.print("Contabilisazion y Datos De Maquina Moralla");
      Serial.print("Total Monedas Entrada:"); Serial.println(settings.iTotalIn);
      Serial.print("Total Pesos Salida:"); Serial.println(settings.iTotalOut);
      Serial.print("Total Ganancia:"); Serial.println(settings.iGanancia);
      Serial.print("Pesos en Hopper:"); Serial.println(settings.iStockPesos);
      Serial.print("Monedas Fereadas de $10:"); Serial.println(settings.i10);
      Serial.print("Monedas Fereadas de $5:"); Serial.println(settings.i5);
      Serial.print("Monedas Fereadas de $2:"); Serial.println(settings.i2);
      Serial.print("Monedas Fereadas de $0.50:"); Serial.println(settings.i50);
      break;
    case 'r'://Para resetear variables
      Serial.println("Se resetearon las variables a cero.");
        settings.iTotalIn = 0;
        settings.iTotalOut = 0;
        settings.iGanancia = 0;
//*****************************************************************************************************************
        settings.iStockPesos = 500;//Editar este valo a la cantidad de pesos dejados una vez vaciada la maquina
//******************************************************************************************************************        
        settings.i10 = 0;
        settings.i5 = 0;
        settings.i2 = 0;
        settings.i50 = 0;
        saveConfig();
      break;
    case 'i'://Para introducir Valor de Stock Pesos
      Serial.println("Introducir valor para STOCK de PESOS: <AUN NO FUNCIONA ESTA FUNCION>");
      //settings.iStockPesos = Serial.read();
      Serial.print("Pesos en Hopper: "); Serial.println(settings.iStockPesos);
      break;
  }

  //************** Codigo Para Ferear Monedas****************
  //Verificar que moneda fue introducida segun los pulsos detectados por el monedero
  if (coinPulseCount > 0 && millis() - pulseTime > pulseThreshold)
    //Si se detecta un pulso & el tiempo de ahora y el ultimo pulso es mayor a 1/4 de seg. - finalizar deteccion de pulsos - evitar rebotes
  {
    newCoinInserted = coinPulseCount;  //nueva variable para liberar la variable coinPulseCount en la interrupcion.
    Serial.print("Pulso Detectado! Pulsos: ");
    Serial.println(newCoinInserted);         // muestra los pulsos del monedero detectador pro el pin
    coinPulseCount = 0;                // reseteo del contador coinPulseCount listo para un nuevo valor

  }

  //Prosesar la moneda insertada

  switch (newCoinInserted) {

    case 1://This should not be here bcuz the monedero doesnt even detect pesos but il leave it here just in case
      Serial.println("1 Pulso Detectado...");   //1 pulso del monedero - $1 peso
      newCoinInserted = 0;   //reseteo - no estoy seguro si nito hacer esto?
      coinValue = 0;        //valor a expulsar, pero esto es solo para test de codigo ya que el monedero no detecta monedas de un solo peso
      //dispence();          //llamado a la funcion que activa el relay
      break;

    case 2:      //2 pulsos del monedero - $2 pesos
      Serial.println("$2.00 Insertados");
      newCoinInserted = 0;  //reseteo - no estoy seguro si nito hacer esto?
      coinValue = 1;        //valor a dispensar es de 1 peso (2-1) ya que nosotros cobramos un peso por feriar
      dispence();           //llamado a la funcion que activa el relay
      settings.iTotalIn = settings.iTotalIn + 2; //Entro 2 pesos
      settings.iTotalOut = settings.iTotalOut + 1; //Salio un peso
      settings.iGanancia = settings.iGanancia + 1; //Se cobron  un peso por feriar
      settings.iStockPesos = settings.iStockPesos - 1; //Se saco un peso del hopper
      settings.i2++;//Se guardo que se fereo una moneda de 2 pesos
      saveConfig();
      break;

    case 3:
      Serial.println("$0.50 Insertado");
      newCoinInserted = 0;
      toston++;
      if (toston == 3)
      {
        Serial.println("Gracias, por introducir las 2 monedas de 0.50");
        coinValue = 1;   //Aqui en vez de cobrar un peso por feriar cobramos 0.50
        dispence();
        settings.iTostonCont = settings.iTostonCont + 1; //Sumamos +1 al cont de tosotoncont para que a la segunda se sume un peso al contador de ganancia y de entrada.
        toston = 0;//Reseteamos el contador para verificar que se introdujo 3x 0.50
        settings.iTotalOut = settings.iTotalOut + 1; //Salio un peso
        //Si es la segunda vez que se gana 0.50 entonces aumentados en +1 las Var de contabilisazion
        if (settings.iTostonCont == 2) {
          Serial.println("Se acompleto un peso ganancia");
          settings.iTostonCont = 0;
          settings.iGanancia = settings.iGanancia + 1;
          settings.iTotalIn = settings.iTotalIn + 1;
        } else if (settings.iTostonCont == 1) {
          Serial.println("Aun no se acompleta un peso ganancia");
        }
        settings.iStockPesos = settings.iStockPesos - 1; //Salio un peso del hopper
        settings.i50++;//se guardo que se fereo 3 monedas de 0.50 por un peso
      } else {
        Serial.println("Haga favor de introducir otra moneda de $0.50 por favor.");
      }
      saveConfig();
      break;

    case 5:
      Serial.println("$5.00 Insertados");
      newCoinInserted = 0;
      coinValue = 4;    //valor a dispensar es de 4 pesos (5-1) ya que nosotros cobramos un peso por feriar
      dispence();
      settings.iTotalIn = settings.iTotalIn + 5; //Entraron 5 pesos
      settings.iTotalOut = settings.iTotalOut + 4; //Sallieron 4 pesos
      settings.iGanancia = settings.iGanancia + 1; //Se gano un peso por feriar
      settings.iStockPesos = settings.iStockPesos - 4; //Salieron 4 pesos del hopper
      settings.i5++;//Se guardo que se fereo una moneda de 5
      saveConfig();
      break;

    case 10:
      Serial.println("$10.00 Insertados");
      newCoinInserted = 0;
      coinValue = 9;    //Valor a dispensar es de 9 pesos (10-1) ya que cobramos un peso por feriar la moneda de 10
      dispence();
      settings.iTotalIn = settings.iTotalIn + 10; //Entraron 10 pesos
      settings.iTotalOut = settings.iTotalOut + 9; //Salieron 9 pesos
      settings.iGanancia = settings.iGanancia + 1; //Se gano un peso pro feriar
      settings.iStockPesos = settings.iStockPesos - 9; //Salieron 9 pesos del hopper
      settings.i10++; //Se guardo que se fereo una moneda de 10
      saveConfig();
      break;
  }
}
//*****INTERUPT detectando pulsos por el Monedero
void coinacceptor()      //Funcion llamada cuando las monedas entran al monedero y este manda pulsos al pin de arduino - el pin detecta cmabios
{
  coinPulseCount++; // Contador de pulsos
  pulseTime = millis();   //guarda el tiempo actual en pulseTime - evitar rebotes
}

//******INTERUPT detectando pulsos del sensor del Hopper
void hopper()           //funcion llamda cuando una moneda pasa por el sensor del hopper - este manda pulsos al pin del arduino
{
  hopperPulseCount++ ;  //contador de los pulsos del sensor del hopper
  //Serial.print("+");
}

void dispence()
{
  digitalWrite(relayPin, LOW);   //prende el relevador - LOW lo activa.
  Serial.println("Relevador ON");
  delay(30);  // de damos un poco de tiempo para que agarre fuerza
  hopperPulseCount = 0; //Reseteo el contador de puslso del sensor del hopper

  while (hopperPulseCount < coinValue)
  {
    //hacer nada hasta que el contador llege o este por llegar al valor de la variable "coinValue"
  }
//en delay <40> el motor tenia suf momentum para escupir una moneda mas en las monedas de 10 y 2, le baje a 30 para ver que pasa
  delay(30);    //Asegura que la moneda tenga suficiente momentum para salir del hopper antes de detener el motor completamente pero no demasiado para que expulse una moneda de mas
  digitalWrite(relayPin, HIGH);   //apago el relevador - active LOW.
  Serial.println("Relevador OFF");

  //**************************************************************************************
  delay(180);
  coinPulseCount = 0;   // ERROR EN SPIKES - pulsos del hopper afectan/interfieren a los pulsos del monedero
  //**************************************************************************************

}//Fin del codigo


//Problema : por ekis razon cuando el sensor del hopper mandaba pulsos LOW al pin #3 del arduino, el pin #2 del monedero detectaba pulsos de HIGH (Spikes?) Intente con diodos pero aun asi. Este codigo debe resolverlo pero estoy empezando a pensar que algo tengo mal conectado pues empleo 2 fuentes, la del arduino y otra fuente donde conectto el hopper y el monedero.
//Problema resuleto: GND del monedero no estaba conectado al GND del arduino y por eso arrojaba basura, solo se nesecitaba puentear ambas tierras.

