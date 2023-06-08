import cv2
import tkinter as tk
from tkinter import filedialog

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

        self.btn_open_webcam = tk.Button(window, text="Open Webcam", width=25, command=self.open_webcam)
        self.btn_open_webcam.grid(column=0, row=0)

        self.btn_open_image = tk.Button(window, text="Open Image", width=25, command=self.open_image)
        self.btn_open_image.grid(column=1, row=0)

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

    def update_frame(self):
        if self.is_video:
            ret, frame = self.capture.read()
        else:
            frame = self.frame.copy()

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
