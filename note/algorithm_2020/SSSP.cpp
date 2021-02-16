#include <iostream>
#include <queue>
#include <vector>
#pragma warning (disable : 4996)

using namespace std;

#define INF 1000000000
#define SIZE 10

vector <pair<int, int>> graph[SIZE]; // graph[n]��° ��尡 <n��° ���Ϳ���,cost�� n>
int d[SIZE]; // vertex�� �ּҺ��
int pre[SIZE]; // ��𼭿Դ��� node����
int i, j;
int src = 0; // ��𼭺�������Ұ����� (����0��° node�� ����)

void initialization() {

	for (i = 0; i < sizeof(d) / sizeof(int); i++) {
		d[i] = INF;
		pre[i] = NULL;
	}
	d[src] = 0; // initialiazation ����, d[]�� ��� ���Ѵ��, pre�� ��� null������ ����
}

void dijkstra(int start) {

	initialization(); 
	d[SIZE] = 0;
	pre[SIZE] = 0;

	// priority queue�� ����
	priority_queue <pair<int, int>> pq; 
	pq.push(make_pair(start, 0));
	
	while (!pq.empty()) {

		// extract - min(Q)
		int current = pq.top().first; // ���� node
		int dist = -pq.top().second; // ���� node�� ���
		pq.pop();
		
		if (d[current] < dist) continue; // �ִܰŸ��� �ƴ� ��� ��ŵ

		for (i = 0; i < graph[current].size(); i++) {

			// ���õ� ����� ���� ��忡 ���� relax
			int next = graph[current][i].first; // next node
			int nextdist = dist + graph[current][i].second; // next node�� ���

			// ������ �ּ� ��뺸�� �� ������ ��ü
			if (nextdist < d[next]) {
				d[next] = nextdist;
				pq.push(make_pair(next, -nextdist)); //min priority queue ���� ���� ����� -������, ������ -�ٿ��� ����
				pre[next] = current; //pre�� ��𼭿Դ��� ����
			}
		}
	}
	cout << "dijkstra�� ������\n\n";
}

bool bellmanford(int edge[][SIZE]) {

	initialization();
	d[SIZE] = 0;
	pre[SIZE] = 0;

	// relax����
	for (int count = 0; count < SIZE - 1; count++) {
		for (i = 0; i < sizeof(d) / sizeof(int); i++) {
			for (j = 0; j < sizeof(d) / sizeof(int); j++) {
				if (edge[i][j] != INF && d[j] > d[i] + edge[i][j]) { // �� ���� ������� ��ü
					d[j] = d[i] + edge[i][j];
					pre[j] = i;
				}
			}
		}
	}

	// negative cycle ã�� ����
	for (i = 0; i < sizeof(d) / sizeof(int); i++) {
		for (j = 0; j < sizeof(d) / sizeof(int); j++) {
			if (d[j] > d[i] + edge[i][j]) {
				cout << "negative cycle�� �߰ߵǾ �����մϴ�.\n\n";
				return false;
			}
		}
	}

	cout << "bellmanford�� ������\n\n";
	return true;
}


void tracking_dist(int k) { 

	// �� node�� ���ؼ� ��� �� �ּҺ������ ����Լ��� ����
	if (pre[k] != src) {
		tracking_dist(pre[k]);
		cout << pre[k] << "->";
	}
	else {
		cout <<  src << "->";
	}
	return;
}


void printSSSP() {// print �Լ�

	for (i = 1; i < SIZE; i++) {
		if (d[i] != INF) {
			cout << src << "���� " << i << "������ �ִ� ��� : ";
			tracking_dist(i);

			cout << i << " cost : " << d[i] << "\n\n\n";
		}
	}
}

bool check(int edge[][SIZE]) {

	bool checkpoint = 1; // negative edge�ִ��� Ȯ��

	for (i = 0; i < SIZE; i++) {
		for (j = 0; j < SIZE; j++) {
			if (edge[i][j] < 0) {
				checkpoint = 0; // ������ checkpoint 0���� ����
			}
		}
	}
	return checkpoint;
}

