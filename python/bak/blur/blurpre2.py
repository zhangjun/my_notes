import cv2
import numpy
from PIL import Image

def blurpre(src):
    '''
    input: a 3 channels or 1 channel image
    output: return 0 means sharp image, return 1 means blur image
    '''

    gray = cv2.cvtColor(numpy.asarray(src),cv2.COLOR_RGB2GRAY)  

    
    #c = src.shape
    #
    #if len(c) == 2 or c[2] == 1:
    #    gray = src
    #elif c[2] == 3:
    #    gray = cv2.cvtColor(src, cv2.COLOR_RGB2GRAY)
    #else:
    #    print("input image error!")
    #    return 0

    lap = cv2.Laplacian(gray, cv2.CV_64FC1, 3)
    s = abs(lap).var()
    print(s)
    if s < 3:
        ret = 1
    else:
        ret = 0

    return ret
    


#img = cv2.imread("../dst.jpg")
img = Image.open("../dst.jpg")
status = blurpre(img)
print(status)
