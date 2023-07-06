# coding:utf-8
# @Email: wangguisen@donews.com
# @Time: 2023/3/22 22:58
# @File: att_test.py
'''
Self Attention
Multi-Head Attention
Cross Attention
'''
import torch
import torch.nn as nn
import torch.nn.functional as F
import math
from einops import rearrange, repeat
from torch.nn import MultiheadAttention

class SelfAttention(nn.Module):
    def __init__(self, emb_dim):
        super(SelfAttention, self).__init__()
        self.emb_dim = emb_dim

        self.Wq = nn.Linear(emb_dim, emb_dim, bias=False)
        self.Wk = nn.Linear(emb_dim, emb_dim, bias=False)
        self.Wv = nn.Linear(emb_dim, emb_dim, bias=False)

        self.fc = nn.Linear(emb_dim, emb_dim)

    def forward(self, x, pad_mask=None):
        # [batch_szie, seq_len, emb_dim] = [3, 5, 512]

        Q = self.Wq(x)
        K = self.Wk(x)
        V = self.Wv(x)

        att_weights = torch.bmm(Q, K.transpose(1, 2))   # [batch_szie, seq_len, seq_len] = [3, 5, 5]
        att_weights = att_weights / math.sqrt(self.emb_dim)

        if pad_mask is not None:
            att_weights = att_weights.masked_fill(pad_mask, -1e9)

        att_weights = F.softmax(att_weights, dim=-1)
        output = torch.bmm(att_weights, V)   # [batch_szie, seq_len, emb_dim] = [3, 5, 512]
        output = self.fc(output)

        return output, att_weights

class MultiHeadAttention(nn.Module):
    def __init__(self, emb_dim, num_heads, att_dropout=0.0):
        super(MultiHeadAttention, self).__init__()
        self.emb_dim = emb_dim
        self.num_heads = num_heads
        self.att_dropout = att_dropout

        assert emb_dim % num_heads == 0, "emb_dim must be divisible by num_heads"
        self.depth = emb_dim // num_heads

        self.Wq = nn.Linear(emb_dim, emb_dim, bias=False)
        self.Wk = nn.Linear(emb_dim, emb_dim, bias=False)
        self.Wv = nn.Linear(emb_dim, emb_dim, bias=False)

        self.fc = nn.Linear(emb_dim, emb_dim)

    def forward(self, x, pad_mask=None):
        # [batch_szie, seq_len, emb_dim] = [3, 5, 512]
        batch_size = x.size(0)

        # [batch_szie, seq_len, emb_dim] = [3, 5, 512]
        Q = self.Wq(x)
        K = self.Wk(x)
        V = self.Wv(x)

        # 分头 [batch_szie, num_heads, seq_len, depth] = [3, 8, 5, 512/8=64]
        Q = Q.view(batch_size, -1, self.num_heads, self.depth).transpose(1, 2)
        K = K.view(batch_size, -1, self.num_heads, self.depth).transpose(1, 2)
        V = V.view(batch_size, -1, self.num_heads, self.depth).transpose(1, 2)

        # [batch_szie, num_heads, seq_len, seq_len] = [3, 8, 5, 5]
        att_weights = torch.matmul(Q, K.transpose(-2, -1))
        att_weights = att_weights / math.sqrt(self.depth)

        if pad_mask is not None:
            # 因为是多头，所以mask矩阵维度要扩充到4维  [batch_size, seq_len, seq_len] -> [batch_size, nums_head, seq_len, seq_len]
            pad_mask = pad_mask.unsqueeze(1).repeat(1, self.num_heads, 1, 1)
            att_weights = att_weights.masked_fill(pad_mask, -1e9)

        att_weights = F.softmax(att_weights, dim=-1)

        # 自己的多头注意力效果没有torch的好，我猜是因为它的dropout给了att权重，而不是fc
        if self.att_dropout > 0.0:
            att_weights = F.dropout(att_weights, p=self.att_dropout)

        # [batch_szie, num_heads, seq_len, depth] = [3, 8, 5, 64]
        output = torch.matmul(att_weights, V)

        # 不同头的结果拼接 [batch_szie, seq_len, emb_dim] = [3, 5, 512]
        output = output.transpose(1, 2).contiguous().view(batch_size, -1, self.emb_dim)

        output = self.fc(output)

        return output, att_weights

