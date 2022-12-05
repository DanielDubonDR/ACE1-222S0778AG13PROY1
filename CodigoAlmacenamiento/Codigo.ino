#include <LiquidCrystal.h>
#include <Keypad.h>
#include <EEPROM.h>
LiquidCrystal lcd(22,23,24,25,26,27);

const byte FILAS = 4;
const byte COLUMNAS = 4;
char keys[FILAS][COLUMNAS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'},
};

byte pinesFilas[FILAS] = {28,29,30,31};
byte pinesColumnas[COLUMNAS] = {32,33,34,35};

Keypad teclado = Keypad(makeKeymap(keys), pinesFilas, pinesColumnas, FILAS, COLUMNAS);
char TECLA;

struct Usuario
{
  char usuario[7];
  char password[9]; 
  char nombre[17];
};

struct Admin
{
  char usuario[7];
  char password[7]; 
  char nombre[10];
};

byte fder[] = {
  B00000,
  B00100,
  B00110,
  B11111,
  B11111,
  B00110,
  B00100,
  B00000
};
byte fizq[] = {
  B00000,
  B00100,
  B01100,
  B11111,
  B11111,
  B01100,
  B00100,
  B00000
};

String msg, usuario, password, token, usuarioApp, passwordApp, tokenApp;
bool ingresoUsuario = false, ingresoPassword = false, coneccionBluetooth=true;
bool esperandoUsuarioApp=true, esperandoPasswordApp=true, esperandoTokenApp=true, ingresoToken=true;
bool reinicio = false;
int eeAddress;
int estado = 2, estadoLogin=0;
int contadorTeclas = 0, contadorError = 0, contadorUsuario = 0;
String typeUser="";

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial2.begin(9600);
  lcd.begin(16,2);
  msgInicio();
}

void loop() 
{
  switch(estado)
  {
    case 1:
    {
      //Login
      login();
    }break;
    case 2:
    {
      //Menu Admin parte 1
      menu_Admin();
    }break;
    case 3:
    {
      //Menu Admin parte 2
      menu_Admin_1();
    }break;
    case 4:
    {
      //Menu del crud parte 1
      menu_Crud_Admin();
    }break;
    case 5:
    {
      //Menu del crud parte 2
      menu_Crud_Admin_1();
    }break;
    case 6:
    {
      //Menu del crud parte 3
      menu_Crud_Admin_2();
    }break;
    case 7:
    {
      
      menu_Crud_Admin_Crear();
    }break;
     case 8:
    {
      
      menu_Crud_Admin_Modificar();
    }break;
    case 9:
    {
      
      menu_Crud_Admin_VerUsuarios();
    }break;
    case 10:
    {
      
      menu_Crud_Admin_Eliminar();
    }break;
     case 30:
    {
      gestionarPaquetes();
    }break;
  }
}

void msgInicio()
{
  lcd.clear();
  lcd.createChar(0, fder);
  lcd.setCursor(2,0);
  lcd.write(byte(0));
  lcd.setCursor(3,0);
  lcd.print("Bienvenido");
  lcd.createChar(1, fizq);
  lcd.setCursor(13,0);
  lcd.write(byte(1));
  lcd.setCursor(0,1);
  lcd.print("Grupo13-SeccionA");
  delay(3000);
  lcd.clear();
  lcd.setCursor(5,0);
  lcd.print("LOGIN");
  delay(1000);
  lcd.setCursor(1,1);
  lcd.print("Inicie Sesion");
  delay(2000);
  lcd.clear();
}

