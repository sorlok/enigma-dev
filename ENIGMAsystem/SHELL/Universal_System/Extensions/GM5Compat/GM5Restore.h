/** 
 * TODO: Currently unsure of how the licensing here works.
**/

#ifndef ENIGMA_GM5COMPAT_FUNCTIONS__H
#define ENIGMA_GM5COMPAT_FUNCTIONS__H


namespace enigma_user
{

//Sprites
void sprite_discard(int num);
void sprite_restore(int num);

//Backgrounds
void background_discard(int num);
void background_restore(int num);

//Sounds
void sound_discard(int num);
void sound_restore(int num);

//General
void discard_all();

}

#endif // ENIGMA_GM5COMPAT_FUNCTIONS__H