class Cross_MultiAttention(nn.Module):
    def __init__(self, in_channels, emb_dim, num_heads, att_dropout=0.0, aropout=0.0):
        super(Cross_MultiAttention, self).__init__()
        self.emb_dim = emb_dim
        self.num_heads = num_heads
        self.scale = emb_dim ** -0.5

        assert emb_dim % num_heads == 0, "emb_dim must be divisible by num_heads"
        self.depth = emb_dim // num_heads


        self.proj_in = nn.Conv2d(in_channels, emb_dim, kernel_size=1, stride=1, padding=0)

        self.Wq = nn.Linear(emb_dim, emb_dim)
        self.Wk = nn.Linear(emb_dim, emb_dim)
        self.Wv = nn.Linear(emb_dim, emb_dim)

        self.proj_out = nn.Conv2d(emb_dim, in_channels, kernel_size=1, stride=1, padding=0)


    def forward(self, x, context, pad_mask=None):
        '''

        :param x: [batch_size, c, h, w]
        :param context: [batch_szie, seq_len, emb_dim]
        :param pad_mask: [batch_size, seq_len, seq_len]
        :return:
        '''
        b, c, h, w = x.shape

        x = self.proj_in(x)   # [batch_size, c, h, w] = [3, 512, 512, 512]
        x = rearrange(x, 'b c h w -> b (h w) c')   # [batch_size, h*w, c] = [3, 262144, 512]

        Q = self.Wq(x)  # [batch_size, h*w, emb_dim] = [3, 262144, 512]
        K = self.Wk(context)  # [batch_szie, seq_len, emb_dim] = [3, 5, 512]
        V = self.Wv(context)

        Q = Q.view(batch_size, -1, self.num_heads, self.depth).transpose(1, 2)  # [batch_size, num_heads, h*w, depth]
        K = K.view(batch_size, -1, self.num_heads, self.depth).transpose(1, 2)  # [batch_size, num_heads, seq_len, depth]
        V = V.view(batch_size, -1, self.num_heads, self.depth).transpose(1, 2)

        # [batch_size, num_heads, h*w, seq_len]
        att_weights = torch.einsum('bnid,bnjd -> bnij', Q, K)
        att_weights = att_weights * self.scale

        if pad_mask is not None:
            # 因为是多头，所以mask矩阵维度要扩充到4维  [batch_size, h*w, seq_len] -> [batch_size, nums_head, h*w, seq_len]
            pad_mask = pad_mask.unsqueeze(1).repeat(1, self.num_heads, 1, 1)
            att_weights = att_weights.masked_fill(pad_mask, -1e9)

        att_weights = F.softmax(att_weights, dim=-1)
        out = torch.einsum('bnij, bnjd -> bnid', att_weights, V)
        out = out.transpose(1, 2).contiguous().view(batch_size, -1, self.emb_dim)   # [batch_size, h*w, emb_dim]

        print(out.shape)

        out = rearrange(out, 'b (h w) c -> b c h w', h=h, w=w)   # [batch_size, c, h, w]
        out = self.proj_out(out)   # [batch_size, c, h, w]

        return out, att_weights

class CrossAttention(nn.Module):
    def __init__(self, in_channels, emb_dim, att_dropout=0.0, aropout=0.0):
        super(CrossAttention, self).__init__()
        self.emb_dim = emb_dim
        self.scale = emb_dim ** -0.5

        self.proj_in = nn.Conv2d(in_channels, emb_dim, kernel_size=1, stride=1, padding=0)

        self.Wq = nn.Linear(emb_dim, emb_dim)
        self.Wk = nn.Linear(emb_dim, emb_dim)
        self.Wv = nn.Linear(emb_dim, emb_dim)

        self.proj_out = nn.Conv2d(emb_dim, in_channels, kernel_size=1, stride=1, padding=0)

    def forward(self, x, context, pad_mask=None):
        '''

        :param x: [batch_size, c, h, w]
        :param context: [batch_szie, seq_len, emb_dim]
        :param pad_mask: [batch_size, seq_len, seq_len]
        :return:
        '''
        b, c, h, w = x.shape

        x = self.proj_in(x)   # [batch_size, c, h, w] = [3, 512, 512, 512]
        x = rearrange(x, 'b c h w -> b (h w) c')   # [batch_size, h*w, c] = [3, 262144, 512]

        Q = self.Wq(x)  # [batch_size, h*w, emb_dim] = [3, 262144, 512]
        K = self.Wk(context)  # [batch_szie, seq_len, emb_dim] = [3, 5, 512]
        V = self.Wv(context)

        # [batch_size, h*w, seq_len]
        att_weights = torch.einsum('bid,bjd -> bij', Q, K)
        att_weights = att_weights * self.scale

        if pad_mask is not None:
            # [batch_size, h*w, seq_len]
            att_weights = att_weights.masked_fill(pad_mask, -1e9)

        att_weights = F.softmax(att_weights, dim=-1)
        out = torch.einsum('bij, bjd -> bid', att_weights, V)   # [batch_size, h*w, emb_dim]

        out = rearrange(out, 'b (h w) c -> b c h w', h=h, w=w)   # [batch_size, c, h, w]
        out = self.proj_out(out)   # [batch_size, c, h, w]

        print(out.shape)

        return out, att_weights


if __name__ == '__main__':
    '''
    '''

    '''
    假设词表映射后输入 
    batch_size = 3
    seq_len = max_len = 5
    pad = 0
    emb_dim = 512
    '''
    batch_size = 3
    seq_len = 5
    emb_dim = 512
    # 本例子则词表大小为 301
    vocab_size = 301

    input_ids = torch.tensor([[100, 200, 300, 300, 0],
                 [22, 33, 44, 0, 0],
                 [66, 55, 66, 30, 0]], dtype=torch.long)

    pad_mask = input_ids.eq(0)  # 逻辑矩阵pad_mask：将填充位置标记为True，其他位置标记为False
    # pad_mask = pad_mask.unsqueeze(1).expand(batch_size, seq_len, seq_len)  # [batch_size, seq_len, seq_len] = [3, 5, 5]

    inputs = nn.Embedding(vocab_size, embedding_dim=emb_dim)(input_ids)   # [batch_szie, seq_len, emb_dim] = [3, 5, 512]

    # self_att = SelfAttention(emb_dim=emb_dim)
    # self_att(inputs, pad_mask=pad_mask)

    # multi_att = MultiHeadAttention(emb_dim=emb_dim, num_heads=8)
    # multi_att(inputs, pad_mask=pad_mask)

    # 定义图片数据  [batch_size, c, h, w]
    input_img = torch.randn((3, 3, 512, 512))
    pad_mask = pad_mask.unsqueeze(1).expand(batch_size, 512*512, seq_len)
    # cross_att = Cross_MultiAttention(in_channels=3, emb_dim=emb_dim, num_heads=8, att_dropout=0.0, aropout=0.0)
    # cross_att(x=input_img, context=inputs, pad_mask=pad_mask)
    cross_att = CrossAttention(in_channels=3, emb_dim=emb_dim, att_dropout=0.0, aropout=0.0)
    cross_att(x=input_img, context=inputs, pad_mask=pad_mask)
