TailMan
=======
TailMan is a tab based application for monitoring multiple remote (or local) log files over ssh.

![ScreenShot](https://raw.github.com/martinhjartmyr/tailman/master/images/tailman-gui.png)


Installation
-------------------

A package for [ArchLinux](http://archlinux.org>) is available [here](https://aur.archlinux.org/packages/tailman/).
Contributions, so that packages for other distros can be made, are always welcome.


Manual installation
-------------------
First make sure you have you have the required libs below installed. Then open your `terminal`:

```bash
git clone https://github.com/martinhjartmyr/tailman.git
cd tailman
qmake-qt4
make
sudo make install
```

Dependencies
------------
- [libssh](http://www.libssh.org/get-it/)
- [qt4+](http://qt-project.org/downloads)


Customization
-------------
#### Change tail text/background color:
With your favorite text editor open `~/.config/tailman/tailman.conf` and append these lines with the colors of your choice:
```
color_bg=#121212
color_text=#00BD0D
```