void login()
{
  while(ingresoUsuario==false)
  {
    lcd.setCursor(0,0);
    lcd.print("Ingrese Usuario");
    lcd.setCursor(4,1);
    lcd.print(usuario);
    TECLA = teclado.waitForKey();
    usuario+=TECLA;
    contadorTeclas++;
    if(TECLA=='*')
    {
      contadorError++;
    }
    if(contadorError>=2)
    {
      contadorTeclas=0;
      contadorError=0;
      usuario="";
      lcd.clear();
    }
    if(contadorTeclas==6)
    {
      ingresoUsuario=true;
      contadorTeclas=0;
      lcd.clear();
    }
  }
  
  while(ingresoPassword==false)
  {
    lcd.setCursor(0,0);
    lcd.print("Ingrese Password");
    TECLA = teclado.waitForKey();
    password+=TECLA;
    lcd.setCursor(contadorTeclas+5,1);
    lcd.print("*");
    contadorTeclas++;
    if(contadorTeclas==6)
    {
      ingresoPassword=true;
      contadorTeclas=0;
      lcd.clear();
    }
  }
  if(usuario=="111111" && password=="000000")
  {
    lcd.setCursor(0,0);
    lcd.print("Credenciales");
    lcd.setCursor(0,1);
    lcd.print("Correctas");
    delay(3000);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Bienvenido");
    lcd.setCursor(0,1);
    lcd.print("Admin");
    typeUser="admin";
    delay(3000);
    lcd.clear();
    if(reinicio==false)
    {
      coneccionBluetooth=false;
      lcd.setCursor(3,0);
      lcd.print("Vincule la");
      lcd.setCursor(3,1);
      lcd.print("aplicacion");
    }
    reinicio=false;
  }
  else if(usuario=="222222" && password=="000000")
  {
    lcd.setCursor(0,0);
    lcd.print("Credenciales");
    lcd.setCursor(0,1);
    lcd.print("Correctas");
    delay(3000);
    lcd.clear();
    lcd.setCursor(0,0);
    typeUser="user";
    lcd.print("Bienvenido");
    lcd.setCursor(0,1);
    lcd.print("Usuario");
    delay(3000);
    lcd.clear();
    if(reinicio==false)
    {
      coneccionBluetooth=false;
      lcd.setCursor(3,0);
      lcd.print("Vincule la");
      lcd.setCursor(3,1);
      lcd.print("aplicacion");
    }
    reinicio=false;
  }
  else
  {
    lcd.setCursor(0,0);
    lcd.print("Credenciales");
    lcd.setCursor(0,1);
    lcd.print("Incorrectas");
    delay(3000);
    resetLogin();
    lcd.clear();
  }
  
  while(coneccionBluetooth==false)
  {
    if(Serial1.available()>0)
    {
      msg=Serial1.readStringUntil('\n');
    }
    if(msg=="conectado")
    {
      lcd.clear();
      lcd.setCursor(3,0);
      lcd.print("Aplicacion");
      lcd.setCursor(3,1);
      lcd.print("Vinculada");
      delay(3000);
      coneccionBluetooth=true;
      esperandoUsuarioApp=false;
      lcd.clear();
    }
  }

  while(esperandoUsuarioApp==false)
  {
    lcd.setCursor(0,0);
    lcd.print("Obteniendo");
    lcd.setCursor(0,1);
    lcd.print("usuario app");
    if(Serial1.available()>0)
    {
      usuarioApp=Serial1.readStringUntil('\n');
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Usuario obtenido");
      lcd.setCursor(0,1);
      lcd.print(usuarioApp);
      delay(3000);
      Serial1.print("correcto");
      esperandoUsuarioApp=true;
      esperandoPasswordApp=false;
      lcd.clear();
    }
  }

  while(esperandoPasswordApp==false)
  {
    lcd.setCursor(0,0);
    lcd.print("Obteniendo");
    lcd.setCursor(0,1);
    lcd.print("password app");
    if(Serial1.available()>0)
    {
      passwordApp=Serial1.readStringUntil('\n');
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Password obtenido");
      lcd.setCursor(0,1);
      lcd.print(passwordApp);
      delay(3000);
      Serial1.print("correcto");
      esperandoPasswordApp=true;
//      esperandoTokenApp=false;
      estadoLogin=1;
      lcd.clear();
    }
  }

  if(estadoLogin==1)
  {
    if(usuario==usuarioApp && password==passwordApp && usuario!="" && password!="")
    {
      esperandoTokenApp=false;
    }
    else
    {
      lcd.setCursor(0,0);
      lcd.print("Credenciales");
      lcd.setCursor(0,1);
      lcd.print("invalidas");
      delay(3000);
      lcd.clear();
      reinicio=true;
      resetLogin1();
      estadoLogin=0;
    }
  }
  
  while(esperandoTokenApp==false)
  {
    lcd.setCursor(0,0);
    lcd.print("Obteniendo");
    lcd.setCursor(0,1);
    lcd.print("token app");
    if(Serial1.available()>0)
    {
      tokenApp=Serial1.readStringUntil('\n');
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Token obtenido");
      lcd.setCursor(0,1);
      lcd.print(tokenApp);
      delay(3000);
      esperandoTokenApp=true;
      ingresoToken=false;
      lcd.clear();
    }
  }

  while(ingresoToken==false)
  {
    lcd.setCursor(0,0);
    lcd.print("Ingrese el token");
    lcd.setCursor(5,1);
    lcd.print(token);
    TECLA = teclado.waitForKey();
    token+=TECLA;
    contadorTeclas++;
    if(contadorTeclas==6)
    {
      ingresoToken=true;
      estadoLogin=2;
      contadorTeclas=0;
      lcd.clear();
    }
  }

  if(estadoLogin==2)
  {
    if(token==tokenApp && token!="" && tokenApp!="")
    {
      Serial1.print("correcto");
      if(typeUser=="admin")
      {
        estado=2;
        lcd.setCursor(0,0);
        lcd.print("Bienvenido");
        lcd.setCursor(0,1);
        lcd.print("Admin");
        delay(3000);
        Serial1.print("admin");
        lcd.clear();
      }
      else if(typeUser=="user")
      {
        estado=30;
        lcd.setCursor(0,0);
        lcd.print("Bienvenido");
        lcd.setCursor(0,1);
        lcd.print("Usuario");
        delay(1500);
        Serial1.print("usuario");
        delay(1500);
        Serial1.print("Pancho");
        lcd.clear();
      }
      else
      {
        lcd.setCursor(1,0);
        lcd.print("Error");
        delay(3000);
        lcd.clear();
        reinicio=true;
        resetLogin1();
        estadoLogin=0;
        Serial1.print("incorrecto");
      }
    }
    else
    {
      lcd.setCursor(1,0);
      lcd.print("Token invalido");
      delay(3000);
      lcd.clear();
      reinicio=true;
      resetLogin1();
      estadoLogin=0;
    }
  }
}

