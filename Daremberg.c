#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include "SFMT.h"/*メルセンヌ・ツイスタをインクルード*/

sfmt_t sfmt;/*メルセンヌ・ツイスタを使う際に必要な構造体への変数*/

/*プロトタイプ宣言*/
double Max1(double, double);
int Max2(int, int);
double Min1(double, double);
int Min2(int, int);
int Gamble(double, int *, int *);
double Winrate(int *, int *, int);

int main(void)
{
  int count=0, count2=0, i=0, j=0, flag1=0, strwin=0, strlose=0, maxstrwin=0, maxstrlose=0, maxstrwin2=0, maxstrlose2=0, maxcountwin=0, maxcountlose=0, mincountwin=0, mincountlose=0, totalcountwin=0, totalcountlose=0, wincount=0, losecount=0, losemaxmoneycount=0,  wincount2=0;
  
  float rate=0.0, winrate=0.0,  money=0.0, budget=0.0, firstbet=0.0, bet=0.0, change=0.0, maxbet=0.0, maxmoney=0.0, minmoney=0.0, totalbet=0.0, totalpayout=0.0, maxbet2=0.0, maxmoney2=0.0, minmoney2=0.0, totalmaxmoney=0.0, maxbetwin=0.0, maxbetlose=0.0, collect1=0.0, losemoney=0.0, maxmaxmoney=0.0, minmaxmoney=0.0, totalpayout2=0.0, totalbet2=0.0, k=0.0, l=0.0;

  
  printf("資金を入力してください:");
  scanf("%f", &money);
  printf("最初の賭け金:");
  scanf("%f", &firstbet);
  printf("bet額の増減:");
  scanf("%f", &change);
  printf("ダランベール法の試行回数の入力:");
  scanf("%d", &count);
  printf("ダランベール法1周あたりの試行最高ゲーム数を入力(0より大きい数)(予め決めない場合は-1を入力)：");
  scanf("%d", &count2);
  printf("使用するゲームの勝率を入力してください:");
  scanf("%f", &rate);
  printf("\n");

  mincountwin = (int)firstbet/change;
  if(mincountwin < 1){
    mincountwin = 1;
  }//成功した時の最小連勝回数の初期化
  k = firstbet;
  l = firstbet;
  while(k <= money){//失敗した時の最小連敗回数の初期化
    mincountlose++;
    l = l + change;
    k += l;
  }

  budget = money;//始めの資金を予算に設定
  maxmaxmoney = budget;
  minmaxmoney = budget;

  sfmt_init_gen_rand(&sfmt, (unsigned)time(NULL));/*現在時間をseedとした乱数の生成*/

  while(i < count){//１つ目のループ　決めた試行回数以内で
    bet = firstbet;//賭け金を最初の賭け金に
    money = budget;//一周ごとに持金を最初の持金（予算）に戻す
    minmoney = budget;//最低持金を最初の持金（予算）に設定
    maxmoney = budget;//最高持金を最初の持金（予算）に設定
    
    while(bet > 0 && money >= 0){//２つ目のループ　bet額が０より大きいかつ持金が無くなっていなかったら
      /*if(money > budget){//利益が出た時点で、ループ抜けるオプション
	break;
      }
      */
      if(count2 != -1){//予め１周単位の最高試行回数を決めたら
	if(j >= count2){//その最高試行回数をこえたら
	  break;//ループ抜ける
	}
      }
      if(bet <= money){//bet額が持金以下なら
	flag1 = Gamble(rate, &strwin, &strlose);/*ゲームの勝敗を決める*/
	totalbet += bet;//１周のトータルのbet額を計算
	maxbet = Max1(maxbet, bet);//一周ごとの最高bet額
	if(flag1 == 1){//ゲームに勝ったら
	  totalpayout += bet*2;//一周のトータルの払い出し額の計算
	  money += bet;//bet額を受け取る
	  //printf("勝ちました　bet額:%.1f　持金:%.1f\n", bet, money);/////////////////////////////////////////////////////////////////
	  bet -= change;//決めた増減額をbet額から差し引く
	  maxmoney = Max1(maxmoney, money);//一周ごとの最高持金
	  maxstrwin = Max2(maxstrwin, strwin);//一周ごとの最高連勝数
	}else if(flag1 == 0){//ゲームに負けたら
	  money -= bet;//持金からbetした額を差し引く
	  //printf("負けました　bet額:%.1f　持金:%.1f\n", bet, money);////////////////////////////////////////////////////////////////
	  bet += change;//決めた増減額をbet額に足す
	  minmoney = Min1(minmoney, money);//一周ごとの最低持金
	  maxstrlose = Max2(maxstrlose, strlose);//一周ごとの最高連敗数
	}
      }else{//bet額が持金を上回っていたら
	break;//ループ抜ける
      }
      j++;//ゲーム数
      winrate = Winrate(&j, &wincount2, flag1);/*数列が消える、または破産するまでの勝率を計算*/
    }//２つめのループ終わり
    
    if(money >= budget){//一周を成功で終わらせられたら
      //printf("成功しました\n\n");///////////////////////////////////////////////////////////////////////////////
      collect1 += (money-budget);//利益の計算
       
      wincount++;//成功回数
      
      minmoney2 = Min1(minmoney2, minmoney);//成功時の全体の最低持金
      maxcountwin = Max2(maxcountwin, j);//全体の成功するまでにかかった最高ゲーム数
      mincountwin = Min2(mincountwin, j);//全体の成功するまでにかかった最低ゲーム数
      totalcountwin += j;//成功までにかかった総ゲーム数→全体の平均を出すために使う
      
      maxmaxmoney = Max1(maxmaxmoney, maxmoney);//成功した時の、最高の最高持金
      minmaxmoney = Min1(minmaxmoney, maxmoney);//成功した時の、最低の最高持金
      totalmaxmoney += maxmoney;//成功した時の最高持金の和→全体の平均を出すために使う

      maxbetwin = Max1(maxbetwin, maxbet);//成功時の最高bet額
    }else{//一周を失敗で終わったら
      //printf("失敗しました\n\n");///////////////////////////////////////////////////////////////////////////////
      losemoney += (budget-money);//負け額を計算
      
      losecount++;//失敗回数の計算
      
      maxcountlose = Max2(maxcountlose, j);//全体の失敗までにかかった最高ゲーム数
      mincountlose = Min2(mincountlose, j);//全体の失敗までにかかった最低ゲーム数
      totalcountlose += j;//失敗までにかかった総ゲーム数→全体の平均を出すために使う
      
      if(maxmoney <= budget){//失敗した時の最高持金が予算と同じまたはそれ以下のとき（ 一度も予算を上回らなかったとき）の回数を計算
	losemaxmoneycount++;
	//printf("%f\n", maxmoney);///////////////////////////////////////////////////////////////////////////////////////////////////////
      }

      maxbetlose = Max1(maxbetlose, maxbet);//失敗した時の最高bet額
      
    }
    maxstrwin2 = Max2(maxstrwin2, maxstrwin);//全体の最高連勝数
    maxstrlose2 = Max2(maxstrlose2, maxstrlose);//全体の最高連敗数

    totalpayout2 += totalpayout;//全体トータルの払い出し額の計算
    totalbet2 += totalbet;//全体トータルのbet額の計算

    //printf("一周ごとの、最高bet額:%.1f, 最高持金:%.1f, 最低持金:%.1f, 最高連勝数:%d, 最高連敗数:%d, かかったゲーム数:%d, 勝率:%.2f％, ペイアウト率:%.2f％, 収支:%.1f\n\n", maxbet, maxmoney, minmoney, maxstrwin, maxstrlose, j, winrate, (totalpayout/totalbet)*100, money-budget);//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////  
    
    /*初期化の必要な変数を初期化*/
    maxbet = 0.0;
    maxmoney = budget;
    strwin = 0;
    maxstrwin = 0;
    minmoney = budget;
    strlose = 0;
    maxstrlose = 0;
    j = 0;
    wincount2 = 0;
    totalbet = 0.0;
    totalpayout = 0.0;
    
    i++;
  }//１つめのループ終わり
  printf("ダランベール法の、\n");
  printf("成功回数:%d, 失敗回数:%d\n\n", wincount, losecount);
  printf("成功時の、かかったゲーム数の範囲:%d回〜%d回, 平均ゲーム数:%d回, 最高持金の範囲:%.1f〜%.1f, 平均最高持金:%.1f, 最高bet額:%.1f, 平均収益:%.1f\n\n", mincountwin, maxcountwin, totalcountwin/wincount, minmaxmoney, maxmaxmoney, totalmaxmoney/wincount, maxbetwin, (float)collect1/wincount);
  printf("失敗時の、かかったゲーム数の範囲:%d回〜%d回, 平均ゲーム数:%d, ただの一度も利益を生まなかった回数:%d, 最高bet額:%.1f\n\n", mincountlose, maxcountlose, totalcountlose/losecount, losemaxmoneycount, maxbetlose);
  printf("最高連勝回数:%d, 最高連敗回数:%d, ペイアウト率:%.2f％, 最終収支:%.1f\n\n", maxstrwin2, maxstrlose2, (totalpayout2/totalbet2)*100, collect1-losemoney);
  
	
  return 0;
}

