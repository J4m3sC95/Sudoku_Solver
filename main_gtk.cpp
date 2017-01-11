/*
* Sudoku main for gtk interface
*/

extern "C" {
	
#include <stdio.h>
#include <gtk/gtk.h>
#include <string.h>
}

#include "sudoku.h"

char filename[100];

unsigned char possibility_matrix[10][82];

// get icon for gdk window
GdkPixbuf *create_pixbuf(const gchar *filename){
	GdkPixbuf *pixbuf;
	GError *error = NULL;
	
	// load new pixbuf from file
	pixbuf = gdk_pixbuf_new_from_file(filename, &error);
	
	// error is displayed if can't open file
	if(!pixbuf){
		fprintf(stderr, "%s\n", error->message);
		g_error_free(error);
	}
	
	return pixbuf;
}

unsigned char sudoku_input[82], sudoku_output[82];

GtkWidget *entry[82];
GtkWidget *output[82];

// function that runs when the solve button is pressed
void solve_button_press(GtkWidget *widget, gpointer window){
	unsigned char n;
	for(n = 0; n<81; n++){
		const char *data = gtk_entry_get_text(GTK_ENTRY(entry[n]));
		if(isdigit(data[0])){
			sudoku_input[n] = data[0] - 48;
		}
		else{
			sudoku_input[n] = 0;
		}
	}
	
	sudoku_solve(sudoku_input, sudoku_output, possibility_matrix);
	for(n = 0; n<81; n++){
		//char fill;
		if(sudoku_output[n] != 0){
			char fill = sudoku_output[n] + 48;
			gtk_entry_set_text(GTK_ENTRY(output[n]), &fill);
		}
		else{
			char fill = '\0';
			gtk_entry_set_text(GTK_ENTRY(output[n]), &fill);
		}
	}
}

// function that runs when the clear button is pressed
void clear_start_button_press(GtkWidget *widget, gpointer window){
	unsigned char n;
	char fill = '\0';
	for(n = 0; n<81; n++){
		gtk_entry_set_text(GTK_ENTRY(entry[n]), &fill);
	}
}

// function that runs when the clear button is pressed
void clear_solution_button_press(GtkWidget *widget, gpointer window){
	unsigned char n;
	char fill = '\0';
	for(n = 0; n<81; n++){
		gtk_entry_set_text(GTK_ENTRY(output[n]), &fill);
	}
}

GtkWidget *filename_entry;

// function to write everything to file
void write_to_file(GtkWidget *widget, gpointer window){
	strcpy(filename, "/home/james/Documents/sudokus/");
	char *filename_suffix = (char *)gtk_entry_get_text(GTK_ENTRY(filename_entry));
	if(filename_suffix == NULL){
		printf("Error!! - Invalid file name\n");
		return;
	}
	strcat(filename, filename_suffix);
	strcat(filename, ".txt");
	printf("Filename = %s\n", filename);
	FILE *data_file = fopen(filename, "w");
	fprintf(data_file, "Starting sudoku:\n");
	sudoku_file_print(sudoku_input, data_file);
	fprintf(data_file, "\nFinal sudoku:\n");
	sudoku_file_print(sudoku_output, data_file);
	fclose(data_file);
}

// function to read everything from file
void read_from_file(GtkWidget *widget, gpointer window){
	strcpy(filename, "/home/james/Documents/sudokus/");
	char *filename_suffix = (char *)gtk_entry_get_text(GTK_ENTRY(filename_entry));
	if(filename_suffix == NULL){
		printf("Error!! - Invalid file name\n");
		return;
	}
	strcat(filename, filename_suffix);
	strcat(filename, ".txt");
	printf("Filename = %s\n", filename);
	FILE *data_file = fopen(filename, "r");
	if(data_file == NULL){
		printf("Error!! - Can't open file\n");
		return;
	}
	char n = 0;
	char fill = '\0';
	unsigned char index = 0;
	while(!feof(data_file)){
		fread(&n, 1, 1, data_file);
		if(isdigit(n)){
			if(index < 81){
				if(n == '0'){
					
					gtk_entry_set_text(GTK_ENTRY(entry[index]), &fill);
				}
				else{
					gtk_entry_set_text(GTK_ENTRY(entry[index]), &n);
				}
			}
			else{
				if(n == '0'){
					gtk_entry_set_text(GTK_ENTRY(output[index-81]), &fill);
				}
				else{
					gtk_entry_set_text(GTK_ENTRY(output[index-81]), &n);
				}
			}
			
			index++;
		}
	}
			
	fclose(data_file);
}
	