void resetLogin()
{
  ingresoUsuario=false;
  ingresoPassword=false;
  usuario="";
  password="";
  contadorTeclas=0;
  typeUser="";
}

void resetLogin1()
{
  ingresoUsuario=false;
  ingresoPassword=false;
  usuario="";
  password="";
  usuarioApp="";
  passwordApp="";
  coneccionBluetooth=true;
  esperandoUsuarioApp=false;
  esperandoPasswordApp=true;
  token="";
  tokenApp="";
  contadorTeclas=0;
  esperandoTokenApp=true;
  ingresoToken=true;
  typeUser="";
}

void menu_Admin()
{
  lcd.setCursor(0,0);
  lcd.print("1.CRUD Usuarios");
  lcd.setCursor(0,1);
  lcd.print("2.Reportes");
  lcd.setCursor(13,1);
  lcd.print("D.");
  lcd.createChar(0, fder);
  lcd.setCursor(15,1);
  lcd.write(byte(0));
  TECLA = teclado.waitForKey();
  if(TECLA=='1')
  {
//    lcd.setCursor(3,0);
//    lcd.print("Gestionar");
//    lcd.setCursor(0,1);
//    lcd.print("Usuarios");
//    delay(3000);
    estado=4;
    lcd.clear();
  }
  else if(TECLA=='2')
  {
    lcd.setCursor(4,0);
    lcd.print("Reportes");
    delay(3000);
    lcd.clear();
  }
  else if(TECLA=='D')
  {
    estado=3;
    lcd.clear();
  }
  else
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Opcion Invalida");
    delay(3000);
    lcd.clear();
  }
}

void menu_Admin_1()
{
  lcd.setCursor(0,0);
  lcd.print("3.Cerrar Sesion");
  lcd.setCursor(13,1);
  lcd.print("D.");
  lcd.createChar(0, fder);
  lcd.setCursor(15,1);
  lcd.write(byte(0));
  TECLA = teclado.waitForKey();
  if(TECLA=='3')
  {
    estado=1;
    lcd.clear();
    resetLogin1();
  }
  else if(TECLA=='D')
  {
    estado=2;
    lcd.clear();
  }
  else
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Opcion Invalida");
    delay(3000);
    lcd.clear();
  }
}

