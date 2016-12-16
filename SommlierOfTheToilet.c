#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

//include <libxml/xmlreader.h>

int kbhit();
int getch();
void toTitle();
void toOpening();

void updateTime();

void toRules();
void showFloor(int fl_num);
void showCollection();

void playHiAndLow();

void loadFloorData();
void setFloorData(int fl_num,int pattern);
void moveSommelier(char c);
void setSommelier(int fl_num);
void setObject(int fl_num);
void setToiCommLock();
void setItem();
void setMirrorComment();

void clearFloor();
void clearHasItem();

int getToiretIn(int fl_num, int x, int y);
int getToiretWarp(int fl_num, int x, int y);
int getToiretLock(int fl_num, int x, int y);
int getToiretNumber(int x, int y);

int generateProbability(int percent);
void changePlayerNum();
int GetRandom(int min,int max);

void checkEvent();
void checkItem();
void checkMirrorComment();

void toElevate();
void toGoal();
void to17f();
//void toTest();
//Item
typedef struct{
  //  int itm_no; //アイ��?��?のナンバ�?�
  char itm_name[256]; //アイ��?��?の名前
  int point; //アイ��?��?のポイン��?
  int main_itm; //メインアイ��?��?かど��?��?
  int has_itm; //ゲー��?中にそ�?�アイ��?��?を手に��?れた��?
  int had_itm; //そ�?�アイ��?��?を手に��?れた経歴があるか
  int itm_x; //アイ��?��?のあるx座��?
  int itm_y; //アイ��?��?のあるy座��?
  int itm_z; //アイ��?��?のあるフロア
  char comm1[64];
  char comm2[64];
  char comm3[64];
} Items;
//Sommelier
typedef struct{
  int now_floor; //現在��?るフロア階層
  int total_point; //拾ったアイ��?��?のト�?�タルポイン��?
  int now_x; // 現在地のx座��?
  int now_y; // 現在地のy座��?
  int pre_x; // 移動前のx座��?
  int pre_y; // 移動前のy座��?
} Sommelier;
//Mirror
typedef struct{
  int mirr_x; //洗面台のあるx座��?
  int mirr_y; //洗面台のあるy座��?
  //  int mirr_z; 洗面台のあるz座��?
  char mirr_comment[256]; //鏡に書��?てある��?容
} Mirror;
//Cupboard
typedef struct{
  int cup_warp; //ワープする掃除用具入れかど��?��?
  int cup_x; //��?除用具入れ�?�あるx座��?
  int cup_y; //��?除用具入れ�?�あるy座��?
  //  int cup_z; ��?除用具入れ�?�あるフロア
  int cup_in; //��?除用具入れに入れる方��?
  char cup_comment[256]; //��?除用具入れに入った時のコメン��?
} Cupboard;
//Toilet
typedef struct{
  int toi_warp; //ワープするトイレかど��?��?
  int uncle; //おじさんがいるか
  int locked; //開いて��?るかど��?��?
  int toi_x; //トイレのx座��?
  int toi_y; //トイレのy座��?
  //  int toi_z; //トイレがあるフロア
  int toi_in; //トイレに入れる方��?
  char toi_comment[256]; //トイレを開けた時�?�コメン��?
} Toiret;


int total_point;
int scene;

int now_floor;
//ゴールのイベントにはいるかどうか
int goal_flag;
//おじさんにかったかどうか
int ojisan_flag;

//時間関係で使うもの
time_t start, end;
int limit_time;
int second, minute;
int sec_count;
int f17_flag;
//ハイアンドローで使用するもの
int dealer_num;
int player_num;
int test_hl;

char ma1[12] = "x";
char ma2[12] = "x";
char ma3[12] = "x";
char ma4[12] = "x";

Toiret toiret[16][6];
Cupboard cupboard[16];
Mirror mirror[16][2];
Sommelier somm;
Items itm[23];

//いま居る座標が何かを覚えておく
char map_tmp[64];

int main(void){
  char c ;
  //乱数をしっかり発生させるおまじない
  srand((int)time(NULL));

  system("clear");
  loadFloorData();
  setItem();
  setMirrorComment();
  toTitle();
  switch (scene) {
    case 1:
      toOpening();
      setToiCommLock();
      clearHasItem();
      limit_time = 0;
      f17_flag = 0;
      ojisan_flag = 0;
      somm.now_floor = 1;
      clearFloor();
      setSommelier(somm.now_floor);
      setObject(somm.now_floor);
      showFloor(somm.now_floor);
      strcpy(map_tmp,"⋅  ");
      //時間を計測開始
      start = time(NULL);
      while(1){
        if(kbhit()){
          c = getch();
          if(c == 'w' || c =='a' || c== 's' || c=='d'){
            moveSommelier(c);
            showFloor(somm.now_floor);
            //printf("now x:%d now y:%d\n", somm.now_x, somm.now_y);
            //printf("pre x:%d pre y:%d\n", somm.pre_x, somm.pre_y);
          }
          if(c == 'x'){
            checkEvent();
            checkItem();
            checkMirrorComment();
          }
          if(c == 'z'){
            break;
          }
          if(c =='i'){
            showCollection();
          }

          else{
            //system("clear");
            //showFloor(somm.now_floor);
            //printf("now x:%d now y:%d\n", somm.now_x, somm.now_y);
            //printf("pre x:%d pre y:%d\n", somm.pre_x, somm.pre_y);
          }
          system("clear");
          showFloor(somm.now_floor);
        }

        //system("clear");
        //showFloor(somm.now_floor);

        //printf("now x:%d now y:%d\n", somm.now_x, somm.now_y);
        //printf("pre x:%d pre y:%d\n", somm.pre_x, somm.pre_y);

        if(limit_time > 900) goal_flag = 1;
        //17fの落とし穴
        if(somm.now_floor == 17){
          if(somm.now_x == 3 && somm.now_y == 5){
            goal_flag = 1;
            printf("うぉぉぉぉぉぉ吸い込まれる！！！\n");
            getchar();
          }
          if(somm.now_x == 4 && somm.now_y == 5){
            goal_flag = 1;
            printf("うぉぉぉぉぉぉ吸い込まれる！！！\n");
            getchar();
          }
          if(somm.now_x == 6 && somm.now_y == 5){
            goal_flag = 1;
            printf("うぉぉぉぉぉぉ吸い込まれる！！！\n");
            getchar();
          }
          if(somm.now_x == 5 && somm.now_y == 7){
            goal_flag = 1;
            printf("うぉぉぉぉぉぉ吸い込まれる！！！\n");
            getchar();
          }
          if(somm.now_x == 7 && somm.now_y == 7){
            goal_flag = 1;
            printf("うぉぉぉぉぉぉ吸い込まれる！！！\n");
            getchar();
          }
        }
        if(goal_flag == 1){
          //getchar();
          end = time(NULL);
          toGoal();
          goal_flag = 0;
          main();
          break;
        }
        //時間を更新
        updateTime();
        usleep(62500);
      }
      break;

    case 2:
      toRules();
      main();
      break;

    case 3:
      getchar();
      clearHasItem();
      showCollection();
      main();
      break;

    case 4:
      printf("\n Thank u 4 playing... Bye.....\n\n\n");
      break;

  }
  return 0;
}

void toTitle(){

  char c, userSelection;
  system("clear");
  printf("\n\n\n");
  printf("   /￣￣ ヽ  \n");
  printf("  / /~￣ ヽヽ \n");
  printf("  | .|  　| |    ┏┓      ┏┓  ┏┓          ┏┓  ┏┓  ┏┓     ┏┓┏┓  ┏━━┓\n");
  printf("  |. | 　 | |    ┃┃     ┏┛┃  ┃┃          ┃┃  ┃┃  ┃┃     ┃┃┃┃  ┗┓┏┛\n");
  printf("  ゝヽ　 .ﾉ ﾉ　  ┃┗━┓  ┏┛┏┛  ┃┃ ┏┓ ┏━┳━┓ ┗┛  ┃┃  ┃┃ ┏┓  ┃┃┃┃   ┃┃\n");
  printf("   \\,￣￣￣￣)   ┃┏━┛  ┗┫┃   ┃┃┏┛┃ ┃ ┃ ┃    ┏┛┃  ┃┗━┛┃  ┗┛┃┃   ┃┃\n");
  printf("    人     Ｙ    ┃┃     ┃┃   ┃┗┛┏┛ ┃ ┃ ┃ ┏━━┛┏┛  ┃┏━┓┃   ┏┛┃  ┏┛┗┓\n");
  printf("   L（______）   ┗┛     ┗┛   ┗━━┛  ┗━┛ ┛ ┗━━━┛   ┗┛ ┗┛   ┗━┛  ┗━━┛\n");
  printf("                                   - う〜ん、このトイレを求めて -\n");
  printf("      1.Start\n");
  printf("       2.Rules\n");
  printf("        3.Collection\n");
  printf("         4.Exit                  Please enter your selection: ");

  c = getchar();
	userSelection = c;

  if (c == '1') {
    system("clear");
    scene = 1;
  }
  else if (c == '2') {
    printf("pressed 2 \n");
    scene = 2;
  }
  else if (c == '3'){
    printf("pressed 3\n");
    scene = 3;
  }
  else if (c == '4') {
    scene = 4;
  }
  else if (c == 'p') {
    scene = 10;
  }
  else{
    system("clear");
    toTitle();
  }
}

void toOpening(){
  printf("\n\n\n");
  printf("・・・2018年.4月　トイレのソムリエがＫ研究所に就職した・・・\n\n\n");
  getchar();
  getchar();
  printf("与えられた初めての仕事、それは・・・\n\n\n");
  getchar();
  printf("『トイレの修理』\n\n\n");
  getchar();
  printf("　！？　と思ったそこのあなた。\n\n\n");
  getchar();
  printf("細かいことは気にせず\n\n\n");
  getchar();
  printf("とりあえずやってみよ？\n\n\n");
  getchar();
  printf("今、ソムリエのプライドをかけた完璧なトイレ作りの旅が始まる...\n\n\n\n");
  getchar();
}

void toRules(){
  system("clear");
  printf("あなたはトイレのソムリエです。\n");
  printf("16階のお客様用のトイレが壊れてしまいました。\n");
  printf("\n");
  printf("制限時間15分以内に各階のトイレを回り、必要なアイテムの便器、便座、タンク、トイレットペーパーの4つを集めて16階へ向かうことになりました。\n");
  printf("しかし、必要なアイテムを集めていないのに16階へ行ってしまうと\n強制的に終わってしまい、Game Overです。\n\n");
  printf("他にも多くのアイテムが存在します。\n");
  printf("そのアイテムにより最終的なスコアも変動します。\n\n");
  printf("移動はエレベーターで行いますがここで注意点。\n4、7、8、13階のボタンが壊れてしまっている為、移動できません。\n");
  printf("しかし、ある条件を満たすと、、、\n\n");
  printf("移動(w :↑  a :←  s :↓  d :→ )\n調べる,エレベーター移動 : x\n");
  printf("トイレ(stall)の出入り口は一ヶ所です。\n\n");
  printf("Please the enter key to go to the title:");
  getchar();
  getchar();
}

