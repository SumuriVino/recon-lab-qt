#include "imagelistview.h"

ImageListView::ImageListView(QWidget* parent)
    : QAbstractItemView(parent)
    , m_thumb_size(MIN_THUMBNAIL_SIZE)
    , m_loadingDelayTimer{ new QTimer{ this } }
    , m_updatingDelayTimer{ new QTimer{ this } }
    , m_imageCache(1)
{
    horizontalScrollBar()->setRange(0, 0);
    verticalScrollBar()->setRange(0, 0);
    setSelectionMode(ExtendedSelection);
    setSelectionBehavior(SelectItems);

    m_loadingDelayTimer->setSingleShot(true);
    connect(m_loadingDelayTimer, &QTimer::timeout, [this] {
        startAsyncImageLoading();
    });
    connect(&m_imageLoadingFutureWatcher,
            &QFutureWatcherBase::progressRangeChanged, [](int min, int max) {
    });
    connect(&m_imageLoadingFutureWatcher,
            &QFutureWatcherBase::progressValueChanged, [](int val) {
    });
    connect(&m_imageLoadingFutureWatcher,
            &QFutureWatcherBase::started, []() {
    });
    connect(&m_imageLoadingFutureWatcher,
            &QFutureWatcherBase::finished, []() {
    });
    connect(&m_imageLoadingFutureWatcher,
            &QFutureWatcherBase::resultReadyAt,
            [](int index) {
    });

    connect(&m_imageLoadingFutureWatcher,
            &QFutureWatcherBase::resultsReadyAt,
            [this](int begin, int end) {
        for (int index = begin; index < end; ++index) {
            auto item = m_imageLoadingFutureWatcher.resultAt(index);
            m_invalidatingModelRows.append(item->row);
            m_imageCache.insert(item->imageFileName, item->image.release());
        }
        if (!m_updatingDelayTimer->isActive())
            m_updatingDelayTimer->start(250);
    });

    //
    m_updatingDelayTimer->setSingleShot(true);
    connect(m_updatingDelayTimer, &QTimer::timeout, [this] {
        QRect invalidatingRect;
        for (auto&& row : m_invalidatingModelRows) {
            auto rect = visualRect(model()->index(row, 0, rootIndex()));
            invalidatingRect = invalidatingRect.united(rect);
        }
        if (viewport()->rect().intersects(invalidatingRect)) {
            viewport()->update(invalidatingRect);
        }
    });
}

void ImageListView::startScrollDelayTimer()
{
    stopScrollDelayTimer();
    m_loadingDelayTimer->start(200);
}

void ImageListView::stopScrollDelayTimer()
{
    m_updatingDelayTimer->stop();
    stopAsyncImageLoading();
    m_loadingDelayTimer->stop();
}

void ImageListView::setThumbnailSize(int size)
{
    m_thumb_size = size;
    setColumnCount(width() / m_thumb_size);
}

int ImageListView::columnCount() const
{
    return width() / m_thumb_size;
}

void ImageListView::setColumnCount(int columnCount)
{
    reset();
}

QPair<int, int> ImageListView::modelRowRangeForViewportRect(const QRect& rect)
{
    QRect r = rect.normalized();
    int rowCount = model()->rowCount(rootIndex());
    int begin = 0;
    {
        QModelIndex startIndex = indexAt(r.topLeft());
        if (startIndex.isValid()) {
            begin = startIndex.row();
        }
    }
    int end = begin;
    {
        QModelIndex finishIndex = indexAt(r.bottomRight());
        if (finishIndex.isValid()) {
            end = finishIndex.row() + 1;
        } else {
            end = rowCount;
        }
    }
    return QPair<int, int>(begin, end);
}

void ImageListView::startAsyncImageLoading()
{
    class ImageLoader {
    public:
        typedef ImageLoadingTaskSharedPtr result_type;

    public:
        ImageLoadingTaskSharedPtr operator()(ImageLoadingTaskSharedPtr task)
        {
            if (!task->image) {
                task->image = std::make_unique<QImage>();
            }
            if (task->image->isNull()) {
                if (!task->image->load(task->imageFileName)) {
                    qWarning() << "Loading" << task->imageFileName << "failed";
                }
            }
            return task;
        }
    };
    stopAsyncImageLoading();
    QPair<int, int> modelRowRange = modelRowRangeForViewportRect(viewport()->rect());
    QList<ImageLoadingTaskSharedPtr> viewportItems;
    viewportItems.reserve(modelRowRange.second - modelRowRange.first);
    for (int row = modelRowRange.first; row < modelRowRange.second; ++row) {
        QModelIndex index = model()->index(row, 0, rootIndex());
        QVariant imageFileNameVariant = model()->data(index);
        QString imageFileName = imageFileNameVariant.toString();
        ImageLoadingTask item{ row,imageFileName };
        QImage* ptr = m_imageCache.take(imageFileName);
        if (ptr) {
            item.image.reset(ptr);
        }
        viewportItems << std::make_shared<ImageLoadingTask>(std::move(item));
    }
    QFuture<ImageLoadingTaskSharedPtr> future = QtConcurrent::mapped(viewportItems, ImageLoader{});
    m_imageLoadingFutureWatcher.setFuture(future);
}

