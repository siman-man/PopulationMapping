#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <limits.h>
#include <string>
#include <string.h>
#include <sstream>
#include <set>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <stack>
#include <queue>

using namespace std;

typedef long long ll;

int queryRegion(int x1, int y1, int x2, int y2){
  int num = 0;
  cout << "?" << endl;
  cout << x1 << ' ' << y1 << ' ' <<  x2 << ' ' << y2 << endl;
  cin >> num;
  return num;
}

// フィールドの高さ
int g_height;
// フィールドの横幅
int g_width;
// 世界の総人口
int g_totalPopulation;
// 割合
int g_maxPercentage;

class PopulationMapping {
  public:
    void init(int maxPercentage, vector<string> worldMap, int totalPopulation){
      g_height = worldMap.size();
      g_width = worldMap[0].size();
      g_totalPopulation = totalPopulation;
      g_maxPercentage = maxPercentage;

      fprintf(stderr,"H = %d, W = %d\n", g_height, g_width);
      fprintf(stderr,"totalPopulation = %d, g_maxPercentage = %d\n", g_totalPopulation, g_maxPercentage);
    }

    vector <string> mapPopulation(int maxPercentage, vector<string> worldMap, int totalPopulation){
      init(maxPercentage, worldMap, totalPopulation);
      vector<string> result;

      int num = queryRegion(0, 0, 5, 5);
      fprintf(stderr,"num = %d\n", num);

      for(int y = 0; y < g_height; y++){
        string line = "";

        for(int x = 0; x < g_width; x++){
          line += '.';
        }
        result.push_back(line);
      }

      return result;
    }
};

int main(){
  int maxPercentage, totalPopulation, height;
  cin >> maxPercentage;
  cin >> height;
  string line;
  vector<string> worldMap;

  for(int i = 0; i < height; i++){
    cin >> line;
    worldMap.push_back(line);
  }

  scanf("%d", &totalPopulation);

  PopulationMapping pm;

  vector<string> ret = pm.mapPopulation(maxPercentage, worldMap, totalPopulation);
  cout << ret.size() << endl;

  for(int i = 0; i < ret.size(); i++){
    cout << ret[i] << endl;
  }

  return 0;
}
