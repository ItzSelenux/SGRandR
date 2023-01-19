import subprocess
import gi
gi.require_version('Gtk', '3.0')
from gi.repository import Gtk, Gdk, GdkX11

class selerandr(Gtk.Window):
    def __init__(self):
        Gtk.Window.__init__(self, title="SeleRandr")

        self.set_default_size(555, 444)

        menubar = Gtk.MenuBar()

        icon_menu_item = Gtk.MenuItem()
        icon_image = Gtk.Image.new_from_icon_name("video-display", Gtk.IconSize.MENU)
        icon_menu_item.add(icon_image)
        icon_menu_item.set_sensitive(False)
        menubar.append(icon_menu_item)

        # Close button
        close_button = Gtk.Button()
        close_button.set_image(Gtk.Image.new_from_icon_name("window-close", Gtk.IconSize.BUTTON))
        close_button.connect("clicked", Gtk.main_quit)
        
        title_box = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL)
        title_box.pack_start(menubar, True, True, 0)
        title_box.pack_end(close_button, False, False, 0)
        
        self.set_titlebar(title_box)

        #help option
        help_menu = Gtk.MenuItem(label="Help")
        help_menu.set_submenu(Gtk.Menu())
        menubar.append(help_menu)
        
        help_item = Gtk.MenuItem(label="About")
        help_item.connect("activate", self.on_about_clicked)
        help_menu.get_submenu().append(help_item)

        
        self.show_all()

        grid = Gtk.Grid()
        grid.set_column_homogeneous(True)
        grid.set_row_homogeneous(True)
        self.add(grid)

        label_resolution = Gtk.Label(label="Resolution:")
        grid.attach(label_resolution, 0, 0, 1, 1)

        self.resolution_combo = Gtk.ComboBoxText()
        resolutions = self.get_resolutions()
        for resolution in resolutions:
            self.resolution_combo.append_text(resolution[0])
        grid.attach(self.resolution_combo, 1, 0, 1, 1)

        label_refresh_rate = Gtk.Label(label="Refresh Rate:")
        grid.attach(label_refresh_rate, 0, 1, 1, 1)

        self.refresh_rate_combo = Gtk.ComboBoxText()
        refresh_rates = set([res[1] for res in resolutions])
        for refresh_rate in refresh_rates:
            self.refresh_rate_combo.append_text(refresh_rate)
        grid.attach(self.refresh_rate_combo, 1, 1, 1, 1)

        label_rotate = Gtk.Label(label="Rotate:")
        grid.attach(label_rotate, 0, 2, 1, 1)

        self.rotate_combo = Gtk.ComboBoxText()
        self.rotate_combo.append_text("normal")
        self.rotate_combo.append_text("left")
        self.rotate_combo.append_text("right")
        self.rotate_combo.append_text("inverted")
        grid.attach(self.rotate_combo, 1, 2, 1, 1)

        label_scale = Gtk.Label(label="Scale (%):")
        grid.attach(label_scale, 0, 4, 1, 1)

        self.scale = Gtk.Scale()
        self.scale.set_range(30, 200)
        self.scale.set_value(100)
        self.scale.set_digits(0)
        grid.attach(self.scale, 1, 4, 1, 1)

        label_output = Gtk.Label(label="Output:")
        grid.attach(label_output, 0, 3, 1, 1)

        self.output_combo = Gtk.ComboBoxText()
        outputs = self.get_outputs()
        for output in outputs:
            self.output_combo.append_text(output)
        grid.attach(self.output_combo, 1, 3, 1, 1)

        self.onoff_combo = Gtk.ComboBoxText()
        self.onoff_combo.append_text("On")
        self.onoff_combo.append_text("Off")
        grid.attach(self.onoff_combo, 2, 3, 1, 1)

        if len(outputs) == 1:
            self.output_combo.set_sensitive(False)
            self.onoff_combo.set_sensitive(False)

        auto_button = Gtk.Button(label="Add Custom Resolution")
        auto_button.connect("clicked", self.on_custom_clicked)
        grid.attach(auto_button, 0, 5, 1, 1)

        auto_button = Gtk.Button(label="Default")
        auto_button.connect("clicked", self.on_auto_clicked)
        grid.attach(auto_button, 1, 5, 1, 1)
       
        apply_button = Gtk.Button(label="Apply")
        apply_button.connect("clicked", self.on_apply_clicked)
        grid.attach(apply_button, 2, 5, 1, 1)


        self.resolution_combo.set_active(0)
        self.refresh_rate_combo.set_active(0)
        self.rotate_combo.set_active(0)
        self.output_combo.set_active(0)
        self.onoff_combo.set_active(0)

    def on_about_clicked(self, widget):
        about_dialog = Gtk.AboutDialog()
        about_dialog.set_program_name("SeleRandr")
        about_dialog.set_version("1.0")
        about_dialog.set_copyright("This program is licensed under MIT License")
        about_dialog.set_comments("SeleRandr is a standalone GUI for Xrandr")
        about_dialog.set_website("https://github.com/ItzSelenux/selerandr")
        about_dialog.set_website_label("Source Code")
        about_dialog.set_authors(["~ItzSelenux"])
        icon_theme = Gtk.IconTheme.get_default()
        icon = icon_theme.load_icon("video-display", 64, 0)
        about_dialog.set_logo(icon)
        about_dialog.run()
        about_dialog.destroy()


    def on_apply_clicked(self, button):
        resolution, refresh_rate = self.resolution_combo.get_active_text(), self.refresh_rate_combo.get_active_text()
        output = self.output_combo.get_active_text()
        rotate = self.rotate_combo.get_active_text() 
        subprocess.run(["xrandr", "--output", output, "--auto", "--rotate", "normal", "--scale", "1"])
        subprocess.run(["xrandr", "--output", output, "--mode", resolution, "--rate", refresh_rate, "--rotate", rotate, "--scale", str(self.scale.get_value()/100)])

    def on_auto_clicked(self, button):
        output = self.output_combo.get_active_text()
        subprocess.run(["xrandr", "--output", output, "--auto", "--rotate", "normal", "--scale", "1"])

    def on_custom_clicked(self, button):
        subprocess.run(["python3", "custom.py"])

    def get_resolutions(self):
        xrandr_output = subprocess.run(["xrandr"], capture_output=True, text=True).stdout
        resolutions = []
        for line in xrandr_output.split("\n"):
            if "  " in line:
                resolution, refresh_rate = line.split()[0], line.split()[1]
                resolutions.append((resolution, refresh_rate))
        return resolutions

    def get_outputs(self):
        xrandr_output = subprocess.run(["xrandr"], capture_output=True, text=True).stdout
        outputs = []
        for line in xrandr_output.split("\n"):
            if " connected" in line:
                outputs.append(line.split()[0])
        return outputs

win = selerandr()
win.connect("delete-event", Gtk.main_quit)
win.show_all()
Gtk.main()
