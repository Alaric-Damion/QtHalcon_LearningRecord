#ifndef CAPTURETHREAD_H
#define CAPTURETHREAD_H

#include <QThread>
#include <QImage>

class CaptureThread : public QThread
{
    Q_OBJECT
public:
    explicit CaptureThread(QObject *parent = 0);

public:
    void run();
    void setIndex(int index);
    void stream();
    void pause();
    void stop();
signals:
    void captured(QImage img, unsigned char * buffer, qint64 ts);
private:
    int index;
    bool work;
    bool term;


    QVector<QRgb> grayColourTable;

public slots:

};

#endif // CAPTURETHREAD_H
