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

    void stream();
    void pause();
    void stop();
    void set_deviceId(int id) { deviceid = id; }
    void set_viewId(int id) { viewid = id; }
signals:
    void captured(int id, QImage img, unsigned char * buffer, qint64 timestamp);
private:
    bool work;
    bool term;
    int deviceid;
    int viewid;


    QVector<QRgb> grayColourTable;
    
public slots:
    
};

#endif // CAPTURETHREAD_H
