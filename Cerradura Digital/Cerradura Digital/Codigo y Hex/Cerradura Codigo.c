
//invluye el pic 
#include<18f4550.h>

//configura el reloj interno del pic
#use delay(clock=10Mhz)

// fusibles
#fuses hs,nowdt

// inclucion del lcd para el teclado matricial
#include <lcd.c>
#include <kbd1.c>


int X=1;
int i,Y;
char K;

//el vector contiene la contraseña que usaremos para nuestra cerradura
//siempre se antepone un 0 antes de la contraseña
char C[]={'0','3','4','5','6'};
char D[]={"00000"};

void main()
{
	//configuraciones de inicio
	//lds  van al puerto A
   set_tris_A(0x00);
   
   //lcd
   lcd_init();
   
   //teclado matricial
   kbd_init();
   port_b_pullups(true);
   
   do
   {
	   //valor que toma el teclado matricial
      K=kbd_getc();
	  //mensaje del lcd ubicado en la fila uno columna uno
      lcd_gotoxy(1,1);	  
      printf(lcd_putc,"Ingresa la Clave");
	  //led del puero A que indica que el puerto esta cerrado
      output_A(0x02);
      
      // confirmacion de clave
	  //se ejecuta cuando  se oprima boton diferente a # y *
	  // k almacena los numeros clickeados
      if((K!=0)&&(K!='#')&&(K!='*'))
      {
		//imprime el valor de k
         lcd_gotoxy(X,2);
         printf(lcd_putc,"%c",K);
		 //guarda el valor de k en el vector 
         D[X]=K;
         X++;
      }
	  
      // para la confirmación de clave
      if((K!=0)&&(K=='*'))
      {		
         i=0;
         for(i=0;i<5;i++)
         {
			 //garantizamos que la contraseña sea correcta, comparando los vectores
            if(D[i]!=C[i])
            {
				//si son diferentes arroja la alerta
               break;
            }
         }
         if(i==5)
         {
            do
            {
               K=kbd_getc();
               lcd_gotoxy(1,2);
               printf(lcd_putc,"CLAVE CORRECTA");
			   //arroja la alerta de clave correcta
               output_A(0x01);
			   //y controla el numero de intentos
               Y=0;
            }while(K!='*');
            X=1;
			//regresa al vector clave a valores iniciales 
            D="00000";
            i=0;
			//limpia pantalla
            printf(lcd_putc,"\f");
			//espera para pulsar el  boton 
            delay_ms(50);
         }
       else
       {
         lcd_gotoxy(1,2);
         printf(lcd_putc,"CLAVE INCORRECTA");
         delay_ms(600);		 
         X=1;
		 //cuando y sea =3 se bloquea
         Y++;
		 // se reinica el vector que almacena la contraaseña para comparar de nuevo
         D="00000";
		 //indide de numeros de la contraseña
         i=0;
		 //limpia la pantalla del cld 
         printf(lcd_putc,"\f");
         
       }
      }
       //cambiar contraseña
       if((K!=0)&&(K=='#'))
       {
		  // se limpia x uno a uno para eliminar los numeros
         X--;
		 //se limpia el vector
         D[X]='0';
		 //elimina el numero del lcd
         lcd_gotoxy(X,2);
		 //se sustituye el numero por un espacio para indicar que se borro
         printf(lcd_putc," ");
         
       }
       // para evitar que se borren numero cuando ya no hay numeros
       if(X==0)
       {
         X=1;
       }
       //si y =3 bloquea el sistema
       if(Y==3)
       {
         do
         {
            printf(lcd_putc,"\fSISTEMA BLOQUEADO");
            delay_ms(10);
         }while(true);
       }
      
   }while(true);
}
