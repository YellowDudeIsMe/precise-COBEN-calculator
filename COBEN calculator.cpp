#pragma GCC optimize("Ofast")
#pragma GCC target("avx,avx2,fma")

#include <fstream>
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/lexical_cast.hpp>

#define nPk(n, k) ((n >= k && k >= 0) ? P[n][k] : 0ULL)
#define nCk(n, k) ((n >= k && k >= 0) ? C[n][k] : 0ULL)

int main(){
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr); std::cout.tie(nullptr);

    //read input file
    std::fstream input("input.txt");
    
    int16_t playerCount;
	int16_t elimCount;
    
    if (input.is_open()){
    	std::string tmp;
    	
    	getline(input, tmp);
    	playerCount = boost::lexical_cast<int16_t>(tmp);
    	
    	getline(input, tmp);
    	elimCount = boost::lexical_cast<int16_t>(tmp);
	}
	
	uint64_t playerScore[playerCount]; //sort in descending order
	uint64_t addScore[playerCount]; //sort in descending order
	
    if (input.is_open()){	
    	std::string tmp;
    	
    	for (int16_t i = 0; i < playerCount; i++){
    		getline(input, tmp, ',');
    		tmp.erase(std::remove(tmp.begin(), tmp.end(), ' '), tmp.end());
    		playerScore[i] = boost::lexical_cast<uint64_t>(tmp);
		}
		
		getline(input, tmp);
		
    	for (int16_t i = 0; i < playerCount; i++){
    		getline(input, tmp, ',');
    		tmp.erase(std::remove(tmp.begin(), tmp.end(), ' '), tmp.end());
    		addScore[i] = boost::lexical_cast<uint64_t>(tmp);
		}
	}
	
	input.close();
	
	boost::multiprecision::int1024_t fact[playerCount+1], P[playerCount+1][playerCount+1], C[playerCount+1][playerCount+1];
	long double coben[playerCount];
    
    std::fill_n(coben, playerCount, 0);

	//create a factorial array fact[n], permutation and combination lookup tables, P[n][k], C[n][k]
	fact[0] = 1ULL;
	P[0][0] = 1ULL; C[0][0] = 1ULL;
	P[0][1] = 0ULL; C[0][1] = 0ULL;
	for (int16_t i = 1; i <= playerCount; i++){
		fact[i] = fact[i-1] * i;
		P[i][0] = 1ULL;
		C[i][0] = 1ULL;
		for (int16_t j = 1; j < i; j++){
			C[i][j] = C[i-1][j-1] + C[i-1][j];
			P[i][j] = C[i][j] * fact[j];
		}
		C[i][i] = C[i-1][i-1];
		P[i][i] = C[i][i] * fact[i];
	}
	
	//a list for repetitive playerScore
	int16_t rep[playerCount];
	for (int16_t i = 0, count = 1; i < playerCount; i++){
		if (i+1 < playerCount && playerScore[i] == playerScore[i+1]){
			rep[i] = 0;
			count++;
		} else {
			rep[i] = count;
			count = 1;
		}
	}
	
	//main code
	for (int16_t i = playerCount-1; i >= 0; i--){
		if (rep[i] == 0){
			coben[i] = coben[i+1];
			continue;
		}
		
		for (int16_t j = playerCount-1; j + i + 1 >= playerCount || (j >= 0 && addScore[j] == addScore[j+1]); j--){
			uint64_t ancSum = playerScore[i] + addScore[j];
			
			//determine "dividers", 0 is for winning, 1 is for ties. 2 is from the rep array, 3 is for the number of passed players (for dp)
			//Note: div doesnt have to go through all players (skip players with same scores)
			int16_t div[playerCount][4], divCount = 0, maxTie = 0;
			
			for (int16_t a = playerCount-1, b = 0; a >= 0; a--){
				if (rep[a] == 0) continue;
				if (a == i){
					if (rep[a] > 1){
						div[divCount][2] = rep[a]-1;
					} else continue;
				} else div[divCount][2] = rep[a];

				if (a >= i) div[divCount][3] = playerCount-1-a;
				else div[divCount][3] = playerCount-2-a;
				
				while (b < playerCount && playerScore[a] + addScore[b] > ancSum) b++;
				if (b > j) div[divCount][0] = b-1;
				else div[divCount][0] = b;
				
				while (b < playerCount && playerScore[a] + addScore[b] == ancSum) b++;
				if (b > j) div[divCount][1] = b-1;
				else div[divCount][1] = b;
				
				if (div[divCount][0] != div[divCount][1]) maxTie += std::min((int16_t)(div[divCount][1] - div[divCount][0]), div[divCount][2]);
				
				divCount++;	
			}
			

			//make the dp table
			boost::multiprecision::int1024_t dp[elimCount][elimCount][maxTie+1]; //actually the whole structure is 4d, but we flatten this
			std::fill_n(&dp[0][0][0], elimCount * elimCount * (maxTie+1), 0ULL);
			dp[0][0][0] = 1ULL;
			
			for (int16_t c = 0; c < divCount; c++){
				int16_t repe = div[c][2];
				int16_t seg_win = div[c][0];
				int16_t seg_tie = div[c][1] - seg_win;
				int16_t seg_lose;
				if (c+1 < divCount) seg_lose = div[c+1][0] - div[c][1];
				else seg_lose = playerCount - 1 - div[c][1];
				int16_t curr = div[c][3];
				
				for (int16_t x = elimCount-1; x >= 0; x--){
					for (int16_t y = elimCount-1; y >= x; y--){
						for (int16_t z = maxTie; z >= 0; z--){
							boost::multiprecision::int1024_t final = 0ULL;
							
							for (int16_t win = 0; win <= repe; win++){
								for (int16_t tie = 0; tie <= repe - win; tie++){
									if (tie > z) break;
									
									int16_t lose = repe - win - tie;
									if (y < lose) continue;
									for (int16_t t = 0; t <= x; t++){
										for (int16_t k = 0; k <= lose; k++){
											if (x < k+t || x-t-k > y-lose) continue;
											
											int16_t tmp = (y-lose) - (x-t-k);
											
											final += dp[x-t-k][y-lose][z-tie]
													*nPk(seg_win - (curr - tmp), win)*nCk(repe, win)
													*nPk(seg_tie, tie)*nCk(lose + tie, tie)
													*nPk(seg_lose, k)*nCk(lose, k)
													*nPk(tmp, t)*nCk(seg_tie + seg_lose - tie - k, t);
										}
									}									
								}
							}
							
							dp[x][y][z] = final;
						}
					}
				}
			}
			
			//tally up the total number of elimination
			for (int16_t a = 0; a < elimCount; a++){
				for (int16_t b = 0; b <= maxTie; b++){
					coben[i] += (long double)dp[a][a][b] * std::min(elimCount - a, b+1) / (b+1);
				}
			}
		}
	}
	
	//print the results
	for (int16_t i = 0; i < playerCount; i++){
		printf("Player #%d : %.18Lf %%\n", i+1, (long double)(coben[i] * 100ULL) / (long double)(fact[playerCount]));
	}
	
	printf("\nPress any key to exit");
	std::getchar();

    return 0;
}
