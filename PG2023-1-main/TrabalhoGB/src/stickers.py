import cv2
import numpy as np

class Sticker:
    @staticmethod
    def sticker1(img):
        return cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)