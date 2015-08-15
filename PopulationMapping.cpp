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

enum MODE {
  DIVIDED,
  FINAL
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
// モード
int g_mode = DIVIDED;

typedef struct Cell {
  int landType;
  bool selected;

  Cell(int landType = OCEAN){
    this->landType = landType;
    this->selected = false;
  }

  bool isLand(){
    return landType == LAND;
  }
} CELL;

typedef struct Area {
  int id;
  int y1;
  int x1;
  int y2;
  int x2;
  int dividedCount;
  int landCount;
  int s;
  int population;

  Area(int y1 = UNDEFINED, int x1 = UNDEFINED, int y2 = UNDEFINED, int x2 = UNDEFINED){
    this->id = g_areaId;
    this->y1 = y1;
    this->dividedCount = 0;
    this->x1 = x1;
    this->y2 = y2;
    this->x2 = x2;
    this->landCount = 0;
    this->s = abs(y2-y1) * abs(x2-x1);
    this->population = UNDEFINED;

    g_areaId += 1;
  }

  bool operator >(const Area &a) const{
    if(g_mode == DIVIDED){
      return population < a.population;
    }else{
      return population/(double)landCount < a.population/(double)a.landCount;
    }
  }
} AREA;

// 世界地図
CELL g_worldMap[MAX_H][MAX_W];


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

    CELL* getCell(int y, int x){
      return &g_worldMap[y][x];
    }

    int calcLandCount(int y1, int x1, int y2, int x2){
      int landCount = 0;

      for(int y = y2; y <= y1; y++){
        for(int x = x1; x <= x2; x++){
          CELL *cell = getCell((g_height-1-y),x);

          landCount += (int)cell->isLand();
        }
      }

      return landCount;
    }

    void selectArea(int y1, int x1, int y2, int x2){
      for(int y = y2; y <= y1; y++){
        for(int x = x1; x <= x2; x++){
          CELL *cell = getCell((g_height-1-y),x);
          if(cell->isLand()){
            cell->selected = true;
          }
        }
      }
    }

    vector<AREA> research(){
      vector<AREA> result;
      priority_queue<AREA, vector<AREA>, greater<AREA> > que;
      priority_queue<AREA, vector<AREA>, greater<AREA> > fque;
      que.push(AREA(g_height-1, 0, 0, g_width-1));

      for(int i = 0; i < 40 && !que.empty(); i++){
        AREA area = que.top(); que.pop();
        //fprintf(stderr,"areaId = %d, population = %d\n", area.id, area.population);

        if(area.dividedCount > 10){
          fque.push(area);
        }

        vector<AREA> areaList = divideArea2(area);

        for(int areaId = 0; areaId < areaList.size(); areaId++){
          AREA a = areaList[areaId];

          int population = queryRegion(a.x1, a.y1, a.x2, a.y2);
          //int population = Population::queryRegion(a.x1, (g_height-1)-a.y1, a.x2, (g_height-1)-a.y2);
          a.population = population;
          a.landCount = calcLandCount(a.y1, a.x1, a.y2, a.x2);
          //fprintf(stderr,"y1 = %d, x1 = %d, y2 = %d, x2 = %d, landCount = %d, population = %d\n", a.y1, a.x1, a.y2, a.x2, a.landCount, population);

          if(a.landCount > 0){
            que.push(a);
          }
        }
      }

      int currentPopulation = g_totalPopulation;
      g_mode = FINAL;

      while(!que.empty()){
        AREA area = que.top(); que.pop();
        fque.push(area);
      }

      while(!fque.empty() && currentPopulation >= g_limitPopulation){
        AREA area = fque.top(); fque.pop();
        currentPopulation -= area.population;
      }

      while(!fque.empty()){
        AREA area = fque.top(); fque.pop();
        selectArea(area.y1, area.x1, area.y2, area.x2);
      }

      return result;
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
    vector<AREA> divideArea2(AREA area){
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

      AREA area1;
      AREA area2;

      if(height > width){
        area1 = AREA(p4.y-1, p4.x, p3.y, p3.x);
        area2 = AREA(p7.y, p7.x, p6.y, p6.x);
      }else{
        area1 = AREA(p7.y, p7.x, p2.y, p2.x);
        area2 = AREA(p8.y, p8.x+1, p3.y, p3.x);
      }

      area1.dividedCount = area.dividedCount + 1;
      area2.dividedCount = area.dividedCount + 1;

      vector<AREA> areaList;
      areaList.push_back(area1);
      areaList.push_back(area2);

      return areaList;
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

      area1.dividedCount = area.dividedCount + 1;
      area2.dividedCount = area.dividedCount + 1;
      area3.dividedCount = area.dividedCount + 1;
      area4.dividedCount = area.dividedCount + 1;

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
      research();

      for(int y = 0; y < g_height; y++){
        string line = "";

        for(int x = 0; x < g_width; x++){
          CELL *cell = getCell(y, x);

          if(cell->selected){
            line += 'X';
          }else{
            line += '.';
          }
        }
        result.push_back(line);
      }

      return result;
    }

    void showMap(){
      for(int y = 0; y < g_height; y++){
        for(int x = 0; x < g_width; x++){
          CELL *cell = getCell(y,x);

          if(cell->isLand()){
            fprintf(stderr,"X");
          }else{
            fprintf(stderr,".");
          }
        }
        fprintf(stderr,"\n");
      }
    }
};

int main(){
  int maxPercentage,totalPopulation,height;string line;vector<string> worldMap;
  cin >> maxPercentage;
  cin >> height;
  
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