char map_frame[11][11][128] = {
  {"   ","_  ","_  ","   ","   ","   ","   ","   ","_  ","_  ","   "},
  {"   ","▬  ","▬  ","   ","   ","   ","   ","   ","▬  ","▬  ","   "},
  {"   ","▩  ","▩  ","_  ","_  ","_  ","_  ","_  ","▩  ","▩  ","   "},
  {"   ","▩  ","▩  ","▬  ","▬  ","▬  ","▬  ","▬  ","▩  ","▩  ","   ❍ : ソムリエ      ☒ : エレベーター"},
  {"   ","   ","▩  ","▩  ","▩  ","▩  ","▩  ","▩  ","▩  ","   ","   ⊽ : 掃除用具入れ  ◇ : 洗面鏡 ⊡ : トイレ"},
  {"   ","   ","▩  ","⋅  ","⋅  ","⋅  ","⋅  ","⋅  ","▩  ","   ","   "},
  {"   ","   ","▩  ","⋅  ","⋅  ","⋅  ","⋅  ","⋅  ","▩  ","   ","  [x]: 調べる/ｴﾚﾍﾞｰﾀｰ [i]: ｺﾚｸｼｮﾝ"},
  {"   ","_  ","▩  ","⋅  ","⋅  ","⋅  ","⋅  ","⋅  ","▩  ","_  ","  [a]: ←    [w]: ↑    [s]: ↓   [d]: →"},
  {"   ","▬  ","▩  ","☒  ","⋅  ","⋅  ","⋅  ","⋅  ","▩  ","▬  ","   "},
  {"   ","▩  ","▩  ","▩  ","▩  ","▩  ","▩  ","▩  ","▩  ","▩  ","                  Tᴀɴᴋ  Pᴀᴘᴇʀ Bᴏᴡʟ Sᴇᴀᴛ"},
  {"   ","▩  ","▩  ","   ","   ","   ","   ","   ","▩  ","▩  ","   Main items   : x     x     x    x "}};

char f16_frame[11][11][64] = {
    {"   ","   ","   ","   ","   ","   ","   ","   ","   ","   ","   "},
    {"   ","   ","   ","   ","   ","   ","   ","   ","   ","   ","   "},
    {"   ","   ","   ","   ","   ","   ","   ","   ","   ","   ","   "},
    {"   ","   ","   ","   ","   ","_  ","_  ","_  ","_  ","_  ","   ❍ : Player   ⊡ : Stall   ◇ : Mirror"},
    {"   ","_  ","_  ","_  ","_  ","▬  ","▬  ","▬  ","▬  ","▬  ","   ⊽ : Box      ☒ : EV "},
    {"   ","▬  ","▬  ","▬  ","▬  ","▩  ","▩  ","▩  ","▩  ","▩  ","   "},
    {"   ","▩  ","▩  ","▩  ","▩  ","▩  ","⋅  ","⋅  ","⋅  ","▩  ","  [x]: look/EV [w]: ↑"},
    {"   ","▩  ","▩  ","☒  ","⋅  ","⋅  ","⋅  ","●  ","⋅  ","▩  ","  [a]: ←       [s]: ↓      [d]: →"},
    {"   ","▩  ","▩  ","▩  ","▩  ","▩  ","⋅  ","⋅  ","⋅  ","▩  ","   "},
    {"   ","   ","   ","   ","   ","▩  ","▩  ","▩  ","▩  ","▩  ","                 Tᴀɴᴋ  Pᴀᴘᴇʀ Bᴏᴡʟ Sᴇᴀᴛ"},
    {"   ","   ","   ","   ","   ","   ","   ","   ","   ","   ","  Main items   : x     x     x    x "}};

void updateTime(){

  end = time(NULL);
  limit_time += (int)(end-start);
  minute = limit_time/60;
  second = limit_time - minute*60;
  if(second>=60){
    second = 0;
  }
  if(limit_time>10 && f17_flag ==0){
      f17_flag = 1;
      printf("  11階から大きな音がした...あの窪みだろうか...  ");
      printf("(Press enter key)");
      getchar();
  }
  if(second<50){
    sprintf(map_frame[1][10], " Deadline %d : %d  Total(%d)", 14-minute,59-second,limit_time);
  }else{
    sprintf(map_frame[1][10], " Deadline %d : 0%d  Total(%d)", 14-minute,59-second,limit_time);
  }
  start = time(NULL);
}

int toiret_pattern[5][6][3] = {{{0,1,6},{0,2,6},{0,3,6},{4,2,9},{4,1,9},{4,0,9}},
                               {{2,0,9},{2,1,9},{2,2,9},{3,2,6},{3,1,6},{3,0,6}},
                               {{1,0,8},{2,0,8},{3,0,8},{2,3,7},{3,3,7},{4,3,7}},
                               {{0,3,7},{1,3,7},{2,3,7},{3,3,7},{4,1,9},{4,0,9}},
                               {{0,1,6},{0,2,6},{1,3,7},{2,3,7},{3,3,7},{-1,-1,-1}}}; //{x座乗, y座乗, 入れる方向(6:左 7:上 8:下 9:右)}
int toi_pt_len = sizeof(toiret_pattern[0]) / sizeof(toiret_pattern[0][0]); //1フロアにあるトイレの個数

int mirror_pattern[5][2][2] = {{{2,3},{2,0}},
                               {{0,1},{0,2}},
                               {{0,3},{1,3}},
                               {{1,0},{2,0}},
                               {{2,0},{3,0}}}; //{x座標,y座標}
int mirr_pt_len = sizeof(mirror_pattern[0][0]) / sizeof(toiret_pattern[0][0][0]); //1フロアにある洗面台の個数

int cupboard_pattern[5][3] = {{4,3,9},
                              {0,3,6},
                              {4,0,8},
                              {4,3,7},
                              {4,0,8}}; //{x座乗, y座乗, 入れる方向(6:左 7:上 8:下 9:右)}
int cup_pt_len = sizeof(cupboard_pattern[0]) / sizeof(cupboard_pattern[0][0]);
int total_itm = (( sizeof itm ) / ( sizeof ( Items ) ) );

void setSommelier(int fl_num){
  /*パターン1の場合
  if(fl_num == 13){
    strcpy(map_frame[8][4],"❍  ");
    somm.now_x = 4;
    somm.now_y = 8;
  }
  */
  //パターン2の場合
  if( fl_num == 5 || fl_num == 9 || fl_num == 14){
    strcpy(map_frame[8][4],"❍  ");
    somm.now_x = 4;
    somm.now_y = 8;
  }
  //パターン3の場合
  if( fl_num == 1 || fl_num == 6 || fl_num == 10){
    strcpy(map_frame[7][3],"❍  ");
    somm.now_x = 3;
    somm.now_y = 7;
  }
  //パターン4の場合
  if( fl_num == 2 || fl_num == 12){
    strcpy(map_frame[7][3],"❍  ");
    somm.now_x = 3;
    somm.now_y = 7;
  }
  //パターン5の場合
  if( fl_num == 3 || fl_num == 11 || fl_num == 15){
    strcpy(map_frame[8][4],"❍  ");
    somm.now_x = 4;
    somm.now_y = 8;
  }
  //エレベーターから1マス動いた事にする
  somm.pre_x = 3;
  somm.pre_y = 8;

  //7F, 8F, 4Fの場合
  if(fl_num == 8 || fl_num == 7 || fl_num == 4){
    strcpy(map_frame[8][6],"❍  ");
    somm.now_x = 6;
    somm.now_y = 8;
    somm.pre_x = 7;
    somm.pre_y = 8;
  }

  //7Fの場合
  if(fl_num == 7){
    strcpy(map_frame[8][6],"❍  ");
    somm.now_x = 6;
    somm.now_y = 8;
    somm.pre_x = 7;
    somm.pre_y = 8;
  }
  //13Fの場合
  if(fl_num == 13){
    strcpy(map_frame[6][6],"❍  ");
    somm.now_x = 6;
    somm.now_y = 6;
    somm.pre_x = 7;
    somm.pre_y = 6;
  }

}

void setFloorData(int fl_num,int pattern){
  pattern--;
  fl_num--;
  int i ;
  for (i=0 ; i < toi_pt_len; i++) {
    toiret[fl_num][i].toi_warp = 0;
    toiret[fl_num][i].uncle = 0;
    toiret[fl_num][i].locked = 0;
    toiret[fl_num][i].toi_x = toiret_pattern[pattern][i][0];
    toiret[fl_num][i].toi_y = toiret_pattern[pattern][i][1];
    toiret[fl_num][i].toi_in = toiret_pattern[pattern][i][2];
    strcpy(toiret[fl_num][i].toi_comment,"...");
  }
  for (i=0 ; i < mirr_pt_len; i++) {
    mirror[fl_num][i].mirr_x = mirror_pattern[pattern][i][0];
    mirror[fl_num][i].mirr_y = mirror_pattern[pattern][i][1];
    strcpy(mirror[fl_num][i].mirr_comment,"特に変わったところはない");
  }
  cupboard[fl_num].cup_warp = 0;
  cupboard[fl_num].cup_x = cupboard_pattern[pattern][0];
  cupboard[fl_num].cup_y = cupboard_pattern[pattern][1];
  cupboard[fl_num].cup_in = cupboard_pattern[pattern][2];
  strcpy(cupboard[fl_num].cup_comment," Nothing is here ... ");


}
/*void setFloorData(int floor, char pattern){

  xmlTextReaderPtr reader;
  int ret;
  reader = xmlNewTextReaderFilename("./Toiret.xml");
  ret = xmlTextReaderRead(reader);
  while (ret == 1) {
    if(xmlTextReaderNodeType(reader) == XML_READER_TYPE_ELEMENT && xmlStrcmp(xmlTextReaderName(reader), BAD_CAST "pattern") == 0){
      ret = xmlTextReaderRead(reader);
      if(xmlTextReaderNodeType(reader) == XML_READER_TYPE_TEXT && xmlStrcmp(xmlTextReaderValue(reader), BAD_CAST "a") == 0){

      }
    }else{
      ret = xmlTextReaderRead(reader);
    }
  }
  xmlFreeTextReader(reader);
  getchar();
}
*/

