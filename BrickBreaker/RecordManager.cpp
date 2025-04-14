#include "pch.h"
#include "RecordManager.h"
#include <fstream>

int HighScoreManager::LoadHighScore() {  
   int highScore = 0;  
   std::ifstream file("highscore.txt");  
   if (file.is_open()) {  
       file >> highScore;  
       file.close();  
   }  
   return highScore;  
}  

void HighScoreManager::SaveHighScore(int score) {  
   std::ofstream file("highscore.txt");
   if (file.is_open()) {  
       file << score;  
       file.close();  
   }  
}
