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
#define SW0 0b00000001
#define SW1 0b00000010
#define NUMOFMINES 9
#define NUMOFSPRITES 37

//BEG---unpened field
#define BEG '@'


int inc1;
int inc2;
int i, x, y, ii, oi, R, G, B, RGB, kolona, red, RGBgray;


int randomMap[3][8][10];



//function that generates random game map
void makeTable() {
	int numOfSprites, row, column, level, i, j, k, r, m;
	int table[3][8][10];

	srand(rand()%50);

	//popunjava matricu sa -1
	for (i = 0; i < 3; i++) {
		for (j = 0; j < 8; j++) {
			for (k = 0; k < 10; k++)
			randomMap[i][j][k] = -1;
		}
	}

	//postavlja random mine
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
			i = y * 320 + x;
			VGA_PERIPH_MEM_mWriteMemory(
					XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR + GRAPHICS_MEM_OFF
							+ i * 4, 0x000055);
		}
	}

	for (x = startX; x < endX; x++) {
		for (y = endY - 2; y < endY; y++) {
			i = y * 320 + x;
			VGA_PERIPH_MEM_mWriteMemory(
					XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR + GRAPHICS_MEM_OFF
							+ i * 4, 0x000055);
		}
	}

	for (x = startX; x < startX + 2; x++) {
		for (y = startY; y < endY; y++) {
			i = y * 320 + x;
			VGA_PERIPH_MEM_mWriteMemory(
					XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR + GRAPHICS_MEM_OFF
							+ i * 4, 0x000055);
		}
	}

	for (x = endX - 2; x < endX; x++) {
		for (y = startY; y < endY; y++) {
			i = y * 320 + x;
			VGA_PERIPH_MEM_mWriteMemory(
					XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR + GRAPHICS_MEM_OFF
							+ i * 4, 0x000055);
		}
	}

}


void drawBlue(int startX, int endX, int startY, int endY){

	for (x = startX; x < endX; x++) {
		for (y = startY; y < endY; y++) {
			i = y * 320 + x;
			VGA_PERIPH_MEM_mWriteMemory(
			XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR + GRAPHICS_MEM_OFF
			+ i * 4, 0x000000);
		}
	}

}

//function that controls switches and buttons