//各フロアのパターンをセットする
void loadFloorData(){
  int countfloor = 1;
  while (countfloor < 16) {
    switch (countfloor) {
      case 1:
        setFloorData(countfloor,3); //(1階をパターン3で初期��?)
        break;
      case 2:
        setFloorData(countfloor,4);
        break;
      case 3:
        setFloorData(countfloor,5);
        break;
      case 4:
        setFloorData(countfloor,1);
        break;
      case 5:
        setFloorData(countfloor,2);
        break;
      case 6:
        setFloorData(countfloor,3);
        break;
      case 7:
        setFloorData(countfloor,4);
        break;
      case 8:
        setFloorData(countfloor,1);
        break;
      case 9:
        setFloorData(countfloor,2);
        break;
      case 10:
        setFloorData(countfloor,3);
        break;
      case 11:
        setFloorData(countfloor,5);
        break;
      case 12:
        setFloorData(countfloor,4);
        break;
      case 13:
        setFloorData(countfloor,1);
        break;
      case 14:
        setFloorData(countfloor,2);
        break;
      case 15:
        setFloorData(countfloor,5);
        break;
    }
    countfloor++;
  }
  //ワープするトイレを設定
  toiret[8][5].toi_warp = 1;
  toiret[13][5].toi_warp = 1;
  toiret[3][3].toi_warp = 1;
  toiret[4][5].toi_warp = 1;
}

//map_frameに上書きした内容を元に戻す
void clearFloor(){
  int i;
  int s;
  for (i = 5; i <= 8; i++) {
    for (s = 3; s <= 7; s++) {
      strcpy(map_frame[i][s],"⋅  ");
    }
  }
  strcpy(map_frame[8][3],"☒  ");
}

//アイテムの所持状況を初期化
void clearHasItem(){
  int i;
  for (i = 0; i < total_itm; i++) {
    itm[i].has_itm = 0;
    //itm[i].had_itm = 1;

  }
  strcpy(ma1,"x");
  strcpy(ma2,"x");
  strcpy(ma3,"x");
  strcpy(ma4,"x");
  sprintf(map_frame[10][10],"   Main items   : x     x     x    x ");
  sprintf(f16_frame[10][10],"   Main items   : x     x     x    x ");
}

//トイレ, 洗面台, 掃除用具入れをmap_frameに反映する
void setObject(int fl_num){
  fl_num --;
  int x;
  int y;
  int i;
  //トイレの座標を取得して、map_frameに⊡で反映する.
  for (i = 0; i < toi_pt_len; i++) {
    x = toiret[fl_num][i].toi_x;
    y = toiret[fl_num][i].toi_y;
    if(x != -1)strcpy(map_frame[8-y][x+3],"⊡  ");
  }
  //洗面台の座標を取得して、map_frameに◇で反映する.
  for (i = 0; i < mirr_pt_len; i++) {
    x = mirror[fl_num][i].mirr_x;
    y = mirror[fl_num][i].mirr_y;
    strcpy(map_frame[8-y][x+3],"◇  ");
  }
  //��?除用具入れ�?�座標を取得して、map_frameに⊽で反映する.
  x = cupboard[fl_num].cup_x;
  y = cupboard[fl_num].cup_y;
  strcpy(map_frame[8-y][x+3],"⊽  ");

}
int firstcheck = 0;
//フロアを表示する
void showFloor(int fl_num){
  system("clear");
  //  clearFloor();
  //  setSommelier(fl_num);
  fl_num --;
  int i;
  int j;

  if(firstcheck == 0){
    printf("\n%dF   天の声 : ⊡（トイレ）の中に入って [x] を押して中を調べてみよう！\n", fl_num+1);
  }else{
    printf("\n%dF\n", fl_num+1);
  }
  for (i = 0; i < 11; i++) {
    for (j = 0; j < 11; j++) {
      printf("%s", map_frame[i][j]);
      if(j == 10)printf("\n");
    }
  }
  printf(" □―――――――――――――――――――――――――――――――――――――――――――――――――――――――――――---‐‐\n");

  //printf(" |");
}

//ソムリエを動かす
void moveSommelier(char c){
  switch (c) {
    //wが押されたら上に動く
    case 'w':
    //移動先がトイレの場合
    if(strcmp(map_frame[somm.now_y-1][somm.now_x],"⊡  ") == 0){
      if(getToiretIn(somm.now_floor, somm.now_x, somm.now_y-1) == 7){
        system("clear");
        showFloor(somm.now_floor);
        printf("   %s  (Press Enter key)\n", toiret[somm.now_floor-1][getToiretNumber(somm.now_x,somm.now_y-1)].toi_comment);
        getchar();
        if(getToiretLock(somm.now_floor, somm.now_x, somm.now_y-1) == 0){
          strcpy(map_frame[somm.now_y][somm.now_x],map_tmp);
          strcpy(map_tmp,map_frame[somm.now_y-1][somm.now_x]);
          strcpy(map_frame[somm.now_y-1][somm.now_x],"❍  ");
          somm.pre_y = somm.now_y;
          somm.pre_x = somm.now_x;
          somm.now_y--;
        }
      }
      break;
    }
    //今いる場所がトイレの場合
    if(strcmp(map_tmp,"⊡  ") == 0){
      if(somm.now_x == somm.pre_x && somm.now_y-1 == somm.pre_y){
        strcpy(map_frame[somm.now_y][somm.now_x],map_tmp);
        strcpy(map_tmp,map_frame[somm.now_y-1][somm.now_x]);
        strcpy(map_frame[somm.now_y-1][somm.now_x],"❍  ");
        somm.pre_x = somm.now_x;
        somm.pre_y = somm.now_y;
        somm.now_y--;
      }
      break;
    }
    //移動先が壁じゃない場合
    if(strcmp(map_frame[somm.now_y-1][somm.now_x],"▩  ") != 0){
      strcpy(map_frame[somm.now_y][somm.now_x],map_tmp);
      strcpy(map_tmp,map_frame[somm.now_y-1][somm.now_x]);
      strcpy(map_frame[somm.now_y-1][somm.now_x],"❍  ");
      somm.pre_y = somm.now_y;
      somm.pre_x = somm.now_x;
      somm.now_y--;
    }
    break;

    //aが押されたら左に動く
    case 'a':
    //移動先がトイレの場合
    if(strcmp(map_frame[somm.now_y][somm.now_x-1],"⊡  ") == 0){
      if(getToiretIn(somm.now_floor, somm.now_x-1, somm.now_y) == 6){
        system("clear");
        showFloor(somm.now_floor);
        printf("   %s  (Press Enter key)\n", toiret[somm.now_floor-1][getToiretNumber(somm.now_x-1,somm.now_y)].toi_comment);
        getchar();
        if(getToiretLock(somm.now_floor, somm.now_x-1, somm.now_y) == 0){
          strcpy(map_frame[somm.now_y][somm.now_x],map_tmp);
          strcpy(map_tmp,map_frame[somm.now_y][somm.now_x-1]);
          strcpy(map_frame[somm.now_y][somm.now_x-1],"❍  ");
          somm.pre_x = somm.now_x;
          somm.pre_y = somm.now_y;
          somm.now_x-- ;
        }
      }
      break;
    }
    //今いる場所がトイレの場合
    if(strcmp(map_tmp,"⊡  ") == 0){
      if(somm.now_x-1 == somm.pre_x && somm.now_y == somm.pre_y){
        strcpy(map_frame[somm.now_y][somm.now_x],map_tmp);
        strcpy(map_tmp,map_frame[somm.now_y][somm.now_x-1]);
        strcpy(map_frame[somm.now_y][somm.now_x-1],"❍  ");
        somm.pre_x = somm.now_x;
        somm.pre_y = somm.now_y;
        somm.now_x--;
      }
      break;
    }

    //移動先が壁じゃない場合
    if(strcmp(map_frame[somm.now_y][somm.now_x-1],"▩  ") != 0){
      strcpy(map_frame[somm.now_y][somm.now_x],map_tmp);
      strcpy(map_tmp,map_frame[somm.now_y][somm.now_x-1]);
      strcpy(map_frame[somm.now_y][somm.now_x-1],"❍  ");
      somm.pre_x = somm.now_x;
      somm.pre_y = somm.now_y;
      somm.now_x--;
    }
    break;

    //sが押されたら下に動く
    case 's':
    //移動先がトイレの場合
    if(strcmp(map_frame[somm.now_y+1][somm.now_x],"⊡  ") == 0){
      if(getToiretIn(somm.now_floor, somm.now_x, somm.now_y+1) == 8){
        system("clear");
        showFloor(somm.now_floor);
        printf("   %s  (Press Enter key)\n", toiret[somm.now_floor-1][getToiretNumber(somm.now_x,somm.now_y+1)].toi_comment);
        getchar();
        if(getToiretLock(somm.now_floor, somm.now_x, somm.now_y+1) == 0){
          strcpy(map_frame[somm.now_y][somm.now_x],map_tmp);
          strcpy(map_tmp,map_frame[somm.now_y+1][somm.now_x]);
          strcpy(map_frame[somm.now_y+1][somm.now_x],"❍  ");
          somm.pre_y = somm.now_y;
          somm.pre_x = somm.now_x;
          somm.now_y++;
        }
      }
      break;
    }
    //今いる場所がトイレの場合
    if(strcmp(map_tmp,"⊡  ") == 0){
      if(somm.now_x == somm.pre_x && somm.now_y+1 == somm.pre_y){
        strcpy(map_frame[somm.now_y][somm.now_x],map_tmp);
        strcpy(map_tmp,map_frame[somm.now_y+1][somm.now_x]);
        strcpy(map_frame[somm.now_y+1][somm.now_x],"❍  ");
        somm.pre_x = somm.now_x;
        somm.pre_y = somm.now_y;
        somm.now_y++;
      }
      break;
    }

    //移動先が壁じゃない場合
    if(strcmp(map_frame[somm.now_y+1][somm.now_x],"▩  ") != 0){
      strcpy(map_frame[somm.now_y][somm.now_x],map_tmp);
      strcpy(map_tmp,map_frame[somm.now_y+1][somm.now_x]);
      strcpy(map_frame[somm.now_y+1][somm.now_x],"❍  ");
      somm.pre_y = somm.now_y;
      somm.pre_x = somm.now_x;
      somm.now_y++;
    }
    break;

    //dが押されたら右に動く
    case 'd':
    //移動先がトイレの場合
    if(strcmp(map_frame[somm.now_y][somm.now_x+1],"⊡  ") == 0){
      if(getToiretIn(somm.now_floor, somm.now_x+1, somm.now_y) == 9){
        system("clear");
        showFloor(somm.now_floor);
        printf("   %s  (Press Enter key)\n", toiret[somm.now_floor-1][getToiretNumber(somm.now_x+1,somm.now_y)].toi_comment);
        getchar();
        if(getToiretLock(somm.now_floor, somm.now_x+1, somm.now_y) == 0){
          strcpy(map_frame[somm.now_y][somm.now_x],map_tmp);
          strcpy(map_tmp,map_frame[somm.now_y][somm.now_x+1]);
          strcpy(map_frame[somm.now_y][somm.now_x+1],"❍  ");
          somm.pre_x = somm.now_x;
          somm.pre_y = somm.now_y;
          somm.now_x++;
        }
      }
      break;
    }
    //今いる場所がトイレの場合
    if(strcmp(map_tmp,"⊡  ") == 0){
      if(somm.now_x+1 == somm.pre_x && somm.now_y == somm.pre_y){
        strcpy(map_frame[somm.now_y][somm.now_x],map_tmp);
        strcpy(map_tmp,map_frame[somm.now_y][somm.now_x+1]);
        strcpy(map_frame[somm.now_y][somm.now_x+1],"❍  ");
        somm.pre_x = somm.now_x;
        somm.pre_y = somm.now_y;
        somm.now_x++;
      }
      break;
    }
    //移動先が壁じゃない場合
    if(strcmp(map_frame[somm.now_y][somm.now_x+1],"▩  ") != 0){
      strcpy(map_frame[somm.now_y][somm.now_x],map_tmp);
      strcpy(map_tmp,map_frame[somm.now_y][somm.now_x+1]);
      strcpy(map_frame[somm.now_y][somm.now_x+1],"❍  ");
      somm.pre_x = somm.now_x;
      somm.pre_y = somm.now_y;
      somm.now_x++;
    }
    break;
  }
}