void ImageListView::stopAsyncImageLoading()
{
    m_imageLoadingFutureWatcher.cancel();
}

QRect ImageListView::visualRect(const QModelIndex& index) const
{
    if (!index.isValid()) {
        return QRect();
    }

    int r ;
    int c;

    if(index.row() < 1)
    {
        r = 0;
        c = 0;
    }
    else
    {
        r = index.row() / (width() / m_thumb_size);
        c = index.row() % (width() / m_thumb_size);

    }

    int x = c * m_thumb_size;
    int y = r * m_thumb_size;

    QRect result{
        x - horizontalOffset(),
                y - verticalOffset(),
                m_thumb_size,
                m_thumb_size
    };
    return result;
}

void ImageListView::scrollTo(const QModelIndex& index, ScrollHint hint)
{
    Q_UNUSED(hint)

    QRect view = viewport()->rect();
    QRect rect = visualRect(index);

    if (rect.top() < view.top()) {
        verticalScrollBar()->setValue(verticalScrollBar()->value() + rect.top() - view.top());
    } else if (rect.bottom() > view.bottom()) {
        verticalScrollBar()->setValue(
                    verticalScrollBar()->value() + qMin(rect.bottom() - view.bottom(), rect.top() - view.top()));
    }
}

QModelIndex ImageListView::indexAt(const QPoint& point) const
{
    if (model())
    {
        QPoint p{ point.x() + horizontalOffset(), point.y() + verticalOffset() };
        int c = p.x() / m_thumb_size;
        int r = p.y() / m_thumb_size;
        int i = r * (width() / m_thumb_size) + c;

        if (i >= 0 && i < model()->rowCount(rootIndex())) {
            return model()->index(i, 0, rootIndex());
        }
    }
    return QModelIndex();
}

QModelIndex ImageListView::moveCursor(CursorAction cursorAction, Qt::KeyboardModifiers modifiers)
{
    Q_UNUSED(modifiers)
    return currentIndex();
}

int ImageListView::horizontalOffset() const
{
    return 0;
}

int ImageListView::verticalOffset() const
{
    return verticalScrollBar()->value();
}

bool ImageListView::isIndexHidden(const QModelIndex& index) const
{
    Q_UNUSED(index);
    return false;
}

void ImageListView::setSelection(const QRect& rect, QItemSelectionModel::SelectionFlags command)
{

}

QRegion ImageListView::visualRegionForSelection(const QItemSelection& selection) const
{
    QModelIndexList list = selection.indexes();
    QRegion region;
    foreach (const QModelIndex& index, list) {
        QRect rect = visualRect(index);
        if (rect.isValid()) {
            region += rect;
        }
    }
    return region;
}

namespace {
void paintOutline(QPainter& painter, const QRect& rect)
{
    QRect r = rect.adjusted(1, 1, -1, -1);
    painter.save();
    painter.drawRect(r);
    painter.restore();
}
}

void ImageListView::paintEvent(QPaintEvent* event)
{
    m_invalidatingModelRows.clear();
    Q_UNUSED(event);
    QList<int> imageIndexList;
    QPair<int, int> rowRange = modelRowRangeForViewportRect(event->rect());
    for (int row = rowRange.first; row < rowRange.second; ++row) {
        imageIndexList.append(row);
    }
    QStylePainter painter(viewport());
    painter.setRenderHints(QPainter::Antialiasing);

    foreach (int row, imageIndexList) {
        QModelIndex index = model()->index(row, 0, rootIndex());
        if (!index.isValid()) {
            continue;
        }
        QRect rect = visualRect(index);
        if (!rect.isValid() || rect.bottom() < 0 || rect.y() > viewport()->height())
            continue;
        QString imageFileName =model()->data(index).toString();
        QImage* ptr = m_imageCache.object(imageFileName);
        if (ptr) {
            QImage* image = ptr;
            QRectF imageRect = image->rect();
            QRectF drawRect = rect.adjusted(2, 2, -2, -24);
            QImage resized = image->scaled(drawRect.width(), drawRect.height(), Qt::IgnoreAspectRatio);
            painter.drawImage(drawRect, resized);
            QRect rect_label = rect.adjusted(2, m_thumb_size - 24, -2, -2);
            painter.drawText(rect_label.adjusted(32, 0, 0, 0), Qt::AlignVCenter | Qt::AlignLeft,QFileInfo(imageFileName).fileName());

            cbOpt.rect = rect_label.adjusted(8, 0, 0, 0);
            cbOpt.state = QStyle::State_Enabled;
            bool isChecked = index.data(Qt::CheckStateRole).toBool();
            if (isChecked)
            {
                cbOpt.state |= QStyle::State_On;
            }
            else
            {
                cbOpt.state |= QStyle::State_Off;
            }
            painter.drawControl(QStyle::CE_CheckBox, cbOpt);
        } else {
            painter.setPen(QPen(QColor("gray"), 1));
            painter.drawText(rect, Qt::AlignCenter, "Loading...");
        }
    }
}

