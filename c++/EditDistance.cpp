/****************************************************************
    File Name    : EditDistance.cpp
    Author       : Jun Zhang
	Mail         : zhangjun9@staff.weibo.com 
	Created Time : 2018-01-04 11-44
*****************************************************************/

#include<iostream>
#include<string>
#include<vector>
using namespace std;

class Solution {
    public:
        int edit(string first, string second){
            int len1 = first.size();
            int len2 = second.size();

            vector< vector<int> > dis(len1 + 1, vector<int>(len2 + 1, 0));
            for(int i = 0; i < len1 + 1; i++)
                dis[i][0] = i;

            for(int j = 0; j < len2 + 1; j ++)
                dis[0][j] = j;

            for(int i = 1; i < len1 + 1; i ++){
                for(int j = 1; j < len2 + 1; j++){
                    int d = min(dis[i][j-1] + 1, dis[i-1][j] + 1);
                    int stride = 0;
                    if(first[i - 1] != second[j - 1])
                        stride = 1;

                    dis[i][j] = min(d, dis[i-1][j-1] + stride);

                }
            }

            int res = dis[len1][len2];
            return res;
        }
};

int main(){
    Solution s;
    string first = "tect";
    string second = "second";
    cout << s.edit(first, second) << endl;
    return 0;
}