//toi[][].lockedを取得
int getToiretLock(int fl_num, int x, int y){
  fl_num--;
  x = x-3;
  y = -(y-8);
  int i;
  for (i = 0; i < 6; i++) {
    if (toiret[fl_num][i].toi_x == x && toiret[fl_num][i].toi_y == y) {
      return toiret[fl_num][i].locked;
    }
  }
  return 0;
}

//toi[][].toi_warpを取得
int getToiretWarp(int fl_num, int x, int y){
  fl_num--;
  x = x-3;
  y = -(y -8);
  int i;
  for (i = 0; i < 6; i++) {
    if (toiret[fl_num][i].toi_x == x && toiret[fl_num][i].toi_y == y) {
      return toiret[fl_num][i].toi_warp;
    }
  }
  return 0;
}

//toi[][].toi_inを取得
int getToiretIn(int fl_num, int x, int y){
  fl_num--;
  x = x - 3;
  y = -(y - 8);
  int i;
  for (i = 0; i < 6; i++) {
    if (toiret[fl_num][i].toi_x == x && toiret[fl_num][i].toi_y == y) {
      return toiret[fl_num][i].toi_in;
    }
  }
  return 0;
}

//トイレの番号を取得 -> コメントを表示するのに使いたい
int getToiretNumber(int x, int y){
  int i;
  x = x - 3;
  y = -(y - 8);
  for (i = 0; i < 6; i++) {
    if(toiret[somm.now_floor-1][i].toi_x == x && toiret[somm.now_floor-1][i].toi_y == y){
      return i;
    }
  }
  return -1;
}

//今いる座標にイベントがあるか判定する.
void checkEvent(){
  //エレベーターの時
  if(somm.now_x == 3 && somm.now_y == 8 && somm.now_floor != 17) {
    toElevate();
  }
  //ワープするトイレの時
  if(getToiretWarp(somm.now_floor, somm.now_x, somm.now_y)==1){
    //9F -> 8F
    if(somm.now_floor == 9){
      system("clear");
      showFloor(somm.now_floor);
      printf("  ゆ、床が抜けた....！！！\n");
      getchar();
      clearFloor();
      somm.now_floor = 8;
      strcpy(map_tmp,"⋅  ");
      setSommelier(somm.now_floor);
      setObject(somm.now_floor);
      showFloor(somm.now_floor);
    }
    //14F -> 7F
    if(somm.now_floor == 14){
      system("clear");
      showFloor(somm.now_floor);
      printf("  なんて勢いだ！トイレに..流される！！！\n");
      getchar();
      clearFloor();
      somm.now_floor = 7;
      strcpy(map_tmp,"⋅  ");
      setSommelier(somm.now_floor);
      setObject(somm.now_floor);
      showFloor(somm.now_floor);
    }
    //4F -> 13F
    if(somm.now_floor == 4){
      system("clear");
      showFloor(somm.now_floor);
      printf("  なんて勢いだ！トイレに..流される！！！\n");
      getchar();
      clearFloor();
      somm.now_floor = 13;
      strcpy(map_tmp,"⋅  ");
      setSommelier(somm.now_floor);
      setObject(somm.now_floor);
      showFloor(somm.now_floor);
    }
    //5F -> 4F
    if(somm.now_floor == 5){
      system("clear");
      showFloor(somm.now_floor);
      printf("  ゆ、床が抜けた....！！！\n");
      getchar();
      clearFloor();
      somm.now_floor = 4;
      strcpy(map_tmp,"⋅  ");
      setSommelier(somm.now_floor);
      setObject(somm.now_floor);
      showFloor(somm.now_floor);
    }
  }
  //おじさんと戦闘
  if(somm.now_x == 6 && somm.now_y == 6 && somm.now_floor == 7) {
    char c[5];
    if(ojisan_flag == 0){
      system("clear");
      showFloor(somm.now_floor);
      printf("  おじさん「なぁ、ハイアンドローがしたくてたまらないんだ.....」");
      getchar();

      system("clear");
      showFloor(somm.now_floor);
      printf("  おじさん「付き合ってもらえないか？」(y/n) : ");
      while(1){
        scanf("%s",c);
        if(strcmp(c,"y")==0){
          system("clear");
          showFloor(somm.now_floor);
          printf("  おじさん「そうかそうか...さっそく始めるぞ！！」\n");
          getchar();
          getchar();
          playHiAndLow();
          break;
        }
        else if(strcmp(c,"n")==0){
          system("clear");
          showFloor(somm.now_floor);
          printf("  おじさん「あとで後悔しても知らないぞ......」\n");
          getchar();
          getchar();
          break;
        }
        else{
          system("clear");
          showFloor(somm.now_floor);
          printf("  Please type 'y' or 'n' key \n");
          getchar();
        }
      }
      return;
    }
    if(ojisan_flag == -1){system("clear");
      showFloor(somm.now_floor);
      printf("  おじさん「あまり再戦を挑むなよ..弱く見えるぞ？」\n");
      printf("  おじさん「...まずは私からカードを切ろう...」");
      getchar();
      playHiAndLow();
      return;
    }
  }
  //おじさんがいたところ
  if(somm.now_x == 6 && somm.now_y == 5 && somm.now_floor == 7) {
    system("clear");
    showFloor(somm.now_floor);
    printf("  おじさんがここに居たと...いつから錯覚していたのだろうか...\n");
    getchar();
  }
  //隠し扉イベント
  if(somm.now_x == 7 && somm.now_y == 5 && somm.now_floor == 11) {
    if (f17_flag == 1) {
      system("clear");
      showFloor(somm.now_floor);
      printf("  こんな場所に隠し扉が！！入りますか？(y/n) : ");
      char c = getchar();
      if(c == 'y'){
        to17f();
      }
    }else{
      system("clear");
      showFloor(somm.now_floor);
      printf("  なんだこの違和感は.....？ ");
      getchar();
    }
  }
  //鏡ロックイベント
  if(somm.now_x == 3 && somm.now_y == 6 && somm.now_floor == 5){
    char mirr[50];
    if(itm[21].has_itm == 1){
      showFloor(somm.now_floor);
      printf("  既に鏡のトリックは見抜いている...\n");
      getchar();
    }
    else{
      showFloor(somm.now_floor);
      printf("  鏡に細工がしてある・・・文字を入力出来るみたいだ \n");
      printf("  「豚が離婚する時に食べるお菓子はなに？」 \n");
      printf("  入力キー :  ");
      scanf("%s",mirr);
      if(strcmp(mirr, "とんがりコーン") == 0 || strcmp(mirr, "トンガリコーン") == 0){
        printf("  正解！\n");
        getchar();
        showFloor(somm.now_floor);
        printf("  カレンダーを手に入れた\n");
        getchar();
        itm[21].has_itm = 1;
        itm[21].had_itm = 1;
      }else{
        printf("  不正解！\n");
        getchar();
        getchar();
      }
    }
  }

}
//ハイアンドロー
void playHiAndLow(){

  int itm_p = 0;

	char test[64];
  system("clear");
  showFloor(somm.now_floor);
  if(ojisan_flag != -1){
  printf("  ~ルール説明~\n");
	printf("   もっている必須アイテムの個数で勝率が変わります\n");
  printf("   初めにソムリエとおじさんに１～１４の数字が配られます。\n");
  printf("   おじさんの数字だけが見えるようになり、その数字より\n");
  printf("   自分の数字が大きいか小さいかを当てるゲームです。 \n");

  getchar();
  }
  //アイテムをどれだけもってるか確認
  //便座
  if (itm[0].has_itm ==1) {
    itm_p++;
  }
  //タンク
  if (itm[3].has_itm ==1) {
    itm_p++;
  }
  //トイレットペーパー
  if (itm[12].has_itm ==1) {
    itm_p++;
  }
  //テスト用 とりあえず勝ちたいときに
  //itm_p=3;
  system("clear");
  showFloor(somm.now_floor);
	switch(itm_p){
			//アイテムが0個
			case 0:
				dealer_num = GetRandom(4,10);
				player_num = GetRandom(1,14);
        //プレイヤーが判断
        printf("  おじさん「私は%dを引いたぞ...どうするかい？」\n",dealer_num);
        printf("  High なら'1'Low なら'0'を入力してください : ");
        scanf("%s", test);
        test_hl = atoi(test);
				//lowを選んだ時
				if(test_hl == 0){
					if(player_num < dealer_num){
						if(generateProbability(99) == 1){
							changePlayerNum();
						}
						if(player_num < dealer_num){
							ojisan_flag=1;

						}
					}
				}
				//highを選んだ時
				if(test_hl == 1){
					if(player_num > dealer_num){
						if(generateProbability(99) == 1){
							changePlayerNum();
						}
						if(player_num > dealer_num){
							ojisan_flag=1;
						}
					}
				}
				break;

			case 1:
				dealer_num = GetRandom(6,8);
				player_num = GetRandom(1,14);
        printf("  おじさん「私は%dを引いたぞ...どうするかい？」\n",dealer_num);
        printf("  High なら'1'Low なら'0'を入力してください : ");
        scanf("%s", test);
        test_hl = atoi(test);
				//lowを選んだ時
					if(test_hl == 0){
						if(player_num < dealer_num){
							if(generateProbability(70) == 1){
								changePlayerNum();
							}
							if(player_num < dealer_num){
								ojisan_flag=1;
							}
						}
					}
					//highを選んだ時
					if(test_hl == 1){
						if(player_num > dealer_num){
							if(generateProbability(70) == 1){
								changePlayerNum();
							}
							if(player_num > dealer_num){
								ojisan_flag=1;
							}
						}
					}
					break;

			case 2:
				dealer_num = GetRandom(10,14);
				if (generateProbability(50) == 1)	dealer_num = GetRandom(1,5);
				player_num = GetRandom(1,14);
        printf("  おじさん「私は%dを引いたぞ...どうするかい？」\n",dealer_num);
        printf("  High なら'1'Low なら'0'を入力してください : ");
        scanf("%s", test);
        test_hl = atoi(test);
				//lowを選んだ時
				if(test_hl == 0){
					if(player_num < dealer_num){
						if(generateProbability(50) == 1){
							changePlayerNum();
						}
					if(player_num < dealer_num){
							ojisan_flag=1;
						}
					}
				}
				//highを選んだ時
				if(test_hl == 1){
					if(player_num > dealer_num){
						if(generateProbability(50) == 1){
							changePlayerNum();
						}
						if(player_num > dealer_num){
							ojisan_flag=1;
						}
					}
				}
				break;

			case 3:
				dealer_num = GetRandom(11,14);
				if (generateProbability(50) == 1)	dealer_num = GetRandom(1,4);
				player_num = GetRandom(1,12);
        //プレイヤーが判断
        printf("  おじさん「私は%dを引いたぞ...どうするかい？」\n",dealer_num);
        printf("  High なら'1'Low なら'0'を入力してください : ");
        scanf("%s", test);
        test_hl = atoi(test);
				if(test_hl == 0){
					if(player_num < dealer_num){
						ojisan_flag=1;
					}else{}
				}else{
					if(player_num > dealer_num){
						ojisan_flag=1;
					}else{}
				}
				break;

			default:
				break;
		}
  system("clear");
  showFloor(somm.now_floor);
  printf("  ソムリエ　: %d\n  おじさん  : %d \n", player_num,dealer_num);
  if(player_num == dealer_num){
    getchar();
    printf("  おじさん「引き分けはあなたの負けだっ！！！」");
    getchar();
    return;
  }
  if(ojisan_flag ==1) {
    printf("  おじさん「私は...なにを...？」\n");
    printf("  おじさん「これが...そうか...この掌にあるものが...」\n");
    getchar();
    getchar();
    itm[6].has_itm =1;
    itm[6].had_itm =1;
    system("clear");
    showFloor(somm.now_floor);
    printf("  \" 便器 \" を手に入れた\n");
    ojisan_flag = 1;
    strcpy(toiret[6][3].toi_comment,"おじさんの霊圧が消えた...？");
    toiret[6][3].locked = 0;

    getchar();
  }
  else{
    printf("  おじさん「また勝ってしまった...故にHigh&Low...」\n");
    strcpy(toiret[6][3].toi_comment,"おじさんがこちらをジッと見つめている\n   [x] で再戦を挑もう");
    ojisan_flag = -1;
    getchar();
    getchar();
  }

}

