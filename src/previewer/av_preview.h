#ifndef AV_PREVIEW_H
#define AV_PREVIEW_H

#include <QFrame>
#include <QtGui>
#include <QtCore>
//#include <Phonon>
#include <QtMultimedia>
#include <QtMultimediaWidgets>

namespace Ui {
class av_preview;
}

class av_preview : public QFrame
{
    Q_OBJECT
    
public:
    explicit av_preview(QWidget *parent = 0);
    ~av_preview();
    void load_source(QString);
    void play_media();
    void stop_media();

signals:

public slots:
    void mediaStateChanged(QMediaPlayer::PlaybackState state);
    void positionChanged(qint64 position);
    void durationChanged(qint64 duration);
    void setPosition(int position);
    void play();

private:
    Ui::av_preview *ui;

    QMediaPlayer mediaPlayer;
    QAbstractButton *playButton;
    QSlider *positionSlider;
    QAudioOutput audio_output;
    QAudioDevice audio_device ;

};

#endif // AV_PREVIEW_H
