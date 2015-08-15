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

const int MAX_H = 500;
const int MAX_W = 500;
const int UNDEFINED = -1;

unsigned long long xor128(){
  static unsigned long long rx=123456789, ry=362436069, rz=521288629, rw=88675123;
  unsigned long long rt = (rx ^ (rx<<11));
  rx=ry; ry=rz; rz=rw;
  return (rw=(rw^(rw>>19))^(rt^(rt>>8)));
}

enum CellType{
  LAND,
  OCEAN,
  CITY
};

typedef struct Coord {
  int y;
  int x;
  
  Coord(int y = UNDEFINED, int x = UNDEFINED){
    this->y = y;
    this->x = x;
  }
} COORD;

// フィールドの高さ
int g_height;
// フィールドの横幅
int g_width;
// 世界の総人口
int g_totalPopulation;
// 割合
int g_maxPercentage;
// 制限人口
double g_limitPopulation;
// 陸地の数
int g_landCount = 0;
// エリアID
int g_areaId = 0;

typedef struct Cell {
  int landType;

  Cell(int landType = OCEAN){
    this->landType = landType;
  }
} CELL;

typedef struct Area {
  int id;
  int y1;
  int x1;
  int y2;
  int x2;
  int population;

  Area(int y1, int x1, int y2, int x2){
    this->id = g_areaId;
    this->y1 = y1;
    this->x1 = x1;
    this->y2 = y2;
    this->x2 = x2;
    this->population = UNDEFINED;

    g_areaId += 1;
  }

  bool operator >(const Area &a) const{
    return population < a.population;
  }
} AREA;

// 世界地図
CELL g_worldMap[MAX_H][MAX_W];

int queryRegion(int x1, int y1, int x2, int y2){
  y1 = (g_height-1) - y1;
  y2 = (g_height-1) - y2;
  int num = 0;
  //fprintf(stderr,"send query: y1 = %d, x1 = %d, y2 = %d, x2 = %d\n", y1, x1, y2, x2);
  cout << "?" << endl;
  cout << x1 << ' ' << y1 << ' ' <<  x2 << ' ' << y2 << endl;
  cin >> num;
  return num;
}

class PopulationMapping {
  public:
    void init(int maxPercentage, vector<string> worldMap, int totalPopulation){
      g_height = worldMap.size();
      g_width = worldMap[0].size();
      g_totalPopulation = totalPopulation;
      g_maxPercentage = maxPercentage;
      g_limitPopulation = g_totalPopulation * (g_maxPercentage/100.0);

      g_landCount = 0;

      for(int y = 0; y < g_height; y++){
        for(int x = 0; x < g_width; x++){
          CELL cell;
          char ch = worldMap[y][x];

          if(ch == 'X'){
            cell.landType = LAND;
            g_landCount += 1;
          }else{
            cell.landType = OCEAN;
          }

          g_worldMap[y][x] = cell;
        }
      }

      fprintf(stderr,"H = %d, W = %d\n", g_height, g_width);
      fprintf(stderr,"totalPopulation = %d, g_maxPercentage = %d\n", g_totalPopulation, g_maxPercentage);
      fprintf(stderr,"limitPopulation = %4.2f\n", g_limitPopulation);
    }

    CELL* getCell(int y, int x){
      return &g_worldMap[y][x];
    }

    void research(){
      priority_queue<AREA, vector<AREA>, greater<AREA> > que;
      que.push(AREA(g_height-1, 0, 0, g_width-1));

      for(int i = 0; i < 20 && !que.empty(); i++){
        AREA area = que.top(); que.pop();
        fprintf(stderr,"areaId = %d, population = %d\n", area.id, area.population);

        vector<AREA> areaList = divideArea4(area);

        for(int areaId = 0; areaId < areaList.size(); areaId++){
          AREA a = areaList[areaId];

          int population = queryRegion(a.x1, a.y1, a.x2, a.y2);
          a.population = population;
          fprintf(stderr,"y1 = %d, x1 = %d, y2 = %d, x2 = %d, population = %d\n", a.y1, a.x1, a.y2, a.x2, population);

          que.push(a);
        }
      }
    }

    /*
     *   p1  p2  p3
     *    +--+--+
     *    |  |  |
     * p4 +--+--+ p6
     *    |  |  |
     *    +--+--+
     *  p7  p8  p9
     */
    vector<AREA> divideArea4(AREA area){
      int height = area.y1 - area.y2 + 1;
      int width  = area.x2 - area.x1 + 1;
      //fprintf(stderr,"height = %d, width = %d\n", height, width);
      int diff_h = height/2-1;
      int diff_w = width/2-1;

      COORD p1(area.y2,        area.x1);
      COORD p2(area.y2,        area.x1+diff_w);
      COORD p3(area.y2,        area.x2);
      COORD p4(area.y2+diff_h, area.x1);
      COORD p5(area.y2+diff_h, area.x1+diff_w);
      COORD p6(area.y2+diff_h, area.x2);
      COORD p7(area.y1,        area.x1);
      COORD p8(area.y1,        area.x1+diff_w);
      COORD p9(area.y1,        area.x2);

      AREA area1 = AREA(p4.y, p4.x, p2.y, p2.x);
      AREA area2 = AREA(p5.y, p5.x+1, p3.y, p3.x);
      AREA area3 = AREA(p7.y, p7.x, p5.y+1, p5.x);
      AREA area4 = AREA(p8.y, p8.x+1, p6.y+1, p6.x);

      vector<AREA> areaList;
      areaList.push_back(area1);
      areaList.push_back(area2);
      areaList.push_back(area3);
      areaList.push_back(area4);

      return areaList;
    }

    vector <string> mapPopulation(int maxPercentage, vector<string> worldMap, int totalPopulation){
      init(maxPercentage, worldMap, totalPopulation);
      vector<string> result;

      for(int y = 0; y < g_height; y++){
        string line = "";

        for(int x = 0; x < g_width; x++){
          CELL *cell = getCell(y, x);

          if(cell->landType == LAND && xor128() % 100 < 10){
            line += 'X';
          }else{
            line += '.';
          }
        }
        result.push_back(line);
      }

      research();

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