//今いる座標にアイテムがあるか判定する.
void checkItem(){
  int i;
  int j = 0; // アイテムを拾ったか拾ってないか
  int x = somm.now_x-3;
  int y = -(somm.now_y-8);
  for (i = 0; i < total_itm; i++) {
    if (itm[i].itm_x == x && itm[i].itm_y == y && itm[i].itm_z == somm.now_floor-1 ) {
      if(itm[i].has_itm !=1&& i != 21){
        if(firstcheck == 0 )firstcheck = 1;
        system("clear");
        showFloor(somm.now_floor);
        printf("   \" %s \" を手に入れた ... (Press enter key)\n",itm[i].itm_name);
        j = 1;
        itm[i].has_itm = 1;
        itm[i].had_itm = 1;
        total_point += itm[i].point;
        //ドライバーを手に入れ時に開放
        if(i==1){
          strcpy(toiret[0][3].toi_comment,"ドライバーでドアを開けた");
          toiret[0][3].locked = 0;
        }
        //ハンマーを手に入れた時に開放
        if(i == 17){
          strcpy(toiret[3][5].toi_comment,"力の限りハンマーを振り回した");
          toiret[3][5].locked = 0;
          strcpy(toiret[4][4].toi_comment,"力の限りハンマーを振り回した");
          toiret[4][4].locked = 0;
        }
        //塩を手に入れた時に開放
        if(i == 18){
          toiret[7][2].locked = 0;
          strcpy(toiret[7][2].toi_comment,"魑魅魍魎よ...静まれ....");
        }
        //トイレットペーパーを手に入れた時に開放
        if(i == 12){
          toiret[5][4].locked = 0;
          strcpy(toiret[5][4].toi_comment,"「ありがとう...とりあえず中に入ってくれ」");
        }
        getchar();
        break;
      }
    }
  }
  if(j == 0 && strcmp(map_tmp,"◇  ")!=0 && strcmp(map_tmp,"⋅  ")!=0){
    system("clear");
    showFloor(somm.now_floor);
    printf("  使えそうなアイテムはなさそうだ...\n");
    if(firstcheck == 0 )firstcheck = 1;
    getchar();
  }
  //17fの例外
  if(somm.now_floor ==17 && j == 0 ){
    if(somm.now_x != 3 || somm.now_y!= 8){
      system("clear");
      showFloor(somm.now_floor);
      printf("  使えそうなアイテムはなさそうだ...\n");
      getchar();
    }
  }
  if(itm[3].has_itm == 1) strcpy(ma1,"⦿");
  if(itm[12].has_itm == 1) strcpy(ma2,"⦿");
  if(itm[6].has_itm == 1) strcpy(ma3,"⦿");
  if(itm[0].has_itm == 1) strcpy(ma4,"⦿");
  sprintf(map_frame[10][10],"   Main items   : %s     %s     %s    %s ",ma1,ma2,ma3,ma4);
  sprintf(f16_frame[10][10],"   Main items   : %s     %s     %s    %s ",ma1,ma2,ma3,ma4);

}

//今いる座標が鏡の場合にコメントを表示する
void checkMirrorComment(){
  int x = somm.now_x-3;
  int y = -(somm.now_y-8);
  if(strcmp(map_tmp,"◇  ") == 0){
    //カレンダーのときスルーしたい
    if(x == 0 && y == 2 &&
      mirror[somm.now_floor-1][1].mirr_x == x && mirror[somm.now_floor-1][1].mirr_y == y && somm.now_floor == 5){
      return;
    }
    if(mirror[somm.now_floor-1][0].mirr_x == x && mirror[somm.now_floor-1][0].mirr_y == y){
      system("clear");
      showFloor(somm.now_floor);
      printf("   %s",mirror[somm.now_floor-1][0].mirr_comment);
      getchar();
    }
    else{
      system("clear");
      showFloor(somm.now_floor);
      printf("   %s",mirror[somm.now_floor-1][1].mirr_comment);
      getchar();
    }

  }
}

//エレベーター画面
void toElevate(){
  int fl;
  system("clear");
  printf("\n -- F \n");
  printf("   _  _                 _  _  \n");
  printf("   ▬  ▬  _  _  _  _  _  ▬  ▬  \n");
  printf("   ┏┿━――――――――――――――――――――― 、 \n");
  printf("   ╂┘    1F    9F    15F    |    ❍ : Player   ⊡ : Stall   ◇ : Mirror \n");
  printf("   |     2F    10F   16F    |    ⊽ : Box      ☒ : EV\n");
  printf("   |     3F    11F          |  \n");
  printf("   |     5F    12F    ▲     |   [x]: look/EV [w]: ↑\n");
  printf("   |     6F    14F    ▼    ┌╂   [a]: ←       [s]: ↓      [d]: →\n");
  printf("    `――――――――――――――――――――――┿┛  \n");
  printf("   ▩  ▩  ▩  ▩  ▩  ▩  ▩  ▩  ▩                    Tᴀɴᴋ  Pᴀᴘᴇʀ Bᴏᴡʟ Sᴇᴀᴛ\n");
  printf("   ▩  ▩                 ▩  ▩  %s\n",map_frame[10][10]);
  printf("\n   Please enter floor number ? (number only) : ");
  scanf("%d",&fl);
  if(fl==1||fl==2||fl==3||fl==5||fl==6||fl==9||fl==10||fl==11||fl==12||fl==14||fl==15){
    somm.now_floor=fl;
    strcpy(map_tmp,"⋅  ");
    clearFloor();
    setSommelier(somm.now_floor);
    setObject(somm.now_floor);
    showFloor(somm.now_floor);
  }
  else if(fl==16){
    char c;
    printf("  Can't return once you go , is it OK ? (y\\n) : ");
    getchar();
    c = getchar();
    if(c=='y'){
        goal_flag = 1;
    }
    if(c=='n'){
    }
  }
  else{
    printf(" Can't go there .... press enter key  ");
    getchar();
    getchar();
    checkEvent();
  }
}