void move() {
	int startX = 60, startY = 8, endX = 80, endY = 36;	//cursor start, end, coordinates
	int oldStartX, oldStartY, oldEndX, oldEndY;
	int sadasnji_nivo, sledeci_nivo, razlika;
	int x, y, ic, ib, i = 0, j = 0, ii, jj, k;
	int flagJedanMarkiran = 0, flagX = -1, flagY = -1, flagVisina = -1, flag = 0, flagForCursor = 0;
	int prethodnoStanje;
	typedef enum {
		NOTHING_PRESSED, SOMETHING_PRESSED
	} btn_state_t;
	btn_state_t btn_state = NOTHING_PRESSED;

//	makeTable(solvedMap);
	drawingCursor(startX, startY, endX, endY);

	while (1) {

		if (btn_state == NOTHING_PRESSED) {
			btn_state = SOMETHING_PRESSED;


			if(randomMap[0][i][j] != -1){
				sadasnji_nivo = 0;
			}
			if(randomMap[1][i][j] != -1){
				sadasnji_nivo = 1;
			}
			if(randomMap[2][i][j] != -1){
				sadasnji_nivo = 2;
			}

			if ((Xil_In32(XPAR_MY_PERIPHERAL_0_BASEADDR) & DOWN) == 0) {




				if (endY < 213) {

					if(randomMap[0][i+1][j] != -1){
						sledeci_nivo = 0;
					}
					if(randomMap[1][i+1][j] != -1){
						sledeci_nivo = 1;
					}
					if(randomMap[2][i+1][j] != -1){
						sledeci_nivo = 2;
					}

					razlika = sadasnji_nivo - sledeci_nivo;

						i = i + 1;

					oldStartX = startX;
					oldStartY = startY;
					oldEndY = endY;
					oldEndX = endX;
					startY += 28;
					endY += 28;

					startX = startX -(razlika*(2));
					startY = startY +(razlika*(3));
					endX = endX-(razlika*(2));
					endY = endY+(razlika*(3));


					drawingCursor(startX, startY, endX, endY);
					//openField(startX, oldStartY, blankMap);
					if(flagForCursor == 0){
						if(sadasnji_nivo == 0 && randomMap[sadasnji_nivo][i-1][j] == -1){
							drawBlue(oldStartX, oldEndX, oldStartY, oldEndY);
						}else{
							drawMap(randomMap[sadasnji_nivo][i-1][j]*20,0,oldStartX,oldStartY, 20, 28 );
						}
					}else{
						flagForCursor = 0;
					}
				}

			}

			else if ((Xil_In32(XPAR_MY_PERIPHERAL_0_BASEADDR) & RIGHT) == 0) {



				if (endX < 251) {

					if(randomMap[0][i][j+1] != -1){
						sledeci_nivo = 0;
					}
					if(randomMap[1][i][j+1] != -1){
						sledeci_nivo = 1;
					}
					if(randomMap[2][i][j+1] != -1){
						sledeci_nivo = 2;
					}

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
					//openField(oldStartX, startY, blankMap);
					if(flagForCursor == 0){
						if(sadasnji_nivo == 0 && randomMap[sadasnji_nivo][i][j-1] == -1){
							drawBlue(oldStartX, oldEndX, oldStartY, oldEndY);
						}else{
							drawMap(randomMap[sadasnji_nivo][i][j-1]*20,0,oldStartX,oldStartY, 20, 28 );
						}
					}else{
						flagForCursor = 0;
					}

				}
			} else if ((Xil_In32(XPAR_MY_PERIPHERAL_0_BASEADDR) & LEFT) == 0) {



				if (startX > 79) {

					if(randomMap[0][i][j-1] != -1){
						sledeci_nivo = 0;
					}
					if(randomMap[1][i][j-1] != -1){
						sledeci_nivo = 1;
					}
					if(randomMap[2][i][j-1] != -1){
						sledeci_nivo = 2;
					}

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
					//openField(oldStartX, startY, blankMap);
					if(flagForCursor == 0){
						if(sadasnji_nivo == 0 && randomMap[sadasnji_nivo][i][j+1] == -1){
							drawBlue(oldStartX, oldEndX, oldStartY, oldEndY);
						}else{
							drawMap(randomMap[sadasnji_nivo][i][j+1]*20,0,oldStartX,oldStartY, 20, 28 );
						}
					}else{
						flagForCursor = 0;
					}
				}

			} else if ((Xil_In32(XPAR_MY_PERIPHERAL_0_BASEADDR) & UP) == 0) {



				if (startY > 27) {

					if(randomMap[0][i-1][j] != -1){
						sledeci_nivo = 0;
					}
					if(randomMap[1][i-1][j] != -1){
						sledeci_nivo = 1;
					}
					if(randomMap[2][i-1][j] != -1){
						sledeci_nivo = 2;
					}

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
					//openField(startX, oldStartY, blankMap);
					if(flagForCursor == 0){
						if(sadasnji_nivo == 0 && randomMap[sadasnji_nivo][i+1][j] == -1){
							drawBlue(oldStartX, oldEndX, oldStartY, oldEndY);
						}else{
							drawMap(randomMap[sadasnji_nivo][i+1][j]*20,0,oldStartX,oldStartY, 20, 28 );
						}
					}else{
						flagForCursor = 0;

					}
				}

			} else if ((Xil_In32(XPAR_MY_PERIPHERAL_0_BASEADDR) & CENTER) == 0) {

				if(flag == 0){
					if(randomMap[sadasnji_nivo][i][j-1] == -1 || randomMap[sadasnji_nivo][i][j+1] == -1 || j == 0 || j == 9){
						flagX = i;
						flagY = j;
						flagVisina = sadasnji_nivo;
						flagForCursor = 1;
						flag = 1;
					}
				}else if(flag == 1){
					if(randomMap[sadasnji_nivo][i][j] == randomMap[flagVisina][flagX][flagY]){
						if(sadasnji_nivo != flagVisina || i != flagX || j != flagY){
							randomMap[sadasnji_nivo][i][j] = -1;
							randomMap[flagVisina][flagX][flagY] = -1;
							flag = 0;

							int nova_pozicija = 0;

							//postavljanje kursora na prvu gornju poziciju nakon brisanja nekih plocica
							for (ii = 2; ii >= 0; ii--) {
								for (jj = 0; jj < 8; jj++) {
									for (k = 0; k < 10; k++){
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
												startX = 60 + j*20 +2;
												startY = 8 + i*28 -3;
												endX = 60 + j*20 + 20 +2;
												endY = 8 + i*28 + 28 -3;
											} else if (sadasnji_nivo == 2) {
												startX = 60 + j*20 +4;
												startY = 8 + i*28 -6;
												endX = 60 + j*20 + 20 +4;
												endY = 8 + i*28 + 28 -6;

											}
										}

									}
									}
								}
							}




							//drawMap(randomMap[flagVisina][flagX][flagY]*20,0,oldStartX,oldStartY, 20, 28 );

							for (ii = 0; ii < 3; ii++) {
								for (jj = 0; jj < 8; jj++) {
									for (k = 0; k < 10; k++){
										if(randomMap[ii][jj][k] != -1){
											if( ii == 0){
												drawMap(randomMap[ii][jj][k]*20, 0, 60+k*20, 8+jj*28, 20, 28 );
											}else if(ii == 1){
												if( jj > 0 && jj < 7 && k > 0 && k < 9){
												drawMap(randomMap[ii][jj][k]*20, 0, 60+k*20+2, 8+jj*28-3, 20, 28 );
												}
											}else{
												if( jj > 1 && jj < 6 && k > 1 && k < 8){
												drawMap(randomMap[ii][jj][k]*20, 0, 60+k*20+4, 8+jj*28-6, 20, 28 );
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




						}
					}
				}

/*
				for (i = 0; i < SIZE; i++) {
					for (j = 0; j < SIZE; j++) {
						if (indicationMap[i][j] == 'x') {
							ii = (i * 16) + 80;
							jj = (j * 16) + 80;

							if (solvedMap[i][j] == BLANK) {
								drawMap(0, 0, ii, jj, 16, 16);
								blankMap[i][j] = BLANK;
							}
							if (solvedMap[i][j] == NUM2) {
								drawMap(32, 0, ii, jj, 16, 16);
								blankMap[i][j] = NUM2;
							}
							if (solvedMap[i][j] == NUM1) {
								drawMap(16, 0, ii, jj, 16, 16);
								blankMap[i][j] = NUM1;
							}
							if (solvedMap[i][j] == NUM3) {
								drawMap(48, 0, ii, jj, 16, 16);
								blankMap[i][j] = NUM3;
							}
							if (solvedMap[i][j] == NUM4) {
								drawMap(64, 0, ii, jj, 16, 16);
								blankMap[i][j] = NUM4;
							}
						}
					}
				}

			*/} /*else if ((Xil_In32(XPAR_MY_PERIPHERAL_0_BASEADDR) & SW0) != 0) { //flag

				if (numOfFlags > 0 && numOfFlags <= NUMOFMINES) {
					int x = (startX - 80) / 16;
					int y = (startY - 80) / 16;
					if (blankMap[x][y] != FLAG && blankMap[x][y] == BEG) {
						drawMap(64, 16, startX - 1, startY - 1, 16, 16);

						blankMap[x][y] = FLAG;

						numOfFlags--;
						//checks if the flag is in the right place
						if (solvedMap[x][y] == BOMB) {
							flagTrue++;
							if (flagTrue == NUMOFMINES) {

								endOfGame = 1;
								drawMap(103, 54, 120, 54, 27, 26);
							}
						}

					}
					//prints out flag counter
					switch (numOfFlags) {
					case 9:
						drawMap(116, 32, 168, 54, 13, 23);
						break;
					case 8:
						drawMap(103, 32, 168, 54, 13, 23);
						break;
					case 7:
						drawMap(90, 32, 168, 54, 13, 23);
						break;
					case 6:
						drawMap(77, 32, 168, 54, 13, 23);
						break;
					case 5:
						drawMap(64, 32, 168, 54, 14, 23);
						break;
					case 4:
						drawMap(51, 32, 168, 54, 13, 23);
						break;
					case 3:
						drawMap(38, 32, 168, 54, 13, 23);
						break;
					case 2:
						drawMap(25, 32, 168, 54, 13, 23);
						break;
					case 1:
						drawMap(13, 32, 168, 54, 13, 23);
						break;
					case 0:
						drawMap(0, 32, 168, 54, 13, 23);
						break;

					}

				}
			} else if ((Xil_In32(XPAR_MY_PERIPHERAL_0_BASEADDR) & SW1) != 0) {
				if (numOfFlags < NUMOFMINES) {

					int x = (startX - 80) / 16;
					int y = (startY - 80) / 16;
					if (blankMap[x][y] == FLAG) {
						drawMap(80, 16, startX - 1, startY - 1, 16, 16);

						blankMap[x][y] = BEG;

						numOfFlags++;

						if (solvedMap[x][y] == BOMB) {
							flagTrue--;
						}

						switch (numOfFlags) {
						case 9:
							drawMap(116, 32, 168, 54, 13, 23);
							break;
						case 8:
							drawMap(103, 32, 168, 54, 13, 23);
							break;
						case 7:
							drawMap(90, 32, 168, 54, 13, 23);
							break;
						case 6:
							drawMap(77, 32, 168, 54, 13, 23);
							break;
						case 5:
							drawMap(64, 32, 168, 54, 13, 23);
							break;
						case 4:
							drawMap(51, 32, 168, 54, 13, 23);
							break;
						case 3:
							drawMap(38, 32, 168, 54, 13, 23);
							break;
						case 2:
							drawMap(25, 32, 168, 54, 13, 23);
							break;
						case 1:
							drawMap(13, 32, 168, 54, 13, 23);
							break;
						case 0:
							drawMap(0, 32, 168, 54, 13, 23);
							break;
						}
					}
				}

			}*/ else {
				btn_state = NOTHING_PRESSED;
			}
		} else { // SOMETHING_PRESSED
			if ((Xil_In32(XPAR_MY_PERIPHERAL_0_BASEADDR) & DOWN) == 0) {
			} else if ((Xil_In32(XPAR_MY_PERIPHERAL_0_BASEADDR) & RIGHT) == 0) {
			} else if ((Xil_In32(XPAR_MY_PERIPHERAL_0_BASEADDR) & LEFT) == 0) {
			} else if ((Xil_In32(XPAR_MY_PERIPHERAL_0_BASEADDR) & UP) == 0) {
			} else if ((Xil_In32(XPAR_MY_PERIPHERAL_0_BASEADDR) & CENTER)
					== 0) {
			} else if ((Xil_In32(XPAR_MY_PERIPHERAL_0_BASEADDR) & SW0) != 0) {
			} else if ((Xil_In32(XPAR_MY_PERIPHERAL_0_BASEADDR) & SW1) != 0) {
			} else {
				btn_state = NOTHING_PRESSED;
			}
		}

	}

}



int main() {

	int j, p, r, i, k;
	inc1 = 0;
	inc2 = 0;


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

	//black background
	for (x = 0; x < 320; x++) {
		for (y = 0; y < 240; y++) {
			i = y * 320 + x;
			VGA_PERIPH_MEM_mWriteMemory(
					XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR + GRAPHICS_MEM_OFF
							+ i * 4, 0x000000);
		}
	}

	//drawing a map
	for (kolona = 0; kolona < 9; kolona++) {
		for (red = 0; red < 9; red++) {
			drawMap(80, 16, 80 + red * 16, 80 + kolona * 16, 16, 16);
		}
	}


	// nas pokusaj pravljenja mape - relativno uspjesan, napravili smo mapu sa tri nivoa

	makeTable();


	for (i = 0; i < 3; i++) {
		for (j = 0; j < 8; j++) {
			for (k = 0; k < 10; k++){
				if( i == 0){
					drawMap(randomMap[i][j][k]*20, 0, 60+k*20, 8+j*28, 20, 28 );
				}else if(i == 1){
					if( j > 0 && j < 7 && k > 0 && k < 9){
					drawMap(randomMap[i][j][k]*20, 0, 60+k*20+2, 8+j*28-3, 20, 28 );
					}
				}else{
					if( j > 1 && j < 6 && k > 1 && k < 8){
					drawMap(randomMap[i][j][k]*20, 0, 60+k*20+4, 8+j*28-6, 20, 28 );
					}
				}
			}
		}
	}




	//moving through the table
	move();

	cleanup_platform();

	return 0;
}
