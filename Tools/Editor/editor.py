import gtk
import gtk.glade
import sys

class Application:
    def __init__(self):
        self.gladefile = "editor.glade"
        self.builder = gtk.Builder()
        self.builder.add_from_file("editor.xml")
        self.builder.connect_signals(self)
        self.window = self.builder.get_object("window")
        self.window.show()

    def on_window_destroy(self, widget, data=None):
        gtk.main_quit()

    def on_button_clicked(self, widget, data=None):
        gtk.main_quit()
        

Application()
gtk.main()
