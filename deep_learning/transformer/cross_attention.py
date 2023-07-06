import torch
import torch.nn as nn
import torch.nn.functional as F

class CrossAttention(nn.Module):
    def __init__(self, in_dim, out_dim,in_q_dim,hid_q_dim):
        super(CrossAttention, self).__init__()
        self.in_dim = in_dim
        self.out_dim = out_dim
        self.in_q_dim = in_q_dim #新增
        self.hid_q_dim = hid_q_dim #新增
        # 定义查询、键、值三个线性变换
        self.query = nn.Linear(in_q_dim, hid_q_dim, bias=False) #变化
        self.key = nn.Linear(in_dim, out_dim, bias=False)
        self.value = nn.Linear(in_dim, out_dim, bias=False)
        
    def forward(self, x, y):
        # 对输入进行维度变换，为了方便后面计算注意力分数
        batch_size = x.shape[0]   # batch size
        num_queries = x.shape[1]  # 查询矩阵中的元素个数
        num_keys = y.shape[1]     # 键值矩阵中的元素个数
        x = self.query(x)  # 查询矩阵 
        y = self.key(y)    # 键值矩阵
        # 计算注意力分数
        attn_scores = torch.matmul(x, y.transpose(-2, -1)) / (self.out_dim ** 0.5)  # 计算注意力分数，注意力分数矩阵的大小为 batch_size x num_queries x num_keys x num_keys
        attn_weights = F.softmax(attn_scores, dim=-1)  # 对注意力分数进行 softmax 归一化
        # 计算加权和
        V = self.value(y)  # 通过值变换得到值矩阵 V
        output = torch.bmm(attn_weights, V)  # 计算加权和，output 的大小为 batch_size x num_queries x num_keys x out_dim
       
        return output

# 定义输入矩阵 x 和 y，其大小分别为 1024, 512 和 1024, 1024
x = torch.randn(1, 1024, 512)
y = torch.randn(1, 1024, 1024)

# 创建 CrossAttention 模型，并对输入进行前向传播
cross_attn = CrossAttention(in_dim=1024, out_dim=1024,in_q_dim=512,hid_q_dim=1024)
output = cross_attn(x=x, y=y)

# 输出新的矩阵大小
print(output.shape) # (1, 1024,1024,1024)
print(output)
