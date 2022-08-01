# Paddle 算子改进建议
## 背景
由于部分 Paddle 算子具备独有性，在硬件适配过程中，很难或者不方便直接映射到硬件厂商算子上，只能通过一些 tricks 或者多个算子组合方式进行映射，不具备鲁棒性和高效性，针对此类 Paddle 算子，需要从训练框架上给予一些清扫或改进。

## 张量
### 量化信息
#### 问题
- Paddle IR 对于量化信息目前存放在 op 中，例如某个 op 有输入、输出量化信息，与其相邻的op 也有，存在量化信息冗余，而且在 Pass 优化处理时更新此类信息不方便，原则上量化信息应当是 tensor 的属性，例如：TFLite或 NNAPI 中 tensor 基本数据新增了量化类型：https://developer.android.com/ndk/reference/group/neural-networks#group___neural_networks_1ggaf06d1affd33f3bc698d0c04eceb23298a07984961d5c7c12f0f8c811bedd85dc3
![图片](http://agroup.baidu-int.com/file/stream/bj/bj-26f5f622bf9fc6f94f6d0a2493c594f4c44f9b57)

#### 建议
- tensor 基本类型新增量化数据类型， scale 和 zero_point 等量化信息以属性存放在 tensor 中 。
### LoD信息
####问题
- Paddle 有一些算子，例如nms2、roi_align等算子会存放 lod 信息来进行后续的计算，而这种信息无法直接映射到NNAdapter内部。
####建议
- 用具体的输入输出代替原有lod，如nms2->nms3，将输入和输出lod信息对应为RoisNum和NmsRoisNum。
-![图片](http://agroup.baidu-int.com/file/stream/bj/bj-bc0020a4f12eaef968cc1cd3704edbb4c25da7dc)
### Tensor Array信息
#### 问题
- Paddle 有一些算子在实现时为了高效方便支持 tensor array 输入， 但是该算子本身并不是类似于concat算子一样需要对多个tensor进行操作。例如 expand_v2算子的 expand_shapes_tensor， 这种信息如果在模型里面存在，一般硬件不会支持，NNAdapter算子映射需要算子组合来完成，或者干脆不支持。
#### 建议
- 非必要取消这些tensor array的支持

## 算子
### mul
#### 问题
- 其它框架 mul 一般指 eltwise_mul，容易引起理解偏差 https://github.com/onnx/onnx/blob/main/docs/Operators.md#Mul。
-  一般与eltwise_add 一起出现用于实现 fc，双激活可以直接使用 mat_mul，因此，mul 没有单独存在的必要；
-  x_num_col_dims 和 y_num_col_dims 属性用处不大，其它框架没有这两个属性 https://developer.android.com/ndk/reference/group/neural-networks#group___neural_networks_1ggaabbe492c60331b13038e39d4207940e0aaada7a3dbaf4676aba560c933ff610c5，对于fc，可以通过weights的2-d 维度信息推导出这两个值，如果存在会使得转换逻辑非常复杂，特别是动态 shape 场景，由于硬件厂商只支持 fc或双激活matmul，需要在转换时根据 x_num_col_dims 和 y_num_col_dims 前后插入 reshape 且可能需要 shape+slice+concat 获取相应维度信息。
#### 建议
- 删除，直接使用 mat_mul_v2 实现并清理 mat_mul 算子。

### fill_constant_batch_size_like
#### 问题
- 其它框架没有该算子（ONNX  只有 ConstantOfShape、TF keras 有 full_like、 PyTorch 有 full_like、Paddle API有full_like），导致我们需要通过 shape+slice+concat 算子组合方式提取batch_size维度 https://github.com/PaddlePaddle/Paddle-Lite/blob/a7fc3455cf83844d7790b2a72e925a9d51e77292/lite/kernels/nnadapter/converter/fill_constant_batch_size_like.cc#L149，然后在用 fill 算子，但由于 shape 算子涉及到动态 shape，在大部分 SoC 硬件中无法获得支持，因此，只能约束在静态 shape 场景。
#### 建议
- 删除，使用 full_like 替代。

### argmax
#### 问题
- 目前argmax默认输出类型是int64（可选int32），实际很多边缘移动端仅支持int32类型的argmax计算，大部分场景下，int32也足够了。
![图片](http://agroup.baidu-int.com/file/stream/bj/bj-5fe932542d30f946d4f2730479ae43dca6066023)
#### 建议
- 默认输出类型为int32。

### elementwise ops
#### 问题
- broadcast 规则依赖 axis 属性，其它框架均没有类似属性 https://github.com/onnx/onnx/blob/main/docs/Operators.md#Add 
- 在转换成硬件厂商layer（厂商的broadcast规则是参考 numpy 的右对齐）时，需要插入 unsequeeze。
#### 建议
- 去掉 axis 属性，参考 numpy 的boradcast 规则（右对齐）。

### dropout
#### 问题
- 训练算子，不应该在预测模型中存在，可以被mul+add或scale取代。
#### 建议
- 导出预测模型时，删除或使用mul+add、scale 代替。

### expand、reshape
#### 问题
- 支持的 shape 类型太多，Shape attr、Shape tensor、expand_shapes_tensor（tensor 数组），导致转换逻辑异常复杂，其它框架都只有 Shape tensor。
#### 建议
- 只保留 Shape  tensor。

### fill_constant
#### 问题
- 支持的 value 类型太多，有 Value Tensor、str_value、 value attr。
#### 建议
- 是否能只保留 Value Tensor，Tensor类型中增加 string？

### flatten、flatten_contiguous_range
#### 问题
- flatten 实现太多，flatten_contiguous_range 的功能包含 flatten，取名非常奇怪。
#### 建议
- 只保留一个 flatten。

### gather
#### 问题
- 支持 Axis tensor、axis 属性，是否只保留一个。
#### 建议
- 只保留 Axis tensor。

### transpose2、reshape2 等
#### 问题
- 多了训练需要的 XShape tensor
#### 建议
- 导出预测模型时删掉 XShape 的输出

### Interpolate ops 
#### 问题
- Output shape 存在多种输入情况：SizeTensor、OutSize、out_h&out_w attrs，判断逻辑复杂
- Scale 存在多种输入情况：Scale Tensor、scale attr 
#### 建议
- 只保留 Size 和 Scale tensor。

### prelu
#### 问题
- 是否有必要 mode 属性，是否直接从slope 的维度判断是否是"all", "channel"，其它框架没有这个属性 https://github.com/onnx/onnx/blob/main/docs/Operators.md#prelu
#### 建议
- 删除 mode 属性

### reduce
#### 问题
- reduce_all 属性是否有必要存在，axes 属性可以覆盖 reduce_all 的情况
#### 建议
- 删除 reduce_all 属性

### scale
#### 问题
- 可以使用eltwise_mul 和 eltwise_add 组合实现，是否有必要存在？
#### 建议
- 删除 scale 算子

### slice 和 strided_slice
#### 问题
-  stride_slice 包含 slice 功能
- decrease_axis 属性是否多余，是否可通过增加 squeeze 实现？
#### 建议
- slice 和 strided_slice 合并成 slice
- 去掉 descrease_axis 属性，使用 squeeze 算子实现。

### split
#### 问题
- 代表切分分量类型有很多：AxisTensor、axis attr、SectionsTensorList 和 sections attr。
#### 建议
- 只保留 axis tensor 和 sections tensor。

### tile
#### 问题
- 代表 repeats 有 RepeatTimes tensor、repeat_times_tensor和 repeat_times attr
#### 建议
- 只保留 repeat_times tensor

## 其它问题
### 动态图转静态图保存预测模型量 shape + slice +concat op
#### 问题
- 支持可变输入的模型中，存在大量 shape + slice + concat op 用户获取中间 tensor 的输出 shape，提取 batch_size 或 width/height，在大量 soc 硬件中不支持动态输入，也不支持 shape 算子。
#### 建议
- 是否能尽可能减少将此类算子的组合

### 模型保存
#### 问题
- 目前save后的静态模型，最后都会带一个scale OP（scale==1；bias==0），当前lite的做法是通过pass删除该节点，但是部分拓扑中无法完美覆盖，导致任有多余的scale OP流入下层，此时如果是量化场景，还需要设计反量化的过程（该冗余scale OP是float类型）
#### 建议
- 保存模型时不在输入前插入scale算子

### 动态图静态图量化
#### 问题
- 目前动态图和静态图在经过量化训练后，导出的静态推理模型表述方法不统一。
- 静态图
![图片](http://agroup.baidu-int.com/file/stream/bj/bj-31912e47c5c61de4d7127c1ad52513f68694c89e)
- 动态图
![图片](http://agroup.baidu-int.com/file/stream/bj/bj-81612c8553e18876ff5d655159082a4cc18f6f57)
#### 建议
- 统一静态图、动态图、PTQ、QAT、量化后的模型格式。

### 算子属性 None
#### 问题
-部分算子的axis轴为None时需要额外的属性来支撑这一None的行为，如cumsum算子，axis == None表示平铺展开tensor再进行累加计算，但是这个行为对于导出的的模型需要额外的flatten属性支持，标准的cumsum算子则没有该flatten属性。
####建议
- 删除None，默认值采用对应类型的数值，如-1。
