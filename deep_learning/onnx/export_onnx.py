import numpy as np
import onnx
import onnx.checker
import torch
import torch.nn as nn

inputs = {
    'i3': np.random.rand(1, 4, 30, 30).astype(np.float32),
}
class Model(torch.nn.Module):
    # def __init__(self):
    #     # super(Net, self).__init__()
    #     self.relu = nn.ReLU()
    #     self.conv1 = nn.Conv2d(in_channels=1, out_channels=5, kernel_size=3, padding=1)
    #     self.conv2 = nn.Conv2d(in_channels=5, out_channels=10, kernel_size=3, padding=1)
    #     self.pool = nn.MaxPool2d(2, 2)
    #     self.fc = nn.Linear(7 * 7 * 10, 10)
    #     self.dropout = nn.Dropout(p=0.2)

    @torch.no_grad()
    def forward(self, x):
        x = x.argmin(dim=0)
        return x


model = Model()
model.eval()
torch_inp = {k: torch.from_numpy(v) for k, v in inputs.items()}
torch.onnx.export(
    model, tuple(torch_inp.values()),
    "output.onnx", input_names=list(inputs.keys()),
    verbose=False, opset_version=13)

onnx_model = onnx.load("output.onnx")
onnx.checker.check_model(onnx_model, full_check=True)