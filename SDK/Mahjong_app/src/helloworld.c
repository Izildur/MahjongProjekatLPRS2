/*
 * Copyright (c) 2009-2012 Xilinx, Inc.  All rights reserved.
 *
 * Xilinx, Inc.
 * XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS" AS A
 * COURTESY TO YOU.  BY PROVIDING THIS DESIGN, CODE, OR INFORMATION AS
 * ONE POSSIBLE   IMPLEMENTATION OF THIS FEATURE, APPLICATION OR
 * STANDARD, XILINX IS MAKING NO REPRESENTATION THAT THIS IMPLEMENTATION
 * IS FREE FROM ANY CLAIMS OF INFRINGEMENT, AND YOU ARE RESPONSIBLE
 * FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE FOR YOUR IMPLEMENTATION.
 * XILINX EXPRESSLY DISCLAIMS ANY WARRANTY WHATSOEVER WITH RESPECT TO
 * THE ADEQUACY OF THE IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO
 * ANY WARRANTIES OR REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE
 * FROM CLAIMS OF INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 */

/*
 * helloworld.c: simple test application
 *
 * This application configures UART 16550 to baud rate 9600.
 * PS7 UART (Zynq) is not initialized by this application, since
 * bootrom/bsp configures it to baud rate 115200
 *
 * ------------------------------------------------
 * | UART TYPE   BAUD RATE                        |
 * ------------------------------------------------
 *   uartns550   9600
 *   uartlite    Configurable only in HW design
 *   ps7_uart    115200 (configured by bootrom/bsp)
 *
 *
 *
 */

#include <stdio.h>
#include "platform.h"
#include "xparameters.h"
#include "xio.h"
#include "xil_exception.h"
#include "vga_periph_mem.h"
#include "mahjong_sprites.h"
#include <stdlib.h>     /* srand, rand */
#include <time.h>
#define SIZE 9
#define UP 0b01000000
#define DOWN 0b00000100
#define LEFT 0b00100000
#define RIGHT 0b00001000
#define CENTER 0b00010000
#define NUMOFSPRITES 37

//BEG---unpened field
#define BEG '@'

int q, x, y, ii, oi, R, G, B, RGB, kolona, red, RGBgray;
int cX = 0, cY = 0, cL = 0;

int randomMap[3][8][10];

typedef enum {
	IDLE,
	LEFT_PRESSED,
	RIGHT_PRESSED,
	CENTER_PRESSED,
	DOWN_PRESSED,
	UP_PRESSED
} state_t;

//function that generates random game map
void makeTable() {
	int numOfSprites, row, column, level, i, j, k, r;

	//popunjava matricu sa -1
	for (i = 0; i < 3; i++) {
		for (j = 0; j < 8; j++) {
			for (k = 0; k < 10; k++)
			randomMap[i][j][k] = -1;
		}
	}

	//postavlja random karata
	for(r = 0; r < 4; r++){
		numOfSprites = NUMOFSPRITES;
		while (numOfSprites >= 0) {
			level = rand() % 3;
			if(level == 0){
				row = rand() % 8;
				column = rand() % 10;
			} else if(level == 1){
				row = rand() % 6;
				column = rand() % 8;
				row += 1;
				column += 1;
			}else{
				row = rand() % 4;
				column = rand() % 6;
				row += 2;
				column += 2;
			}
			if (randomMap[level][row][column] == -1) {
				randomMap[level][row][column] = numOfSprites;
				numOfSprites--;
			}
		}
	}

}

//extracting pixel data from a picture for printing out on the display
void drawMap(int in_x, int in_y, int out_x, int out_y, int width, int height) {
	int ox, oy, oi, iy, ix, ii;
	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			ox = out_x + x;
			oy = out_y + y;
			oi = oy * 320 + ox;
			ix = in_x + x;
			iy = in_y + y;
			ii = iy * mahjong_sprites.width + ix;
			R = mahjong_sprites.pixel_data[ii
					* mahjong_sprites.bytes_per_pixel] >> 5;
			G = mahjong_sprites.pixel_data[ii
					* mahjong_sprites.bytes_per_pixel + 1] >> 5;
			B = mahjong_sprites.pixel_data[ii
					* mahjong_sprites.bytes_per_pixel + 2] >> 5;
			R <<= 6;
			G <<= 3;
			RGB = R | G | B;

			VGA_PERIPH_MEM_mWriteMemory(
					XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR + GRAPHICS_MEM_OFF
							+ oi * 4, RGB);
		}
	}

}

