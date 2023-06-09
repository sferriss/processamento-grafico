import cv2
import numpy as np


class Filter:
    @staticmethod
    def gray(img):
        return cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

    @staticmethod
    def blur(img):
        return cv2.GaussianBlur(img, (13, 13), 5, 0)

    @staticmethod
    def canny(img):
        return cv2.Canny(img, 100, 200)

    @staticmethod
    def equalize(img):
        gray_img = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
        return cv2.equalizeHist(gray_img)

    @staticmethod
    def dilate(img):
        kernel = np.ones((5, 5), np.uint8)
        return cv2.dilate(img, kernel, iterations=1)

    @staticmethod
    def erode(img):
        kernel = cv2.getStructuringElement(cv2.MORPH_RECT, (5, 5))
        return cv2.erode(img, kernel)

    @staticmethod
    def brightness(img):
        return cv2.convertScaleAbs(img, beta=60)

    @staticmethod
    def sharp(img):
        kernel = np.array([[-1, -1, -1], [-1, 9.5, -1], [-1, -1, -1]])
        return cv2.filter2D(img, -1, kernel)

    @staticmethod
    def sepia(img):
        img = np.array(img, dtype=np.float64)
        img = cv2.transform(img, np.matrix([[0.272, 0.534, 0.131],
                                            [0.349, 0.686, 0.168],
                                            [0.393, 0.769, 0.189]]))
        img[np.where(img > 255)] = 255
        return np.array(img, dtype=np.uint8)

    @staticmethod
    def invert_color(img):
        return cv2.bitwise_not(img)

    @staticmethod
    def binary(img):
        gray_img = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
        _, binary_img = cv2.threshold(gray_img, 127, 255, cv2.THRESH_BINARY)
        return binary_img

    @staticmethod
    def enhancement(img):
        gray_img = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

        laplacian = cv2.Laplacian(gray_img, cv2.CV_64F)

        abs_laplacian = np.absolute(laplacian)
        enhanced_img = np.uint8(abs_laplacian)

        return enhanced_img

    @staticmethod
    def hdr(img):
        return cv2.detailEnhance(img, sigma_s=12, sigma_r=0.15)

    @staticmethod
    def winter(img):
        blue_tint = np.array([255, 220, 180], dtype=np.uint8)
        blue_mask = np.full_like(img, blue_tint)
        winter_img = cv2.addWeighted(img, 0.7, blue_mask, 0.3, 0)

        blurred_img = cv2.GaussianBlur(winter_img, (5, 5), 0)

        return blurred_img

    @staticmethod
    def summer(img):
        yellow_tint = np.array([0, 100, 200], dtype=np.uint8)
        yellow_mask = np.full_like(img, yellow_tint)
        summer_img = cv2.addWeighted(img, 0.8, yellow_mask, 0.2, 0)

        increased_contrast = cv2.convertScaleAbs(summer_img, alpha=1.2, beta=10)

        return increased_contrast

    @staticmethod
    def sobel(image):
        gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)

        sobelx = cv2.Sobel(gray, cv2.CV_64F, 1, 0, ksize=5)  # Sobel x
        sobely = cv2.Sobel(gray, cv2.CV_64F, 0, 1, ksize=5)  # Sobel y

        combined = cv2.magnitude(sobelx, sobely)

        normalized = cv2.normalize(combined, None, 0, 255, cv2.NORM_MINMAX, dtype=cv2.CV_8U)

        return normalized

    @staticmethod
    def red(image):
        red_image = image.copy()

        red_image[:, :, 0] = 0
        red_image[:, :, 1] = 0

        return red_image

    @staticmethod
    def green(image):
        green_image = image.copy()

        green_image[:, :, 0] = 0
        green_image[:, :, 2] = 0

        return green_image

    @staticmethod
    def blue(image):
        blue_image = image.copy()

        blue_image[:, :, 1] = 0
        blue_image[:, :, 2] = 0

        return blue_image
