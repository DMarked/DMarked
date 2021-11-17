#ifndef MAPPAGESIZE_H
#define MAPPAGESIZE_H

#include <QPageLayout>
#include <QMap>

class MapPageSize
{
public:
    MapPageSize();
    QMap<QString, QPageSize::PageSizeId> mapPageSize;

    static MapPageSize* getInstance();
};

#endif // MAPPAGESIZE_H
