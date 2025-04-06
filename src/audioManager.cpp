#include "../inc/audioManager.hpp"


Sound smack01;
Sound smack02;
Sound smack03;
Sound smack04;
Sound smack05;
Sound woosh01;
Sound woosh02;
Sound rarewoosh03;
Sound rarewoosh04;
Sound crowdSound01; 
Sound crowdSound02;
Sound crowdSound03;
Sound crowdSound04;
Sound crowdSound05;
Sound crowdSound06;
Sound crowdSound07;

// dont ask
Texture2D blood1;
Texture2D	blood2;
Texture2D	blood3;

void initSounds()
{
	smack01 = LoadSound("resources/audio/Smack-01.wav"); 
	smack02 = LoadSound("resources/audio/Smack-02.wav");
	smack03 = LoadSound("resources/audio/Smack-03.wav");
	smack04 = LoadSound("resources/audio/Smack-04.wav");
	smack05 = LoadSound("resources/audio/Smack-05.wav");

	woosh01 		= LoadSound("resources/audio/Woosh-01.wav");
	woosh02			= LoadSound("resources/audio/Woosh-02.wav");
	rarewoosh03	= LoadSound("resources/audio/Woosh-03.wav");
	rarewoosh04	= LoadSound("resources/audio/Woosh-04.wav");

	crowdSound01 = LoadSound("resources/audio/crowd-01.wav");
	crowdSound02 = LoadSound("resources/audio/crowd-02.wav");
	crowdSound03 = LoadSound("resources/audio/crowd-03.wav");
	crowdSound04 = LoadSound("resources/audio/crowd-04.wav");
	crowdSound05 = LoadSound("resources/audio/crowd-05.wav");
	crowdSound06 = LoadSound("resources/audio/crowd-06.wav");
	crowdSound07 = LoadSound("resources/audio/crowd-07.wav");

	// fuck it
	blood1 = LoadTexture("resources/sprites/Blood1.png"); 
  blood2 = LoadTexture("resources/sprites/Blood2.png");
  blood3 = LoadTexture("resources/sprites/Blood3.png");
}

Texture2D getRandomBlood()
{
	switch(rand()%3)
	{
		case 0: return blood1;
		case 1: return blood2;
		case 2: return blood3;
		default: return blood2;
	}
}

void playRandomCrowdAudio()
{
	// Advanced crowd A.I.
	if (rand()%100 < 98) return;

	switch(rand()%7)
	{
		case 0: PlaySound(crowdSound01); break;  
		case 1: PlaySound(crowdSound02); break;
		case 2: PlaySound(crowdSound03); break;
		case 3: PlaySound(crowdSound04); break;
		case 4: PlaySound(crowdSound05); break;
		case 5: PlaySound(crowdSound06); break;
		case 6: PlaySound(crowdSound07); break;
	}
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

void playRandomWoosh()
{
	switch(rand()%2)
	{
		case 0: PlaySound(woosh01); break;
		case 1: PlaySound(woosh02); break;
		default: return;
	}

	switch(rand()%10)
	{
		case 3: PlaySound(rarewoosh03); break;
		case 7: PlaySound(rarewoosh04); break;
		default: return;
	}
}