//drawing cursor for indicating position
void drawingCursor(int startX, int startY, int endX, int endY) {

	for (x = startX; x < endX; x++) {
		for (y = startY; y < startY + 2; y++) {
			q = y * 320 + x;
			VGA_PERIPH_MEM_mWriteMemory(
					XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR + GRAPHICS_MEM_OFF
							+ q * 4, 0x000055);
		}
	}

	for (x = startX; x < endX; x++) {
		for (y = endY - 2; y < endY; y++) {
			q = y * 320 + x;
			VGA_PERIPH_MEM_mWriteMemory(
					XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR + GRAPHICS_MEM_OFF
							+ q * 4, 0x000055);
		}
	}

	for (x = startX; x < startX + 2; x++) {
		for (y = startY; y < endY; y++) {
			q = y * 320 + x;
			VGA_PERIPH_MEM_mWriteMemory(
					XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR + GRAPHICS_MEM_OFF
							+ q * 4, 0x000055);
		}
	}

	for (x = endX - 2; x < endX; x++) {
		for (y = startY; y < endY; y++) {
			q = y * 320 + x;
			VGA_PERIPH_MEM_mWriteMemory(
					XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR + GRAPHICS_MEM_OFF
							+ q * 4, 0x000055);
		}
	}

}

void drawBlue(int startX, int endX, int startY, int endY){

	for (x = startX; x < endX; x++) {
		for (y = startY; y < endY; y++) {
			q = y * 320 + x;
			VGA_PERIPH_MEM_mWriteMemory(
			XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR + GRAPHICS_MEM_OFF
			+ q * 4, 0x000000);
		}
	}

}

int calculate_level(int i, int j, state_t state){

	int sledeci_nivo = 0;

	switch(state){

		case DOWN_PRESSED:
			if(randomMap[0][i+1][j] != -1){
				sledeci_nivo = 0;
			}
			if(randomMap[1][i+1][j] != -1){
				sledeci_nivo = 1;
			}
			if(randomMap[2][i+1][j] != -1){
				sledeci_nivo = 2;
			}
		break;

		case UP_PRESSED:
			if(randomMap[0][i-1][j] != -1){
				sledeci_nivo = 0;
			}
			if(randomMap[1][i-1][j] != -1){
				sledeci_nivo = 1;
			}
			if(randomMap[2][i-1][j] != -1){
				sledeci_nivo = 2;
			}
		break;

		case RIGHT_PRESSED:
			if(randomMap[0][i][j+1] != -1){
				sledeci_nivo = 0;
			}
			if(randomMap[1][i][j+1] != -1){
				sledeci_nivo = 1;
			}
			if(randomMap[2][i][j+1] != -1){
				sledeci_nivo = 2;
			}
		break;

		case LEFT_PRESSED:
			if(randomMap[0][i][j-1] != -1){
				sledeci_nivo = 0;
			}
			if(randomMap[1][i][j-1] != -1){
				sledeci_nivo = 1;
			}
			if(randomMap[2][i][j-1] != -1){
				sledeci_nivo = 2;
			}
		break;

		default:
			if(randomMap[0][i][j] != -1){
				sledeci_nivo = 0;
			}
			if(randomMap[1][i][j] != -1){
				sledeci_nivo = 1;
			}
			if(randomMap[2][i][j] != -1){
				sledeci_nivo = 2;
			}
			break;
	}

	return sledeci_nivo;

}

