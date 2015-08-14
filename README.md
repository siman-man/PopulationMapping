# PopulationMapping
MM87

# 問題文
  
The world, and all the people that live in it, is represented as a rectangular grid, 
この世界とそこに住んでいる人々を四角のグリッドで表します。

with a certain number of people living in each cell of the grid. 
各セルには一定数の人々が暮らしています。

For the sake of making pretty maps, you are working on finding areas of the world with population densities of particular interest.
この大きなマップを調査するために、あなたはある特定の大きな地域の人口密度の調査を行います。


Initially you are given the map of the world's oceans and land in terms of grid units, 
まず始めにあなたには海と陸地のグリッドで構成されたマップが与えられます

and the total population of the world. People live only on land cells. 
そして世界の総人口数も与えられます。人々は陸地セルでしか生きられません。

Because of the wealth of research that various others have already done, 
世界の富のための調査各国が既に行っているような

it is possible to determine the population of any rectangular section of the world 
各四角に住んでいる人口の数を決めることが出来ます

(without regard to the various densities within that region, or the exact population of any cell included in that region). 

In your quest to answer this question, you may issue any number of such queries. 
あなたの目的はこの問に答えることです。 あなたはいくつかの問い合わせを発行することができます

Of course, answering each query comes with a cost, and thus your goal is to balance 
もちろん、 各質問の答えにはコストがかかります。　そしてあなたの目的はこの答えの質と

the accuracy of your answer against the total number of queries you need to issue.
質問のコストの精度を考えることです。


Your task is to find the largest possible area of the world such that its total population 
あなたのタスクは最小限の質問で世界人口を超えないような住める人々の数をきめた四角を見つけ出すことです。

does not exceed a given percentage of the world's total population, using the least amount of queries.


# 実装詳細

Your code should implement the method mapPopulation(int maxPercentage, String[] worldMap, int totalPopulation). 
あなたはmapPopulation(int maxPercentage, String[] worldMap, int totalPopulation)を実装する必要があります

Your mapPopulation method will be called once and should return a String[] containing your selected area.
あなたのmapPopulationはString[]を返し、これにはあなたが選択したエリアが含まれています


- maxPercentage gives you the maximum percentage of the total population that your selected area may contain.
- maxPercentage : あなたが選択したエリアに含まれている、総人口数の割合です

- worldMap gives you the world map containing W columns and H rows. Each String contains a row of the map. 
           Ocean is denoted by the '.' character and land by the 'X' character.
- worldMap : あなたに与えられる高さH横幅Wのマップです、各要素には文字列が含まれています。海は「.」で陸地は「X」で表されます

- totalPopulation gives you the total population on the entire given world map.
- totalPopulation : この世界の総人口数

You must return a String[] containing your selected area. The size of your return must be the same as the input worldMap. 
あなたが返す値のString[]には選択されたエリアが含まれていなければいけません。 さいずはworldMapと一緒な必要があります。

Each String contains a row of the map. Each cell that you want to select must contain the 'X' character, otherwise unselected cells must contain the '.' character.

You can call the queryRegion(int x1, int y1, int x2, int y2) method to get the population size within a specific rectangular region. 
あなたはqueryRegionを呼ぶことが出来る。このメソッドでは指定した範囲の人口数を取得することが出来る。

(x1,y1) specifies the lower left corner and (x2,y2) the upper right corner of the rectangular region. 
(x1,y1)は左下、(x2, y2)は右上を表す

Coordinates are inclusive. A row is specified by y1 and y2. A column is specified by x1 and x2. 
座標は世界に含まれている。 yは行でxが列である。

The method will return an int containing the population size within the region. The following rules apply:
このメソッドは指定したエリアの人口数を返すが以下のルールが適用される

- x1 <= x2 and y1 <= y2
- 0 <= x1,x2 < W
- 0 <= y1,y2 < H

# テストケースの生成

Please look at the visualizer source code for the exact details about test case generation. 
正確なスコア生成やマップ生成を知りたいのであればビジュアライザのソースコードを参照してください。

The world size W and H is randomly selected between 50 and 500. The world is filled with ocean and land. 
世界の大きさは50 - 500の間からランダムに選択される。 世界は陸地と海で構成されている。

A number of cities between 5 and (W+H)/100 is chosen and the population spread to be more dense near cities. Example map below:
町の数は5 + (W+H)/100で作成され、住民はこの町の周辺に住んでいる


# ツール
An offline tester/visualizer is available here. You can use it to test/debug your solution locally. 
You can also check its source code for exact implementation of test case generation, simulation and score calculation.


# 注意点
The first few examples have artificially small size limits, to help facilitate basic testing and for illustrative purposes.
最初のテストケースは最小値よりさらに小さくなっています。これは基礎的な理解を助けるために用意されたものです。

- The time limit is 20 seconds per test case (this includes only the time spent in your code). The memory limit is 1024 megabytes.
制限時間は20secでメモリは1GBまでが上限です。

- There is no explicit code size limit. The implicit source code size limit is around 1 MB 
(it is not advisable to submit codes of size close to that or larger). Once your code is compiled, 
the binary size should not exceed 1 MB.
コードのサイズは1MBを上限とします。

- The compilation time limit is 30 seconds. You can find information about compilers that we use and compilation options here.
コンパイル時間は30秒を上限とします。

- There are 10 example test cases and 100 full submission (provisional) test cases.
テストケースは10個用意されており、本番では100個のケースで実行されます。
 