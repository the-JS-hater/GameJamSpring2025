#include "../inc/audioManager.hpp"


Sound smack01;
Sound smack02;
Sound smack03;
Sound smack04;
Sound smack05;


void initSmacks()
{
	smack01 = LoadSound("resources/audio/Smack-01.wav"); 
	smack02 = LoadSound("resources/audio/Smack-02.wav");
	smack03 = LoadSound("resources/audio/Smack-03.wav");
	smack04 = LoadSound("resources/audio/Smack-04.wav");
	smack05 = LoadSound("resources/audio/Smack-05.wav");
}

void playRandomSmack()
{
	switch(rand()%5)
	{
		case 0: PlaySound(smack01); break;
		case 1: PlaySound(smack02); break;
		case 2: PlaySound(smack03); break;
		case 3: PlaySound(smack04); break;
		case 4: PlaySound(smack05); break;
		default: return;
	}
}