void calculate_coordinates(state_t state){

	switch(state){

		case DOWN_PRESSED:
			cY++;
		break;

		case UP_PRESSED:
			cY--;
		break;

		case LEFT_PRESSED:
			cX--;
		break;

		case RIGHT_PRESSED:
			cX++;
		break;

		default:
		break;
	}

}

void drawingCursor2(int j, int k) {
	//TODO Calculate i by checking randomMap at (j, k) is it different than -1. (3)
	//TODO Calculate startX, startY, endX, endY (2)
	//TODO Call drawingCursor() (1)
	int startX = 60, startY = 8, endX = 80, endY = 36;
	drawingCursor(startX, startY, endX, endY);
}

state_t detect_keypress() {
	state_t state = IDLE;
	int button = Xil_In32(XPAR_MY_PERIPHERAL_0_BASEADDR);
	if ((button & UP) == 0) {
		state = UP_PRESSED;
	} else if ((button & DOWN) == 0) {
		state = DOWN_PRESSED;
	} else if ((button & RIGHT) == 0) {
		state = RIGHT_PRESSED;
	} else if ((button & LEFT) == 0) {
		state = LEFT_PRESSED;
	} else if ((button & CENTER) == 0) {
		state = CENTER_PRESSED;
	} else {
		state = IDLE;
	}

	return state;
}