//エンディング
void toGoal(){
  strcpy(f16_frame[7][7],"●  ");
  int count=0;
  int i;
  int j;
  while (1) {
    system("clear");

    printf("\nTop floor\n");
    for (i = 0; i < 11; i++) {
      for (j = 0; j < 11; j++) {
        printf("%s", f16_frame[i][j]);
        if(j == 10)printf("\n");
      }
    }
    printf(" □――――――――――――――――――――――――――――――――――――――――――――――――――――――――---‐‐\n");
    while(1){
      if(kbhit()){
        char c;
        c = getch();
        if(c != '!'){
          strcpy(f16_frame[7][4+count],"❍  ");
          if(count!=0){
            strcpy(f16_frame[7][3+count],"⋅  ");
          }
          count++;
          break;
        }
      }
    }
    if(count == 4) break;
    //usleep(62500);
  }
  system("clear");
  printf("\nTop floor\n");
  for (i = 0; i < 11; i++) {
    for (j = 0; j < 11; j++) {
      printf("%s", f16_frame[i][j]);
      if(j == 10)printf("\n");
    }
  }
  printf(" □――――――――――――――――――――――――――――――――――――――――――――――――――――――――---‐‐\n");
  //timeover
  if(limit_time>900){
    printf("  お偉いさん\n");
    printf("  　ソムリエ君.........................\n");
    printf("  　ばっかもーーーーん！遅い！なーにをもたもたしとるんじゃ！こんなんでは日が暮れてしまうじゃろ！\n");
    getchar();
    system("clear");
    printf("\n\nこのトイレのランクは..... \n\n");usleep(250000);
    printf(" ┏┿━━・・・\n");usleep(190000);
    printf(" ╂┘      ,o888888o.   \n"); usleep(190000);
    printf(" ┃      8888     `88. \n"); usleep(190000);
    printf(" :   ,8 8888       `8.\n");usleep(190000);
    printf(" :   88 8888              おえらいさんからのお言葉\n");usleep(190000);
    printf("     88 8888          　　□―――――――――――――――――――――――□ \n");usleep(130000);
    printf("     88 8888               また明日も来てくれるんじゃな？  \n");usleep(90000);
    printf("     88 8888               次はもう少し俊敏なソムリエ君を期待しておるぞ\n");usleep(90000);
    printf("     `8 8888       .8'  ･\n");usleep(90000);
    printf("        8888     ,88'   ┃  (Press Enter key...)\n");usleep(90000);
    printf("         `8888888P'    ┌╂ \n");usleep(90000);
    printf("　             ･ ･ ･･━━┿┛\n");
  }
  //hyperRank
  else if(itm[20].has_itm == 1){
    printf("  お偉いさん\n");
    printf("  　よく来てくれた！トイレのソムリエよ！\n");
    printf("  　おお！このトイレは私好みだ！私の大好きなギャル物のAVがあるじゃないか！\n");
    getchar();
    system("clear");
    printf("\n\nこのトイレのランクは..... \n\n");usleep(250000);
    printf(" ┏┿━━・・・\n");usleep(190000);
    printf(" ╂┘       d888888o.     \n"); usleep(190000);
    printf(" ┃      .`8888:' `88. \n"); usleep(190000);
    printf(" :     8 .`8888.   Y8   \n");usleep(190000);
    printf(" :     `8.`8888.            おえらいさんからのお言葉\n");usleep(190000);
    printf("        `8.`8888.       　  □―――――――――――――――――――――――□ \n");usleep(130000);
    printf("         `8.`8888.            君はこの会社にとどまるのは惜しい！  \n");usleep(90000);
    printf("          `8.`8888.           T〇T〇に就職するべきだよ！\n");usleep(90000);
    printf("      8b   `8.`8888.     ･    これからもより良いトイレを作ってくれよ！\n");usleep(90000);
    printf("      `8b.  ;8.`8888     ┃    (Press Enter key...)\n");usleep(90000);
    printf("　     `Y8888P ,88P'    ┌╂  \n");
    printf("                ･ ･ ･･━━┿┛\n");//AVget
  }
  else if(itm[0].has_itm == 1 && itm[3].has_itm == 1 && itm[6].has_itm == 1 && itm[12].has_itm == 1){
      printf("  お偉いさん\n");
      printf("  　よく来てくれた！トイレのソムリエよ！\n");
      printf("  　ふむふむ、どうにかトイレを作ることは出来たようじゃな.......\n");
      getchar();
    //hiscore
    if(total_point >= 26){
      system("clear");
      printf("\n\nこのトイレのランクは..... \n\n");usleep(250000);
      printf(" ┏┿━━・・・\n");usleep(190000);
      printf(" ╂┘        .8.   \n"); usleep(190000);
      printf(" ┃        .888.  \n"); usleep(190000);
      printf(" :       :88888.\n");usleep(190000);
      printf(" :      . `88888.         おえらいさんからのお言葉\n");usleep(190000);
      printf("       .8. `88888.  　  　□―――――――――――――――――――――――□ \n");usleep(130000);
      printf("      .8`8. `88888.        うーん、このトイレは実に良い！良いぞ！  \n");usleep(90000);
      printf("     .8' `8. `88888.       さすがソムリエ君、君をトイレ会長に任命しよう。\n");usleep(90000);
      printf("    .8'   `8. `88888.   ･\n");usleep(90000);
      printf("   .888888888. `88888.  ┃  (Press Enter key...)\n");usleep(90000);
      printf("  .8'       `8. `88888.┌╂ \n");usleep(90000);
      printf("　             ･ ･ ･･━━┿┛\n");

    }
    //lowscore
    else{
      system("clear");
      printf("\n\nこのトイレのランクは..... \n\n");usleep(250000);
      printf(" ┏┿━━・・・\n");usleep(190000);
      printf(" ╂┘  8 888888888o   \n"); usleep(190000);
      printf(" ┃   8 8888    `88.  \n"); usleep(190000);
      printf(" :   8 8888     `88\n");usleep(190000);
      printf(" :   8 8888     ,88       おえらいさんからのお言葉\n");usleep(190000);
      printf("     8 8888.   ,88'   　　□―――――――――――――――――――――――□ \n");usleep(130000);
      printf("     8 8888888888          うーん、このトイレは最悪だね。最悪。  \n");usleep(90000);
      printf("     8 8888    `88.        そんなソムリエ君、君は明日から来なくていいよ。自宅待機で。\n");usleep(90000);
      printf("     8 8888      88     ･\n");usleep(90000);
      printf("     8 8888    ,88'     ┃  (Press Enter key...)\n");usleep(90000);
      printf("     8 888888888P      ┌╂ \n");usleep(90000);
      printf("　             ･ ･ ･･━━┿┛\n");

    }
  }
  //Badend.集めていない
  else{

    printf("  お偉いさん\n");
    printf("  　よく来てくれた！トイレのソムリエよ！\n");
    printf("  　おお、ソムリエ君！必要なアイテムを集めずに\n");
    printf("  　ここに来てしまうとは何事だ！仕方ないやつだな。(Press enter key ...)\n");
    getchar();
    system("clear");
    printf("\n\nこのトイレのランクは..... \n\n");usleep(250000);
    printf(" ┏┿━━・・・\n");usleep(190000);
    printf(" ╂┘      ,o888888o.   \n"); usleep(190000);
    printf(" ┃      8888     `88. \n"); usleep(190000);
    printf(" :   ,8 8888       `8.\n");usleep(190000);
    printf(" :   88 8888              おえらいさんからのお言葉\n");usleep(190000);
    printf("     88 8888          　　□―――――――――――――――――――――――□ \n");usleep(130000);
    printf("     88 8888               お前にもう一度機会を与えよう！  \n");usleep(90000);
    printf("     88 8888               再びこのようなことが起こらぬことをわしは祈っている！ \n");usleep(90000);
    printf("     `8 8888       .8'  ･\n");usleep(90000);
    printf("        8888     ,88'   ┃  (Press Enter key...)\n");usleep(90000);
    printf("         `8888888P'    ┌╂ \n");usleep(90000);
    printf("　             ･ ･ ･･━━┿┛\n");
  }
  getchar();
}

//トイレのコメントとロックされているかを設定する
void setToiCommLock(){
  strcpy(toiret[0][3].toi_comment,"ドアがネジ止めされている...ドライバーを探せ！");
  toiret[0][3].locked = 1;

  strcpy(toiret[0][4].toi_comment,"やめてください、入ってます");

  strcpy(toiret[0][2].toi_comment,"誰か入っているみたいだ");
  toiret[0][2].locked = 1;

  strcpy(toiret[0][5].toi_comment,"黒ギャル「ｷｬｰｰｰｰｰｰｰ////」");
  //1Fここまで

  strcpy(toiret[1][2].toi_comment,"人が入っている・・・・");
  toiret[1][2].locked = 1;

  strcpy(toiret[1][4].toi_comment,"変態!!!!入らないで!//");
  //2Fここまで
  strcpy(toiret[2][1].toi_comment,"誰かが入っている、ここには入れない。");
  toiret[2][1].locked = 1;
  //3Fここまで
  strcpy(toiret[3][2].toi_comment,"誰か入っているみたいだ");
  toiret[3][2].locked = 1;

  strcpy(toiret[3][4].toi_comment,"金髪のお兄さん「なんか用かよ！！！」");

  strcpy(toiret[3][5].toi_comment,"ハンマーがあれば入れそうだ・・・・");
  toiret[3][5].locked = 1;
  //4Fここまで
  strcpy(toiret[4][2].toi_comment,"誰か入っているみたいだ");
  toiret[4][2].locked = 1;

  strcpy(toiret[4][4].toi_comment,"ハンマーがあれば入れそうだ・・・・");
  toiret[4][4].locked = 1;
  //5Fここまで
  strcpy(toiret[5][1].toi_comment,"誰か入っていて開けられない！！");
  toiret[5][1].locked = 1;

  strcpy(toiret[5][3].toi_comment,"このトイレまじ臭え...必要なアイテムはなさそうだ");

  strcpy(toiret[5][4].toi_comment,"「おい！頼む！トイレットペーパーをくれ！」");
  toiret[5][4].locked = 1;
  //6Fここまで
  strcpy(toiret[6][0].toi_comment,"ﾇﾁｬ....何か踏んだようだ");

  strcpy(toiret[6][4].toi_comment,"便所飯食ってる！！！");

  strcpy(toiret[6][3].toi_comment,"おじさんが話しかけてきた。[x]を押してみよう");
  toiret[6][3].locked = 1;

  strcpy(toiret[6][5].toi_comment,"美少女「ｷｬ！ｰー変態」");
  //7Fここまで
  strcpy(toiret[7][4].toi_comment,"香水の匂い...きっと前の人は可愛いのだろう..");
  strcpy(toiret[7][2].toi_comment,"第六感が塩をまけと言っている...");
  toiret[7][2].locked = 1;
  //8Fここまで
  strcpy(toiret[8][0].toi_comment,"黒ギャルがトイレで化粧をしている");
  toiret[8][0].locked = 1;

  strcpy(toiret[8][3].toi_comment,"老朽化が進んでて開きそうにない...");
  toiret[8][3].locked = 1;
  //9Fここまで

  strcpy(toiret[9][2].toi_comment,"流れてない！！......");

  strcpy(toiret[9][3].toi_comment,"中からいやらしい声が聞こえてくる...");
  toiret[9][3].locked = 1;

  //10Fここまで
  strcpy(toiret[10][1].toi_comment,"和式だ！！！！！");

  strcpy(toiret[10][3].toi_comment,"水でﾋﾞﾁｬﾋﾞﾁｬ！！！");
  //11Fここまで

  strcpy(toiret[11][1].toi_comment,"開かないみたいだ");
  toiret[11][1].locked = 1;

  strcpy(toiret[11][4].toi_comment,"美人な女性「金とるわよ！！」");
  //12Fここまで
  strcpy(toiret[12][2].toi_comment,"ちぢれ毛が散乱している...");

  strcpy(toiret[12][4].toi_comment,"開かないみたいだ");
  toiret[12][4].locked = 1;
  //13Fここまで

  strcpy(toiret[13][3].toi_comment,"開きそうにない!!!");
  toiret[13][3].locked = 1;
  //14Fここまで
  strcpy(toiret[14][1].toi_comment,"黒ギャル「ｷｬｰｰｰｰｰｰ！！！！」");
  //15Fここまで
  //16Fここまで
  //17Fここまで
}

