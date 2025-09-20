#pragma once

#include <QWidget>
#include <QPixmap>
#include <QPainter>
#include "VeyonCore.h"

class VEYON_CORE_EXPORT LockWidget : public QWidget
{
    Q_OBJECT
public:
    enum Mode
    {
        DesktopVisible,   // Show desktop behind lock
        BackgroundPixmap, // Show a background image
        NoBackground      // Solid color background
    };

    explicit LockWidget(QWidget* parent = nullptr)
        : QWidget(parent)
    {
        setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
        setAttribute(Qt::WA_DeleteOnClose);

        // Load the image from resources
        if (m_background.load(":/images/lock_background.png"))
            m_mode = BackgroundPixmap;
        else
            m_mode = NoBackground;
    }

protected:
    void paintEvent(QPaintEvent* event) override
    {
        Q_UNUSED(event);
        QPainter painter(this);

        switch (m_mode)
        {
            case BackgroundPixmap:
            {
                // Scale the image smoothly to fit the widget
                QPixmap scaled = m_background.scaled(size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
                
                // Center the image
                int x = (width() - scaled.width()) / 2;
                int y = (height() - scaled.height()) / 2;
                painter.drawPixmap(x, y, scaled);
                break;
            }

            case DesktopVisible:
                painter.fillRect(rect(), Qt::transparent);
                break;

            case NoBackground:
            default:
                painter.fillRect(rect(), Qt::black);
                break;
        }

        // Optional: overlay lock message
        painter.setPen(Qt::white);
        painter.setFont(QFont("Arial", 24, QFont::Bold));
        painter.drawText(rect(), Qt::AlignCenter, "This computer is locked");
    }

private:
    QPixmap m_background;
    Mode m_mode;
};
