#ifndef IMAGELISTVIEW_H
#define IMAGELISTVIEW_H

#include <QAbstractItemView>
#include <QCache>
#include <QFuture>
#include <QFutureWatcher>
#include <QImage>
#include <QMetaObject>
#include <memory>
#include <QImage>
#include <QPaintEvent>
#include <QScrollBar>
#include <QStylePainter>
#include <QThread>
#include <QTimer>
#include <QMenu>
#include <QtConcurrent>
#include <QtDebug>

#define MIN_THUMBNAIL_SIZE 150
#define MAX_THUMBNAIL_SIZE 400

class QTimer;

struct ImageLoadingTask {
    int row;
    QString imageFileName;
    std::unique_ptr<QImage> image;
};
using ImageLoadingTaskSharedPtr = std::shared_ptr<ImageLoadingTask>;
using ImageLoadingTaskFutureWatcher = QFutureWatcher<ImageLoadingTaskSharedPtr>;

class ImageListView : public QAbstractItemView {
    Q_OBJECT
public:
    ImageListView(QWidget* parent = Q_NULLPTR);

public:
    void setThumbnailSize(int size);

    int columnCount() const;
    void setColumnCount(int columnCount);
protected:
    QPair<int, int> modelRowRangeForViewportRect(const QRect& rect);

    void startScrollDelayTimer();
    void stopScrollDelayTimer();
    void startAsyncImageLoading();
    void stopAsyncImageLoading();

public:
    virtual QRect visualRect(const QModelIndex& index) const override;
    virtual QModelIndex indexAt(const QPoint& point) const override;
    virtual void scrollTo(const QModelIndex& index, ScrollHint hint) override;
    virtual void setModel(QAbstractItemModel* model) override;

public slots:
    virtual void reset() override;
    void OnExportToDesktop();


protected:
    virtual QModelIndex moveCursor(CursorAction cursorAction, Qt::KeyboardModifiers modifiers) override;
    virtual int horizontalOffset() const override;
    virtual int verticalOffset() const override;
    virtual bool isIndexHidden(const QModelIndex& index) const override;
    virtual void setSelection(const QRect& rect, QItemSelectionModel::SelectionFlags command) override;
    virtual QRegion visualRegionForSelection(const QItemSelection& selection) const override;

protected slots:
    virtual void updateGeometries() override;
    virtual void verticalScrollbarValueChanged(int value) override;

protected:
    virtual void paintEvent(QPaintEvent* event) override;
    virtual void resizeEvent(QResizeEvent* event) override;
    virtual void mousePressEvent(QMouseEvent *e) override;
    virtual void keyPressEvent(QKeyEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event);


signals:

    void signal_item_clicked(qint64 index);
    void signal_spacebar_pressed(qint64 index);
    void signal_control_e_key_pressed(qint64 index);
    void signal_list_view_invalid_item_clicked();
    void signal_item_double_clicked(qint64 index);


private:
    int m_thumb_size;
    QString m_strFilePath;
    QTimer* m_loadingDelayTimer = nullptr;
    QTimer* m_updatingDelayTimer = nullptr;
    ImageLoadingTaskFutureWatcher m_imageLoadingFutureWatcher;
    QList<int> m_invalidatingModelRows;
    QCache<QString, QImage> m_imageCache;
    QStyleOptionButton cbOpt;

    qint64 index_keypress_event = -1;
};

#endif // IMAGELISTVIEW_H