//アイテムを設定する
void setItem(){
  strcpy(itm[0].itm_name,"便座");
  itm[0].point = 0;
  itm[0].main_itm = 1;
  itm[0].itm_x = 2;
  itm[0].itm_y = 3;
  itm[0].itm_z = 0;
  strcpy(itm[0].comm1,"メインアイテムの一つ");
  strcpy(itm[0].comm2,"これが無くても座ることは出来るが");
  strcpy(itm[0].comm3,"衛生的、倫理的にも必需品であると言える");

  //1F.item
  strcpy(itm[1].itm_name,"ドライバー");
  itm[1].point = 2;
  itm[1].main_itm = 0;
  itm[1].itm_x = 4;
  itm[1].itm_y = 3;
  itm[1].itm_z = 1;
  strcpy(itm[1].comm1,"ネジを回すことが出来る工具");
  strcpy(itm[1].comm2,"実はマイナス型やプラス型だけではなく");
  strcpy(itm[1].comm3,"星型、Y字型、三角形など種類が豊富である");
  //2F.item
  strcpy(itm[22].itm_name,"便座カバー");
  itm[22].point = 7;
  itm[22].main_itm = 0;
  itm[22].itm_x = 4;
  itm[22].itm_y = 0;
  itm[22].itm_z = 3;
  strcpy(itm[22].comm1,"その名の通り便座に付けるカバー");
  strcpy(itm[22].comm2,"便座が程よく暖かくなるようになってから");
  strcpy(itm[22].comm3,"少し存在感が薄くなっているのが悩みどころ");
  //4F.item
  strcpy(itm[2].itm_name,"紙やすり");
  itm[2].point = 2;
  itm[2].main_itm = 0;
  itm[2].itm_x = 4;
  itm[2].itm_y = 3;
  itm[2].itm_z = 5;
  strcpy(itm[2].comm1,"木材や金属に擦って使用する");
  strcpy(itm[2].comm2,"もしトイレットペーパーがない場合に");
  strcpy(itm[2].comm3,"紙やすりを使うかどうかは議論が必要である");

  strcpy(itm[3].itm_name,"タンク");
  itm[3].point = 0;
  itm[3].main_itm = 1;
  itm[3].itm_x = 3;
  itm[3].itm_y = 1;
  itm[3].itm_z = 4;
  strcpy(itm[3].comm1,"メインアイテムの一つ");
  strcpy(itm[3].comm2,"水の入ったペットボトルを入れれば");
  strcpy(itm[3].comm3,"流水量を調整可能で節水ブームの火付け役");

  strcpy(itm[21].itm_name,"カレンダー");
  itm[21].point = 0;
  itm[21].main_itm = 0;
  itm[21].itm_x = 0;
  itm[21].itm_y = 2;
  itm[21].itm_z = 4;
  strcpy(itm[21].comm1,"今日が何日か分かる一品");
  strcpy(itm[21].comm2,"家庭のトイレに貼ったりすると");
  strcpy(itm[21].comm3,"両親の誕生日や記念日が確認出来ることも");

  //
  strcpy(itm[4].itm_name,"消臭スプレー");
  itm[4].point = 2;
  itm[4].main_itm = 0;
  itm[4].itm_x = 3;
  itm[4].itm_y = 3;
  itm[4].itm_z = 5;
  strcpy(itm[4].comm1,"1PUSHで嫌な匂いとおさらば出来る");
  strcpy(itm[4].comm2,"間違っても人の口に使用してはいけないが");
  strcpy(itm[4].comm3,"驚くことに犬用の口腔消臭スプレーは存在する");

  strcpy(itm[5].itm_name,"ブラシ");
  itm[5].point = 2;
  itm[5].main_itm = 0;
  itm[5].itm_x = 4;
  itm[5].itm_y = 0;
  itm[5].itm_z = 5;
  strcpy(itm[5].comm1,"トイレの清掃用具といえばコレ");
  strcpy(itm[5].comm2,"清掃後に濡れたまま戻してしまいがち");
  strcpy(itm[5].comm3,"次回カビだらけというサプライズが待っている");
  //6F.item
  strcpy(itm[6].itm_name,"便器");
  itm[6].point = 0;
  itm[6].main_itm = 1;
  itm[6].itm_x = 3;
  itm[6].itm_y = 3;
  itm[6].itm_z = 6;
  strcpy(itm[6].comm1,"メインアイテムの一つ");
  strcpy(itm[6].comm2,"これ無しでトイレを語ることは出来ない");
  strcpy(itm[6].comm3,"日本の小学校の洋式導入率は約４０％未満");

  //7.item
  strcpy(itm[7].itm_name,"壁紙");
  itm[7].point = 2;
  itm[7].main_itm = 0;
  itm[7].itm_x = 4;
  itm[7].itm_y = 3;
  itm[7].itm_z = 6;
  strcpy(itm[7].comm1,"補強と装飾とをかけて壁に貼る紙。");
  strcpy(itm[7].comm2,"壁紙の後ろにお金を隠し");
  strcpy(itm[7].comm3,"脱税する人が後を絶たない");
  //7F.item
  strcpy(itm[8].itm_name,"日本人形");
  itm[8].point = -5;
  itm[8].main_itm = 0;
  itm[8].itm_x = 0;
  itm[8].itm_y = 3;
  itm[8].itm_z = 7;
  strcpy(itm[8].comm1,"日本の伝統的な風俗を移した人形");
  strcpy(itm[8].comm2,"リカちゃん人形とは親友であり");
  strcpy(itm[8].comm3,"永遠のライバル");

  strcpy(itm[9].itm_name,"トイレマット");
  itm[9].point = 2;
  itm[9].main_itm = 0;
  itm[9].itm_x = 4;
  itm[9].itm_y = 3;
  itm[9].itm_z = 7;
  strcpy(itm[9].comm1,"トイレ内に敷くマット");
  strcpy(itm[9].comm2,"洗濯は２週間に一度程度でよい");
  strcpy(itm[9].comm3,"抗菌仕様");
  //8F.item
  strcpy(itm[10].itm_name,"芳香剤");
  itm[10].point = 2;
  itm[10].main_itm = 0;
  itm[10].itm_x = 0;
  itm[10].itm_y = 3;
  itm[10].itm_z = 8;
  strcpy(itm[10].comm1,"悪臭をマスキングする剤");
  strcpy(itm[10].comm2,"芳香剤と消臭剤は違う");
  strcpy(itm[10].comm3,"ラベンダーの香り");

  strcpy(itm[11].itm_name,"週刊少年JAMP");
  itm[11].point = 2;
  itm[11].main_itm = 0;
  itm[11].itm_x = 2;
  itm[11].itm_y = 1;
  itm[11].itm_z = 8;
  strcpy(itm[11].comm1,"男性向け週刊誌主にアダルトな面は少ない");
  strcpy(itm[11].comm2,"過去の連載作品の「To LOVEる」は");
  strcpy(itm[11].comm3,"男子小学生に夢を与えてくれた");


  //9F.item
  strcpy(itm[12].itm_name,"トイレットペーパー");
  itm[12].point = 0;
  itm[12].main_itm = 1;
  itm[12].itm_x = 4;
  itm[12].itm_y = 0;
  itm[12].itm_z = 9;
  strcpy(itm[12].comm1,"メインアイテムの一つ");
  strcpy(itm[12].comm2,"水で溶ける紙");
  strcpy(itm[12].comm3,"横幅114ｍｍに統一されている");

  //10F.item
  strcpy(itm[13].itm_name,"トイレ用スリッパ");
  itm[13].point = 2;
  itm[13].main_itm = 0;
  itm[13].itm_x = 1;
  itm[13].itm_y = 3;
  itm[13].itm_z = 10;
  strcpy(itm[13].comm1,"足の前側部分のみを覆う履き物");
  strcpy(itm[13].comm2,"来客の際には必須といえる");
  strcpy(itm[13].comm3,"履いたまま出てきてしまう人は多い");


  strcpy(itm[14].itm_name,"泡ハンドソープ");
  itm[14].point = 2;
  itm[14].main_itm = 0;
  itm[14].itm_x = 2;
  itm[14].itm_y = 0;
  itm[14].itm_z = 10;
  strcpy(itm[14].comm1,"手を洗う際に使用する");
  strcpy(itm[14].comm2,"指の間までモミモミ");
  strcpy(itm[14].comm3,"することが重要な使用用法");

  //11F.item
  strcpy(itm[15].itm_name,"バニーガール");
  itm[15].point = 2;
  itm[15].main_itm = 0;
  itm[15].itm_x = 0;
  itm[15].itm_y = 2;
  itm[15].itm_z = 12;
  strcpy(itm[15].comm1,"トイレ用のホステス");
  strcpy(itm[15].comm2,"トイレットペーパーを切り取る");
  strcpy(itm[15].comm3,"大切な役割を果たす");


  strcpy(itm[16].itm_name,"灰皿");
  itm[16].point = 2;
  itm[16].main_itm = 0;
  itm[16].itm_x = 2;
  itm[16].itm_y = 3;
  itm[16].itm_z = 12;
  strcpy(itm[16].comm1,"煙草の喫煙する際に出た灰を");
  strcpy(itm[16].comm2,"入れるためののもの");
  strcpy(itm[16].comm3,"明治時代では小学生では煙草が吸えた");

  //13F.item
  strcpy(itm[17].itm_name,"ショックレスハンマー");
  itm[17].point = 2;
  itm[17].main_itm = 0;
  itm[17].itm_x = 2;
  itm[17].itm_y = 1;
  itm[17].itm_z = 13;
  strcpy(itm[17].comm1,"物を打ち付けたり、壊すために使う");
  strcpy(itm[17].comm2,"重りの移動によって反動を吸収し");
  strcpy(itm[17].comm3,"衝撃を吸収する");

  //14F.item
  strcpy(itm[18].itm_name,"塩");
  itm[18].point = 2;
  itm[18].main_itm = 0;
  itm[18].itm_x = 3;
  itm[18].itm_y = 0;
  itm[18].itm_z = 14;
  strcpy(itm[18].comm1,"料理、魔除けに使用される");
  strcpy(itm[18].comm2,"とてもしょっぱく");
  strcpy(itm[18].comm3,"ナメクジの天敵");


  strcpy(itm[19].itm_name,"ウォシュレット");
  itm[19].point = 2;
  itm[19].main_itm = 0;
  itm[19].itm_x = 3;
  itm[19].itm_y = 3;
  itm[19].itm_z = 14;
  strcpy(itm[19].comm1,"尻を洗浄する");
  strcpy(itm[19].comm2,"水圧が大きいと");
  strcpy(itm[19].comm3,"とてもびっくりする");

  //15F.item
  strcpy(itm[20].itm_name,"AV");
  itm[20].point = 8;
  itm[20].main_itm = 0;
  itm[20].itm_x = 3;
  itm[20].itm_y = 2;
  itm[20].itm_z = 16;
  strcpy(itm[20].comm1,"マジックミラー号収録");
  strcpy(itm[20].comm2,"様々なトラブルを招く");
  strcpy(itm[20].comm3,"取扱注意");

  //17F.item
}
//鏡のコメントを設定する
void setMirrorComment(){
  //1F - 1番目のトイレのコメント
  strcpy(mirror[0][0].mirr_comment,"だいぶ汚れている");
  strcpy(mirror[0][1].mirr_comment,"自分の顔が映っている...");
  strcpy(mirror[1][0].mirr_comment,"「豚（トン）」\n何かのヒントだろうか...");
  strcpy(mirror[1][1].mirr_comment,"この鏡めっちゃ曇ってる...");
  strcpy(mirror[2][0].mirr_comment,"ヌメリがすごい...");
  strcpy(mirror[3][0].mirr_comment,"自分の顔が映っている...");
  strcpy(mirror[4][0].mirr_comment,"「分からなければヒントを探せ」と書いてある");
  strcpy(mirror[4][1].mirr_comment,"");
  strcpy(mirror[5][1].mirr_comment,"「尖っている」\n何かのヒントだろうか.");
  strcpy(mirror[6][0].mirr_comment,"この鏡めっちゃ割れてる..");
  strcpy(mirror[7][0].mirr_comment,"鏡に血が付いている...");
  strcpy(mirror[7][1].mirr_comment,"ｻｰｰｰｰ....背後に人影のようなものが通ったような気がした...");
  strcpy(mirror[8][0].mirr_comment,"毛がすげえ!!!!");
  strcpy(mirror[8][1].mirr_comment,"自分の顔見てみろブス！！と書いてある...");
  strcpy(mirror[9][0].mirr_comment,"「離婚(リコン)」何かのヒントだろうか.");
  strcpy(mirror[9][1].mirr_comment,"自分の顔が映っている");
  strcpy(mirror[10][1].mirr_comment,"おれの顔マジ汚なくね？");
  strcpy(mirror[11][0].mirr_comment,"ヌメリがすごい...");
  strcpy(mirror[11][1].mirr_comment,"鏡がとても曇っている");
  strcpy(mirror[12][0].mirr_comment,"蛇口やレバーがやけにキレイだ...");
  strcpy(mirror[13][1].mirr_comment,"「絶対食べたことある」\n何かのヒントだろうか...");
  strcpy(mirror[14][0].mirr_comment,"「一度は指にはめたことがあるはず」\n何かのヒントだろうか...");
  strcpy(mirror[14][1].mirr_comment,"丁寧に塩が盛られている");
}

