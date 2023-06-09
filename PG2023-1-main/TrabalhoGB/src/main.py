import cv2
import tkinter as tk
from tkinter import filedialog

import numpy as np

from src.filters import Filter
from src.sticker import Sticker


def overlay_sticker(image, sticker, x, y):
    # Verifique se o sticker tem um canal alfa (transparência)
    if sticker.shape[2] < 4:
        # Se não tiver, adicione um
        sticker = cv2.cvtColor(sticker, cv2.COLOR_BGR2BGRA)

    # Redimensione o sticker para um tamanho menor
    sticker = cv2.resize(sticker, (110, 110))

    # Verifique se a imagem tem três canais (cor)
    if len(image.shape) < 3 or image.shape[2] < 3:
        # Se não tiver, converta para colorida
        image = cv2.cvtColor(image, cv2.COLOR_GRAY2BGR)

    # Defina a região de interesse na imagem de entrada para o tamanho do sticker
    roi = image[y:y + sticker.shape[0], x:x + sticker.shape[1]]

    # Crie uma máscara usando o canal alfa do sticker e inverta
    mask = sticker[..., 3] / 255.0
    inverse_mask = 1 - mask

    # Usando a máscara e a máscara inversa, combine o sticker e a região de interesse (ROI)
    for c in range(0, 3):
        roi[..., c] = (mask * sticker[..., c] + inverse_mask * roi[..., c]).astype(np.uint8)

    return image


class Application:
    def __init__(self, window):
        self.window = window
        self.window.title("Filter tools")
        self.capture = None
        self.frame = None
        self.is_video = False
        self.filters = Filter()
        self.active_filters = {}
        self.stickers = []
        self.sticker = None

        self.btn_open_webcam = tk.Button(window, text="Open Webcam", width=25, command=self.open_webcam)
        self.btn_open_webcam.grid(column=0, row=0)

        self.btn_open_image = tk.Button(window, text="Open Image", width=25, command=self.open_image)
        self.btn_open_image.grid(column=1, row=0)

        self.btn_add_sticker = tk.Button(window, text="Add Sticker", width=52, command=self.add_sticker)
        self.btn_add_sticker.grid(column=0, row=1, columnspan=2)

        self.btn_clear = tk.Button(window, text="Clear", width=52, command=self.clear_all)
        self.btn_clear.grid(column=0, row=2, columnspan=2)

        filter_methods = [method for method in dir(Filter) if not method.startswith("__")]
        total_methods = len(filter_methods)

        row = 3
        column = 0

        for i, filter_name in enumerate(filter_methods):
            btn_filter = tk.Button(window, text=f"Apply {filter_name.capitalize()} Filter", width=25,
                                   command=lambda name=filter_name: self.toggle_filter(name))
            btn_filter.grid(row=row, column=column)

            if i == total_methods // 2:
                column = 1
                row = 2
            row += 1

    def mouse_click(self, event, x, y, flags, param):
        # Verifica se o botão esquerdo do mouse foi clicado
        if event == cv2.EVENT_LBUTTONDOWN and self.sticker is not None:
            sticker = Sticker(x, y, self.sticker)
            self.stickers.append(sticker)
            self.sticker = None
            if not self.is_video:
                self.update_frame()

    def toggle_filter(self, filter_name):
        if filter_name in self.active_filters:
            self.active_filters.pop(filter_name)
        else:
            self.active_filters[filter_name] = True

        self.update_frame()

    def open_webcam(self):
        self.capture = cv2.VideoCapture(0)
        self.is_video = True
        self.update_webcam_button()
        self.window.after(100, self.update_frame)

        self.clear_stickers()
        self.clear_filters()

    def open_image(self):
        filepath = filedialog.askopenfilename(filetypes=[("Images", "*.jpg;*.jpeg;*.png")])
        if filepath:
            self.frame = cv2.imread(filepath)
            self.clear_stickers()
            self.clear_filters()

            if self.is_video:
                self.is_video = False
                self.update_webcam_button()
                self.capture.release()
            self.update_frame()

    def clear_stickers(self):
        if len(self.stickers) > 0:
            self.stickers = []

    def clear_filters(self):
        self.active_filters.clear()

    def clear_all(self):
        self.clear_filters()
        self.clear_stickers()

        if not self.is_video:
            self.update_frame()

    def update_webcam_button(self):
        if self.is_video:
            self.btn_open_webcam.configure(state="disabled")
        else:
            self.btn_open_webcam.configure(state="normal")

    def add_sticker(self):
        filepath = filedialog.askopenfilename(filetypes=[("Images", "*.png")])
        if filepath:
            self.sticker = cv2.imread(filepath, cv2.IMREAD_UNCHANGED)
            cv2.setMouseCallback("Image", self.mouse_click)

    def update_frame(self):
        if self.is_video:
            ret, frame = self.capture.read()
        else:
            frame = self.frame.copy()

        for sticker in self.stickers:
            overlay_sticker(frame, sticker.image, sticker.x, sticker.y)

        for filter_name, active in self.active_filters.items():
            if active and hasattr(self.filters, filter_name):
                frame = getattr(self.filters, filter_name)(frame)

        frame = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
        self.show_image(frame)

        if self.is_video:
            self.window.after(100, self.update_frame)

    def show_image(self, frame):
        cv2.namedWindow("Image")

        frame = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
        cv2.imshow("Image", frame)
        cv2.waitKey(1)


window = tk.Tk()
app = Application(window)
window.mainloop()
