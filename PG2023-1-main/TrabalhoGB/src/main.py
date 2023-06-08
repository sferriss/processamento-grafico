import cv2
import tkinter as tk
from tkinter import filedialog

import numpy as np

from src.filters import Filter


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

        self.btn_open_webcam = tk.Button(window, text="Open Webcam", width=25, command=self.open_webcam)
        self.btn_open_webcam.grid(column=0, row=0)

        self.btn_open_image = tk.Button(window, text="Open Image", width=25, command=self.open_image)
        self.btn_open_image.grid(column=1, row=0)

        # self.btn_add_sticker = tk.Button(window, text="Add Sticker", width=25, command=self.add_sticker)
        # self.btn_add_sticker.grid(column=1, row=1)

        row = 1
        column = 0
        for filter_name in dir(Filter):
            if not filter_name.startswith("__"):
                btn_filter = tk.Button(window, text=f"Apply {filter_name.capitalize()} Filter", width=25,
                                       command=lambda name=filter_name: self.toggle_filter(name))
                btn_filter.grid(row=row, column=column)

                # btn_remove_filter = tk.Button(window, text=f"Sticker {filter_name.capitalize()}", width=25,
                #                               command=lambda name=filter_name: self.toggle_filter(name))
                # btn_remove_filter.grid(row=row, column=column + 1)

                column += 2
                if column > 1:
                    column = 0
                    row += 1

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

    def open_image(self):
        filepath = filedialog.askopenfilename(filetypes=[("Images", "*.jpg;*.jpeg;*.png")])
        if filepath:
            self.frame = cv2.imread(filepath)
            if len(self.stickers) > 0:
                self.stickers = []
            if self.is_video:
                self.is_video = False
                self.update_webcam_button()
                self.capture.release()
            self.update_frame()

    def update_webcam_button(self):
        if self.is_video:
            self.btn_open_webcam.configure(state="disabled")
        else:
            self.btn_open_webcam.configure(state="normal")

    def overlay_sticker(self, frame, sticker_img, x, y, max_width=100):

        sticker_h, sticker_w, _ = sticker_img.shape
        roi = frame[y:y + sticker_h, x:x + sticker_w]

        # Verifica se o sticker é maior do que a região de interesse
        if sticker_h > roi.shape[0] or sticker_w > roi.shape[1]:
            # Redimensiona o sticker para as dimensões da região de interesse
            sticker_img = cv2.resize(sticker_img, (roi.shape[1], roi.shape[0]))

        if sticker_img.shape[2] == 4:  # Verifica se o sticker possui um canal alfa
            alpha = sticker_img[:, :, 3] / 255.0  # Canal alfa do sticker
            sticker = sticker_img[:, :, :3]  # Componentes de cor do sticker
        else:
            alpha = np.ones((roi.shape[0], roi.shape[1]))  # Transparência total
            sticker = sticker_img

        # Redimensiona o sticker proporcionalmente para uma largura máxima desejada
        scale_factor = max_width / sticker.shape[1]
        sticker = cv2.resize(sticker, (max_width, int(sticker.shape[0] * scale_factor)))

        # Redimensiona o canal alfa para as mesmas dimensões do sticker, se necessário
        if alpha.shape[:2] != sticker.shape[:2]:
            alpha = cv2.resize(alpha, (sticker.shape[1], sticker.shape[0]))

        # Redimensiona a região de interesse para ter o mesmo tamanho do sticker
        roi = cv2.resize(roi, (sticker.shape[1], sticker.shape[0]))

        # Realiza a combinação das imagens levando em consideração o canal alfa
        result = np.zeros_like(roi)
        result[:, :, :3] = (1 - alpha[:, :, np.newaxis]) * roi[:, :, :3] + alpha[:, :, np.newaxis] * sticker

        # Atualiza a região de interesse com o sticker sobreposto
        frame[y:y + sticker.shape[0], x:x + sticker.shape[1]] = result.astype(np.uint8)

    def add_sticker(self):
        filepath = filedialog.askopenfilename(filetypes=[("Images", "*.png")])
        if filepath:
            sticker = cv2.imread(filepath, cv2.IMREAD_UNCHANGED)
            self.stickers.append(sticker)

    def update_frame(self):
        if self.is_video:
            ret, frame = self.capture.read()
        else:
            frame = self.frame.copy()

        # for sticker in self.stickers:
        #     x, y = 2, 2  # Exemplo: coordenadas onde o sticker será colocado
        #     self.overlay_sticker(frame, sticker, x, y)

        for filter_name, active in self.active_filters.items():
            if active and hasattr(self.filters, filter_name):
                frame = getattr(self.filters, filter_name)(frame)

        frame = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
        self.show_image(frame)

        if self.is_video:
            self.window.after(100, self.update_frame)

    def show_image(self, frame):
        frame = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
        cv2.imshow("Image", frame)
        cv2.waitKey(1)


window = tk.Tk()
app = Application(window)
window.mainloop()
