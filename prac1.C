#include <stdio.h>
#include <dos.h>

void gotoxy(int x, int y) {
    union REGS regs;
    regs.h.ah = 0x02; // Función para colocar el cursor
    regs.h.bh = 0x00; // Página de video 0
    regs.h.dh = y;    // Fila
    regs.h.dl = x;    // Columna
    int86(0x10, &regs, &regs); // Interrupción BIOS de vídeo
}

void setcursortype(int tipo_cursor) {
    union REGS regs;
    regs.h.ah = 0x01; // Función para fijar el tamaño del cursor
    switch (tipo_cursor) {
        case 0: // INVISIBLE
            regs.h.ch = 0x32; // Inicio del cursor (fuera de rango)
            regs.h.cl = 0x00; // Fin del cursor
            break;
        case 1: // NORMAL
            regs.h.ch = 0x06; // Inicio del cursor
            regs.h.cl = 0x07; // Fin del cursor
            break;
	case 2: // GRUESO
            regs.h.ch = 0x00; // Inicio del cursor
            regs.h.cl = 0x0B; // Fin del cursor
            break;
    }
    int86(0x10, &regs, &regs); // Interrupción BIOS de vídeo
}

void setvideomode(unsigned char mode) {
    union REGS regs;
    regs.h.ah = 0x00; // Función para establecer el modo de vídeo
    regs.h.al = mode; // Modo de vídeo
    int86(0x10, &regs, &regs); // Interrupción BIOS de vídeo
}

unsigned char getvideomode() {
    union REGS regs;
    regs.h.ah = 0x0F; // Función para obtener el modo de vídeo actual
    int86(0x10, &regs, &regs); // Interrupción BIOS de vídeo
    return regs.h.al; // Retorna el modo de vídeo actual
}

// Esta función requiere mantener un registro del color actual del texto para usarlo al escribir caracteres.
static unsigned char currentTextColor = 0x07; // Gris sobre negro por defecto

void textcolor(unsigned char color) {
    currentTextColor = color; // Actualiza el color actual del texto
    printf("\033[3%dm]", color);
}

// Esta función modificaría la manera en que se escriben los caracteres en pantalla para incluir el color de fondo.
static unsigned char currentBackgroundColor = 0x00; // Fondo negro por defecto

void textbackground(unsigned char color) {
    currentBackgroundColor = color << 4; // Los 4 bits más altos son para el color de fondo
        printf("\033[4%dm]", color);

}

void clrscr() {
	setvideomode(0x04); //MODO GRÁFICO
	setvideomode(0x02); //MODO TEXTO
}

void cputchar(char c) {
    union REGS regs, outregs;
    regs.h.ah = 0x09; // Función para escribir un carácter en pantalla
    regs.h.al = c;    // Carácter a escribir
    regs.h.bl = currentTextColor | currentBackgroundColor << 4; // Color de texto y fondo
    regs.h.bh = 0x00; // Página de video 0
    regs.x.cx = 1;    // Número de veces que se repite el carácter
    int86(0x10, &regs, &outregs); // Interrupción BIOS de vídeo
    return;
}

int getche() {
    union REGS inregs, outregs;
    inregs.h.ah = 0x00; // Función para leer un carácter con eco
    int86(0x16, &inregs, &outregs); // Interrupción BIOS del teclado

    cputchar(outregs.h.al); // Muestra el carácter en pantalla
    return outregs.h.al; // Retorna el carácter leído
}

void mi_pausa(){
   union REGS inregs, outregs;
	 inregs.h.ah = 8;
	 int86(0x21, &inregs, &outregs);
}

/*OPCIONAL 1*/

// Definiciones para el modo gráfico CGA
#define CGA_MODE 0x04
#define CGA_MEMORY_BASE 0xB800

void setCGAMode() {
    union REGS regs, out;
    regs.h.ah = 0x00; // Función para establecer el modo de video
    regs.h.al = CGA_MODE; // Modo gráfico CGA
    int86(0x10, &regs, &out); // Llamada a la interrupción de video BIOS
    return;

}

void dibujarRecuadro(int x1, int y1, int x2, int y2, int colort, int colorf){
	int i;
	int j;
	textcolor(colort);
	textbackground(colorf);
	for(i=x1; i<=x2;i++){
		gotoxy(i,y1);
		cputchar('O');
		gotoxy(i,y2);
		cputchar('O');
	}

	for(i=y1; i<=y2;i++){
		gotoxy(x1,i);
		cputchar('O');
		gotoxy(x2,i);
		cputchar('O');
	}

	
	for(i=(x1+1);i<x2;i++){
		for(j=(y1+1);j<y2;j++){
			gotoxy(i,j);
			cputchar('M');
		}
	}
	
}

void pixel(int x, int y, unsigned char C){
	union REGS inregs, outregs;
	inregs.x.cx = x;
	inregs.x.dx = y;
	inregs.h.al = C;
	inregs.h.ah = 0x0C;
	int86 (0x10, &inregs, &outregs);
}