/*
最高掛け金を更新する関数
仮引数：今までの最高賭け金、今回のbet額
戻り値：更新後の最高賭け金
*/
double Max1(double max, double current)
{
  if(current > max){/*今回の賭け金が今までよりも高ければ*/
    max = current;/*更新*/
  }

  return max;
}

/*
最高掛け金を更新する関数
仮引数：今までの最高賭け金、今回のbet額
戻り値：更新後の最高賭け金
*/ 
int Max2(int max, int current)
{
  if(current > max){/*今回の賭け金が今までよりも高ければ*/
    max = current;/*更新*/
  }

  return max;
}


/*
最高掛け金を更新する関数
仮引数：今までの最高賭け金、今回のbet額
戻り値：更新後の最高賭け金
*/
double Min1(double min, double current)
{
  if(current < min){/*今回の賭け金が今までよりも低ければ*/
    min = current;/*更新*/
  }

  return min;
}

/*
最高掛け金を更新する関数
仮引数：今までの最高賭け金、今回のbet額
戻り値：更新後の最高賭け金
*/
int Min2(int min, int current)
{
  if(current < min){/*今回の賭け金が今までよりも低ければ*/
    min = current;/*更新*/
  }

  return min;
}

/*
乱数生成器 メルセンヌ・ツイスタを用いて、入力された確率通りのゲームの勝敗の抽選を行う関数
仮引数：入力したゲームの勝率
戻り値：勝ったか負けたか
*/
int Gamble(double rate, int *strwin, int *strlose)
{
  int flag1;/*勝敗を示すフラグ*/
  
  if( sfmt_genrand_real2( &sfmt ) <= rate / 100 ){/*０から１の実数乱数を生成し、それが入力した確率より数値が低いかどうか*/
    
  //if( genrand_int32() % 1000 + 1 <= (int)(rate * 10)  ){
  //if(rand() % 1000 + 1 <= rate * 10){
    /*乱数割る1000の余り+1が入力した勝率✕10の値以下かどうか（入力した勝率で勝ったかどうか）*/
    flag1 = 1;/*勝ったら1を*/
    (*strlose) = 0;//連敗ストップ
    (*strwin)++;//連勝数の更新
  }else{
    flag1 = 0;/*負けたら0を*/
    (*strwin) = 0;//連勝ストップ
    (*strlose)++;//連敗数の更新
  }
  return flag1;/*1か0をmainに返す*/
}



/*
数列が消えるか、破産するまでの勝率を計算する関数
仮引数：ゲーム数を指すポインタ、勝った回数を指すポインタ、勝敗を表すフラグ
戻り値：計算した勝率を返す。
*/    
double Winrate(int *j, int *wincount2, int flag1)
{
  double winrate;/*計算する勝率*/
  
  if(flag1 == 1){/*ゲームに勝ったら*/
    (*wincount2)++;/*ゲームに勝った回数を+1*/
  }
  winrate = ( (double)*wincount2 / (double)*j ) * 100;/*勝率 = 勝った回数割るゲーム回数*/

  return winrate;
}
    



