#include <iostream>
#include <queue>
#include <vector>
#pragma warning (disable : 4996)

using namespace std;

#define INF 1000000000
#define SIZE 10

vector <pair<int, int>> graph[SIZE]; // graph[n]번째 노드가 <n번째 노드와연결,cost는 n>
int d[SIZE]; // vertex의 최소비용
int pre[SIZE]; // 어디서왔는지 node저장
int i, j;
int src = 0; // 어디서부터출발할것인지 (전부0번째 node로 설정)

void initialization() {

	for (i = 0; i < sizeof(d) / sizeof(int); i++) {
		d[i] = INF;
		pre[i] = NULL;
	}
	d[src] = 0; // initialiazation 과정, d[]는 모두 무한대로, pre는 모두 null값으로 설정
}

void dijkstra(int start) {

	initialization(); 
	d[SIZE] = 0;
	pre[SIZE] = 0;

	// priority queue로 구성
	priority_queue <pair<int, int>> pq; 
	pq.push(make_pair(start, 0));
	
	while (!pq.empty()) {

		// extract - min(Q)
		int current = pq.top().first; // 현재 node
		int dist = -pq.top().second; // 현재 node의 비용
		pq.pop();
		
		if (d[current] < dist) continue; // 최단거리가 아닌 경우 스킵

		for (i = 0; i < graph[current].size(); i++) {

			// 선택된 노드의 인접 노드에 대해 relax
			int next = graph[current][i].first; // next node
			int nextdist = dist + graph[current][i].second; // next node의 비용

			// 기존의 최소 비용보다 더 적으면 교체
			if (nextdist < d[next]) {
				d[next] = nextdist;
				pq.push(make_pair(next, -nextdist)); //min priority queue 구성 위해 비용을 -로저장, 꺼낼때 -붙여서 꺼냄
				pre[next] = current; //pre에 어디서왔는지 저장
			}
		}
	}
	cout << "dijkstra로 구현함\n\n";
}

bool bellmanford(int edge[][SIZE]) {

	initialization();
	d[SIZE] = 0;
	pre[SIZE] = 0;

	// relax과정
	for (int count = 0; count < SIZE - 1; count++) {
		for (i = 0; i < sizeof(d) / sizeof(int); i++) {
			for (j = 0; j < sizeof(d) / sizeof(int); j++) {
				if (edge[i][j] != INF && d[j] > d[i] + edge[i][j]) { // 더 작은 비용으로 교체
					d[j] = d[i] + edge[i][j];
					pre[j] = i;
				}
			}
		}
	}

	// negative cycle 찾는 과정
	for (i = 0; i < sizeof(d) / sizeof(int); i++) {
		for (j = 0; j < sizeof(d) / sizeof(int); j++) {
			if (d[j] > d[i] + edge[i][j]) {
				cout << "negative cycle이 발견되어서 종료합니다.\n\n";
				return false;
			}
		}
	}

	cout << "bellmanford로 구현함\n\n";
	return true;
}


void tracking_dist(int k) { 

	// 각 node에 대해서 어디서 온 최소비용인지 재귀함수로 구현
	if (pre[k] != src) {
		tracking_dist(pre[k]);
		cout << pre[k] << "->";
	}
	else {
		cout <<  src << "->";
	}
	return;
}


void printSSSP() {// print 함수

	for (i = 1; i < SIZE; i++) {
		if (d[i] != INF) {
			cout << src << "부터 " << i << "까지의 최단 경로 : ";
			tracking_dist(i);

			cout << i << " cost : " << d[i] << "\n\n\n";
		}
	}
}

bool check(int edge[][SIZE]) {

	bool checkpoint = 1; // negative edge있는지 확인

	for (i = 0; i < SIZE; i++) {
		for (j = 0; j < SIZE; j++) {
			if (edge[i][j] < 0) {
				checkpoint = 0; // 있으면 checkpoint 0으로 설정
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
	}; // 방향 , NEGATIVE x

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
	}; //방향, NEGATIVE O CYCLE O

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
	}; //방향, NEGATIVE O CYCLE X

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
	}; //무방향, NEGATIVE X
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
	}; //무방향, NEGATIVE O, CYCLE O

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