int main() {

	int edge1[SIZE][SIZE] = {
		{0,INF,6,INF,INF,INF,INF,INF,INF,INF},
		{3,0,1,INF,INF,INF,INF,INF,INF,INF},
		{INF,INF,0,2,INF,INF,INF,INF,INF,INF},
		{INF,1,INF,0,INF,INF,INF,INF,INF,INF},
		{INF,4,INF,3,0,5,2,INF,INF,INF},
		{INF,INF,INF,INF,INF,0,1,4,INF,INF},
		{INF,INF,INF,INF,INF,INF,0,INF,INF,INF},
		{INF,INF,INF,INF,INF,INF,2,0,INF,INF},
		{INF,INF,INF,INF,INF,INF,1,3,0,INF},
		{INF,INF,INF,INF,INF,INF,INF,4,INF,0},
	}; // ���� , NEGATIVE x

	cout << "------------case1---------------\n";
	if (check(edge1)) {
		for (i = 0; i < SIZE; i++) {
			for (j = 0; j < SIZE; j++) {
				if (edge1[i][j] != INF && edge1[i][j] != 0) graph[i].push_back(make_pair(j, edge1[i][j]));
			}
		}
		dijkstra(src);	printSSSP();
	}
	else {
		if (bellmanford(edge1)) printSSSP();
	}

	int edge2[SIZE][SIZE] = {
		{0,INF,-2,INF,INF,INF,INF,INF,INF,INF},
		{3,0,1,INF,INF,INF,INF,INF,INF,INF},
		{INF,INF,0,2,INF,INF,INF,INF,INF,INF},
		{INF,-7,INF,0,INF,INF,INF,INF,INF,INF},
		{INF,4,INF,3,0,5,2,INF,INF,INF},
		{INF,INF,INF,INF,INF,0,1,-3,INF,INF},
		{INF,INF,INF,INF,INF,INF,0,INF,INF,INF},
		{INF,INF,INF,INF,INF,INF,2,0,INF,INF},
		{INF,INF,INF,INF,INF,INF,1,3,0,INF},
		{INF,INF,INF,INF,INF,INF,INF,4,INF,0},
	}; //����, NEGATIVE O CYCLE O

	cout << "------------case2---------------\n";
	if (check(edge2)) {
		for (i = 0; i < SIZE; i++) {
			for (j = 0; j < SIZE; j++) {
				if (edge2[i][j] != INF && edge2[i][j] != 0) graph[i].push_back(make_pair(j, edge2[i][j]));
			}
		}
		dijkstra(src);	printSSSP();
	}
	else {
		if (bellmanford(edge2)) printSSSP();
	}

	int edge3[SIZE][SIZE] = {
		{0,5,2,INF,INF,INF,INF,INF,INF,INF},
		{INF,0,INF,2,INF,INF,INF,INF,INF,INF},
		{INF,INF,0,INF,4,INF,INF,INF,INF,INF},
		{INF,INF,INF,0,INF,3,INF,INF,INF,INF},
		{INF,INF,INF,INF,0,INF,8,INF,INF,INF},
		{INF,INF,INF,INF,INF,0,INF,-5,INF,INF},
		{INF,INF,INF,INF,INF,INF,0,7,INF,INF},
		{INF,INF,INF,INF,INF,INF,INF,0,-2,INF},
		{INF,INF,INF,INF,INF,INF,INF,INF,0,4},
		{INF,INF,INF,INF,INF,INF,INF,INF,INF,0},
	}; //����, NEGATIVE O CYCLE X

	cout << "------------case3---------------\n";
	if (check(edge3)) {
		for (i = 0; i < SIZE; i++) {
			for (j = 0; j < SIZE; j++) {
				if (edge3[i][j] != INF && edge3[i][j] != 0) graph[i].push_back(make_pair(j, edge3[i][j]));
			}
		}
		dijkstra(src);	printSSSP();
	}
	else {
		if (bellmanford(edge3)) printSSSP();
	}

	int edge4[SIZE][SIZE] = {
		{0,2,INF,4,INF,3,INF,INF,INF,INF},
		{2,0,5,1,INF,INF,INF,INF,INF,INF},
		{INF,5,0,INF,2,INF,INF,INF,INF,INF},
		{4,1,INF,0,INF,INF,INF,INF,INF,INF},
		{INF,INF,2,INF,0,3,INF,INF,INF,INF},
		{3,INF,INF,INF,3,0,9,6,INF,INF},
		{INF,INF,INF,INF,INF,9,0,INF,8,INF},
		{INF,INF,INF,INF,INF,6,INF,0,INF,INF},
		{INF,INF,INF,INF,INF,INF,8,INF,0,7},
		{INF,INF,INF,INF,INF,INF,INF,INF,7,0},
	}; //������, NEGATIVE X
	cout << "------------case4---------------\n";
	if (check(edge4)) {
		for (i = 0; i < SIZE; i++) {
			for (j = 0; j < SIZE; j++) {
				if (edge4[i][j] != INF && edge4[i][j] != 0) graph[i].push_back(make_pair(j, edge4[i][j]));
			}
		}
		dijkstra(src);	printSSSP();
	}
	else {
		if (bellmanford(edge4)) printSSSP();
	}

	int edge5[SIZE][SIZE] = {
		{0,3,INF,INF,INF,INF,2,INF,INF,INF},
		{3,0,5,INF,INF,INF,INF,INF,INF,INF},
		{INF,5,0,-3,INF,INF,INF,INF,INF,INF},
		{INF,INF,-3,0,2,INF,INF,INF,INF,INF},
		{INF,INF,INF,2,0,6,INF,INF,INF,INF},
		{INF,INF,INF,INF,6,0,INF,INF,INF,INF},
		{2,INF,INF,INF,INF,INF,0,-7,INF,INF},
		{INF,INF,INF,INF,INF,INF,-7,0,4,INF},
		{INF,INF,INF,INF,INF,INF,INF,4,0,-3},
		{INF,INF,INF,INF,INF,INF,INF,INF,-3,0},
	}; //������, NEGATIVE O, CYCLE O

	cout << "------------case5---------------\n";
	if (check(edge5)) {
		for (i = 0; i < SIZE; i++) {
			for (j = 0; j < SIZE; j++) {
				if (edge5[i][j] != INF && edge5[i][j] != 0) graph[i].push_back(make_pair(j, edge5[i][j]));
			}
		}
		dijkstra(src);	printSSSP();
	}
	else {
		if (bellmanford(edge5)) printSSSP();
	}

	return 0;
}
