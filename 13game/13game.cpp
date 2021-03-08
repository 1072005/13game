#include<iostream>
#include<Windows.h>
#include <iomanip>
#include <algorithm>
//
int i;
using namespace std;

extern "C" {
	__declspec(dllexport) bool __cdecl PokerGame(int* card, int* yourcardstyle, LPCWSTR enemy)
	{
		int icard[13] = { 0 };//卡牌號碼(原始1~52)
		int ivector[13] = { 0 };//卡牌號碼(轉換後1~13)

		int front[3] = { 0 };//頭墩
		int mid[5] = { 0 };//中墩
		int back[5] = { 0 };//尾墩

		int finaly[13] = { 0 };//最終回傳的牌

		//分類為 單張 對子 三條 鐵支(四張同數)
		//開頭加X為紀錄原始號碼1~52
		int a[13] = { 0 };
		int along[13] = { 0 };
		int Xalong[13] = { 0 };

		int pair1[13] = { 0 };
		int Xpair[13] = { 0 };

		int three[13] = { 0 };
		int Xthree[13] = { 0 };

		int four[13] = { 0 };
		int Xfour[13] = { 0 };

		int strai[13] = { 0 };
		int Xstrai[13] = { 0 };
		//紀錄牌形
		for (i = 0; i < 13; i++) {


			yourcardstyle[i] = card[i];


		}
		for (i = 0; i < 13; i++) {


			a[i] = yourcardstyle[i];


		}
		for (int i = 0; i < 13; i++)
		{
			ivector[i] = a[i];
			icard[i] = a[i];
		}
		//ivector % 13 轉換為 1~13
		for (int i = 0; i < 13; i++) {
			ivector[i] = ivector[i] % 13;
			if (ivector[i] == 0)
				ivector[i] = 13;
		}

		for (int i = 12; i >= 0; i--) {
			for (int j = 0; j < i; j++) {
				if (ivector[j] > ivector[j + 1]) {
					//ivector排序
					int change;
					change = ivector[j];
					ivector[j] = ivector[j + 1];
					ivector[j + 1] = change;
					//icard跟著ivector排序
					change = icard[j];
					icard[j] = icard[j + 1];
					icard[j + 1] = change;
				}
			}
		}

		int f = 0;//記錄a有幾個挪幾次
		for (int i = 0; i < 13; i++) {

			if (ivector[i] == 1)
			{
				f++;
			}
		}
		//////////////////////////////////////////
			//向前挪f次
			//若k大於0 執行動的動作

		for (int i = 0; i < f; i++) {
			int change = ivector[0];
			for (int j = 0; j < 12; j++) {
				ivector[j] = ivector[j + 1];
			}
			ivector[12] = change;//最後一個位置放第一個
			change = icard[0];
			for (int j = 0; j < 12; j++) {
				icard[j] = icard[j + 1];
			}
			icard[12] = change;//最後一個位置放第一個
		}

		//判斷
		//找出雜牌紀錄到along &Xalong
		int n = 0;//陣列位置
		if (ivector[0] != ivector[1]) {//第1張
			along[n] = ivector[0];
			Xalong[n] = icard[0];
			n++;
		}
		for (int i = 1; i < 13; i++) {//從第2張開始(與前面和後面比較)
			if ((ivector[i] != ivector[i + 1]) && (ivector[i] != ivector[i - 1])) {
				along[n] = ivector[i];
				Xalong[n] = icard[i];
				n++;
			}
		}

		/*//雜牌中找順子
		for (int i = 1; i < 9; i++) {//從第2張開始(與前面和後面比較)
			if ((along[i + 4] - along[i]) == 4) {
				strai[]=(ivector[i])
			}
		}*/
		//找鐵支(四張相同)記錄到  four & Xfour
		n = 0;
		for (int i = 0; i < 13; i++) {
			if (ivector[i] == ivector[i + 1] && ivector[i + 1] == ivector[i + 2] && ivector[i + 2] == ivector[i + 3]) {
				for (int j = 0; j < 4; j++) {
					four[n] = (ivector[i + j]);//記錄到FOUR陣列
					Xfour[n] = (icard[i + j]);//記錄原始到XFOUR陣列
					//紀錄後值改為0
					ivector[i + j] = 0;
					n++;
				}

			}
		}



		//找三條(三張相同)記錄到 three & Xthree
		n = 0;
		for (int i = 0; i < 13; i++)
		{
			if (ivector[i] == ivector[i + 1] && ivector[i + 1] == ivector[i + 2] && ivector[i + 2] != ivector[i + 3]) {
				if (ivector[i] != 0) { //沒有被記錄過
					for (int j = 0; j < 3; j++) {
						three[n] = (ivector[i + j]);//記錄到THREE陣列
						Xthree[n] = (icard[i + j]);//記錄原始到XTHREE陣列
						//紀錄後值改為0
						ivector[i + j] = 0;
						n++;
					}
				}
			}
		}

		//找對子
		n = 0;
		for (int i = 0; i < 13; i++) {
			if (ivector[i] == ivector[i + 1]) {
				if (ivector[i] != 0) { //鐵支與三條已讀取的值已改為0 若符合條件但為0者便不被記錄下
					for (int j = 0; j < 2; j++) {
						pair1[n] = (ivector[i + j]);
						Xpair[n] = (icard[i + j]);
						n++;
					}
				}
			}
		}

		////////////////////////////////////////////
		//組合
		int s = 0;//紀錄單張拿到第幾張
		int p = 0; //紀錄對子拿到第幾張

		//若兩份鐵支 拿第二支 放後墩;
		if (Xfour[4] > 0) {
			for (int i = 0; i < 4; i++) {
				back[i] = Xfour[i + 4];
				Xfour[i + 4] = 0;//把使用過的紀錄為0
			}
			if (Xalong[s] > 0) {//放單張到後墩第五張
				back[4] = Xalong[s];
				Xalong[s] = 0;
				s++;
			}
			else {
				back[4] = Xpair[p];//沒有單張 放對子到後墩第五張
				Xpair[p] = 0;
				p++;
			}
		}
		//若一份鐵支 ;
		if (Xfour[0] != 0) {
			if (back[0] == 0) {//判斷尾墩為空 放入

				for (int i = 0; i < 4; i++) {
					back[i] = Xfour[i];
					Xfour[i] = 0;
				}
				if (Xalong[s] != 0) {
					back[4] = Xalong[s]; //後墩的第五張放一個單張
					Xalong[s] = 0;
					s++;
				}
				else {
					back[4] = Xpair[p];
					Xpair[p] = 0;
					p++;
				}

			}
			//若兩份鐵支拿前面那份
			else if (mid[0] == 0) {//判斷中墩為空 放入

				for (int i = 0; i < 4; i++) {
					mid[i] = Xfour[i];
					Xfour[i] = 0;
				}
				if (Xalong[s] != 0) {
					mid[4] = Xalong[s];
					Xalong[s] = 0;
					s++;
				}
				else {
					mid[4] = Xpair[p];
					Xpair[p] = 0;
					p++;
				}

			}
		}
		//葫蘆

		//若兩份葫蘆 拿第二支;
		if (Xthree[3] > 0 && Xpair[2] > 0)
		{
			if (back[0] == 0) {//判斷尾墩為空 放入
				for (int i = 0; i < 3; i++) {
					back[i] = Xthree[i + 3];//拿第二對的三條
					Xthree[i + 3] = 0;
				}
				for (int i = 0; i < 2; i++) {
					back[i + 3] = Xpair[i + 2];//拿第二對的對子
					Xpair[i + 2] = 0;
				}
			}

			else if (mid[0] == 0) {//判斷中墩為空 放入
				for (int i = 0; i < 3; i++) {
					mid[i] = Xthree[i + 3];
					Xthree[i + 3] = 0;
				}
				for (int i = 0; i < 2; i++) {
					mid[i + 3] = Xpair[i];
					Xpair[i] = 0;
				}
			}
		}
		//若一份葫蘆
		if (Xthree[0] > 0 && Xpair[0] > 0) {
			if (back[0] == 0) {//判斷尾墩為空 放入
				for (int i = 0; i < 3; i++) {
					back[i] = Xthree[i];
					Xthree[i] = 0;
				}
				for (int i = 0; i < 2; i++) {
					back[i + 3] = Xpair[i];
					Xpair[i] = 0;
				}
			}
			//若兩份葫蘆 或一份鐵支一份葫蘆 放小的胡盧
			else if (mid[0] == 0) {//判斷中墩為空 放入
				for (int i = 0; i < 3; i++) {
					mid[i] = Xthree[i + 3];
					Xthree[i + 3] = 0;
				}
				for (int i = 0; i < 2; i++) {
					mid[i + 3] = Xpair[i];
					Xpair[i] = 0;
				}
			}
		}
		//三條
		//若只有兩份三條 塞第二份三條
		if (Xthree[3] > 0)
		{
			if (back[0] == 0) {//判斷尾墩為空 放入
				for (int i = 0; i < 3; i++) {//放三條到後墩第123張
					back[i] = Xthree[i + 3];
					Xthree[i + 3] = 0;
				}//後面兩個空的位置先空著，下面的單張判斷在放入單張

			}
			else if (mid[0] == 0) {//後面有東西，判斷中墩為空 放入

				for (int i = 0; i < 3; i++) {//放三條到中墩第123張
					mid[i] = Xthree[i + 3];
					Xthree[i + 3] = 0;
				}

			}

		}
		//若1.兩份三條 塞第一份三條 或 2.一份三條
		if (Xthree[0] > 0)
		{
			if (back[0] == 0) {//判斷尾墩為空 放入
				for (int i = 0; i < 3; i++) {//放三條到後墩第123張
					back[i] = Xthree[i];
					Xthree[i] = 0;
				}

			}
			else if (mid[0] == 0) {//判斷中墩為空 放入

				for (int i = 0; i < 3; i++) {//放三條到中墩第123張
					mid[i] = Xthree[i];
					Xthree[i] = 0;
				}

			}

		}
		//對子
		//將對子方向顛倒 改為大到小排列
		int b[13] = { 0 };
		for (int i = 0; i < 13; i++) {
			b[i] = Xpair[12 - i];//複製顛倒陣列
		}

		for (int i = 0; i < 13; i++) {
			Xpair[i] = b[i];//將顛倒陣列貼上
		}

		//顛倒後前方數值為0 此步驟將對子往前推回最前方  

		int k = 0;//
		f = 0;//記錄挪幾次
		for (int i = 0; i < 13; i++) {
			if (Xpair[i] > 0) {
				k++;
			}
			if (Xpair[i] == 0 && k == 0)
			{
				f++;
			}
		}
		//////////////////////////////////////////
			//向前挪f次
			//若k大於0 執行動的動作
		if (k > 0) {
			for (int i = 0; i < f; i++) {
				for (int j = 0; j < 12; j++) {
					Xpair[j] = Xpair[j + 1];
				}
				Xpair[12] = 0;//最後一個位置放0
			}
		}




		//用t來記錄 目前有幾組對子
		int t = 0;
		for (int i = 0; i < 10; i = i + 2) {
			if (Xpair[i] > 0) {
				t++;
			}
		}
		p = 0;//重製使用過的對子拿到第幾張
		if (t == 5)//五組對子的情況
		{
			if (back[0] == 0)//尾墩為空，代表手上沒有鐵支、三條、葫蘆
			{
				for (int i = 0; i < 4; i++) {//後墩塞兩副對子
					back[i] = Xpair[p];
					p++;
				}
				for (int i = 0; i < 4; i++) {//中墩塞兩副對子
					mid[i] = Xpair[p];
					p++;
				}
				for (int i = 0; i < 2; i++) {//前墩塞一副對子
					front[i] = Xpair[p];
					p++;
				}
			}

		}
		if (t == 4)//四組對子的情況
		{
			if (back[0] == 0)//尾墩為空
			{
				for (int i = 0; i < 4; i++) {//後墩塞兩副對子
					back[i] = Xpair[p];
					p++;
				}
				for (int i = 0; i < 2; i++) {//中墩塞一副對子
					mid[i] = Xpair[p];
					p++;
				}
				for (int i = 0; i < 2; i++) {//前墩塞一副對子
					front[i] = Xpair[p];
					p++;
				}
			}
			//若4個對子 後墩又滿，可能為一副鐵支或葫蘆
			//4組對子因 前墩中墩為3張5張 要將一組對子拆開，目前還未完成這部芬的程式，且機率太小
		}
		if (t == 3)//三組對子的情況
		{
			if (back[0] == 0)//尾墩為空
			{
				for (int i = 0; i < 4; i++) {//後墩塞兩副對子
					back[i] = Xpair[p];
					p++;
				}
				for (int i = 0; i < 2; i++) {//中墩塞一副對子
					mid[i] = Xpair[p];
					p++;
				}//前墩烏龍

			}
			else if (mid[0] == 0) {//判斷中墩為空 放入
				for (int i = 0; i < 4; i++) {//中墩塞兩副對子
					mid[i] = Xpair[p];
					p++;
				}
				for (int i = 0; i < 2; i++) {//前墩塞一副對子
					front[i] = Xpair[p];
					p++;
				}
			}
		}
		if (t == 2)//二組對子的情況
		{
			if (back[0] == 0)//尾墩為空
			{
				for (int i = 0; i < 4; i++) {//後墩塞兩副對子
					back[i] = Xpair[p];
					p++;
				}
			}
			else if (mid[0] == 0) {//判斷中墩為空 放入
				for (int i = 0; i < 4; i++) {//中墩塞兩副對子，前墩烏龍
					mid[i] = Xpair[p];
					p++;
				}
			}
		}
		if (t == 1)//一組對子的情況
		{
			if (back[0] == 0)//尾墩為空
			{
				for (int i = 0; i < 2; i++) {//烏龍、烏龍，後墩塞一副對子
					back[i] = Xpair[p];
					p++;
				}
			}
			else if (mid[0] == 0) {//判斷中墩為空 放入
				for (int i = 0; i < 2; i++) {//中墩塞一副對子
					mid[i] = Xpair[p];
					p++;
				}
			}
			else {//中墩後墩皆滿 放入前墩
				for (int i = 0; i < 2; i++) {//中墩塞一副對子
					front[i] = Xpair[p];
					p++;
				}
			}
		}
		//單張
		//塞前墩
		for (int i = 0; i < 3; i++)
		{
			if (front[i] == 0) {//判斷前墩內有無空位
				front[i] = Xalong[s];
				s++;
			}
		}
		//塞中墩
		for (int i = 0; i < 5; i++)
		{
			if (mid[i] == 0) { //判斷中墩內有無空位
				mid[i] = Xalong[s];
				s++;
			}
		}
		//塞後墩
		for (int i = 0; i < 5; i++)
		{
			if (back[i] == 0) {
				back[i] = Xalong[s];
				s++;
			}
		}


		//合併
		for (int i = 0; i < 3; i++)//0-2
		{
			finaly[i] = front[i];
		}
		for (int i = 3; i < 8; i++)//3-7
		{
			finaly[i] = mid[i - 3];//final的第3位置	要放中墩的第0位置
		}
		for (int i = 8; i < 13; i++)//8-12位置
		{
			finaly[i] = back[i - 8];
		}
		for (i = 0; i < 13; i++) {

			yourcardstyle[i] = finaly[i]; //回傳

		}

		return true;
	}

}