void ImageListView::updateGeometries()
{
    int columnCount = (width() / m_thumb_size );
    QRect viewportRect = viewport()->rect();
    int viewportWidth = width();
    int verticalScrollBarWidth = verticalScrollBar()->width();
    int modelRowCount = model()->rowCount(rootIndex());
    int windowRowCount = modelRowCount / columnCount + ((modelRowCount % columnCount) ? 1 : 0);
    int imageWidth = m_thumb_size;
    int imageHeight = m_thumb_size;
    if (imageHeight) {
        int viewportRowCount = (viewportRect.height() / imageHeight + 1);
        m_imageCache.setMaxCost(viewportRowCount * columnCount * 2);
    }

    if (windowRowCount * imageHeight > viewportRect.height()) {
        viewportWidth -= verticalScrollBarWidth;
        int verticalScrollBarMaximum = windowRowCount * imageHeight;
        if (verticalScrollBarMaximum < viewportRect.height()) {
            verticalScrollBarMaximum = 1;
        } else {
            verticalScrollBarMaximum -= viewportRect.height();
        }
        verticalScrollBar()->setRange(0, verticalScrollBarMaximum);

        verticalScrollBar()->setPageStep(imageHeight);
        verticalScrollBar()->setSingleStep(imageHeight);

    } else {
        verticalScrollBar()->setRange(0, 0);
    }
}

void ImageListView::verticalScrollbarValueChanged(int value)
{
    QAbstractItemView::verticalScrollbarValueChanged(value);
    startScrollDelayTimer();
}

void ImageListView::resizeEvent(QResizeEvent* event)
{
    QAbstractItemView::resizeEvent(event);
    startScrollDelayTimer();
}

void ImageListView::mouseDoubleClickEvent(QMouseEvent *event)
{
    QPoint pos = event->pos();
    auto nIndex = indexAt(event->pos());

    if(event->button()==Qt::RightButton)
    {
        if((nIndex.row() >= 0))
        {
            index_keypress_event = nIndex.row();
            emit signal_item_double_clicked(nIndex.row());
        }
        else
        {
            emit signal_list_view_invalid_item_clicked();
        }

    }
    else
    {
        QPoint p{ pos.x() + horizontalOffset(), pos.y() + verticalOffset() + 24 };

        int c = p.x() / m_thumb_size;
        int r = p.y() / m_thumb_size;
        int i = r * (width() / m_thumb_size) + c;
        auto q = model()->index(i, 0, rootIndex());

        if( q.row() != nIndex.row())
        {
            model()->setData(nIndex, Qt::Checked, Qt::CheckStateRole);
        }

        if(nIndex.row() >= 0)
        {
            index_keypress_event = nIndex.row();
            emit signal_item_double_clicked(nIndex.row());
        } else
        {
            emit signal_list_view_invalid_item_clicked();
        }

        QAbstractItemView::mousePressEvent(event);
        QAbstractItemView::reset();
    }
}

void ImageListView::mousePressEvent(QMouseEvent *event)
{
    QPoint pos = event->pos();
    auto nIndex = indexAt(event->pos());

    if(event->button()==Qt::RightButton)
    {
        if((nIndex.row() >= 0))
        {
            index_keypress_event = nIndex.row();
            emit signal_item_clicked(nIndex.row());
        }
        else
        {
            emit signal_list_view_invalid_item_clicked();
        }

    }
    else
    {
        QPoint p{ pos.x() + horizontalOffset(), pos.y() + verticalOffset() + 24 };

        int c = p.x() / m_thumb_size;
        int r = p.y() / m_thumb_size;
        int i = r * (width() / m_thumb_size) + c;
        auto q = model()->index(i, 0, rootIndex());

        if( q.row() != nIndex.row())
        {
            model()->setData(nIndex, Qt::Checked, Qt::CheckStateRole);
        }

        if(nIndex.row() >= 0)
        {
            index_keypress_event = nIndex.row();
            emit signal_item_clicked(nIndex.row());
        } else
        {
            emit signal_list_view_invalid_item_clicked();
        }

        QAbstractItemView::mousePressEvent(event);
        QAbstractItemView::reset();
    }
}

void ImageListView::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_Space:
    {
        if(index_keypress_event >= 0)
        {
            emit signal_spacebar_pressed(index_keypress_event);
        }

        break;
    }
    case Qt::Key_E:
    {
        if(event->modifiers() & Qt::ControlModifier)
        {
            if(index_keypress_event >= 0)
            {
                emit signal_control_e_key_pressed(index_keypress_event);
            }
        }
        break;
    }
    default:
        //keyPressEvent(event);
        break;
    }

}

void ImageListView::setModel(QAbstractItemModel* model)
{
    QAbstractItemView::setModel(model);
}

void ImageListView::reset()
{
    m_imageCache.clear();
    m_invalidatingModelRows.clear();
    QAbstractItemView::reset();
    startScrollDelayTimer();
}

void ImageListView::OnExportToDesktop()
{
    QFileInfo fi(m_strFilePath);
    QString fileName = fi.fileName();
    QString desktopPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    QString destinationPath= desktopPath + QDir::separator() + fileName;
    QFile::copy(m_strFilePath, destinationPath);

}

