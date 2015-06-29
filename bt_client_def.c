#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>


void main(int argc, char **argv)
{
   struct sockaddr_rc addr = { 0 };
   char buffer[18] =  {0};
   char aux;
   int sock, status, data=0;
   char direcc[18] = {"00:06:66:4B:34:E2"};

   printf("\n--- Programa Cliente Bluetooth ---\n\n");
   printf("La direccion a conectar es %s\n",direcc);

   // abrir socket
   sock = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

   if (sock == -1)
      {
      printf("Error_socket\n\n");
      exit(0);
      }
   else if (sock!=-1)
      {
      printf("Socket abierto\n");
      }

    // enlazar socket al canal 1
    addr.rc_family = AF_BLUETOOTH;
    addr.rc_channel = (uint8_t) 1;
    str2ba(direcc, &addr.rc_bdaddr);
   
    // conexion
    status = connect(sock, (struct sockaddr *)&addr, sizeof(addr));

   if (status == -1)
      {
      printf("Error_connect\n\n");
      exit(0);
      }
   else if (status != -1)
      {
      printf("Conexion efectuada\n\n");
      }

    // lectura de datos
   while(1)
   {
      printf("[OK] (");
      
      do      
      {
         data = recv(sock,buffer,sizeof(buffer),0);
     
         if (data > 0)
            {
            aux=buffer[data-1];
            printf("%s",buffer);
            memset(buffer, 0, sizeof(buffer));
            }
         else if (data == 0)
            {
            printf("Socket cerrado\n");      
            }
         else
            {
            printf("Error_recv\n");
            }

      } while (aux != ')');
   
      printf("\n\n");   
               
   }

    // cerrar conexion
    close(sock);
    
}
