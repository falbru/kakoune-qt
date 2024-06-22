#include "keybindings.hpp"

QList<KeyBinding> keybindings = {
    {QKeyCombination(Qt::ControlModifier | Qt::ShiftModifier, Qt::Key_H),
     [](MainWindow *window) { window->focusLeft(); }},
    {QKeyCombination(Qt::ControlModifier | Qt::ShiftModifier, Qt::Key_L),
     [](MainWindow *window) { window->focusRight(); }},
    {QKeyCombination(Qt::ControlModifier | Qt::ShiftModifier, Qt::Key_J),
     [](MainWindow *window) { window->focusDown(); }},
    {QKeyCombination(Qt::ControlModifier | Qt::ShiftModifier, Qt::Key_K),
     [](MainWindow *window) { window->focusUp(); }},
};

KeyBindingsFilter::KeyBindingsFilter(MainWindow *main_window) : m_main_window(main_window)
{
}

bool KeyBindingsFilter::eventFilter(QObject *object, QEvent *event)
{
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        for (const KeyBinding &binding : keybindings)
        {
            if (binding.keyCombination == keyEvent->keyCombination())
            {
                binding.action(m_main_window);
                return true;
            }
        }
    }
    return false;
}
