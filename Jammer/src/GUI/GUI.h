/*
 * GUI.h
 *
 *  Created on: Feb 12, 2022
 *      Author: tornado
 */
#include <gtk/gtk.h>
#include <semaphore.h>
#include <stdlib.h>

#ifndef GUI_GUI_H_
#define GUI_GUI_H_

#define WINDOW_BOARDER_WIDTH 20
#define WINDOW_TITLE "Jam Daddy"
#define WINDOW_WIDTH 20
#define WINDOW_HEIGHT 20


#endif /* GUI_GUI_H_ */

void createWindow();
int initGUI();
void createWindow();
void destroy(GtkWidget *widget, gpointer data);
void createJammerButtons();
void activateJammerButtonPressed(GtkWidget *widget, gpointer data );
void deactivateJammerButtonPressed(GtkWidget *widget, gpointer data );
void createStatusObj();
void createIdentifyUEObj();
void findUEButtonPressed(GtkWidget *widget, gpointer data );
void plot_PDCCH(int count, float *x, float *y);
void plotButtonPressed(GtkWidget *widget, gpointer data);
void entryChanged(GtkWidget *widget, gpointer data );
void createEntryField();
void plotSpectrogramButtonPressed(GtkWidget *widget, gpointer data);
void PUSCHentryChanged(GtkWidget *widget, gpointer data );


extern GtkWidget *PDCCHPerformanceLabel;
extern GtkWidget *syncStatusLamp;
extern GtkWidget *listItem;
extern GtkWidget *SNRLabel;

extern int plotFlag;
extern int done;
extern int mode;
extern int plot_spec_flag;
extern int PUSCHOffset;
extern unsigned short UERNTI;
extern sem_t plotSpec;
