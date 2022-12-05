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

struct Admin
{
  char nombre[10];
  char usuario[7]; 
  char password[7];
};

struct User
{
  char usuario[7];
  char password[9]; 
  char nombre[17];
  int pppequeno;
  int ppmediano;
  int ppgrande;
  int pdpequeno;
  int pdmediano;
  int pdgrande;
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
bool reinicio = false, msgLogin=false, encontrado=false;
int estado = 1, estadoLogin=0;
int contadorTeclas = 0, contadorError = 0, contadorUsuarios = 0;
String typeUser="";
String newUser, newPassword, codigoUsuario;
bool ingresoNewUsuario=false, ingresoNewPassword=false;
int newContLetras=0, newContLetras2=0;
bool esperandoCodigoUsuario=false, esperandoNombreUsuario=false, esperandoNewPassUsuario=false;
bool encontrado2=false, encontrado3=false;
String nuevoNombre, changePassword, codUser, codUser2, codUser3;
int addressNuevoNombre;
bool ingresandoUser=false, esperandoCodigoUsuarioDel=false, encontrado8=false;
int contAuxi=0;
int iterador=0;
String msgEstadoArduino="Procesar paquete", msgEstadoArduino1="disponible";
int pos=0, pos1=3;
int paquetePP, paquetePM, paquetePG, paqueteDP, paqueteDM, paqueteDG, paqueteAP, paqueteAM, paqueteAG, userLogAdress;
User userLog;
User renameUser;
User searchUser;
User userControl;

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial2.begin(9600);
  lcd.begin(16,2);
  writeAdmin(); //size 24 25
  writeUsers(); //size 45 46
  EEPROM.get(4000,contadorUsuarios);
  Serial.print("Usuarios: ");
  Serial.println(contadorUsuarios);
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
    case 11:
    {
      modificarNombreUsuario();
    }break;
    case 12:
    {
      modificarPasswordUsuario();
    }break;
    case 18:
    {
      cerrarSesionAdmin();
    }break;
    case 20:
    {
      verReportes();
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
}

void login()
{
  while(ingresoUsuario==false)
  {
    if(msgLogin==false)
    {
      lcd.setCursor(5,0);
      lcd.print("LOGIN");
      delay(1000);
      lcd.setCursor(1,1);
      lcd.print("Inicie Sesion");
      delay(2000);
      lcd.clear();
      msgLogin=true;
    }
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
    if(contadorError>=1)
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
    if(TECLA!='#')
    {
      password+=TECLA;
    }
    lcd.setCursor(contadorTeclas+4,1);
    lcd.print("*");
    contadorTeclas++;
    if(TECLA=='#')
    {
      ingresoPassword=true;
      contadorTeclas=0;
      lcd.clear();
    }
    if(contadorTeclas==8)
    {
      ingresoPassword=true;
      contadorTeclas=0;
      lcd.clear();
    }
    else if(TECLA=='*')
    {
      contadorTeclas=0;
      password="";
      lcd.clear();
    }
  }

  Admin adm;
  EEPROM.get(0, adm);
  Serial.println(adm.usuario);
  Serial.println(adm.password);
  if(usuario==adm.usuario && password==adm.password)
  {
    lcd.setCursor(2,0);
    lcd.print("Credenciales");
    lcd.setCursor(3,1);
    lcd.print("Correctas");
    delay(3000);
    lcd.clear();
//    lcd.setCursor(0,0);
//    lcd.print("Bienvenido");
//    lcd.setCursor(0,1);
//    lcd.print("Admin");
    typeUser="admin";
//    delay(3000);
//    lcd.clear();
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
    Serial.println("entre");
    for(int i=0; i<contadorUsuarios; i++)
    {
      User aux;
      EEPROM.get(25+(i*46),aux);
      if(usuario==aux.usuario && password==aux.password)
      {
        encontrado = true;
        typeUser="user";
        userLog=aux;
        userLogAdress=25+(i*46);
        break;
      }
    }
      if(encontrado)
      {
        lcd.setCursor(0,0);
        lcd.print("Credenciales");
        lcd.setCursor(0,1);
        lcd.print("Correctas");
        delay(3000);
        lcd.clear();
//        lcd.setCursor(0,0);
//        lcd.print("Bienvenido");
//        lcd.setCursor(0,1);
//        lcd.print(userLog.nombre);
        typeUser="user";
//        delay(3000);
//        lcd.clear();
        if(reinicio==false)
        {
          coneccionBluetooth=false;
          lcd.setCursor(3,0);
          lcd.print("Vincule la");
          lcd.setCursor(3,1);
          lcd.print("aplicacion");
        }
        reinicio=false;
        encontrado=false;
      }
      else
      {
        lcd.setCursor(2,0);
        lcd.print("Credenciales");
        lcd.setCursor(3,1);
        lcd.print("Incorrectas");
        delay(3000);
        resetLogin();
        lcd.clear();
      }
    
  }
//  else
//  {
//    lcd.setCursor(0,0);
//    lcd.print("Credenciales");
//    lcd.setCursor(0,1);
//    lcd.print("Incorrectas");
//    delay(3000);
//    resetLogin();
//    lcd.clear();
//  }
  
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
//      lcd.setCursor(0,1);
//      lcd.print(usuarioApp);
      delay(3000);
      Serial1.print("1");
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
      lcd.setCursor(4,0);
      lcd.print("Password");
      lcd.setCursor(4,1);
      lcd.print("obtenido");
      delay(3000);
      Serial1.print("1");
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
//      lcd.setCursor(0,1);
//      lcd.print(tokenApp);
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
//      lcd.clear();
    }
    if(TECLA=='*')
    {
      token="";
      contadorTeclas=0;
      lcd.clear();
    }
  }

  if(estadoLogin==2)
  {
    if(token==tokenApp && token!="" && tokenApp!="")
    {
      Serial1.print("1");
      delay(500);
      lcd.clear();
      if(typeUser=="admin")
      {
        estado=2;
        lcd.setCursor(0,0);
        lcd.print("Bienvenido");
        lcd.setCursor(0,1);
        lcd.print("Admin-13A");
        delay(1000);
        Serial1.print("a");
        delay(2000);
        lcd.clear();
        sincronizarApp();
      }
      else if(typeUser=="user")
      {
        estado=30;
        lcd.setCursor(0,0);
        lcd.print("Bienvenido");
        lcd.setCursor(0,1);
        lcd.print(userLog.nombre);
        delay(1000);
        Serial1.print("u");
        delay(2000);
        Serial1.print(userLog.nombre);
        paquetePP=userLog.pppequeno;
        paquetePM=userLog.ppmediano;
        paquetePG=userLog.ppgrande;
        paqueteDP=userLog.pdpequeno;
        paqueteDM=userLog.pdmediano;
        paqueteDG=userLog.pdgrande;
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
        Serial1.print("0");
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
  msgLogin=true;
}

void resetLogin1()
{
  ingresoUsuario=false;
  ingresoPassword=false;
  usuario="";
  password="";
  usuarioApp="";
  passwordApp="";
  coneccionBluetooth=false;
  esperandoUsuarioApp=false;
  esperandoPasswordApp=true;
  token="";
  tokenApp="";
  contadorTeclas=0;
  esperandoTokenApp=true;
  ingresoToken=true;
  typeUser="";
  msgLogin=false;
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
//    lcd.setCursor(4,0);
//    lcd.print("Reportes");
//    delay(3000);
    estado=20;
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
    estado=18;
    lcd.clear();
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
  while(ingresoNewUsuario==false)
  {
    lcd.setCursor(0,0);
    lcd.print("Ingrese Usuario");
    lcd.setCursor(4,1);
    lcd.print(newUser);
    TECLA = teclado.waitForKey();
    newUser+=TECLA;
    newContLetras++;
    if(TECLA=='*')
    {
      newContLetras=0;
      newUser="";
      lcd.clear();
    }
    if(newContLetras==6)
    {
      ingresoNewUsuario=true;
      newContLetras=0;
      lcd.clear();
    }
  }

  while(ingresoNewPassword==false)
  {
    lcd.setCursor(0,0);
    lcd.print("Ingrese Password");
    TECLA = teclado.waitForKey();
    if(TECLA!='#')
    {
      newPassword+=TECLA;
    }
    lcd.setCursor(newContLetras+4,1);
    lcd.print("*");
    newContLetras++;
    if(TECLA=='#')
    {
      ingresoNewPassword=true;
      newContLetras=0;
      lcd.clear();
    }
    if(newContLetras==8)
    {
      ingresoNewPassword=true;
      newContLetras=0;
      lcd.clear();
    }
    else if(TECLA=='*')
    {
      contadorTeclas=0;
      password="";
      lcd.clear();
    }
  }

  User user0;

  char a1[7], a2[9];
  newUser.toCharArray(a1,7);
  newPassword.toCharArray(a2,9);
  Serial.println(a1);
  Serial.println(a2);
  strcpy(user0.usuario, a1);
  strcpy(user0.password, a2);
  strcpy(user0.nombre, "Unknow");
  user0.pppequeno=0;
  user0.ppmediano=0;
  user0.ppgrande=0;
  user0.pdpequeno=0;
  user0.pdmediano=0;
  user0.pdgrande=0;
  Serial.println(sizeof(user0));
  EEPROM.put(25+(46*contadorUsuarios),user0);
  Serial.println(25+(46*contadorUsuarios));
  contadorUsuarios++;
  EEPROM.update(4000, contadorUsuarios);

  lcd.setCursor(1,0);
  lcd.print("Usuario creado");
  lcd.setCursor(3,1);
  lcd.print("con exito");
  delay(3000);
  lcd.clear();
  ingresoNewUsuario=false;
  ingresoNewPassword=false;
  estado=4;
  
}

void menu_Crud_Admin_Modificar()
{
//  aca logia de modificar un usuario
  lcd.setCursor(0,0);
  lcd.print("1.Nombre");
  lcd.setCursor(0,1);
  lcd.print("2.Password");
  lcd.setCursor(13,1);
  lcd.print("B.");
  lcd.createChar(0, fizq);
  lcd.setCursor(15,1);
  lcd.write(byte(0));
  TECLA = teclado.waitForKey();
  if(TECLA=='1')
  {
      estado=11;
      lcd.clear();
  }
  else if(TECLA=='2')
  {
      estado=12;
      lcd.clear();
  }
  else if(TECLA=='B')
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

void menu_Crud_Admin_VerUsuarios()
{
//  aca logia de ver los usuarios registrados
  while(ingresandoUser==false)
  {
    lcd.setCursor(0,0);
    lcd.print("Ingrese Usuario");
    lcd.setCursor(4,1);
    lcd.print(codUser2);
    TECLA = teclado.waitForKey();
    codUser2+=TECLA;
    newContLetras2++;
    if(TECLA=='*')
    {
      newContLetras2=0;
      codUser2="";
      lcd.clear();
    }
    if(newContLetras2==6)
    {
      ingresandoUser=true;
//      esperandoNewPassUsuario=true;
      newContLetras2=0;
      lcd.clear();
    }
  }

  for(int i=0; i<contadorUsuarios; i++)
  {
    User aux;
    EEPROM.get(25+(i*46),aux);
    if(codUser2==aux.usuario)
    {
      encontrado3 = true;
      searchUser=aux;
      break;
    }
  }
  if(encontrado3)
  {
    lcd.setCursor(0,0);
    lcd.print("Nombre:");
    lcd.setCursor(0,1);
    lcd.print(searchUser.nombre);
    delay(3000);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Password:");
    lcd.setCursor(0,1);
    lcd.print(searchUser.password);
    delay(3000);
    lcd.clear();
    estado=4;
    encontrado3=false;
    ingresandoUser=false;
    codUser2="";
  }
  else
  {
    lcd.setCursor(3,0);
    lcd.print("Usuario no");
    lcd.setCursor(3,1);
    lcd.print("Encontrado");
    delay(3000);
    ingresandoUser=false;
    codUser2="";
    lcd.clear();
  }
}

void menu_Crud_Admin_Eliminar()
{
//  aca logia de ver los usuarios registrados
  while(esperandoCodigoUsuarioDel==false)
  {
    lcd.setCursor(0,0);
    lcd.print("Ingrese Usuario");
    lcd.setCursor(4,1);
    lcd.print(codUser3);
    TECLA = teclado.waitForKey();
    codUser3+=TECLA;
    contAuxi++;
    if(TECLA=='*')
    {
      contAuxi=0;
      codUser3="";
      lcd.clear();
    }
    if(contAuxi==6)
    {
      esperandoCodigoUsuarioDel=true;
      contAuxi=0;
      lcd.clear();
    }
  }

  for(int i=0; i<contadorUsuarios; i++)
  {
    User aux;
    EEPROM.get(25+(i*46),aux);
    if(codUser3==aux.usuario)
    {
      encontrado8 = true;
      renameUser=aux;
      addressNuevoNombre=25+(i*46);
      break;
    }
  }
  if(encontrado8)
  {
    strcpy(renameUser.usuario, "NONE");
    EEPROM.put(addressNuevoNombre, renameUser);

    lcd.setCursor(4,0);
    lcd.print("Usuario");
    lcd.setCursor(3,1);
    lcd.print("eliminado");
    delay(3000);
    encontrado8=false;
    lcd.clear();
    estado=4;
  }
  else
  {
    lcd.setCursor(3,0);
    lcd.print("Usuario no");
    lcd.setCursor(3,1);
    lcd.print("Encontrado");
    delay(3000);
    esperandoCodigoUsuarioDel=false;
    codUser3="";
    lcd.clear();
    estado=4;
  }
}

void gestionarPaquetes()
{
  if(Serial1.available()>0)
  {
    msg=Serial1.readStringUntil('\n');
    Serial.println(msg);
    if(msg=="pequeno")
    {
      paquetePP++;
      Serial2.print("pequeno");
      Serial2.print('\n');
      msgEstadoArduino="Paquete en";
      msgEstadoArduino1="proceso";
      pos=3;
      pos1=4;
      lcd.clear();
    }
    else if(msg=="mediano")
    {
      paquetePM++;
      Serial2.print("mediano");
      Serial2.print('\n');
      msgEstadoArduino="Paquete en";
      msgEstadoArduino1="proceso";
      pos=3;
      pos1=4;
      lcd.clear();
    }
    else if(msg=="grande")
    {
      paquetePG++;
      Serial2.print("grande");
      Serial2.print('\n');
      msgEstadoArduino="Paquete en";
      msgEstadoArduino1="proceso";
      pos=3;
      pos1=4;
      lcd.clear();
    }
    else if(msg=="b1")
    {
      Serial2.print("b1");
      Serial2.print('\n');
      paqueteDP+=paqueteAP;
      lcd.clear();
      lcd.setCursor(1,0);
      lcd.print("P. Despachados");
      lcd.setCursor(0,1);
      lcd.print("P:");
      lcd.print(paqueteDP);
      lcd.setCursor(6,1);
      lcd.print("M:");
      lcd.print(paqueteDM);
      lcd.setCursor(12,1);
      lcd.print("G:");
      lcd.print(paqueteDG);
      delay(900);
      lcd.clear();
      paqueteAP=0;
    }
    else if(msg=="b2")
    {
      Serial2.print("b2");
      Serial2.print('\n');
      paqueteDM+=paqueteAM;
      lcd.clear();
      lcd.setCursor(1,0);
      lcd.print("P. Despachados");
      lcd.setCursor(0,1);
      lcd.print("P:");
      lcd.print(paqueteDP);
      lcd.setCursor(6,1);
      lcd.print("M:");
      lcd.print(paqueteDM);
      lcd.setCursor(12,1);
      lcd.print("G:");
      lcd.print(paqueteDG);
      delay(900);
      lcd.clear();
      paqueteAM=0;
    }
    else if(msg=="b3")
    {
      Serial2.print("b3");
      Serial2.print('\n');
      paqueteDG+=paqueteAG;
      lcd.clear();
      lcd.setCursor(1,0);
      lcd.print("P. Despachados");
      lcd.setCursor(0,1);
      lcd.print("P:");
      lcd.print(paqueteDP);
      lcd.setCursor(6,1);
      lcd.print("M:");
      lcd.print(paqueteDM);
      lcd.setCursor(12,1);
      lcd.print("G:");
      lcd.print(paqueteDG);
      delay(900);
      lcd.clear();
      paqueteAG=0;
    }
    else if(msg=="cerrar")
    {
      userLog.pppequeno=paquetePP;
      userLog.ppmediano=paquetePM;
      userLog.ppgrande=paquetePG;
      userLog.pdpequeno=paqueteDP;
      userLog.pdmediano=paqueteDM;
      userLog.pdgrande=paqueteDG;
      EEPROM.put(userLogAdress,userLog);
      estado=1;
      resetLogin1();
      lcd.clear();
    }
  }

  if(Serial2.available()>0)
  {
    msg=Serial2.readStringUntil('\n');
    Serial.println(msg);
    if(msg=="ok")
    {
      msgEstadoArduino="Procesar paquete";
      msgEstadoArduino1="disponible";
      pos=0;
      pos1=3;
      lcd.clear();
    }
    else if(msg=="pp")
    {
      paqueteAP++;
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Paquete pequeno");
      lcd.setCursor(3,1);
      lcd.print("en puerta");
      delay(800);
      lcd.clear();
      lcd.setCursor(1,0);
      lcd.print("P. Acumulados");
      lcd.setCursor(0,1);
      lcd.print("P:");
      lcd.print(paqueteAP);
      lcd.setCursor(6,1);
      lcd.print("M:");
      lcd.print(paqueteAM);
      lcd.setCursor(12,1);
      lcd.print("G:");
      lcd.print(paqueteAG);
      delay(800);
      lcd.clear();
    }
    else if(msg=="pm")
    {
      paqueteAM++;
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Paquete mediano");
      lcd.setCursor(3,1);
      lcd.print("en puerta");
      delay(800);
      lcd.clear();
      lcd.setCursor(1,0);
      lcd.print("P. Acumulados");
      lcd.setCursor(0,1);
      lcd.print("P:");
      lcd.print(paqueteAP);
      lcd.setCursor(6,1);
      lcd.print("M:");
      lcd.print(paqueteAM);
      lcd.setCursor(12,1);
      lcd.print("G:");
      lcd.print(paqueteAG);
      delay(800);
      lcd.clear();
    }
    else if(msg=="pg")
    {
      paqueteAG++;
      lcd.clear();
      lcd.setCursor(1,0);
      lcd.print("Paquete grande");
      lcd.setCursor(3,1);
      lcd.print("en puerta");
      delay(800);
      lcd.clear();
      lcd.setCursor(1,0);
      lcd.print("P. Acumulados");
      lcd.setCursor(0,1);
      lcd.print("P:");
      lcd.print(paqueteAP);
      lcd.setCursor(6,1);
      lcd.print("M:");
      lcd.print(paqueteAM);
      lcd.setCursor(12,1);
      lcd.print("G:");
      lcd.print(paqueteAG);
      delay(800);
      lcd.clear();
    }
  }

  TECLA = teclado.getKey();
  if(TECLA=='A')
  {
      lcd.clear();
      lcd.setCursor(1,0);
      lcd.print("P. Procesados");
      lcd.setCursor(0,1);
      lcd.print("P:");
      lcd.print(paquetePP);
      lcd.setCursor(6,1);
      lcd.print("M:");
      lcd.print(paquetePM);
      lcd.setCursor(12,1);
      lcd.print("G:");
      lcd.print(paquetePG);
      delay(900);
      lcd.clear();
      lcd.setCursor(1,0);
      lcd.print("P. Acumulados");
      lcd.setCursor(0,1);
      lcd.print("P:");
      lcd.print(paqueteAP);
      lcd.setCursor(6,1);
      lcd.print("M:");
      lcd.print(paqueteAM);
      lcd.setCursor(12,1);
      lcd.print("G:");
      lcd.print(paqueteAG);
      delay(900);
      lcd.clear();
      lcd.setCursor(1,0);
      lcd.print("P. Despachados");
      lcd.setCursor(0,1);
      lcd.print("P:");
      lcd.print(paqueteDP);
      lcd.setCursor(6,1);
      lcd.print("M:");
      lcd.print(paqueteDM);
      lcd.setCursor(12,1);
      lcd.print("G:");
      lcd.print(paqueteDG);
      delay(900);
      lcd.clear();
  }

  lcd.setCursor(pos,0);
  lcd.print(msgEstadoArduino);
  lcd.setCursor(pos1,1);
  lcd.print(msgEstadoArduino1);
}

void writeAdmin()
{
  for(int i=0; i<=100; i++)
  {
    Serial.println(EEPROM.read(i));
  }
  Admin adm;
  
  strcpy(adm.nombre, "Admin-13A");
  strcpy(adm.usuario, "A1973C");
  strcpy(adm.password, "AD13DA");
  
  Serial.println(sizeof(adm));
  EEPROM.put(0,adm);
  for(int i=0; i<=100; i++)
  {
    Serial.println(EEPROM.read(i));
  }
}

void writeUsers()
{
  User user0;
  
  strcpy(user0.usuario, "111111");
  strcpy(user0.password, "111111");
  strcpy(user0.nombre, "Daniel D");
  user0.pppequeno=1;
  user0.ppmediano=2;
  user0.ppgrande=3;
  user0.pdpequeno=4;
  user0.pdmediano=5;
  user0.pdgrande=6;

  User user1;
  
  strcpy(user1.usuario, "222222");
  strcpy(user1.password, "222222");
  strcpy(user1.nombre, "Edmy");
  user1.pppequeno=0;
  user1.ppmediano=0;
  user1.ppgrande=0;
  user1.pdpequeno=0;
  user1.pdmediano=0;
  user1.pdgrande=0;

  User user2;
  
  strcpy(user2.usuario, "333333");
  strcpy(user2.password, "333333");
  strcpy(user2.nombre, "Kevin");
  user2.pppequeno=6;
  user2.ppmediano=5;
  user2.ppgrande=4;
  user2.pdpequeno=8;
  user2.pdmediano=3;
  user2.pdgrande=10;

  User user3;
  
  strcpy(user3.usuario, "444444");
  strcpy(user3.password, "444444");
  strcpy(user3.nombre, "Erick R");
  user3.pppequeno=6;
  user3.ppmediano=50;
  user3.ppgrande=40;
  user3.pdpequeno=80;
  user3.pdmediano=30;
  user3.pdgrande=10;

  User user4;
  
  strcpy(user4.usuario, "555555");
  strcpy(user4.password, "555555");
  strcpy(user4.nombre, "Reginaldo D");
  user4.pppequeno=0;
  user4.ppmediano=0;
  user4.ppgrande=0;
  user4.pdpequeno=0;
  user4.pdmediano=0;
  user4.pdgrande=0;
  
  Serial.println(sizeof(user0));
  Serial.println(sizeof(user1));
  Serial.println(sizeof(user2));
  Serial.println(sizeof(user3));
  Serial.println(sizeof(user4));
  EEPROM.put(25,user0);
  EEPROM.put(71,user1);
  EEPROM.put(117,user2);
  EEPROM.put(163,user3);
  EEPROM.put(209,user4);

  User aa;
  EEPROM.get(25, aa);
  Serial.println(aa.usuario);
  Serial.println(aa.password);
  Serial.println(aa.nombre);
  Serial.println(aa.pppequeno);
  Serial.println(aa.ppmediano);
  Serial.println(aa.ppgrande);
  Serial.println(aa.pdpequeno);
  Serial.println(aa.pdmediano);
  Serial.println(aa.pdgrande);
  User aaa;
  EEPROM.get(209, aaa);
  Serial.println(aaa.usuario);
  Serial.println(aaa.password);
  Serial.println(aaa.nombre);
  Serial.println(aaa.pppequeno);
  Serial.println(aaa.ppmediano);
  Serial.println(aaa.ppgrande);
  Serial.println(aaa.pdpequeno);
  Serial.println(aaa.pdmediano);
  Serial.println(aaa.pdgrande);

  for(int i=0; i<=100; i++)
  {
    Serial.println(EEPROM.read(i));
  }
  int valIn = 5;
  EEPROM.put(4000, valIn);
}

void modificarNombreUsuario()
{
  while(esperandoCodigoUsuario==false)
  {
    lcd.setCursor(3,0);
    lcd.print("Obteniendo");
    lcd.setCursor(1,1);
    lcd.print("codigo usuario");
    if(Serial1.available()>0)
    {
      codigoUsuario=Serial1.readStringUntil('\n');
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Usuario obtenido");
      delay(3000);
      Serial1.print("1");
      esperandoCodigoUsuario=true;
      esperandoNombreUsuario=true;
      lcd.clear();
    }
  }

  for(int i=0; i<contadorUsuarios; i++)
  {
    User aux;
    EEPROM.get(25+(i*46),aux);
    if(codigoUsuario==aux.usuario)
    {
      encontrado2 = true;
      renameUser=aux;
      addressNuevoNombre=25+(i*46);
      break;
    }
  }
  if(encontrado2)
  {
    esperandoNombreUsuario=false;
//    encontrado=false;
  }
  else
  {
    lcd.setCursor(3,0);
    lcd.print("Usuario no");
    lcd.setCursor(3,1);
    lcd.print("Encontrado");
    delay(3000);
    esperandoCodigoUsuario=false;
    esperandoNombreUsuario=true;
    codigoUsuario="";
    lcd.clear();
  }

  while(esperandoNombreUsuario==false)
  {
    lcd.setCursor(3,0);
    lcd.print("Obteniendo");
    lcd.setCursor(5,1);
    lcd.print("nombre");
    if(Serial1.available()>0)
    {
      nuevoNombre=Serial1.readStringUntil('\n');
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Nombre obtenido");
      delay(3000);
      Serial1.print("1");
      esperandoNombreUsuario=true;
      lcd.clear();
    }
  }

  if(encontrado2)
  {
    char a1[17];
    nuevoNombre.toCharArray(a1,17);
    strcpy(renameUser.nombre, a1);
    EEPROM.put(addressNuevoNombre, renameUser); 
    esperandoCodigoUsuario=false;
    esperandoNombreUsuario=true;
    lcd.setCursor(4,0);
    lcd.print("Nombre");
    lcd.setCursor(2,1);
    lcd.print("actualizado");
    delay(3000);
    lcd.clear();
    encontrado2=false;
    estado=4; 
  }
   else
  {
    encontrado2=false;
    estado=4; 
  }
}

void modificarPasswordUsuario()
{
  while(esperandoCodigoUsuario==false)
  {
    lcd.setCursor(0,0);
    lcd.print("Ingrese Usuario");
    lcd.setCursor(4,1);
    lcd.print(codUser);
    TECLA = teclado.waitForKey();
    codUser+=TECLA;
    newContLetras++;
    if(TECLA=='*')
    {
      newContLetras=0;
      codUser="";
      lcd.clear();
    }
    if(newContLetras==6)
    {
      esperandoCodigoUsuario=true;
      esperandoNewPassUsuario=true;
      newContLetras=0;
      lcd.clear();
    }
  }

  for(int i=0; i<contadorUsuarios; i++)
  {
    User aux;
    EEPROM.get(25+(i*46),aux);
    if(codUser==aux.usuario)
    {
      encontrado2 = true;
      renameUser=aux;
      addressNuevoNombre=25+(i*46);
      break;
    }
  }
  if(encontrado2)
  {
    esperandoNewPassUsuario=false;
//    encontrado=false;
  }
  else
  {
    lcd.setCursor(3,0);
    lcd.print("Usuario no");
    lcd.setCursor(3,1);
    lcd.print("Encontrado");
    delay(3000);
    esperandoCodigoUsuario=false;
    esperandoNewPassUsuario=true;
    codigoUsuario="";
    codUser="";
    lcd.clear();
  }

  while(esperandoNewPassUsuario==false)
  {
    lcd.setCursor(0,0);
    lcd.print("Ingrese Password");
    TECLA = teclado.waitForKey();
    if(TECLA!='#')
    {
      changePassword+=TECLA;
    }
    lcd.setCursor(newContLetras+4,1);
    lcd.print("*");
    newContLetras++;
    if(TECLA=='#')
    {
      esperandoNewPassUsuario=true;
      newContLetras=0;
      lcd.clear();
    }
    if(newContLetras==8)
    {
      esperandoNewPassUsuario=true;
      newContLetras=0;
      lcd.clear();
    }
    else if(TECLA=='*')
    {
      contadorTeclas=0;
      changePassword="";
      lcd.clear();
    }
  }

//  ojo aca verificar el estado y que no se salte del whiel si no se encuentra usuario
  if(encontrado2)
  {
    char a1[9];
    changePassword.toCharArray(a1,9);
    strcpy(renameUser.password, a1);
    EEPROM.put(addressNuevoNombre, renameUser); 
    esperandoCodigoUsuario=false;
    esperandoNewPassUsuario=true;
    lcd.setCursor(3,0);
    lcd.print("Password");
    lcd.setCursor(2,1);
    lcd.print("actualizado");
    delay(3000);
    lcd.clear();
    encontrado2=false;
    changePassword="";
    estado=4; 
  }
  else
  {
    encontrado2=false;
    estado=4; 
  }
}

void sincronizarApp()
{
  lcd.setCursor(1,0);
  lcd.print("Sincronizando");
  lcd.setCursor(5,1);
  lcd.print("datos");
  for(int i=0; i<contadorUsuarios; i++)
  {
    User aux;
    EEPROM.get(25+(i*46),aux);
    if(strcmp(aux.usuario,"NONE")!=0)
    {
      Serial1.print(aux.usuario);
      delay(800);
      Serial1.print(aux.password);
      delay(800);
      Serial1.print(aux.nombre);
      delay(800);
      Serial1.print(aux.pppequeno);
      delay(800);
      Serial1.print(aux.ppmediano);
      delay(800);
      Serial1.print(aux.ppgrande);
      delay(800);
      Serial1.print(aux.pdpequeno);
      delay(800);
      Serial1.print(aux.pdmediano);
      delay(800);
      Serial1.print(aux.pdgrande);
      delay(800);
    }
  }
  lcd.clear();
}

void cerrarSesionAdmin()
{
  lcd.setCursor(2,0);
  lcd.print("Cerrando...");
  if(Serial1.available()>0)
  {
    msg=Serial1.readStringUntil('\n');
    if(msg=="cerrar")
    {
      estado=1;
      resetLogin1();
      lcd.clear();
    }
  }
}

void verReportes()
{
    for(int i=0; i<contadorUsuarios; i++)
    {
      User aux;
      EEPROM.get(25+(i*46),aux);
      if(strcmp(aux.usuario,"NONE")!=0)
      {
        lcd.setCursor(0,0);
        lcd.print(aux.nombre);
        lcd.setCursor(0,1);
        lcd.print("1. Enter");
        lcd.setCursor(13,1);
        lcd.print("D.");
        lcd.createChar(0, fder);
        lcd.setCursor(15,1);
        lcd.write(byte(0));
        TECLA = teclado.waitForKey();
        lcd.clear();
        if(TECLA=='1')
        {
          lcd.clear();
          userControl=aux;
          break;
        }
      }
      if(i==contadorUsuarios-1)
      {
        i=0;
      }
    }

    lcd.setCursor(4,0);
    lcd.print("Usuario");
    lcd.setCursor(5,0);
    lcd.print(userControl.usuario);
    delay(3000);
    lcd.clear();
    lcd.setCursor(1,0);
    lcd.print("P. Procesados");
    lcd.setCursor(0,1);
    lcd.print("P:");
    lcd.print(userControl.pppequeno);
    lcd.setCursor(6,1);
    lcd.print("M:");
    lcd.print(userControl.ppmediano);
    lcd.setCursor(12,1);
    lcd.print("G:");
    lcd.print(userControl.ppgrande);
    delay(3000);
    lcd.clear();
    lcd.setCursor(1,0);
    lcd.print("P. Despachados");
    lcd.setCursor(0,1);
    lcd.print("P:");
    lcd.print(userControl.pdpequeno);
    lcd.setCursor(6,1);
    lcd.print("M:");
    lcd.print(userControl.pdmediano);
    lcd.setCursor(12,1);
    lcd.print("G:");
    lcd.print(userControl.pdgrande);
    delay(3000);
    lcd.clear();
    estado=2;
}
