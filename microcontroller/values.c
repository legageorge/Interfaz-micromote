#include "values.h"

/* Declaración de Constantes */
#define CSneg PIN_C2
#define PIN_Tap PIN_A0
#define user_delay 500
#define sensitivity_z 256
#define umbral_reposo 60


/* Declaración de Variables globales */
int datos[6];
int jdevid=0, jbw_rate=0, jpower_ctl=0, jdata_format=0, jacttap_status=0, jint_source=0;
signed int16 eje_x=0, eje_y=0, eje_z=0, eje_x_real=0, eje_x_abs=0, eje_y_abs=0;
signed int16 lect_x[10], lect_y[10], lect_z[10];
signed int16 suma_x=0, suma_y=0, suma_z=0;
signed int16 media_inic_x=0, media_inic_y=0, media_inic_z=0, z_cerog=0;
signed int ajuste_x=0, ajuste_y=0, ajuste_z=0;
int cuadrante=0, flag=0, flag_tap=0, numtaps=0;
char accion[15]="______________"; 


/* Declaración de Funciones */

//Configuración PIC
void config_PIC(void)
{
   set_tris_a(0xFF);                                                    //pines puerto A como entradas
   set_tris_c(0x93);                                                    //pines C6,C5,C3,C2 como salidas, resto como entradas
   output_high(CSneg);
   setup_spi(SPI_MASTER|SPI_H_TO_L|SPI_CLK_DIV_4|SPI_SAMPLE_AT_END);    //config spi:master,flanco de bajada,veloc:1Mhz,muestra al final
}

//Lectura SPI (con multipleBytes)
void readADX(int address, int numB, int *datos)           //parametros:direccion registro adx,numero bytes(minimo 2), puntero al array datos
{
   int highbyte,i;
   highbyte=0x80|address;                    //pone bit lectura/escritura a 1 para leer y concatena con direccion de registro
   if (numB>1) {
               highbyte=0x40|highbyte;       //si se lee mas de 1 byte seguido, se pone a 1 el bit multiplebyte
               }
   output_low(CSneg);                        //habilita comunicacion
   spi_write(highbyte);                      //envia byte alto
   for (i=0;i<numB;i++)                      
               {
               datos[i]=spi_read(0);         //lee byte a byte y almacena en el array datos
               }
   output_high(CSneg);                       //deshabilita comunicacion
}

//Escritura SPI
void writeADX(int address, int valor)        //parametros:direccion registro adx,valor a escribir
{
   output_low(CSneg);                        //habilita comunicacion
   spi_write(address);                       //envia byte alto:bit escritura 0,bit multiplebyte 0,direccion registro 
   spi_write(valor);                         //envia valor a escribir
   output_high(CSneg);                       //deshabilita comunicacion
}

//Configuración ADXL345
void config_ADX(void)
{
   writeADX(THRESH_TAP,0x30);                //Umbral tap 3 g
   writeADX(DUR,0x30);                       //Duracion max tap 30 ms
   writeADX(LATENT,0x18);                    //Espera desde tap hasta window2tap 30 ms
   writeADX(WINDOW,0x40);                    //Duracion window2tap 80 ms
   writeADX(TAP_AXES,0x01);                  //Habilitado solo tap eje z
   writeADX(INT_MAP,0x20);                   //Tap en INT1, 2Tap en INT2
   writeADX(BW_RATE,0x0D);                   //Data rate 800 Hz (pues spi_rate=1Mhz)
   writeADX(DATA_FORMAT,0x00);               //4 wire SPI, 10 bits, right justified, +/- 2g
   writeADX(INT_ENABLE,0x00);                //Deshabilitadas todas las interrupciones
   writeADX(POWER_CTL,0x08);                 //Modo Measure
}

//Lectura Ejes
void lectura_ejes(void)
{
   readADX(DATAX0,6,datos);                  //lee los registros de los ejes x,y,z byte a byte
   eje_x=((signed int16)datos[1])<<8;        //reconstruccion de los bytes para formar la palabra de 10 bits de cada eje
   eje_x=eje_x|((signed int16)datos[0]);
   eje_y=((signed int16)datos[3])<<8;
   eje_y=eje_y|((signed int16)datos[2]);
   eje_z=((signed int16)datos[5])<<8;
   eje_z=eje_z|((signed int16)datos[4]);
}

//Lectura Inicial
void lectura_inic(void)
{   
   int i,j,k,l,m;
         
   for (j=0;j<10;j++)                  //inicializacion array x
         {lect_x[j]=0;}
   for (k=0;k<10;k++)                  //inicializacion array y
         {lect_y[k]=0;}
   for (l=0;l<10;l++)                  //inicializacion array z
         {lect_z[l]=0;}
   
   for (i=0;i<10;i++)                  //toma 10 muestras de la posicion inicial
         {
         delay_ms(user_delay);
         lectura_ejes();
         lect_x[i]=eje_x;
         lect_y[i]=eje_y;
         lect_z[i]=eje_z;
         }
   
   for (m=0;m<10;m++)                  //media aritmetica de cada eje                       
         {
         suma_x+=lect_x[m];
         suma_y+=lect_y[m];
         suma_z+=lect_z[m];
         }
   
   media_inic_x=(suma_x/10);
   media_inic_y=(suma_y/10);
   media_inic_z=(suma_z/10);
}