//function that controls switches and buttons
void move() {
	int startX = 60, startY = 8, endX = 80, endY = 36;	//cursor start, end, coordinates
	int oldStartX, oldStartY, oldEndX, oldEndY;
	int sadasnji_nivo, sledeci_nivo, razlika;
	int i = 0, j = 0, ii, jj, k;
	int flagX = -1, flagY = -1, flagVisina = -1, flag = 0, flagForCursor = 0;
	state_t prethodno_stanje = IDLE, sledece_stanje = IDLE;

	drawingCursor(startX, startY, endX, endY);

	while(1){

		sadasnji_nivo = calculate_level(i, j, IDLE);

		prethodno_stanje = sledece_stanje;
		sledece_stanje = detect_keypress();

		while(prethodno_stanje != sledece_stanje){

			prethodno_stanje = sledece_stanje;
			sledece_stanje = detect_keypress();

			switch(sledece_stanje){

				case DOWN_PRESSED:
					if (endY < 213) {

						sledeci_nivo = calculate_level(i, j, sledece_stanje);

						razlika = sadasnji_nivo - sledeci_nivo;

						i = i + 1;

						oldStartX = startX;
						oldStartY = startY;
						oldEndY = endY;
						oldEndX = endX;
						startY += 28;
						endY += 28;
						//calculate_coordinates(sledece_stanje); //ovde koordinate el matrice

						startX = startX -(razlika*(2));
						startY = startY +(razlika*(3));
						endX = endX-(razlika*(2));
						endY = endY+(razlika*(3));

						drawingCursor(startX, startY, endX, endY);

						if(flagForCursor == 0){
							if(sadasnji_nivo == 0 && randomMap[sadasnji_nivo][i-1][j] == -1){
								drawBlue(oldStartX, oldEndX, oldStartY, oldEndY);
							}else{
								drawMap(randomMap[sadasnji_nivo][i-1][j]*20, 0, oldStartX, oldStartY, 20, 28);
							}
						}else{
							flagForCursor = 0;
						}

						break;
					}
				break;

				case UP_PRESSED:
					if (startY > 27) {

						sledeci_nivo = calculate_level(i, j, sledece_stanje);

						razlika = sadasnji_nivo - sledeci_nivo;

						i = i - 1;

						oldStartX = startX;
						oldStartY = startY;
						oldEndY = endY;
						oldEndX = endX;
						startY -= 28;
						endY -= 28;

						startX = startX -(razlika*(2));
						startY = startY +(razlika*(3));
						endX = endX-(razlika*(2));
						endY = endY+(razlika*(3));

						drawingCursor(startX, startY, endX, endY);

						if(flagForCursor == 0){
							if(sadasnji_nivo == 0 && randomMap[sadasnji_nivo][i+1][j] == -1){
								drawBlue(oldStartX, oldEndX, oldStartY, oldEndY);
							} else{
								drawMap(randomMap[sadasnji_nivo][i+1][j]*20,0,oldStartX,oldStartY, 20, 28 );
							}
						} else{
								flagForCursor = 0;
						  }

						break;
					}
				break;


				case LEFT_PRESSED:

					if (startX > 79) {

						sledeci_nivo = calculate_level(i, j, sledece_stanje);

						razlika = sadasnji_nivo - sledeci_nivo;

						j = j - 1;

						oldStartX = startX;
						oldStartY = startY;
						oldEndY = endY;
						oldEndX = endX;
						startX -= 20;
						endX -= 20;
						startX = startX -(razlika*(2));
						startY = startY +(razlika*(3));
						endX = endX-(razlika*(2));
						endY = endY+(razlika*(3));

						drawingCursor(startX, startY, endX, endY);
						if(flagForCursor == 0){
							if(sadasnji_nivo == 0 && randomMap[sadasnji_nivo][i][j+1] == -1){
								drawBlue(oldStartX, oldEndX, oldStartY, oldEndY);
							}else{
								drawMap(randomMap[sadasnji_nivo][i][j+1]*20, 0, oldStartX, oldStartY, 20, 28 );
							}
						}else{
							flagForCursor = 0;
							}

						break;
					}
				break;

				case RIGHT_PRESSED:

					if (endX < 251) {

						sledeci_nivo = calculate_level(i, j, sledece_stanje);

						razlika = sadasnji_nivo - sledeci_nivo;

						j = j + 1;

						oldStartX = startX;
						oldStartY = startY;
						oldEndY = endY;
						oldEndX = endX;
						startX += 20;
						endX += 20;
						startX = startX -(razlika*(2));
						startY = startY +(razlika*(3));
						endX = endX-(razlika*(2));
						endY = endY+(razlika*(3));

						drawingCursor(startX, startY, endX, endY);

						if(flagForCursor == 0){
							if(sadasnji_nivo == 0 && randomMap[sadasnji_nivo][i][j-1] == -1){
								drawBlue(oldStartX, oldEndX, oldStartY, oldEndY);
							}else{
								drawMap(randomMap[sadasnji_nivo][i][j-1]*20, 0, oldStartX, oldStartY, 20, 28 );
							}
						}else{
							flagForCursor = 0;
							}

						//for(kk = 0; kk < 10000; kk++){}
						break;
					}
				break;

				case CENTER_PRESSED:
					if(flag == 0){ // nije selektovano
											if(randomMap[sadasnji_nivo][i][j-1] == -1 || randomMap[sadasnji_nivo][i][j+1] == -1 || j == 0 || j == 9){ //da li moze da se selektuje(slobodne bocne ivice)
												if(randomMap[sadasnji_nivo][i][j] != -1){
													flagX = i;
													flagY = j;
													flagVisina = sadasnji_nivo;
													flagForCursor = 1;
													flag = 1;
												}
											}
										}else if(flag == 1){
											if(randomMap[sadasnji_nivo][i][j-1] == -1 || randomMap[sadasnji_nivo][i][j+1] == -1 || j == 0 || j == 9){
												if(randomMap[sadasnji_nivo][i][j] == randomMap[flagVisina][flagX][flagY]){ //da li su selektovana dva ista
													if(sadasnji_nivo != flagVisina || i != flagX || j != flagY){ //da li selektujemo samog sebe
														randomMap[sadasnji_nivo][i][j] = -1;
														randomMap[flagVisina][flagX][flagY] = -1;
														flag = 0;

														drawMap(randomMap[sadasnji_nivo][i+1][j]*20, 0, oldStartX, oldStartY, 20, 28);

														int nova_pozicija = 0;

														//postavljanje kursora na prvu gornju poziciju nakon brisanja nekih plocica
														for (ii = 0; ii <= 2; ii++) { //nivoi
															for (jj = 0; jj < 8; jj++) { //duzina
																for (k = 0; k < 10; k++){ //sirina
																	if(nova_pozicija == 0) {
																		if (randomMap[ii][jj][k] != -1) {
																			nova_pozicija = 1;
																			i = jj;
																			j = k;
																			sadasnji_nivo = ii;

																			//int startX = 60, startY = 8, endX = 80, endY = 36;

																			if (sadasnji_nivo == 0) {
																				startX = 60 + j*20;
																				startY = 8 + i*28;
																				endX = 60 + j*20 + 20;
																				endY = 8 + i*28 + 28;

																			} else if (sadasnji_nivo == 1) {
																				startX = 60 + j*20 + 2;
																				startY = 8 + i*28 - 3;
																				endX = 60 + j*20 + 20 + 2;
																				endY = 8 + i*28 + 28 - 3;
																			} else {
																				startX = 60 + j*20 + 4;
																				startY = 8 + i*28 - 6;
																				endX = 60 + j*20 + 20 + 4;
																				endY = 8 + i*28 + 28 - 6;

																			}
																		}

																	}
																}
															}
														}

														drawMap(randomMap[flagVisina][flagX][flagY]*20,0,oldStartX,oldStartY, 20, 28);

														for (ii = 0; ii < 3; ii++) {
															for (jj = 0; jj < 8; jj++) {
																for (k = 0; k < 10; k++){
																	if(randomMap[ii][jj][k] != -1){
																		if(ii == 0){
																			drawMap(randomMap[ii][jj][k]*20, 0, 60+k*20, 8+jj*28, 20, 28);
																		}else if(ii == 1){
																				if( jj > 0 && jj < 7 && k > 0 && k < 9){
																					drawMap(randomMap[ii][jj][k]*20, 0, 60+k*20+2, 8+jj*28-3, 20, 28);
																				}
																		}else{
																				if(jj > 1 && jj < 6 && k > 1 && k < 8){
																					drawMap(randomMap[ii][jj][k]*20, 0, 60+k*20+4, 8+jj*28-6, 20, 28);
																				}
																		}
																	}else{
																		if(ii == 0){

																			drawBlue(60 + k*20, 60+ k*20+20, 8+jj*28, 8+jj*28+28);

																		}
																	}
																}
															}
														}

														drawingCursor(startX, startY, endX, endY);

																	} else{



																	}


																}
															}
														}

										break;

									case IDLE:
										break;


			}

		}

	}

}

