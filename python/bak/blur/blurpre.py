import cv2

img = cv2.imread("../dst.jpg", 0)

gray_lab =cv2.Laplacian(img, cv2.CV_64FC1, ksize = 3)
lab = cv2.convertScaleAbs(gray_lab)

(mean, std) = cv2.meanStdDev(lab)

print(mean.var())
print(std.var())