void menu_Crud_Admin()
{
  lcd.setCursor(0,0);
  lcd.print("1.Crear");
  lcd.setCursor(0,1);
  lcd.print("2.Modificar");
  lcd.setCursor(13,1);
  lcd.print("D.");
  lcd.createChar(0, fder);
  lcd.setCursor(15,1);
  lcd.write(byte(0));
  TECLA = teclado.waitForKey();
  if(TECLA=='1')
  {
      estado=7;
      lcd.clear();
  }
  else if(TECLA=='2')
  {
      estado=8;
      lcd.clear();
  }
  else if(TECLA=='D')
  {
    estado=5;
    lcd.clear();
  }
  else
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Opcion Invalida");
    delay(3000);
    lcd.clear();
  }
}

void menu_Crud_Admin_1()
{
  lcd.setCursor(0,0);
  lcd.print("3.Ver Usuarios");
  lcd.setCursor(0,1);
  lcd.print("4.Eliminar");
  lcd.setCursor(13,1);
  lcd.print("D.");
  lcd.createChar(0, fder);
  lcd.setCursor(15,1);
  lcd.write(byte(0));
  TECLA = teclado.waitForKey();
  if(TECLA=='3')
  {
      estado=9;
      lcd.clear();
  }
  else if(TECLA=='4')
  {
      estado=10;
      lcd.clear();
  }
  else if(TECLA=='D')
  {
    estado=6;
    lcd.clear();
  }
  else
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Opcion Invalida");
    delay(3000);
    lcd.clear();
  }
}

void menu_Crud_Admin_2()
{
  lcd.setCursor(0,0);
  lcd.print("5.Salir");
  lcd.setCursor(13,1);
  lcd.print("D.");
  lcd.createChar(0, fder);
  lcd.setCursor(15,1);
  lcd.write(byte(0));
  TECLA = teclado.waitForKey();
  if(TECLA=='5')
  {
    estado=2;
    lcd.clear();
  }
  else if(TECLA=='D')
  {
    estado=4;
    lcd.clear();
  }
  else
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Opcion Invalida");
    delay(3000);
    lcd.clear();
  }
}

void menu_Crud_Admin_Crear()
{
//  aca logia de crear un usuario
  contadorTeclas = 0;
  String nuevoUsuario, nuevoPassword;
  lcd.clear();
  lcd.setCursor(1,0);
  lcd.print("Ingrese Usuario");
  while(contadorTeclas < 6){
    lcd.setCursor(4,1);
    lcd.print(nuevoUsuario);
    TECLA = teclado.waitForKey();
    nuevoUsuario+=TECLA;
    contadorTeclas++;
  }

  contadorTeclas = 0;
  lcd.clear();
  

  for(int i = 0; i<nuevoUsuario.length(); i++){
    Serial.print(nuevoUsuario.charAt(i));
    EEPROM.write(contadorUsuario,nuevoUsuario.charAt(i));
  }
  
  contadorTeclas=0;
  lcd.clear();
    
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Ingrese Password");
  while(contadorTeclas < 8){
    lcd.setCursor(4,1);
    lcd.print(nuevoPassword);
    TECLA = teclado.waitForKey();
    if(TECLA == '*'){
      contadorTeclas = 8;
    }
    nuevoPassword+=TECLA;
    contadorTeclas++;
  }

  contadorTeclas = 0;
  lcd.clear();

  for(int i = 0; i<nuevoPassword.length(); i++){
    Serial.print(nuevoPassword.charAt(i));
    EEPROM.write(contadorUsuario,nuevoPassword.charAt(i));
  }
  
  lcd.setCursor(1,0);
  lcd.print("Usuario creado");
  delay(3000);
  lcd.clear();
  contadorUsuario = contadorUsuario + 14;
  estado=4;
}

