import pycuda
import pycuda.driver as cuda
from pycuda.driver import Context
import threading

# arch = "sm_%d%d" % Context.get_device().compute_capability()
class GPUThread(threading.Thread):
    def __init__(self, device_id):
        threading.Thread.__init__(self)
        self.device_id = device_id

    def run(self):
        self.dev = cuda.Device(self.device_id)
        self.ctx = self.dev.make_context()
        arch = "sm_%d%d" % self.dev.compute_capability()
        print(arch)
        self.ctx.pop()

        del self.ctx


cuda.init()
device_num = cuda.Device.count()

gpu_thread_list = []
for i in range(device_num):
    gpu_thread = GPUThread(i)
    gpu_thread.start()
    gpu_thread_list.append(gpu_thread)
