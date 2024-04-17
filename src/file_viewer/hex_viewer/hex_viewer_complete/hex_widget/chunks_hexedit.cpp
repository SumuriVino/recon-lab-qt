#include "chunks_hexedit.h"
#include <limits.h>

#define NORMAL 0
#define HIGHLIGHTED 1

#define BUFFER_SIZE 0x10000
#define CHUNK_SIZE 0x1000
#define READ_CHUNK_MASK Q_INT64_C(0xfffffffffffff000)

// ***************************************** Constructors and file settings

Chunks_hexedit::Chunks_hexedit(QObject *parent): QObject(parent)
{
    QBuffer *buf = new QBuffer(this);
    setIODevice(*buf);
}

Chunks_hexedit::Chunks_hexedit(QIODevice &ioDevice, QObject *parent): QObject(parent)
{
    setIODevice(ioDevice);
}

bool Chunks_hexedit::setIODevice(QIODevice &ioDevice)
{
    _ioDevice = &ioDevice;
    bool ok = _ioDevice->open(QIODevice::ReadOnly);
    if (ok)   // Try to open IODevice
    {
        //_size = _ioDevice->size();
        _size = source_file_size;
        _ioDevice->close();
    }
    else                                        // Fallback is an empty buffer
    {

        QBuffer *buf = new QBuffer(this);
        _ioDevice = buf;
        _size = 0;
    }
    _chunks.clear();
    _pos = 0;
    return ok;
}


// ***************************************** Getting data out of Chunks

QByteArray Chunks_hexedit::data(qint64 pos, qint64 maxSize, QByteArray *highlighted)
{
    qint64 ioDelta = 0;
    int chunkIdx = 0;

    Chunk_hexedit_struct chunk;
    QByteArray buffer;

    // Do some checks and some arrangements
    if (highlighted)
        highlighted->clear();

    if (pos >= _size)
        return buffer;

    if (maxSize < 0)
        maxSize = _size;
    else
        if ((pos + maxSize) > _size)
            maxSize = _size - pos;

    _ioDevice->open(QIODevice::ReadOnly);

    while (maxSize > 0)
    {
        chunk.absPos = LLONG_MAX;
        bool chunksLoopOngoing = true;
        while ((chunkIdx < _chunks.count()) && chunksLoopOngoing)
        {
            // In this section, we track changes before our required data and
            // we take the editdet data, if availible. ioDelta is a difference
            // counter to justify the read pointer to the original data, if
            // data in between was deleted or inserted.

            chunk = _chunks[chunkIdx];
            if (chunk.absPos > pos)
                chunksLoopOngoing = false;
            else
            {
                chunkIdx += 1;
                qint64 count;
                qint64 chunkOfs = pos - chunk.absPos;
                if (maxSize > ((qint64)chunk.data.size() - chunkOfs))
                {
                    count = (qint64)chunk.data.size() - chunkOfs;
                    ioDelta += CHUNK_SIZE - chunk.data.size();
                }
                else
                    count = maxSize;
                if (count > 0)
                {
                    buffer += chunk.data.mid(chunkOfs, (int)count);
                    maxSize -= count;
                    pos += count;
                    if (highlighted)
                        *highlighted += chunk.dataChanged.mid(chunkOfs, (int)count);
                }
            }
        }

        if ((maxSize > 0) && (pos < chunk.absPos))
        {
            // In this section, we read data from the original source. This only will
            // happen, whe no copied data is available

            qint64 byteCount;
            QByteArray readBuffer;
            if ((chunk.absPos - pos) > maxSize)
                byteCount = maxSize;
            else
                byteCount = chunk.absPos - pos;

            maxSize -= byteCount;


//            _ioDevice->seek(pos + ioDelta);
//            readBuffer = _ioDevice->read(byteCount);

            int offset_in_block = (pos + ioDelta) % 512;

            _ioDevice->seek(pos + ioDelta - offset_in_block);
            readBuffer = _ioDevice->read(byteCount + offset_in_block);

            readBuffer.remove(0, offset_in_block);

            buffer += readBuffer;
            if (highlighted)
                *highlighted += QByteArray(readBuffer.size(), NORMAL);
            pos += readBuffer.size();
        }
    }
    _ioDevice->close();
    return buffer;
}

bool Chunks_hexedit::write(QIODevice &iODevice, qint64 pos, qint64 count)
{

    if (count == -1)
        count = _size;
    bool ok = iODevice.open(QIODevice::WriteOnly);

    if (ok)
    {
        for (qint64 idx=pos; idx < count; idx += BUFFER_SIZE)
        {
            QByteArray ba = data(idx, BUFFER_SIZE);
            iODevice.write(ba);
        }

        iODevice.close();
    }
    return ok;
}


// ***************************************** Set and get highlighting infos

void Chunks_hexedit::setDataChanged(qint64 pos, bool dataChanged)
{
    if ((pos < 0) || (pos >= _size))
        return;
    int chunkIdx = getChunkIndex(pos);
    qint64 posInBa = pos - _chunks[chunkIdx].absPos;
    _chunks[chunkIdx].dataChanged[(int)posInBa] = char(dataChanged);
}

bool Chunks_hexedit::dataChanged(qint64 pos)
{
    QByteArray highlighted;
    data(pos, 1, &highlighted);
    return bool(highlighted.at(0));
}


