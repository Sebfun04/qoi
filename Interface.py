import tkinter as tk
from tkinter import ttk
import sys

DEBUG = True

if not DEBUG:
    try:
        import intel_jtag_uart
    except ImportError:
        print("Erreur: Le module intel_jtag_uart n'a pas été trouvé.")
        sys.exit(1)
else:
    intel_jtag_uart = None

class App(tk.Tk):
    def __init__(self, *args, **kwargs):
        tk.Tk.__init__(self, *args, **kwargs)
        
        # Format des fenêtres
        width = 300
        height = 150
        self.title("Interface de l'opérateur")
        self.resizable( False, False)
        screen_width = self.winfo_screenwidth()
        screen_height = self.winfo_screenheight()
        x_coord = int((screen_width/2)-(width/2))
        y_coord = int((screen_height/2)-(height/2))
        self.geometry(f"{width}x{height}+{x_coord}+{y_coord}")
        
        # Définition du mode debug
        self.debug = intel_jtag_uart is None

       # Création de l'objet JTAG UART seulement si nous ne sommes pas en mode debug
        if not self.debug:
            try:
                self.jtag = intel_jtag_uart.intel_jtag_uart()
            except Exception as e:
                print(f"Erreur lors de la création de l'objet JTAG UART: {e}")
                self.debug = True  # Bascule en mode debug si l'objet JTAG UART ne peut pas être créé
                
        # Création du conteneur de frames
        container = tk.Frame(self)
        container.pack(side="top", fill="both", expand=True)
        container.grid_rowconfigure(0, weight=1)
        container.grid_columnconfigure(0, weight=1)

        # Dictionnaire pour stocker les frames
        self.frames = {}

        # Création de frames
        for F in (MainPage, LoadingPage):
            frame = F(container, self)
            self.frames[F] = frame
            frame.grid(row=0, column=0, sticky="nsew")

        # Affichage de la première frame
        self.show_frame(MainPage)

    def show_frame(self, cont):
        frame = self.frames[cont]
        frame.tkraise()

    def send(self, data):
        # Envoie des données via l'interface JTAG si nous ne sommes pas en mode debug
        if not self.debug:
            self.jtag.write(data)
        else:
            print(f"DEBUG: Envoi de '{data}'")

    def receive(self):
        # Reçoit des données via l'interface JTAG si nous ne sommes pas en mode debug
        if not self.debug:
            return self.jtag.read()
        else:
            print("DEBUG: Lecture de données")

class MainPage(tk.Frame):
    def __init__(self, parent, controller):
        tk.Frame.__init__(self, parent)
        self.controller = controller

        # Boutons pour envoyer les chiffres 1, 2, ou 3 et montrer la page de chargement
        button1 = ttk.Button(self, text="Decode QOI", command=lambda: [self.controller.send(b'1'), self.controller.show_frame(LoadingPage)])
        button2 = ttk.Button(self, text="Encode QOI", command=lambda: [self.controller.send(b'2'), self.controller.show_frame(LoadingPage)])
        button3 = ttk.Button(self, text="Affiche VGA", command=lambda: [self.controller.send(b'3'), self.controller.show_frame(LoadingPage)])

        button1.pack(pady=10)
        button2.pack(pady=10)
        button3.pack(pady=10)


class LoadingPage(tk.Frame):
    def __init__(self, parent, controller):
        tk.Frame.__init__(self, parent)
        self.controller = controller

        label = tk.Label(self, text="Loading...")
        label.pack(pady=10,padx=10)

        button1 = ttk.Button(self, text="Back to Main Menu",
                            command=lambda: controller.show_frame(MainPage))
        button1.pack()


if __name__ == "__main__":
    app = App()
    app.mainloop()