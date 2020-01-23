#include <iostream> 
#include <string> 

#include "olcConsoleGameEngine.h"

 

class As_FlappyBird : public olcConsoleGameEngine
{
public:
	As_FlappyBird()
	{
		m_sAppName = L"As Flappy Bird, but more better";
	}

private:
	float fBirdPositionInTheWorld = 0.0f;
	float fBirdVelocityInTheWorld = 0.0f;
	float fBirdAccelerationInTheWorld = 0.0f;

	float fGravityOfTheWorld = 100.0f;

	float fSectionWidth;
	std::list<int> listSection;
	float fLevelPosition = 0.0f;

	bool birdHasCollided = false, birdResetGame;
	int nNumberOfAttempts = 0;
	int nFlapingCount = 0;
	int nHighScoreFlapCount = 0;
protected:
	virtual bool OnUserCreate()
	{
		birdResetGame = true;
		listSection = { 0, 0, 0, 0, 0, 0 };
		fSectionWidth = (float)ScreenWidth() / (float)(listSection.size() - 1); // because cycled
		return true;
	}
 

	virtual bool OnUserUpdate(float fElapsedTime)
	{
		if (birdResetGame)
		{

			if (nFlapingCount > nHighScoreFlapCount)
				nHighScoreFlapCount = nFlapingCount;

			Fill(0, 0, ScreenWidth(), ScreenHeight(), L' ');
			DrawString((ScreenWidth() / 2 - 10), 24,  L"Flap score: " +
				std::to_wstring(nFlapingCount));
			
			DrawString((ScreenWidth() / 2 - 10), 26, L"Highest score: " +
				std::to_wstring(nHighScoreFlapCount));
			
			DrawString((ScreenWidth() / 2 - 10), 28, L"Attempt count: " +
				std::to_wstring(nNumberOfAttempts));
 
			DrawString((ScreenWidth() / 2 - 10), 30, 
				+ L"PLEASE PRESS SPACE FOR RESET");

			if (m_keys[VK_SPACE].bPressed)
			{
				birdHasCollided = false;
				birdResetGame = false;
				nNumberOfAttempts++;
				nFlapingCount = 0;
				fBirdAccelerationInTheWorld = 0.0f;
				fBirdPositionInTheWorld = ScreenWidth() / 2;
				fBirdVelocityInTheWorld = 0.0f;
				listSection = { 0, 0, 0, 0 };
			} 
		}
		if (birdHasCollided)
		{
			if (m_keys[VK_SPACE].bReleased)
				birdResetGame = true;
		} 
		else
		{
			if (m_keys[VK_SPACE].bPressed)
			{
				fBirdAccelerationInTheWorld = 0.0f;
				fBirdVelocityInTheWorld = -fGravityOfTheWorld / 4.0f;
				nFlapingCount++;
			}
			else
			{
				fBirdAccelerationInTheWorld += fGravityOfTheWorld * fElapsedTime;
			}
			Fill(0, 0, ScreenWidth(), ScreenHeight(), L' ');
			int nBirdX = (int)(ScreenWidth() / 3.0f); // Bird always in the same place

			fBirdVelocityInTheWorld += fBirdAccelerationInTheWorld * fElapsedTime;
			fBirdPositionInTheWorld += fBirdVelocityInTheWorld * fElapsedTime;

			fLevelPosition += 14.0f * fElapsedTime;

			if (fLevelPosition > fSectionWidth)
			{
				fLevelPosition -= fSectionWidth;
				listSection.pop_front();

				int someInteger = rand() % (ScreenHeight() - 20);
				someInteger = (someInteger <= 10) ? someInteger = 10 : someInteger; // if that not obstacle at all 
				listSection.push_back(someInteger);
			}

			Fill(0, 0, ScreenWidth(), ScreenHeight(), L' ');

			int sectionNumber = 0;
			for (auto elem : listSection)
			{
				if (elem != 0) // This is where can be rest depending on the pseudo-randomization
				{
					Fill(sectionNumber * fSectionWidth + 10 - fLevelPosition, ScreenHeight() - elem,
						sectionNumber * fSectionWidth + 15 - fLevelPosition, ScreenHeight(),
						PIXEL_SOLID, FG_GREEN);

					Fill(sectionNumber * fSectionWidth + 10 - fLevelPosition, 0,
						sectionNumber * fSectionWidth + 15 - fLevelPosition, ScreenHeight() - elem - 20,
						PIXEL_SOLID, FG_GREEN);
				}
				sectionNumber++;
			}

			// Last that need is a collision detection. That created 
			birdHasCollided = fBirdPositionInTheWorld < 2 || fBirdPositionInTheWorld > ScreenHeight() - 2 ||
				m_bufScreen[(int)(fBirdPositionInTheWorld + 0) * ScreenWidth() + nBirdX].Char.UnicodeChar != L' ' ||
				m_bufScreen[(int)(fBirdPositionInTheWorld + 1) * ScreenWidth() + nBirdX].Char.UnicodeChar != L' ' ||
				m_bufScreen[(int)(fBirdPositionInTheWorld + 0) * ScreenWidth() + nBirdX + 6].Char.UnicodeChar != L' ' ||
				m_bufScreen[(int)(fBirdPositionInTheWorld + 1) * ScreenWidth() + nBirdX + 6].Char.UnicodeChar != L' ';

			if (fBirdVelocityInTheWorld > 0)
			{
				DrawString(nBirdX, fBirdPositionInTheWorld + 0, L"\\\\\\");
				DrawString(nBirdX, fBirdPositionInTheWorld + 1, L"=======Q");
			}
			else
			{
				DrawString(nBirdX, fBirdPositionInTheWorld + 1, L"=======Q");
				DrawString(nBirdX, fBirdPositionInTheWorld - 1, L"///////");
			}
			
			DrawString(1, 1, L"Attempt: " + std::to_wstring(nNumberOfAttempts) + L" Your Flapping count: " + std::to_wstring(nFlapingCount) + L" Your High Score:" + std::to_wstring(nHighScoreFlapCount));
		}
		return true;
	}
};


int main()
{
	As_FlappyBird game;
	// width, height, fontw, fonth
	// world will as cycle from 0 to the end
	game.ConstructConsole(80, 48, 16, 16);
	game.Start();
	return 0;
}