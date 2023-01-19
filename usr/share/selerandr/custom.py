import subprocess
import gi
gi.require_version('Gtk', '3.0')
from gi.repository import Gtk
import os



class custom(Gtk.Window):
    def __init__(self):
        Gtk.Window.__init__(self, title="Type a resolution")

        self.set_default_size(555, 444)

        grid = Gtk.Grid()
        grid.set_column_homogeneous(True)
        grid.set_row_homogeneous(True)
        self.add(grid)

        label_resolution = Gtk.Label(label="Width:")
        grid.attach(label_resolution, 0, 0, 1, 1)

        self.Width = Gtk.Entry()
        self.Width.set_placeholder_text("1920")
        self.Width.set_text("1920")
        self.Width.connect("insert-text", self.on_insert_text)
        grid.attach(self.Width, 1, 0, 1, 1)

        label_resolution = Gtk.Label(label="Height:")
        grid.attach(label_resolution, 0, 1, 1, 1)

        self.Height = Gtk.Entry()
        self.Height.set_placeholder_text("1080")
        self.Height.set_text("1080")
        self.Height.connect("insert-text", self.on_insert_text)
        grid.attach(self.Height, 1, 1, 1, 1)

        Hz = Gtk.Label(label="Refresh Rate (Hz):")
        grid.attach(Hz, 0, 2, 1, 1)

        self.Hz = Gtk.Entry()
        self.Hz.set_placeholder_text("60")
        self.Hz.set_text("60")
        self.Hz.connect("insert-text", self.on_insert_text)
        grid.attach(self.Hz, 1, 2, 1, 1)

        apply_button = Gtk.Button(label="Generate")
        apply_button.connect("clicked", self.on_apply_clicked)
        grid.attach(apply_button, 0, 3, 2, 1)


        apply_text = Gtk.Label(label="WARNING: adding a custom resolution may cause unexpected results, \n like black screen if is unsopported by the Display or errors on resolutions list")
        grid.attach(apply_text, 0, 4, 2, 1)
    def on_insert_text(self, entry, new_text, new_text_length, position):
        # only allow numeric input
        if not new_text.isdigit():
            entry.stop_emission_by_name("insert-text")
        if len(entry.get_text()) > 4:
            entry.set_text(entry.get_text()[:4])

    def on_apply_clicked(self, button):
        self.width = self.Width.get_text()
        self.height = self.Height.get_text()
        self.hz = self.Hz.get_text()
        self.command = "cvt" + " " + self.width + " " + self.height + " " +  self.hz +" | grep Modeline | cut -c 10-"
        self.result = subprocess.run(self.command, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        self.default = "cvt 1300 600 65 | grep Modeline | cut -c 10- "
        self.defaultshow = subprocess.run(self.default, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        self.getdisplay = "xrandr | grep -w " + " connected" + "| awk '{print $1}'"
        self.display = subprocess.run(self.getdisplay, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        self.name = (self.width + "x" + self.height + "_" + self.hz + ".00")
        


        if not self.width or not self.height or not self.hz:
            dialog = Gtk.MessageDialog(parent=self, modal=True, message_type=Gtk.MessageType.ERROR,
    buttons=Gtk.ButtonsType.OK, message_format="Error")
            dialog.format_secondary_text("Width, Height, and Hz are required fields, but this is an example of a custom resolution by 1300*600@65Hz: \n")
    # Create a vertical box


            grid = Gtk.Grid()
            grid.set_column_homogeneous(True)
            grid.set_row_homogeneous(True)
            self.add(grid)

            self.label1 = Gtk.Label(label="xrandr --newmode " + self.defaultshow.stdout.decode())
            self.label1.set_selectable(True)
            grid.attach(self.label1, 1, 0, 1, 1)

            self.label2 = Gtk.Label(label="xrandr --addmode " + self.display.stdout.decode().strip() + " 65.00Hz")
            self.label2.set_selectable(True)
            grid.attach(self.label2, 1, 1, 1, 1)

    # Add the box to the dialog
            dialog.get_content_area().add(grid)
            dialog.show_all()
            dialog.run()
            dialog.destroy()

        else:
            dialog = Gtk.MessageDialog(parent=self, modal=True, message_type=Gtk.MessageType.ERROR,
    buttons=Gtk.ButtonsType.OK, message_format="Generated Resolution")
            dialog.format_secondary_text("copy this commands and add it to your autostart, or execute this on a terminal to use only on this session, you need to restart SeleRandr to see new resolution \n")
    # Create a vertical box


            grid = Gtk.Grid()
            grid.set_column_homogeneous(True)
            grid.set_row_homogeneous(True)
            self.add(grid)

            self.label1 = Gtk.Label(label="xrandr --newmode " + self.result.stdout.decode())
            self.label1.set_selectable(True)
            grid.attach(self.label1, 1, 0, 1, 1)

            self.label2 = Gtk.Label(label="xrandr --addmode " + self.display.stdout.decode().strip() + " " + self.name)
            self.label2.set_selectable(True)
            grid.attach(self.label2, 1, 1, 1, 1)

            session_button = Gtk.Button(label="Add Resolution only on this session")
            session_button.connect("clicked", self.on_session_clicked)
            grid.attach(session_button, 1, 2, 1, 1)

            print_button = Gtk.Button(label="Save commands to a file")
            print_button.connect("clicked", self.on_print_clicked)
            grid.attach(print_button, 1, 3, 1, 1)

            self.label1 = Gtk.Label(label="File Will be saved on ~/NewResolutions, you also can see old saved resolutions")
            grid.attach(self.label1, 1, 4, 1, 1)
    # Add the box to the dialog
            dialog.get_content_area().add(grid)
            dialog.show_all()
            dialog.run()
            dialog.destroy()

    def on_session_clicked(self, button):
        cmd1 = "xrandr --newmode " + self.result.stdout.decode()        
        subprocess.run(cmd1, shell=True)
        cmd2 = "xrandr --addmode " + self.display.stdout.decode().strip() + " " + self.name        
        subprocess.run(cmd2, shell=True)

    def on_print_clicked(self, button):
        home = os.path.expanduser("~" + "/")
        cmd0 = "echo '" + self.name + "\n ----- ' >> " + home + "NewResolutions"
        subprocess.run(cmd0, shell=True)
        cmd1 = "echo " + "xrandr --newmode " + self.result.stdout.decode().strip() + ">>" + home + "NewResolutions"      
        subprocess.run(cmd1, shell=True)
        cmd2 = "echo '" + "xrandr --addmode " + self.display.stdout.decode().strip() + " " + self.name + "\n ----- \n' >> " + home + "NewResolutions"
        subprocess.run(cmd2, shell=True)

window = custom()
window.connect("delete-event", Gtk.main_quit)
window.show_all()
Gtk.main()

