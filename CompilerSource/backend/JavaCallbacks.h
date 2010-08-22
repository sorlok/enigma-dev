/*
 * Copyright (C) 2010 IsmAvatar <IsmAvatar@gmail.com>, Josh Ventura
 * 
 * This file is part of Enigma Plugin.
 * Enigma Plugin is free software and comes with ABSOLUTELY NO WARRANTY.
 * See LICENSE for details. If you can find it. But I mean, this is just
 * a piece of GNU General Public License software. So you can refer to it
 * instead, if you like. Version 3, or any later version.
 */

struct EnigmaCallbacks
{
 //Opens the EnigmaFrame
 void (*dia_open) ();

 //Appends a given text to the frame log
 void (*dia_add) (const char *);

 //Clears the frame log
 void (*dia_clear) ();

 //Sets the progress bar (0-100)
 void (*dia_progress) (int);

 //Applies a given text to the progress bar
 void (*dia_progress_text) (const char *);
};

//Opens the EnigmaFrame
extern void (*ide_dia_open) ();
//Appends a given text to the frame log
extern void (*ide_dia_add) (const char *);
//Clears the frame log
extern void (*ide_dia_clear) ();
//Sets the progress bar (0-100)
extern void (*ide_dia_progress) (int);
//Applies a given text to the progress bar
extern void (*ide_dia_progress_text) (const char *);
