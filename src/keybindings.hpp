#ifndef KEYBINDINGS_HPP
#define KEYBINDINGS_HPP

#include "mainwindow.hpp"
#include <qobject.h>

struct KeyBinding
{
    QKeyCombination keyCombination;
    std::function<void(MainWindow *)> action;
};

class KeyBindingsFilter : public QObject
{
    Q_OBJECT
  public:
    KeyBindingsFilter(MainWindow *main_window);

  protected:
    bool eventFilter(QObject *object, QEvent *event);

  private:
    MainWindow *m_main_window;
};

#endif
