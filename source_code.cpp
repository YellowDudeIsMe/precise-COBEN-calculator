#pragma GCC optimize("Ofast")
#pragma GCC target("avx,avx2,popcnt,lzcnt,abm,bmi,bmi2,fma,tune=native")

#include <fstream>
#include <boost/lexical_cast.hpp>

#define nPk(n, k) ((n >= k && k >= 0) ? P[n][k] : 0.0L)
#define nCk(n, k) ((n >= k && k >= 0) ? C[n][k] : 0.0L)

int main(){
    std::ios_base::sync_with_stdio(false);

    //read input file
    std::fstream input("input.txt");
    
    int playerCount;
	int elimCount;
    
    if (input.is_open()){
    	std::string tmp;
    	
    	getline(input, tmp);
    	playerCount = boost::lexical_cast<int>(tmp);
    	
    	getline(input, tmp);
    	elimCount = std::min(playerCount, boost::lexical_cast<int>(tmp));
	}
	
	unsigned long long playerScore[playerCount]; //sort in descending order
	unsigned long long addScore[playerCount]; //sort in descending order
	
    if (input.is_open()){	
    	std::string tmp;
    	
    	for (int i = 0; i < playerCount; i++){
    		getline(input, tmp, ',');
    		tmp.erase(std::remove(tmp.begin(), tmp.end(), ' '), tmp.end());
    		playerScore[i] = boost::lexical_cast<unsigned long long>(tmp);
		}
		
		getline(input, tmp);
		
    	for (int i = 0; i < playerCount; i++){
    		getline(input, tmp, ',');
    		tmp.erase(std::remove(tmp.begin(), tmp.end(), ' '), tmp.end());
    		addScore[i] = boost::lexical_cast<unsigned long long>(tmp);
		}
	}
	
	input.close();
	
	long double fact[playerCount+1], P[playerCount+1][playerCount+1], C[playerCount+1][playerCount+1];
	long double coben[playerCount];
	int immunity = (elimCount == 0) ? playerCount : 0;
    
    std::fill_n(coben, playerCount, 0.0L);

	//create a factorial array fact[n], permutation and combination lookup tables, P[n][k], C[n][k]
	fact[0] = 1.0L;
	P[0][0] = 1.0L; C[0][0] = 1.0L;
	P[0][1] = 0.0L; C[0][1] = 0.0L;
	for (int i = 1; i <= playerCount; i++){
		fact[i] = fact[i-1] * i;
		P[i][0] = 1.0L;
		C[i][0] = 1.0L;
		for (int j = 1; j < i; j++){
			C[i][j] = C[i-1][j-1] + C[i-1][j];
			P[i][j] = C[i][j] * fact[j];
		}
		C[i][i] = C[i-1][i-1];
		P[i][i] = C[i][i] * fact[i];
	}
	
	//a list for repetitive playerScore
	int rep[playerCount];
	for (int i = 0, count = 1; i < playerCount; i++){
		if (i+1 < playerCount && playerScore[i] == playerScore[i+1]){
			rep[i] = 0;
			count++;
		} else {
			rep[i] = count;
			count = 1;
		}
	}
	
	int div[playerCount<<2]; //"dividers" array, will see again later
	
	//check for true immunity
	while (immunity < playerCount){
		unsigned long long ancSum = playerScore[immunity] + addScore[playerCount-1];
		int tmp = elimCount;
		for (int a = playerCount-1, b = 0; a >= 0; a--){
			if (a == immunity || --tmp > 0) continue;
			
			if (playerScore[a] + addScore[b] < ancSum){
				immunity++;
				goto _X;
			} else b++;
		}
		break;
		_X: continue;
	}
	
	//main code
	for (int i = playerCount-rep[playerCount-1]; i >= 0; i--){
		if (rep[i] == 0){
			coben[i] = coben[i+1];
			continue;
		}
		
		for (int j = playerCount-1; j >= 0 && (j + i + elimCount >= playerCount || addScore[j] == addScore[j+1]); j--){
			unsigned long long ancSum = playerScore[i] + addScore[j];
			
			//determine "dividers", 0 is for winning, 1 is for ties. 2 is from the rep array, 3 is for the number of passed players (for dp)
			//Note: div doesnt have to go through all players (skip players with same scores)
			int divCount = 0, maxTie = 0;
			
			for (int a = playerCount-1, b = 0; a >= 0; a--){
				if (rep[a] == 0) continue;
				if (a == i){
					if (rep[a] > 1){
						div[divCount+2] = rep[a]-1;
					} else continue;
				} else div[divCount+2] = rep[a];

				if (a >= i) div[divCount+3] = playerCount-1-a;
				else div[divCount+3] = playerCount-2-a;
				
				while (b < playerCount && playerScore[a] + addScore[b] > ancSum) b++;
				if (b > j) div[divCount] = b-1;
				else div[divCount] = b;
				
				while (b < playerCount && playerScore[a] + addScore[b] == ancSum) b++;
				if (b > j) div[divCount+1] = b-1;
				else div[divCount+1] = b;
				
				if (div[divCount] != div[divCount+1]) maxTie += std::min(div[divCount+1] - div[divCount], div[divCount+2]);
				
				divCount+=4;
			}
			

			//make the dp table
			long double dp[elimCount][elimCount][maxTie+1]; //actually the whole structure is 4d, but we flatten this
			std::fill_n(&dp[0][0][0], elimCount * elimCount * (maxTie+1), 0.0L);
			dp[0][0][0] = 1.0L;
			
			for (int c = 0; c < divCount; c+=4){
				int repe = div[c+2];
				int seg_win = div[c];
				int seg_tie = div[c+1] - seg_win;
				int seg_lose;
				if (c+4 < divCount) seg_lose = div[c+4] - div[c+1];
				else seg_lose = playerCount - 1 - div[c+1];
				int curr = div[c+3];
				
				for (int x = elimCount-1; x >= 0; x--){
					for (int y = elimCount-1; y >= x; y--){
						for (int z = maxTie; z >= 0; z--){
							long double final = 0.0L;
							
							for (int win = 0; win <= repe; win++){
								for (int tie = 0, lose = repe - win; tie <= repe - win && tie <= z; tie++, lose--){
									if (y < lose) continue;
									
									int tmp1 = y-lose;
									
									for (int t = 0; t <= x; t++){
										for (int k = 0; k <= lose; k++){
											if (x > tmp1+k+t || x < k+t) continue;
											
											int tmp2 = x-t-k;
											
											final += dp[tmp2][tmp1][z-tie]
													*nPk(seg_win - curr + tmp1 - tmp2, win)*nCk(repe, win)
													*nPk(seg_tie, tie)*nCk(lose + tie, tie)
													*nPk(seg_lose, k)*nCk(lose, k)
													*nPk(tmp1 - tmp2, t)*nCk(seg_tie + seg_lose - tie - k, t);
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
			for (int a = 0; a < elimCount; a++){
				for (int b = 0; b <= maxTie; b++){
					coben[i] += dp[a][a][b] * std::min(elimCount - a, b+1) / (b+1);
				}
			}
		}
	}
	
	//print the results
	long double tmp = 100.0L * elimCount, ret;
	int i = 1;
	for (; i <= playerCount-rep[playerCount-1]; i++, tmp -= ret){
		ret = coben[i-1] * 100.0L / fact[playerCount];
		if (i <= immunity) printf("Athlete #%d : %.15Lf %% (IMMUNE!)\n", i, ret);
		else printf("Athlete #%d : %.15Lf %%\n", i, ret);
	}
	
	ret = tmp/rep[playerCount-1];
	
	for (; i <= playerCount; i++){
		if (i <= immunity) printf("Athlete #%d : %.15Lf %% (IMMUNE!)\n", i, ret);
		else printf("Athlete #%d : %.15Lf %%\n", i, ret);
	}
	
	printf("\nPress any key to exit");
	std::getchar();

    return 0;
}