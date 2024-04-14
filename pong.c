#include <ncurses.h>
#include <unistd.h>

#define DELAY 18000

void show_welcome_screen() {
    clear();  // Limpia la pantalla
    printw("Bienvenido al juego Pong!\n\n");
    printw("Este juego fue creado por Kais Bouadi Molina y Javier Martin Alcalde\n\n");
    printw("Controles:\n");
    printw("Jugador de la izquierda: 'W' para subir, 'S' para bajar\n");
    printw("Jugador de la derecha: Flecha arriba para subir, Flecha abajo para bajar\n\n");
    printw("Presiona cualquier tecla para comenzar...");

    refresh();  // Muestra todo en la pantalla
    getch();    // Espera a que el usuario presione una tecla para continuar
}

void show_end_screen(int score_left, int score_right) {
    clear();  // Limpia la pantalla

    // Determina y muestra el ganador
    if (score_left > score_right) {
        printw("¡Felicidades Jugador de la Izquierda!\n");
    } else {
        printw("¡Felicidades Jugador de la Derecha!\n");
    }
    printw("Marcador final: Izquierda %d - Derecha %d\n\n", score_left, score_right);

    // Opciones para el jugador
    printw("Presiona 'r' para jugar de nuevo o 'q' para salir...");

    refresh();  // Muestra todo en la pantalla
    char ch;
    do {
        ch = getch();  // Espera entrada del usuario
    } while (ch != 'r' && ch != 'q');  // Repite hasta que se presione 'r' o 'q'

    if (ch == 'r') {
        clear();  // Si el jugador elige reiniciar, limpia la pantalla y reinicia el juego
    } else {
        endwin();  // Sale del modo ncurses
        exit(0);   // Termina el programa
    }
}

int main(int argc, char *argv[]) {
    int x = 0, y = 0;
    int max_y = 0, max_x = 0;
    int next_x = 0, next_y = 0;
    int direction_x = 1, direction_y = 1;
    int paddle_left_y, paddle_right_y;
    int paddle_size = 3;
    int score_left = 0, score_right = 0;

    initscr();              // Start curses mode
    noecho();               // Don't echo any keypresses
    curs_set(FALSE);        // Don't display a cursor

	show_welcome_screen();

    keypad(stdscr, TRUE);   // Enable keyboard input
    nodelay(stdscr, TRUE);  // Make getch non-blocking
    getmaxyx(stdscr, max_y, max_x); // Get screen size

    // Initialize positions
    x = max_x / 2;
    y = max_y / 2;
    paddle_left_y = max_y / 2 - paddle_size / 2;
    paddle_right_y = max_y / 2 - paddle_size / 2;

    while (score_left < 5 && score_right < 5) {
        clear(); // Clear the screen of all previously-printed characters

        // Draw paddles
        for (int i = 0; i < paddle_size; i++) {
            mvprintw(paddle_left_y + i, 1, "|");
            mvprintw(paddle_right_y + i, max_x - 2, "|");
        }

        // Draw ball
        mvprintw(y, x, "o");

        // Move ball
        next_x = x + direction_x;
        next_y = y + direction_y;

        // Check for collision with borders
        if (next_y >= max_y || next_y < 0) {
            direction_y *= -1;
        } else {
            y += direction_y;
        }

        if (next_x >= max_x) {
            score_left++;  // Left scores
            x = max_x / 2;
            y = max_y / 2;
            direction_x = -1;
        } else if (next_x < 0) {
            score_right++;  // Right scores
            x = max_x / 2;
            y = max_y / 2;
            direction_x = 1;
        } else {
            x += direction_x;
        }

        // Check for collision with paddles
        if (x == 2 && y >= paddle_left_y && y <= paddle_left_y + paddle_size) {
            direction_x = 1;
        }
        if (x == max_x - 3 && y >= paddle_right_y && y <= paddle_right_y + paddle_size) {
            direction_x = -1;
        }

        // Update paddle positions based on key press
 
        int ch = getch();
	switch(ch) {
    		case KEY_UP:
        		if (paddle_right_y > 0) paddle_right_y--;
        		break;
    		case KEY_DOWN:
        		if (paddle_right_y < max_y - paddle_size) paddle_right_y++;
        		break;
    		case 'w':
    		case 'W':
        		if (paddle_left_y > 0) paddle_left_y--;
        		break;
    		case 's':
    		case 'S':
        		if (paddle_left_y < max_y - paddle_size) paddle_left_y++;
       			break;
	}

        // Display score
        mvprintw(0, max_x / 2 - 5, "Score %d - %d", score_left, score_right);
	
	

        refresh(); // Update the screen
        usleep(DELAY); // Short delay
	
	if(score_left == 5 || score_right==5){
		show_end_screen(score_left, score_right);
		score_left=0;
		score_right=0;
	}
    }
     
 
    endwin(); // End curses mode
    return 0;
}
