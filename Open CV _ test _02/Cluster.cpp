#pragma once
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2\opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <windows.h>
#include <vector>
using namespace std;

class Cluster {
	vector<POINT> scores;

public:
	int curX, curY;
	int lastX, lastY;

	size_t Size() { return scores.size(); }
	inline void Add(POINT p) { scores.push_back(p); }

	void Cluster::InitialCenter(int k, Cluster * clusarr, vector<POINT>& vpt) {

		int size = vpt.size();
		int step = size / k;
		int steper = 0;

		for (int i = 0; i < k; i++, steper += step) {
			clusarr[i].curX = vpt[steper].x;
			clusarr[i].curY = vpt[steper].y;
		}
	}

	void SetCenter() {
		int sumX = 0, sumY = 0;
		int i = 0;
		int size = Size();
		for (; i < size; sumX += scores[i].x, i++);
		i = 0;
		for (; i < size; sumY += scores[i].y, i++);
		lastX = curX;
		lastY = curY;
		curX = sumX / size;
		curY = sumY / size;
	}

	void Cluster::Clear() {
		scores.clear();
	}

	Cluster * Cluster::Bind(int k, Cluster * clusarr, vector<POINT>& vpt) {
		for (int j = 0; j < k; j++)
			clusarr[j].Clear();
		int size = vpt.size();
		for (int i = 0; i < size; i++) {
			int min = sqrt(
				pow((float)clusarr[0].curX - vpt[i].x, 2) + pow((float)clusarr[0].curY - vpt[i].y, 2)
			);
			Cluster * cl = &clusarr[0];
			for (int j = 1; j < k; j++) {
				int tmp = sqrt(
					pow((float)clusarr[j].curX - vpt[i].x, 2) + pow((float)clusarr[j].curY - vpt[i].y, 2)
				);
				if (min > tmp) { min = tmp; cl = &clusarr[j]; }
			}
			cl->Add(vpt[i]);
		}
		return clusarr;
	}

	void Cluster::Start(int k, Cluster * clusarr, vector<POINT>& vpt) {
		Cluster::InitialCenter(k, clusarr, vpt);
		for (;;) {//Запускаем основной цикл
			int chk = 0;
			Cluster::Bind(k, clusarr, vpt);//Связываем точки с кластерами
			for (int j = 0; j < k; j++)//Высчитываем новые координаты центроидов 
				clusarr[j].SetCenter();
			for (int p = 0; p < k; p++)//Проверяем не совпадают ли они с предыдущими цент-ми
				if (clusarr[p].curX == clusarr[p].lastX && clusarr[p].curY == clusarr[p].lastY)
					chk++;
			if (chk == k) return;//Если да выходим с цикла
		}
	}

};