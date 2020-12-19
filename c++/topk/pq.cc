#include <iostream>
#include <vector>
#include <queue>

typedef std::pair<int, int> PairNode;

struct cmp {
	bool operator()(const PairNode& lsh, const PairNode& rsh) {
		//return lsh.second < rsh.second;  // less
		return lsh.second > rsh.second;  // great
	}
};

int main() {
	//std::priority_queue<PairNode, std::vector<PairNode>, std::greater<PairNode>> q;
	std::priority_queue<PairNode, std::vector<PairNode>, cmp> q;
	q.push(std::make_pair(1, 5));
	q.push(std::make_pair(4, 9));
	q.push(std::make_pair(2, 3));

	while(!q.empty()) {
		auto item = q.top(); q.pop();
		std::cout << item.first << ", " << item.second << std::endl;
	}
	return 0;
}
