#include<iostream>
#include<vector>

using namespace std;

class Solution {
	public:
		int MaxContinousSum(vector<int> data){
			int res = 0, val = 0;
			for(int i = 0; i < data.size(); i ++){
				// data[i] >= 0, val += data[i]; 
				// data[i] < 0,
				//      val + data[i] < 0: val == 0, res = max(res, val); \
				//      val + data[i] >= 0: val += data[i]
				if(data[i] >= 0) {
					val += data[i];
					res = max(res, val);
				}
				else if(val + data[i] >= 0) {
						val += data[i];
				} 
				else
					val = 0;

			}
			return res;
		}

};

int main(){
	vector<int> data = {3, -1, 4, 1, -5, 7, 9, -3};
	Solution s;
	cout << s.MaxContinousSum(data) << endl;
	return 0;
}