//Ajuste Offsets
void ajuste_offs(void)
{
   ajuste_x=(signed int)(-(floor(media_inic_x/4)));                     //ajuste de offset segun datasheet , Xoff = -Round(medida_x_cerog/4)
   writeADX(OFSX,ajuste_x);
   
   ajuste_y=(signed int)(-(floor(media_inic_y/4)));
   writeADX(OFSY,ajuste_y);
   
   z_cerog=media_inic_z-(signed int16)sensitivity_z;                    //segun datasheet , z_cerog=medida_z_unog - sensitivity_z
   ajuste_z=(signed int)(-(floor(z_cerog/4)));                          //                  Zoff = -Round(z_cerog/4)
   writeADX(OFSZ,ajuste_z);                                             //pues en posicion de reposo gravedad hace eje z 1g
}

//Lectura Actual
void lectura_actual(void)
{
   lectura_ejes();
   eje_x_real=(-eje_x);                //se cambia el signo a eje_x para corresponder con la realidad fisica
   
   /*printeo de valores de cada eje*/
   printf("%Ld  %Ld  %Ld)",eje_x,eje_y,eje_z);
   
   
   if (eje_x_real>=0)                  //determinacion de cuadrante
      {
      if (eje_y>=0)
         cuadrante=1;
      else
         cuadrante=4;
      }
   else
      {
      if (eje_y>=0)
         cuadrante=2;
      else
         cuadrante=3;
      }
   
   eje_x_abs=abs(eje_x);               //se toma el valor absoluto para trabajar mas facilmente en cada cuadrante
   eje_y_abs=abs(eje_y);
   
   
   if (eje_x_abs<umbral_reposo && eje_y_abs<umbral_reposo)                          //condicion reposo
   {
      strcpy(accion,"REPOSO________");
   }
      
   else                                                                             //condicion para movimiento eje x , eje y
      {
      switch(cuadrante)
         {
         case 1:
            {
            if (eje_y_abs>(2*eje_x_abs))
               {
               strcpy(accion,"UP____________");
               }
            else if (eje_y_abs>(0.5*eje_x_abs))
               {
               strcpy(accion,"UP_RIGHT______");
               }
            else
               {
               strcpy(accion,"RIGHT_________");
               }
            break;
            }
         
         case 2:
            {
            if (eje_y_abs>(2*eje_x_abs))
               {
               strcpy(accion,"UP____________");
               }
            else if (eje_y_abs>(0.5*eje_x_abs))
               {
               strcpy(accion,"UP_LEFT_______");
               }
            else
               {
               strcpy(accion,"LEFT__________");
               }
            break;                      
            }
         
         case 3:
            {
            if (eje_y_abs>(2*eje_x_abs))
               {
               strcpy(accion,"DOWN__________");
               }
            else if (eje_y_abs>(0.5*eje_x_abs))
               {
               strcpy(accion,"DOWN_LEFT_____");
               }
            else
               {
               strcpy(accion,"LEFT__________");
               }
            break;                      
            }
         
         case 4:
            {
            if (eje_y_abs>(2*eje_x_abs))
               {
               strcpy(accion,"DOWN__________");
               }
            else if (eje_y_abs>(0.5*eje_x_abs))
               {
               strcpy(accion,"DOWN_RIGHT____");
               }
            else
               {
               strcpy(accion,"RIGHT_________");
               }
            break;                      
            }
         }
      }
   
   
}

//Single Tap
void accion_tap(void)
   {
   flag_tap=1;
   numtaps+=1;
   readADX(ACT_TAP_STATUS,1,datos);          //bit0=1 eje z fuente del tap
   jacttap_status=datos[0];
   readADX(INT_SOURCE,1,datos);              //bit6=1 single tap ; borra la interrupcion
   jint_source=datos[0];
   strcpy(accion,"TAP___________");
   }





/* Programa Principal */

void main()
{
   int j;
   for (j=0;j<6;j++)                      //inicializacion array datos
         {datos[j]=0;}
   
   config_PIC();                          //configuracion PIC
   config_ADX();                          //configuracion ADXL
   
   readADX(DEVID,1,datos);                //lectura registros principales adx
   jdevid=datos[0];
   readADX(BW_RATE,2,datos);
   jbw_rate=datos[0];
   jpower_ctl=datos[1];
   readADX(DATA_FORMAT,1,datos);
   jdata_format=datos[0];
   
   lectura_inic();                        //lectura inicial de ejes
   ajuste_offs();                         //ajuste de offsets
   
   writeADX(INT_ENABLE,0x40);             //Habilitacion de Tap
   
   flag=1;
   
   while(TRUE)
   {
      delay_ms(user_delay);
            
      if (input(PIN_Tap))
         {
         accion_tap();                    //single tap
         }
      else
         {
         lectura_actual();                //lectura actual e interpretacion de datos
         }
      flag=2;
   }
}