void drawMap2(int i, int j, int k){
	if(randomMap[i][j][k] != -1){
		drawMap(randomMap[i][j][k] * 20, 0, 60 + k * 20 + 2*i, 8 + j * 28 -3*i, 20, 28);
	}
}

void drawWholeMap(){
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 8; j++) {
			for (int k = 0; k < 10; k++) {
				drawMap2(i, j, k);
			}
		}
	}
}

int main() {

	init_platform();

	VGA_PERIPH_MEM_mWriteMemory(
			XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR + 0x00, 0x0); // direct mode   0
	VGA_PERIPH_MEM_mWriteMemory(
			XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR + 0x04, 0x3); // display_mode  1
	VGA_PERIPH_MEM_mWriteMemory(
			XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR + 0x08, 0x0); // show frame      2
	VGA_PERIPH_MEM_mWriteMemory(
			XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR + 0x0C, 0xff); // font size       3
	VGA_PERIPH_MEM_mWriteMemory(
			XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR + 0x10, 0xFFFFFF); // foreground 4
	VGA_PERIPH_MEM_mWriteMemory(
			XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR + 0x14, 0x0000FF); // background color 5
	VGA_PERIPH_MEM_mWriteMemory(
			XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR + 0x18, 0xFF0000); // frame color      6
	VGA_PERIPH_MEM_mWriteMemory(
			XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR + 0x20, 1);

	//setting background color
	for (x = 0; x < 320; x++) {
		for (y = 0; y < 240; y++) {
			q = y * 320 + x;
			VGA_PERIPH_MEM_mWriteMemory(
					XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR + GRAPHICS_MEM_OFF
							+ q * 4, 0x000000);
		}
	}

	makeTable();

	drawWholeMap();

	move();

	cleanup_platform();

	return 0;
}
