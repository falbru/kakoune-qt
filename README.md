# Kakoune Qt

Kakoune Qt is a Qt-based GUI for Kakoune written in C++.

![screenshot](https://github.com/user-attachments/assets/8bdba0c8-3921-4b37-94a4-0a599819c58b)

# Features

- Create splits with `:kakqt-split-horizontal` and `:kakqt-split-vertical`

- Focus splits with `:kakqt-focus <client_name>`

- Show and hide clients with `:kakqt-show <client_name>` and `:kakqt-hide <client_name>`

- Navigate between splits with keybindings `<c-h>`, `<c-j>`, `<c-k>` and `<c-l>`

# Install from source

1. Install the necessary build dependencies

`sudo apt install -y git build-essentials cmake qt6-base-dev qt6-svg-dev`

2. Clone the repository

```sh
git clone https://github.com/falbru/kakoune-qt ~/.config/kak/autoload/
```

3. Build the project

```sh
cd ~/.config/kak/autoload/kakoune-qt
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make
```

You can then run the application with `./kak-qt`

> NOTE: Make sure Kakoune is installed on your system!

# Configuration

Kakoune Qt is configured using the `ui_option` in Kakoune. The following options are implemented:

- `kakqt_set_font <font_name>`: Set the font

- `kakqt_set_color_<color_name> <rgb_value>`: Set the default colors (color_name can be: black, red, green, yellow, blue, magenta, cyan or white)

Example configuration:

`set global ui_options 'kakqt_set_font="Ubuntu Mono 12" kakqt_set_color_red="rgb:FF0000"'`
