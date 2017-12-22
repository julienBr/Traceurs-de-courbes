#include <defines.h>

QColor randomColor()
{
    qsrand( QTime::currentTime().msec() );
    int random = qrand() % ((0xFFFFFF + 1) - 0x000000 ) + 0x000000;
    return QColor(random);
}