int main( int argc, char *argv[] ){
	printf("\nJames' Sudoku Solving Program Version 3 with graphical interface gtk\n\n");
	
	// initialise stuff
	unsigned char n, m, position;  
	GdkPixbuf *icon;
    GtkWidget *window;
    GtkWidget *entry_square_frame[10], *output_square_frame[10];
    GtkWidget *entry_frame[82], *output_frame[82];
    GtkWidget *solve_button, *clear_start_button, *clear_solution_button;
    GtkWidget *entry_squares[10], *output_squares[10];       
    GtkWidget *sudoku_table;
    GtkWidget *starting_sudoku_label, *completed_sudoku_label; 
    GtkWidget *button_table;
    
    //initialis gtk
    gtk_init (&argc, &argv);
    
    //create new top level (can contain ther widgets) window
    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    // set window title
    gtk_window_set_title(GTK_WINDOW(window), "Sudoku Solver gtk");
    gtk_container_set_border_width(GTK_CONTAINER(window), 15);
        
    // load icon
    icon = create_pixbuf("/home/james/Documents/Programming/Sudoku_Solver/sudoku.png");
    gtk_window_set_icon(GTK_WINDOW(window), icon);
    g_object_unref(icon);   
    
      
    sudoku_table = gtk_table_new(5,7, FALSE);
        
    for(n=0; n<9; n++){
		// entry
		entry_square_frame[n] = gtk_frame_new(NULL);
		gtk_frame_set_shadow_type(GTK_FRAME(entry_square_frame[n]), (GtkShadowType) GTK_SHADOW_OUT);
		entry_squares[n] = gtk_table_new(3,3,FALSE);
		// output
		output_square_frame[n] = gtk_frame_new(NULL);
		gtk_frame_set_shadow_type(GTK_FRAME(output_square_frame[n]), (GtkShadowType) GTK_SHADOW_OUT);
		output_squares[n] = gtk_table_new(3,3,FALSE);
	}
	for(n=0; n<81; n++){
		// entry
		entry[n] = gtk_entry_new();
		gtk_entry_set_max_length(GTK_ENTRY(entry[n]), 1);
		gtk_entry_set_width_chars(GTK_ENTRY(entry[n]), 1);
		entry_frame[n] = gtk_frame_new(NULL);
		gtk_frame_set_shadow_type(GTK_FRAME(entry_frame[n]), (GtkShadowType) GTK_SHADOW_OUT);
		gtk_container_add(GTK_CONTAINER(entry_frame[n]), entry[n]);
		// ouput
		output[n] = gtk_entry_new();
		gtk_entry_set_max_length(GTK_ENTRY(output[n]), 1);
		gtk_entry_set_width_chars(GTK_ENTRY(output[n]), 1);
		gtk_entry_set_editable(GTK_ENTRY(output[n]), FALSE);
		output_frame[n] = gtk_frame_new(NULL);
		gtk_frame_set_shadow_type(GTK_FRAME(output_frame[n]), (GtkShadowType)GTK_SHADOW_OUT);
		gtk_container_add(GTK_CONTAINER(output_frame[n]), output[n]);
	}
	// entry
    for(n=0; n<9; n++){
		for(m=0;m<9;m++){
			// position = square starting row + square starting column + row + col
			position = ((n/3)*27) + ((n%3)*3) + ((m/3)*9) + (m%3);
			gtk_table_attach(GTK_TABLE(entry_squares[n]), entry_frame[position], m%3, (m%3) + 1, m/3, (m/3) + 1, (GtkAttachOptions) (GTK_FILL | GTK_SHRINK), (GtkAttachOptions) (GTK_FILL | GTK_SHRINK), 5, 5);
		}
		gtk_container_add(GTK_CONTAINER(entry_square_frame[n]), entry_squares[n]);
		gtk_table_attach(GTK_TABLE(sudoku_table), entry_square_frame[n], n%3, (n%3) + 1, (n/3)+1, (n/3) + 2, (GtkAttachOptions) (GTK_FILL | GTK_SHRINK), (GtkAttachOptions) (GTK_FILL | GTK_SHRINK), 0, 0);
	}
	// output
	for(n=0; n<9; n++){
		for(m=0;m<9;m++){
			// position = square starting row + square starting column + row + col
			position = ((n/3)*27) + ((n%3)*3) + ((m/3)*9) + (m%3);
			gtk_table_attach(GTK_TABLE(output_squares[n]), output_frame[position], m%3, (m%3) + 1, m/3, (m/3) + 1, (GtkAttachOptions) (GTK_FILL | GTK_SHRINK), (GtkAttachOptions) (GTK_FILL | GTK_SHRINK), 5, 5);
		}
		gtk_container_add(GTK_CONTAINER(output_square_frame[n]), output_squares[n]);
		gtk_table_attach(GTK_TABLE(sudoku_table), output_square_frame[n], (n%3)+4, (n%3) + 5, (n/3)+1, (n/3) + 2, (GtkAttachOptions) (GTK_FILL | GTK_SHRINK), (GtkAttachOptions) (GTK_FILL | GTK_SHRINK), 0, 0);
	}
	gtk_container_add(GTK_CONTAINER(window), sudoku_table);
	
	// solve and clear buttons
	
	button_table = gtk_table_new(1,2,FALSE);	
	
	solve_button = gtk_button_new_with_label("Solve");
	gtk_table_attach(GTK_TABLE(button_table), solve_button, 0,1,0,1, (GtkAttachOptions) (GTK_FILL | GTK_SHRINK), (GtkAttachOptions) (GTK_FILL | GTK_SHRINK), 5, 5);  
	clear_start_button = gtk_button_new_with_label("Clear Start");
	gtk_table_attach(GTK_TABLE(button_table), clear_start_button, 0,1,1,2, (GtkAttachOptions) (GTK_FILL | GTK_SHRINK), (GtkAttachOptions) (GTK_FILL | GTK_SHRINK), 5, 5);   
	clear_solution_button = gtk_button_new_with_label("Clear Solution");
	gtk_table_attach(GTK_TABLE(button_table), clear_solution_button, 0,1,2,3, (GtkAttachOptions) (GTK_FILL | GTK_SHRINK), (GtkAttachOptions) (GTK_FILL | GTK_SHRINK), 5, 5); 
	
	gtk_table_attach(GTK_TABLE(sudoku_table), button_table, 3,4,2,3, (GtkAttachOptions) (GTK_FILL | GTK_SHRINK), (GtkAttachOptions) (GTK_FILL | GTK_SHRINK), 5, 5);
	
	starting_sudoku_label = gtk_label_new("Enter Starting Sudoku Below:");
	gtk_table_attach(GTK_TABLE(sudoku_table), starting_sudoku_label, 0,2,0,1, (GtkAttachOptions) (GTK_FILL | GTK_SHRINK), (GtkAttachOptions) (GTK_FILL | GTK_SHRINK), 5, 5);  
	completed_sudoku_label = gtk_label_new("Solved Sudoku:");
	gtk_table_attach(GTK_TABLE(sudoku_table), completed_sudoku_label, 4,5,0,1, (GtkAttachOptions) (GTK_FILL | GTK_SHRINK), (GtkAttachOptions) (GTK_FILL | GTK_SHRINK), 5, 5); 
	
	// File I/O stuff
	GtkWidget *file_label;
	GtkWidget *write_file_button;
	GtkWidget *read_file_button;
	
	file_label = gtk_label_new("Filename:");
	filename_entry = gtk_entry_new();
	gtk_entry_set_max_length(GTK_ENTRY(filename_entry), 40);
	write_file_button = gtk_button_new_with_label("write file");
	read_file_button = gtk_button_new_with_label("read file");
	
	gtk_table_attach(GTK_TABLE(sudoku_table), file_label, 0,1,4,5, (GtkAttachOptions) (GTK_FILL | GTK_SHRINK), (GtkAttachOptions) (GTK_FILL | GTK_SHRINK), 5, 20); 
	gtk_table_attach(GTK_TABLE(sudoku_table), filename_entry, 1,4,4,5, (GtkAttachOptions) (GTK_FILL | GTK_SHRINK), (GtkAttachOptions) (GTK_FILL | GTK_SHRINK), 5, 20); 
	gtk_table_attach(GTK_TABLE(sudoku_table), read_file_button, 4,5,4,5, (GtkAttachOptions) (GTK_FILL | GTK_SHRINK), (GtkAttachOptions) (GTK_FILL | GTK_SHRINK), 5, 20);
	gtk_table_attach(GTK_TABLE(sudoku_table), write_file_button, 5,6,4,5, (GtkAttachOptions) (GTK_FILL | GTK_SHRINK), (GtkAttachOptions) (GTK_FILL | GTK_SHRINK), 5, 20);
	
        
    // flag all widgets to be displayed (otherwise it wont show up)
    gtk_widget_show_all  (window);
    
    // connect callback functions to relevant widgets    
    g_signal_connect(solve_button, "clicked", G_CALLBACK(solve_button_press), NULL);
    g_signal_connect(clear_start_button, "clicked", G_CALLBACK(clear_start_button_press), NULL);
    g_signal_connect(clear_solution_button, "clicked", G_CALLBACK(clear_solution_button_press), NULL);
    g_signal_connect(write_file_button, "clicked", G_CALLBACK(write_to_file), NULL);
    g_signal_connect(read_file_button, "clicked", G_CALLBACK(read_from_file), NULL);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    
    
    gtk_main ();
    
    return 0;
}