void menu_Crud_Admin_Modificar()
{
//  aca logia de modificar un usuario
  usuario="";
  password="";
  int number = 0;
  contadorTeclas = 0;

  lcd.clear();
  lcd.setCursor(1,0);
  lcd.print("Ingrese número");
  lcd.setCursor(4,1);
  lcd.print("de usuario:");
  TECLA = teclado.waitForKey();
  usuario+=TECLA;
  number = usuario.toInt();
  eeAddress = (14*(number)-14);
  lcd.clear();
  lcd.setCursor(1,0);
  lcd.print("Ingrese nuevo");
  lcd.setCursor(4,1);
  lcd.print("código:");
  delay(3000);
  lcd.clear();
  while(contadorTeclas < 6){
    lcd.setCursor(4,1);
    lcd.print(usuario);
    TECLA = teclado.waitForKey();
    usuario+=TECLA;
    contadorTeclas++;
  }

  contadorTeclas = 0;
  lcd.clear();
  

  for(int i = 0; i<usuario.length(); i++){
    Serial.print(usuario.charAt(i));
    EEPROM.write(contadorUsuario,usuario.charAt(i));
  }
  
  
  lcd.clear();
  lcd.setCursor(1,0);
  lcd.print("Ingrese nueva");
  lcd.setCursor(4,1);
  lcd.print("contraseña:");
  delay(3000);
  lcd.clear();
  while(contadorTeclas < 8){
    lcd.setCursor(4,1);
    lcd.print(password);
    TECLA = teclado.waitForKey();
    if(TECLA == '*'){
      contadorTeclas = 8;
    }
    password+=TECLA;
    contadorTeclas++;
  }
  
  for(int i = 0; i<password.length(); i++){
    Serial.print(password.charAt(i));
    EEPROM.write(contadorUsuario,password.charAt(i));
  }
  
  lcd.setCursor(1,0);
  lcd.print("Usuario modificado");
  delay(3000);
  lcd.clear();
  estado=4;
}

void menu_Crud_Admin_VerUsuarios()
{
//  aca logia de ver los usuarios registrados
    usuario="";
    password="";
    int number = 0;
  
    lcd.setCursor(0,0);
    lcd.print("Ver usuarios");
    delay(3000);

    lcd.setCursor(1,0);
    lcd.print("Ingrese número");
    lcd.setCursor(4,1);
    lcd.print("de usuario:");
    TECLA = teclado.waitForKey();
    usuario+=TECLA;
    number = usuario.toInt();
    eeAddress = (14*(number)-14);
    
    EEPROM.get(eeAddress,usuario);
    lcd.setCursor(1,0);
    lcd.print(usuario);
      
    EEPROM.get(eeAddress+1,password);
    lcd.setCursor(1,1);
    lcd.print(password);
    delay(3000);
    lcd.clear();
    estado=4;
}

void menu_Crud_Admin_Eliminar()
{
//  aca logia de ver los usuarios registrados
    usuario="";
    int number = 0;
    lcd.setCursor(0,0);
    lcd.print("Eliminar");
    delay(3000);
  
    lcd.setCursor(1,0);
    lcd.print("Ingrese número");
    lcd.setCursor(4,1);
    lcd.print("de usuario:");
    TECLA = teclado.waitForKey();
    usuario+=TECLA;
    number = usuario.toInt();
    eeAddress = (14*(number)-14);
    for (int i = 0; i < 14; i++){
       EEPROM.write(eeAddress+i,0);
    }
    
    lcd.setCursor(0,0);
    lcd.print("Usuario borrado");
    delay(3000);
    estado=4;
    lcd.clear();
}

void gestionarPaquetes()
{
  if(Serial1.available()>0)
  {
    msg=Serial1.readStringUntil('\n');
    Serial.println(msg);
    if(msg=="pequeno")
    {
      Serial2.print("pequeno");
      Serial2.print('\n');
    }
    else if(msg=="mediano")
    {
      Serial2.print("mediano");
      Serial2.print('\n');
    }
    else if(msg=="grande")
    {
      Serial2.print("grande");
      Serial2.print('\n');
    }
    else if(msg=="cerrar")
    {
      estado=1;
      resetLogin1();
    }
  }
}

void writeAdmin(){

  Admin adm;
  
  strcpy(adm.nombre, "Admin-13A");
  strcpy(adm.usuario, "A1973C");
  strcpy(adm.password, "AD13DA");
  
  EEPROM.put(0,adm);
}

void Usuarios(){
  Usuario usuario1;
  strcpy(usuario1.usuario, "111111");
  strcpy(usuario1.password, "111111");

  Usuario usuario2;
  strcpy(usuario2.usuario, "222222");
  strcpy(usuario2.password, "222222");

  Usuario usuario3;
  strcpy(usuario3.usuario, "333333");
  strcpy(usuario3.password, "333333");

  Usuario usuario4;
  strcpy(usuario4.usuario, "444444");
  strcpy(usuario4.password, "444444");

  Usuario usuario5;
  strcpy(usuario5.usuario, "555555");
  strcpy(usuario5.password, "555555");

  EEPROM.put(0,usuario1);
  EEPROM.put(14,usuario2);
  EEPROM.put(28,usuario3);
  EEPROM.put(42,usuario4);
  EEPROM.put(56,usuario5);
}
