#ifndef INTERFACE_H
#define INTERFACE_H

#include "definitions.h" // Butuh definisi umum, meskipun tidak eksplisit di prototipe

void clear_screen();
void pause_screen();
void displayMainMenu();
void handleDoctorManagement();
void handleScheduleManagement();
void handleScheduleView();

#endif // INTERFACE_H