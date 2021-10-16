import cv2 as cv

img = cv.imread('Laberinto.png')

cv.imshow('Imagen de prueba',img)
cv.waitKey(0)
cv.destroyAllWindows()
