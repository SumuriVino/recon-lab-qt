#include "av_preview.h"
#include "ui_av_preview.h"


av_preview::av_preview(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::av_preview)
{
    ui->setupUi(this);

    setWindowFlag(Qt::Window);

    QVideoWidget *videoWidget = new QVideoWidget(this);

    audio_device = QMediaDevices::defaultAudioOutput();
    audio_output.setDevice(audio_device);
    audio_device.swap(audio_device);
    mediaPlayer.setAudioOutput(&audio_output);

    playButton = new QPushButton(this);
    //  playButton->setEnabled(false);
    playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));

    connect(playButton, SIGNAL(clicked()),this, SLOT(play()));

    positionSlider = new QSlider(Qt::Horizontal);
    positionSlider->setRange(0, 0);

    connect(positionSlider, SIGNAL(sliderMoved(int)),this, SLOT(setPosition(int)));


    QBoxLayout *layout = new QVBoxLayout(this);

    QBoxLayout *controlLayout = new QHBoxLayout(layout->widget());
    controlLayout->setContentsMargins(0,0,0,0);
    controlLayout->addWidget(playButton);
    controlLayout->addWidget(positionSlider);

    layout->addWidget(videoWidget);
    layout->addLayout(controlLayout);


    setLayout(layout);

    mediaPlayer.setVideoOutput(videoWidget);
    connect(&mediaPlayer, SIGNAL(playbackStateChanged(QMediaPlayer::PlaybackState)),this, SLOT(mediaStateChanged(QMediaPlayer::PlaybackState)));
    connect(&mediaPlayer, SIGNAL(positionChanged(qint64)), this, SLOT(positionChanged(qint64)));
    connect(&mediaPlayer, SIGNAL(durationChanged(qint64)), this, SLOT(durationChanged(qint64)));

}

av_preview::~av_preview()
{
    delete ui;
}

void av_preview::load_source(QString source_file)
{
    mediaPlayer.setSource(QUrl::fromLocalFile(source_file));
}

void av_preview::play_media()
{
    mediaPlayer.play();
}

void av_preview::stop_media()
{
    mediaPlayer.pause();

    playButton->setEnabled(true);
}


void av_preview::play()
{
    switch(mediaPlayer.playbackState())
    {
    case QMediaPlayer::PlayingState:
        mediaPlayer.pause();
        break;
    default:
        mediaPlayer.play();
        break;
    }
}


void av_preview::setPosition(int position)
{
    mediaPlayer.setPosition(position);
}

void av_preview::mediaStateChanged(QMediaPlayer::PlaybackState state)
{
    switch(state) {
    case QMediaPlayer::PlayingState:
        playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
        break;
    default:
        playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
        break;
    }
}


void av_preview::positionChanged(qint64 position)
{
    positionSlider->setValue(position);
}

void av_preview::durationChanged(qint64 duration)
{
    positionSlider->setRange(0, duration);
}
