/*
 ============================================================================
 Name        : GUI_COde.c
 Author      :
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include "GUI.h"
GtkWidget *window;
GtkWidget *hbox1;
GtkWidget *hbox2;
GtkWidget *hbox3;
GtkWidget *hbox4;
GtkWidget *hbox5;
GtkWidget *hbox6;
GtkWidget *hbox7;


GtkWidget *vbox1;
GtkWidget *vbox2;
GtkWidget *vbox3;

GtkWidget *activateJammerButton;
GtkWidget *deactivateJammerButton;

GtkWidget *syncStatusLabel;
GtkWidget *syncStatusLamp;

GtkWidget *PDCCHPerformanceLabel;
GtkWidget *plotButton;
GtkWidget *plotSpectrogramButton;

GtkWidget *SNRLabel;

GtkWidget *findUEButton;
GtkWidget *listItem;

GtkWidget *entryBox;
GtkWidget *entryLabel;

GtkWidget *entryBoxPUSCH;
GtkWidget *entryLabelPUSCH;



int toggle=1;
int plotFlag = 0;

int initGUI() {

    gtk_init(NULL, NULL);

    createWindow();
    createJammerButtons();
    createStatusObj();
    createIdentifyUEObj();
    createEntryField();

    hbox4 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 1); //First Parameter defines if each element has equal space s
    gtk_box_pack_start(GTK_BOX(hbox4), syncStatusLabel, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(hbox4), syncStatusLamp, FALSE, FALSE, 0);

    hbox5 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 1); //First Parameter defines if each element has equal space s
    gtk_box_pack_start(GTK_BOX(hbox5), SNRLabel, FALSE, FALSE, 0);

    vbox3 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 1); //First Parameter defines if each element has equal space s
	gtk_box_pack_start(GTK_BOX(vbox3), listItem, FALSE, FALSE, 5);


    vbox1 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5); //First Parameter defines if each element has equal space s
    gtk_box_pack_start(GTK_BOX(vbox1), hbox5, TRUE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox1), hbox4, TRUE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox1), PDCCHPerformanceLabel, TRUE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox1), plotButton, TRUE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox1), plotSpectrogramButton, TRUE, FALSE, 5);


    hbox1 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5); //First Parameter defines if each element has equal space
    gtk_box_pack_start(GTK_BOX(hbox1), vbox3, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(hbox1), vbox1, TRUE, FALSE, 5);


    hbox2 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 1); //First Parameter defines if each element has equal space
    gtk_box_pack_start(GTK_BOX(hbox2), activateJammerButton, FALSE, FALSE, 1);
    gtk_box_pack_start(GTK_BOX(hbox2), deactivateJammerButton, FALSE, FALSE, 1);
    gtk_box_pack_start(GTK_BOX(hbox2), findUEButton, FALSE, FALSE, 1);

    hbox6 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 1); //First Parameter defines if each element has equal space s
	gtk_box_pack_start(GTK_BOX(hbox6), entryLabel, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(hbox6), entryBox, FALSE, FALSE, 0);

    hbox7 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 1); //First Parameter defines if each element has equal space s
	gtk_box_pack_start(GTK_BOX(hbox7), entryLabelPUSCH, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(hbox7), entryBoxPUSCH, FALSE, FALSE, 0);



    vbox2 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5); //First Parameter defines if each element has equal space s

    //gtk_box_pack_start(GTK_BOX(vbox2), hbox3, TRUE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox2), hbox1, TRUE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox2), hbox6, TRUE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox2), hbox7, TRUE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox2), hbox2, TRUE, FALSE, 15);


    gtk_container_add(GTK_CONTAINER(window), vbox2);
    gtk_widget_show_all(window); //Activate the window

    gtk_main();
    return 1;
}

//Create the GUI Window
void createWindow(){
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);  //Create Window
    gtk_container_set_border_width(GTK_CONTAINER(window), WINDOW_BOARDER_WIDTH); //Set the Boarder with
    gtk_window_set_title (GTK_WINDOW(window), WINDOW_TITLE); //Set the Title
    gtk_window_set_default_size(GTK_WINDOW(window), WINDOW_WIDTH, WINDOW_HEIGHT); //Set the size of the window
    g_signal_connect(window, "destroy", G_CALLBACK(destroy), NULL); //Attach Callback

}
//Window CallBack Function to quit program
void destroy(GtkWidget *widget, gpointer data){
	done = 0;
	gtk_main_quit();
}





//Functinos to look at togglebutton   checkbutton
void createJammerButtons(){
    activateJammerButton = gtk_toggle_button_new_with_label("Activate Jammer"); //Create Button
    g_signal_connect(GTK_TOGGLE_BUTTON(activateJammerButton), "clicked", G_CALLBACK(activateJammerButtonPressed), "button");

    deactivateJammerButton = gtk_toggle_button_new_with_label("Deactivate Jammer"); //Create Button
    g_signal_connect(GTK_TOGGLE_BUTTON(deactivateJammerButton), "clicked", G_CALLBACK(deactivateJammerButtonPressed), "button");


    //Create drop_down list for UE to JAM or have entry box



}
//Activate the jammer set the status lamp
void activateJammerButtonPressed(GtkWidget *widget, gpointer data ){
	if(toggle){
		toggle = 0;
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(findUEButton), FALSE);
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(activateJammerButton), TRUE);
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(deactivateJammerButton), FALSE);
		mode = 1;
	}
	toggle = 1;
}
//Deactivate the jammer set the status lamp
void deactivateJammerButtonPressed(GtkWidget *widget, gpointer data ){
	if(toggle){
		toggle = 0;
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(findUEButton), FALSE);
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(activateJammerButton), FALSE);
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(deactivateJammerButton), TRUE);
		mode = 2;
	}
	toggle = 1;
}




void createStatusObj(){
    SNRLabel = gtk_label_new("Estimated SNR: 0.00dB"); //Create a label

    syncStatusLabel = gtk_label_new("Frame Syncronization Status:"); //Create a label
    syncStatusLamp = gtk_label_new("          ");
    GdkColor color;

    gdk_color_parse("red", &color);

    gtk_widget_modify_bg(syncStatusLamp, GTK_STATE_NORMAL, &color);

    //gtk_label_set_markup(GTK_LABEL(syncStatusLamp),"background", "red");
    PDCCHPerformanceLabel =  gtk_label_new("Percentage of Decoded PDCCH Candidates: 0.00%"); //Create a label
    plotButton = gtk_toggle_button_new_with_label("Plot Constelation"); //Create Button
    g_signal_connect(GTK_TOGGLE_BUTTON(plotButton), "clicked", G_CALLBACK(plotButtonPressed), "button");

    plotSpectrogramButton = gtk_button_new_with_label("Plot Spectrogram"); //Create Button
    g_signal_connect(GTK_BUTTON(plotSpectrogramButton), "clicked", G_CALLBACK(plotSpectrogramButtonPressed), "button");


}





void createIdentifyUEObj(){

    findUEButton = gtk_toggle_button_new_with_label("Search for UE"); //Create Button
    g_signal_connect(GTK_TOGGLE_BUTTON(findUEButton), "clicked", G_CALLBACK(findUEButtonPressed), NULL);

    listItem = gtk_label_new("List of UE on Network\n");

}


void findUEButtonPressed(GtkWidget *widget, gpointer data ){
	if(toggle){
		toggle = 0;
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(findUEButton), TRUE);
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(activateJammerButton), FALSE);
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(deactivateJammerButton), FALSE);
		mode = 0;
	}
	toggle = 1;
}

void plotButtonPressed(GtkWidget *widget, gpointer data){
	plotFlag = !plotFlag;
}

void plotSpectrogramButtonPressed(GtkWidget *widget, gpointer data){
	//sem_post(&plotSpec);
	plot_spec_flag = 1;
}


void createEntryField(){
	entryBox = gtk_entry_new();
	gtk_entry_set_placeholder_text(GTK_ENTRY(entryBox), "Enter Target RNTI");
	gtk_entry_set_visibility(GTK_ENTRY(entryBox), TRUE);

	entryLabel = gtk_label_new("Target UE to JAM:");
    g_signal_connect(GTK_ENTRY(entryBox), "activate", G_CALLBACK(entryChanged), NULL);

	entryBoxPUSCH = gtk_entry_new();
	gtk_entry_set_placeholder_text(GTK_ENTRY(entryBoxPUSCH), "Offset in slots");
	gtk_entry_set_visibility(GTK_ENTRY(entryBoxPUSCH), TRUE);

	entryLabelPUSCH = gtk_label_new("Jamming Signal Offset:");
    g_signal_connect(GTK_ENTRY(entryBoxPUSCH), "activate", G_CALLBACK(PUSCHentryChanged), NULL);
}

void entryChanged(GtkWidget *widget, gpointer data ){
	char *ptr;
	UERNTI = (unsigned short)strtol(gtk_entry_get_text(GTK_ENTRY(widget)), &ptr, 10);
}

void PUSCHentryChanged(GtkWidget *widget, gpointer data ){
	char *ptr;
	PUSCHOffset = (int)strtol(gtk_entry_get_text(GTK_ENTRY(widget)), &ptr, 10);
	printf("%i \n", PUSCHOffset);
}