char coll_frame[23][64] = {" -"," -"," -"," -"," -"," -",
                                 " -"," -"," -"," -"," -"," -",
                                 " -"," -"," -"," -"," -"," -",
                                 " -"," -"," -"," -"," -"};
//int pointer = 0;
void showCollection(){

  int i;
  int pointer = 0;
  char c;
  char name[64] = "Unknown";
  char cm1[64],cm2[64],cm3[64];
  if(scene == 1){
    for (i = 0; i < total_itm; i++) {
      strcpy(coll_frame[i]," -");
      if(itm[i].has_itm == 1){
        strcpy(coll_frame[i]," ●");
      }
    }
  }

  else{
    for (i = 0; i < total_itm; i++) {
      if(itm[i].had_itm == 1){
        strcpy(coll_frame[i]," ●");
      }
    }
  }
  while(1){

    if(strcmp(coll_frame[pointer]," ●") == 0){
      strcpy(coll_frame[pointer],"➭●");
      strcpy(name,itm[pointer].itm_name);
      strcpy(cm1,itm[pointer].comm1);
      strcpy(cm2,itm[pointer].comm2);
      strcpy(cm3,itm[pointer].comm3);
    }
    if(strcmp(coll_frame[pointer]," -") == 0){
      strcpy(coll_frame[pointer],"➭-");
      strcpy(name,"Unknown");
      strcpy(cm1,"You haven't got it yet ...");
      strcpy(cm2,"");
      strcpy(cm3,"");
    }
    system("clear");
    if (scene == 1) {
      printf("\n\nItem Collection  TotalPoint : %d\n\n",total_point);
    }else{
      printf("\n\nItem Collection\n\n");
    }
    printf("  ┌                         ┐");
    printf("    \" %s \"\n", name);
    printf("    %s  %s  %s  %s  %s  %s   ",coll_frame[0],coll_frame[1],coll_frame[2],coll_frame[3],coll_frame[4],coll_frame[5]);
    printf("   □――――――――――――――――――――――――――――――□\n");
    printf("                                  %s\n",cm1);
    printf("    %s  %s  %s  %s  %s  %s      ",coll_frame[6],coll_frame[7],coll_frame[8],coll_frame[9],coll_frame[10],coll_frame[11]);
    printf("  %s\n",cm2);
    printf("                                  %s\n",cm3);
    printf("    %s  %s  %s  %s  %s  %s   \n",coll_frame[12],coll_frame[13],coll_frame[14],coll_frame[15],coll_frame[16],coll_frame[17]);
    printf("\n");
    printf("    %s  %s  %s  %s  %s           ",coll_frame[18],coll_frame[19],coll_frame[20],coll_frame[21],coll_frame[22]);
    printf("[w]: ↑  [a]: ←  [s]: ↓  [d]: →\n");
    printf("  └                         ┘ ");
    if(scene ==1){
      printf("   [z]: Back to Game\n\n");
    }else{
      printf("   [z]: Back to Title\n\n");
    }
    if(strcmp(coll_frame[pointer],"➭●") == 0) strcpy(coll_frame[pointer]," ●");
    if(strcmp(coll_frame[pointer],"➭-") == 0) strcpy(coll_frame[pointer]," -");


    while(1){
      if(kbhit()){
        c = getch();
        if(c == 'w'){
          if(pointer > 5) pointer -= 6;
          break;
        }
        if(c == 'a'){
          if(pointer%6 != 0) pointer--;
          break;
        }
        if(c == 's'){
          if(pointer < 17) pointer += 6;
          break;
        }
        if(c == 'd'){
          if(pointer%6 != 5 && pointer != 22) pointer++;
          break;
        }
        if(c == 'z'){
          pointer = 0;
          return;
        }
        break;
      /*
      switch (c) {
        case 'w':
        printf("\n");
        if(pointer > 5) pointer -= 6;
        break;

        case 'a':
        printf("\n");
        if(pointer%6 != 0) pointer--;
        break;

        case 's':
        printf("\n");
        if(pointer < 18) pointer += 6;
        break;

        case 'd':
        printf("\n");
        if(pointer%6 != 5 && pointer != 21) pointer++;
        break;

      }
      */
      }
      usleep(62500);
    }
  }
}

void to17f(){
  clearFloor();
  somm.now_floor = 17;
  somm.now_x = 3;
  somm.now_y = 8;
  strcpy(map_frame[8][3],"❍  ");
  showFloor(16);
  getchar();
  printf("  壁に何か書いてある・・・\n  「このフロアには幻のビデオがが存在する・・・・\n  落とし穴だらけなので気を付けろ\n  ヒントはない・・・・さあ・・・・行ってこい！！！」 ");
  getchar();
}

int GetRandom(int min,int max){
	return min + (int)(rand()*(max-min+1.0)/(1.0+RAND_MAX));
}

int generateProbability(int percent){
	int tmp = GetRandom(1,100);
	if (tmp < percent) {
		return 1;
	}
	return 0;
}

void changePlayerNum(){
	//lowを選んだ時
    if(test_hl == 0){
      while(player_num<dealer_num){
			player_num = GetRandom(1,14);
		  }
	  }
	//highを選んだ時
    if(test_hl == 1){
      while(player_num>dealer_num){
        player_num = GetRandom(1,14);
      }
    }

}

//---------------------------------------------------
/*
void toTest(){
  int c,i;
  int x=2;
  int y=0;
  int seiseki[3][3] = {{0, 0, 0},{0, 0, 0},{1, 0, 0}};
  //int seiseki[2][3] = {{72, 67, 84}, {67, 92, 71}};
  puts("何かキーを押したら終��?します��?");
  while (1) {
    if (kbhit()) {
      c = getch();
      if(c =='w'){
        if(x>0){
          seiseki[x][y]=0;
          x--;
          seiseki[x][y]=1;
        }
        printf("wが押されたよ\n");
      }
      if(c=='a'){
        printf("aが押されたよ\n");

      }
      if(c=='d'){
        printf("dが押されたよ\n");
        printf("フロアごとのmirrorの数: %lu\n",sizeof(mirror_pattern[0][0]) / sizeof(toiret_pattern[0][0][0]));
        printf("フロアごとのtoiretの数: %lu\n",sizeof(toiret_pattern[0]) / sizeof(toiret_pattern[0][0]));
        for (size_t i = 0; i < 16; i++) {
          printf("%luF_DATA\n", i+1);
          for (size_t s = 0; s < 6; s++) {
            printf(" toiret-number: %lu\n", s+1);
            printf("  toi_x: %d\n", toiret[i][s].toi_x);
            printf("  toi_y: %d\n", toiret[i][s].toi_y);
            printf("  toi_in: %d\n", toiret[i][s].toi_in);
          }
          for (size_t s = 0; s < 2; s++) {
            printf(" mirror-number: %lu\n", s+1);
            printf("  mirr_x: %d\n", mirror[i][s].mirr_x);
            printf("  mirr_y: %d\n", mirror[i][s].mirr_y);
          }
          printf(" cupboard\n");
          printf("  cup_x: %d\n", cupboard[i].cup_x);
          printf("  cup_y: %d\n", cupboard[i].cup_y);
          printf("  cup_in: %d\n", cupboard[i].cup_in);
        }
        getchar();
      }
      if(c=='s'){
        printf("sが押されたよ\n");
        xmlTextReaderPtr reader;
        int ret;
        reader = xmlNewTextReaderFilename("./Toiret.xml");
        ret = xmlTextReaderRead(reader);
        while (ret == 1) {
          if(xmlTextReaderNodeType(reader) == XML_READER_TYPE_ELEMENT && xmlStrcmp(xmlTextReaderName(reader), BAD_CAST "toiret") == 0){
            printf("toiret node start\n");
            ret = xmlTextReaderRead(reader);
            while (!xmlStrcmp(xmlTextReaderName(reader), BAD_CAST "toiret")==0){
              if(xmlTextReaderNodeType(reader) == XML_READER_TYPE_TEXT){
                printf("%s\n", xmlTextReaderValue(reader));
              }
              ret = xmlTextReaderRead(reader);
            }
          }else{
            ret = xmlTextReaderRead(reader);
          }
        }

        //  Reader のすべてのリソースを開放
        xmlFreeTextReader(reader);

        getchar();
      }
      if(c=='z'){
        printf("zが押されたから抜け�?�すよ\n");
        printf("\n");

        xmlTextReaderPtr reader;
        int ret;
        reader = xmlNewTextReaderFilename("./Toiret.xml");
        ret = xmlTextReaderRead(reader);
        while (ret == 1) {
          //  次のノ�?�ドに移��?
          printf("NodeType: %d\n", xmlTextReaderNodeType(reader));
          printf(" ReaderName: %s\n", xmlTextReaderName(reader));
          printf("  ReaderValue: %s\n", xmlTextReaderValue(reader));
          printf("       compare: %d\n", xmlStrcmp(xmlTextReaderName(reader), BAD_CAST "toiret"));
          ret = xmlTextReaderRead(reader);
        }
        //  Reader のすべてのリソースを開放
        xmlFreeTextReader(reader);

        getchar();
        break;
      }
      system("clear");
      for(i=0; i<3; i++){
        printf("%d ", seiseki[i][0]);
        printf("%d ", seiseki[i][1]);
        printf("%d \n", seiseki[i][2]);
      }

    }
  usleep(62500);
  }
}


*/
//-------------------------------------------------
//リアルタイ��?入力に��?要な関数たち

int kbhit(void){
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }

    return 0;
}

int getch( ) {
  struct termios oldt,
                 newt;
  int            ch;
  tcgetattr( STDIN_FILENO, &oldt );
  newt = oldt;
  newt.c_lflag &= ~( ICANON | ECHO );
  tcsetattr( STDIN_FILENO, TCSANOW, &newt );
  ch = getchar();
  tcsetattr( STDIN_FILENO, TCSANOW, &oldt );
  return ch;
}

//もし libxml2 を使��?たいなら　これでコンパイルする
//gcc -I /usr/include/libxml2 sample.c -lxml2
//