void dibujo(){

    // DIBUJO DE UN SOL

    int i, j, k, center_x, center_y, radius, ray_length;
    setvideomode(0x04);

    // CÍRCULO DEL SOL
    center_x = 50;
    center_y = 50;
    radius = 20;

    for (i = center_x - radius; i <= center_x + radius; i++) {
        for (j = center_y - radius; j <= center_y + radius; j++) {
            if ((i - center_x) * (i - center_x) + (j - center_y) * (j - center_y) <= radius * radius) {
                pixel(i, j, 14); // Color amarillo
            }
        }
    }

    // RAYOS DEL SOL
    ray_length = 40;

    // Horizontal y vertical
    for (i = center_x - ray_length; i <= center_x + ray_length; i++) {
        pixel(i, center_y, 14); // Horizontal
        pixel(center_x, i, 14); // Vertical
    }

    // Diagonal
    for (i = 1; i <= ray_length; i++) {
        pixel(center_x - i, center_y - i, 14); // Superior izquierda
        pixel(center_x + i, center_y - i, 14); // Superior derecha
        pixel(center_x - i, center_y + i, 14); // Inferior izquierda
        pixel(center_x + i, center_y + i, 14); // Inferior derecha
    }
}

int main() {
    char c;
    unsigned char mode;
    int x;
    clrscr();
    printf("\nCOMENZANDO DEMOSTRACIONES\n");

    //Cambiar cursor
    printf("Demostracion de gotoxy(): Moviendo el cursor a diferentes posiciones...\n");
    printf("Cursor en (10, 10)\n");
    gotoxy(10, 10);
    printf("zzzzzzzzz\n");
    printf("Cursor en (20, 10)\n");
    gotoxy(20, 10);
    printf("*********\n");
    mi_pausa();
    clrscr();
    gotoxy(0, 0);

    //SetCursor
    printf("Demostracion de setcursortype(): Cambiando el aspecto del cursor...\n");
    printf("Cursor INVISIBLE. (Presiona cualquier tecla para continuar)\n");
    setcursortype(0); // Cursor INVISIBLE
    getche();

    printf("\nCursor NORMAL. (Presiona cualquier tecla para continuar)\n");
    setcursortype(1); // Cursor NORMAL
    getche();

    printf("\nCursor GRUESO. (Presiona cualquier tecla para continuar)\n");
    setcursortype(2); // Cursor GRUESO
    getche();
    clrscr();

    // Establecer el modo de video a texto para demostrar las funciones
    setvideomode(1); // Modo de texto 80x25
    printf("Estableciendo modo de video a texto AUMENTADO...\n");
    mi_pausa();
    setvideomode(3); // Modo de texto 80x25
    printf("\nEstableciendo modo de video a texto NORMAL...\n");

    //GetVideoMode
    printf("Demostración de getvideomode(): Obteniendo el modo de video actual...\n");
    mode = getvideomode();
    printf("El modo de video actual es: 0x%X\n", mode);
    printf("(Presiona cualquier tecla para continuar)\n");
    getche();
    clrscr();

    //TextColor y TextBackground
    printf("Demostracion de textcolor() y textbackground(): Cambiando colores de texto y fondo.\n");
    textcolor(4); // Rojo
    textbackground(7); // Gris claro
    printf("Texto en azul con fondo gris claro. (Presiona cualquier tecla para continuar)\n");
    getche();
    clrscr();

    // Restaurar colores y cursor
    textcolor(7); // Gris claro
    textbackground(0); // Negro
    setcursortype(1); // Cursor NORMAL

    //CPUTCHAR
    // FONDO AZUL LETRA BLANCA
    printf("Demostracion de cputchar(): FONDO ROJO LETRA AZUL...\n");
    currentBackgroundColor=4;
    currentTextColor=1;
    cputchar('h');
    printf("\n");
    cputchar('o');
    printf("\n");
    cputchar('l');
    printf("\n");
    cputchar('a');
    printf("\n");

    // FONDO BLANCO LETRA AZUL
    printf("Demostracion de cputchar(): FONDO AZUL LETRA ROJA...\n");
    currentBackgroundColor=1;
    currentTextColor=4;
    cputchar('h');
    printf("\n");
    cputchar('o');
    printf("\n");
    cputchar('l');
    printf("\n");
    cputchar('a');
    printf("\n");

    //GetChe
    printf("Demostración de getche(): Obtiene un carácter del teclado y lo muestra en pantalla.\n");
    printf("Escribe algo: ");
    c = getche(); // Se mostrará lo que escribas
    printf("\nEscribiste: %c\n", c);
    mi_pausa();

    clrscr();
    printf("\nOpcional1\n");
    dibujarRecuadro(10,10,20,20,2,3);
    mi_pausa();

    clrscr();
    printf("\nOpcional2\n");
    setCGAMode(4);
    pixel(10,40,0);
    pixel(10,50,1);
    pixel(15,60,2);
    pixel(20,70,3);

    for(x=0; x<100; x++){
        pixel(x, x, x%4 );
    }

    mi_pausa();
    clrscr();
    printf("\n MODO TEXTO \n");

    modovideo(3); //texto
    mi_pausa();

    printf("\nOpcional3\n");
    dibujo();
    mi_pausa();

    clrscr();

    // Restaurar colores y cursor
    textcolor(7); // Gris claro
    textbackground(0); // Negro
    setcursortype(1); // Cursor NORMAL

    return 0;
}