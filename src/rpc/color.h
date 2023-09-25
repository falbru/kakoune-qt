#ifndef RPCCOLOR_H
#define RPCCOLOR_H

#include <QString>
#include <QColor>

namespace RPC {
    class Color {
        public:
            Color(QString value);

            QString getValue() const;
            QColor toQColor(const Color& default_color);
        private:
            QString m_value;
    };
}

#endif