// ***************************************** Search API

qint64 Chunks_hexedit::indexOf(const QByteArray &ba, qint64 from)
{
    qint64 result = -1;
    QByteArray buffer;

    for (qint64 pos=from; (pos < _size) && (result < 0); pos += BUFFER_SIZE)
    {
        buffer = data(pos, BUFFER_SIZE + ba.size() - 1);
        int findPos = buffer.indexOf(ba);
        if (findPos >= 0)
            result = pos + (qint64)findPos;
    }
    return result;
}

qint64 Chunks_hexedit::lastIndexOf(const QByteArray &ba, qint64 from)
{
    qint64 result = -1;
    QByteArray buffer;

    for (qint64 pos=from; (pos > 0) && (result < 0); pos -= BUFFER_SIZE)
    {
        qint64 sPos = pos - BUFFER_SIZE - (qint64)ba.size() + 1;
        if (sPos < 0)
            sPos = 0;
        buffer = data(sPos, pos - sPos);
        int findPos = buffer.lastIndexOf(ba);
        if (findPos >= 0)
            result = sPos + (qint64)findPos;
    }
    return result;
}


// ***************************************** Char manipulations

bool Chunks_hexedit::insert(qint64 pos, char b)
{
    if ((pos < 0) || (pos > _size))
        return false;
    int chunkIdx;
    if (pos == _size)
        chunkIdx = getChunkIndex(pos-1);
    else
        chunkIdx = getChunkIndex(pos);
    qint64 posInBa = pos - _chunks[chunkIdx].absPos;
    _chunks[chunkIdx].data.insert(posInBa, b);
    _chunks[chunkIdx].dataChanged.insert(posInBa, char(1));
    for (int idx=chunkIdx+1; idx < _chunks.size(); idx++)
        _chunks[idx].absPos += 1;
    _size += 1;
    _pos = pos;
    return true;
}

bool Chunks_hexedit::overwrite(qint64 pos, char b)
{
    if ((pos < 0) || (pos >= _size))
        return false;
    int chunkIdx = getChunkIndex(pos);
    qint64 posInBa = pos - _chunks[chunkIdx].absPos;
    _chunks[chunkIdx].data[(int)posInBa] = b;
    _chunks[chunkIdx].dataChanged[(int)posInBa] = char(1);
    _pos = pos;
    return true;
}

bool Chunks_hexedit::removeAt(qint64 pos)
{
    if ((pos < 0) || (pos >= _size))
        return false;
    int chunkIdx = getChunkIndex(pos);
    qint64 posInBa = pos - _chunks[chunkIdx].absPos;
    _chunks[chunkIdx].data.remove(posInBa, 1);
    _chunks[chunkIdx].dataChanged.remove(posInBa, 1);
    for (int idx=chunkIdx+1; idx < _chunks.size(); idx++)
        _chunks[idx].absPos -= 1;
    _size -= 1;
    _pos = pos;
    return true;
}


// ***************************************** Utility functions

char Chunks_hexedit::operator[](qint64 pos)
{
    return data(pos, 1)[0];
}

qint64 Chunks_hexedit::pos()
{
    return _pos;
}

qint64 Chunks_hexedit::size()
{
    return _size;
}

void Chunks_hexedit::pub_set_source_file_path(QString path)
{
    source_file_path = path;
}

void Chunks_hexedit::pub_set_source_size(qint64 source_size)
{
    source_file_size = source_size;
}

void Chunks_hexedit::pub_set_source_block_size(int size)
{
    source_block_size = size;
}

int Chunks_hexedit::getChunkIndex(qint64 absPos)
{
    // This routine checks, if there is already a copied chunk available. If os, it
    // returns a reference to it. If there is no copied chunk available, original
    // data will be copied into a new chunk.

    int foundIdx = -1;
    int insertIdx = 0;
    qint64 ioDelta = 0;


    for (int idx=0; idx < _chunks.size(); idx++)
    {
        Chunk_hexedit_struct chunk = _chunks[idx];
        if ((absPos >= chunk.absPos) && (absPos < (chunk.absPos + chunk.data.size())))
        {
            foundIdx = idx;
            break;
        }
        if (absPos < chunk.absPos)
        {
            insertIdx = idx;
            break;
        }
        ioDelta += chunk.data.size() - CHUNK_SIZE;
        insertIdx = idx + 1;
    }

    if (foundIdx == -1)
    {
        Chunk_hexedit_struct newChunk;
        qint64 readAbsPos = absPos - ioDelta;
        qint64 readPos = (readAbsPos & READ_CHUNK_MASK);
        _ioDevice->open(QIODevice::ReadOnly);
        _ioDevice->seek(readPos);
        newChunk.data = _ioDevice->read(CHUNK_SIZE);
        _ioDevice->close();
        newChunk.absPos = absPos - (readAbsPos - readPos);
        newChunk.dataChanged = QByteArray(newChunk.data.size(), char(0));
        _chunks.insert(insertIdx, newChunk);
        foundIdx = insertIdx;
    }
    return foundIdx;
}


#ifdef MODUL_TEST
int Chunks::chunkSize()
{
    return _chunks.size();
}

#endif
